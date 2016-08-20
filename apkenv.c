
/**
 * apkenv
 * Copyright (c) 2012, Thomas Perl <m@thp.io>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#include <dlfcn.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>

#include "jni/jnienv.h"
#include "jni/shlib.h"
#include "apklib/apklib.h"
#include "imagelib/imagelib_priv.h"
#include "debug/debug.h"
#include "debug/wrappers.h"
#include "compat/gles_wrappers.h"
#include "compat/gles2_wrappers.h"
#include "linker/linker.h"
#include "compat/hooks.h"

#include "apkenv.h"

extern void *apkenv_android_dlopen(const char *filename, int flag);
extern int apkenv_android_dlclose(void *handle);
/* Global application state */
struct GlobalState global;
struct ModuleHacks global_module_hacks;

static void *
lookup_symbol_impl(const char *method)
{
    return jni_shlib_resolve(&global, method);
}

static void *
lookup_lib_symbol_impl(const char *lib, const char *method)
{
    return jni_shlib_lib_resolve(&global, lib, method);
}

static void
foreach_file_impl(const char *prefix, apk_for_each_file_callback callback)
{
    apk_for_each_file(global.apklib_handle, prefix, callback);
}

static int
read_file_impl(const char *filename, char **buffer, size_t *size)
{
    return (apk_read_file(global.apklib_handle,
                filename, buffer, size) == APK_OK);
}

static void *
read_file_to_jni_array_impl(const char *filename)
{
    char *buffer = NULL;
    size_t size = 0;

    if (apk_read_file(global.apklib_handle, filename, &buffer, &size) == APK_OK) {
        if(!global.use_dvm)
        {
            struct dummy_array *array = NULL;
            array = malloc(sizeof(*array));
            if (array == NULL)
                return NULL;
            array->data = buffer;
            array->length = size;
            array->element_size = 1;

            return array;
        }
        else
        {
            const struct JNINativeInterface *env = global._env->functions;
            jarray *array = env->NewByteArray(ENV((&global)), size);
            jbyte *elements = env->GetByteArrayElements(ENV((&global)), array, 0);
            memcpy(elements, buffer, size);
            free(buffer);
            env->ReleaseByteArrayElements(ENV((&global)), array, elements, JNI_ABORT);

            return array;
        }
    }

    return NULL;
}

static const char *
lookup_resource_impl(const char *key)
{
    size_t i;

    if (strcmp(key, "app_name") == 0)
        return global.resource_strings.app_name;

    for (i = 0; i < global.resource_strings.count; i++)
        if (strcmp(key, global.resource_strings.entries[i].key) == 0)
            return global.resource_strings.entries[i].value;

    return NULL;
}

static void
register_module(struct SupportModule *module)
{
    struct SupportModule **current = &(global.support_modules);

    while ((*current) != NULL) {
        if ((*current)->priority < module->priority) {
            break;
        }
        current = &((*current)->next);
    }

    module->next = *current;
    *current = module;
}

static void
load_module(const char *filename)
{
    void *dl = dlopen(filename, RTLD_LAZY);
    if (dl == NULL) {
        printf("Cannot load module: %s\n", filename);
        return;
    }

    struct SupportModule *module = calloc(1, sizeof(struct SupportModule));
    module->global = &global;
    module->filename = strdup(filename);

    apkenv_module_init_t module_init = (apkenv_module_init_t)dlsym(dl,
            APKENV_MODULE_INIT);

    if (module_init != NULL) {
        int version = module_init(APKENV_MODULE_VERSION, module);
        switch (version) {
            case 0:
                printf("Cannot init module: %s\n", filename);
                break;
            case APKENV_MODULE_VERSION:
                printf("Loaded module: %s (%d)\n", filename, module->priority);
                register_module(module);
                return;
            default:
                printf("Wrong module ABI version: %x (need %x)\n",
                        version, APKENV_MODULE_VERSION);
                break;
        }
    } else {
        printf("Cannot find %s(): %s\n", APKENV_MODULE_INIT, filename);
    }

    free(module->filename);
    free(module);
    dlclose(dl);
}

static void
load_modules(const char *dirname)
{
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        return;
    }

    char tmp[PATH_MAX];
    strcpy(tmp, dirname);
    strcat(tmp, "/");
    char *filename = tmp + strlen(dirname) + 1;

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        int len = strlen(ent->d_name) - strlen(APKENV_MODULE_SUFFIX);
        if (len > 0) {
            char *suffix = ent->d_name + len;
            if (strcmp(suffix, APKENV_MODULE_SUFFIX) == 0) {
                strcpy(filename, ent->d_name);
                load_module(tmp);
            }
        }
    }
    closedir(dir);
}

static void
install_overrides(struct SupportModule *module)
{
    int i;
    void **override, **fake;

    /* Install overrides for JNIEnv */
    override = (void**)(&(module->override_env));
    fake = (void**)(&(global.fake_env));
    for (i=0; i<sizeof(module->override_env)/sizeof(void*); i++) {
        if (override[i]) {
            fake[i] = override[i];
        }
    }

    /* Install overrides for JavaVM */
    override = (void**)(&(module->override_vm));
    fake = (void**)(&(global.fake_vm));
    for (i=0; i<sizeof(module->override_vm)/sizeof(void*); i++) {
        if (override[i]) {
            fake[i] = override[i];
        }
    }
}

void
dalvikvm_JNIEnv_DeleteLocalRef(JNIEnv* p0, jobject p1)
{
#ifdef APKENV_DEBUG
    printf("dalvikvm_JNIEnv_DeleteLocalRef(%x)\n", p1);
#endif
    global.dalvik_copy_env.DeleteLocalRef(p0, p1);
    (*p0)->ExceptionClear(p0);
}


static void
install_default_dalvik_overrides()
{
    struct JNINativeInterface *dalvik_env = (struct JNINativeInterface *)global._env->functions;

    int pagesize = sysconf(_SC_PAGESIZE);
    mprotect((char*)((int)dalvik_env & ~(pagesize-1)), pagesize*2, PROT_READ | PROT_WRITE);

    dalvik_env->NewGlobalRef = JNIEnv_NewGlobalRef;
    dalvik_env->DeleteGlobalRef = JNIEnv_DeleteGlobalRef;

    dalvik_env->GetObjectClass = JNIEnv_GetObjectClass;

    dalvik_env->FindClass = JNIEnv_FindClass;
    dalvik_env->GetMethodID = JNIEnv_GetMethodID;
    dalvik_env->GetFieldID = JNIEnv_GetFieldID;
    dalvik_env->GetStaticFieldID = JNIEnv_GetStaticFieldID;
    dalvik_env->GetStaticObjectField = JNIEnv_GetStaticObjectField;
    dalvik_env->GetStaticMethodID = JNIEnv_GetStaticMethodID;
    dalvik_env->GetObjectField = JNIEnv_GetObjectField;

    dalvik_env->DeleteLocalRef = dalvikvm_JNIEnv_DeleteLocalRef;

    dalvik_env->RegisterNatives = JNIEnv_RegisterNatives;

    mprotect((char*)((int)dalvik_env & ~(pagesize-1)), pagesize*2, PROT_READ);
}

static void
install_dalvik_overrides(struct SupportModule *module)
{
    int i;
    void **dalvik, **override;

    int pagesize = sysconf(_SC_PAGESIZE);

    /* Install overrides for JNIEnv */
    dalvik = (void**)(global._env->functions);
    mprotect((char*)((int)dalvik & ~(pagesize-1)), pagesize*2, PROT_READ | PROT_WRITE);
    override = (void**)(&(module->override_env));
    for (i=0; i<sizeof(*global._env->functions)/sizeof(void*); i++) {
        if (override[i]) {
            dalvik[i] = override[i];
        }
        else {
            dalvik[i] = create_wrapper(envnames[i], dalvik[i], WRAPPER_DALVIK);
        }
    }
    mprotect((char*)((int)dalvik & ~(pagesize-1)), pagesize*2, PROT_READ);

    /* Install overrides for JavaVM */
    dalvik = (void**)(global._vm->functions);
    mprotect((char*)((int)dalvik & ~(pagesize-1)), pagesize, PROT_READ | PROT_WRITE);
    override = (void**)(&(module->override_vm));
    for (i=0; i<sizeof(*global._vm->functions)/sizeof(void*); i++) {
        if (override[i]) {
            dalvik[i] = override[i];
        }
        else {
            dalvik[i] = create_wrapper(vmnames[i], dalvik[i], WRAPPER_DALVIK);
        }
    }
    mprotect((char*)((int)dalvik & ~(pagesize-1)), pagesize, PROT_READ);
}

static void
usage()
{
    if (global.platform->get_path(PLATFORM_PATH_INSTALL_DIRECTORY) != NULL) {
        printf("Usage: %s [options] <file.apk>\n",global.apkenv_executable);
        printf("Available options are:\n");
    } else {
        printf("Usage: %s [options] <file.apk>\n",global.apkenv_executable);
        printf("Available options are:\n");
        printf("\t--install\tinstall the apk and set up desktop entries.\n");
    }
    printf("\t--enable-trace-latehooked|-l\t\tEnable tracing of late hooked functions.\n");
    printf("\t--enable-trace_unhooked|-u\t\tEnable tracing of unhooked functions.\n");
    printf("\t--enable-trace-dynhook|-d\t\tEnable tracing of dynamically loaded functions.\n");
    printf("\t--enable-trace-arm-injection|-ai\tEnable tracing of unhooked internal ARM functions. (EXPERIMENTAL)\n");
    printf("\t--enable-trace-thumb-injection|-ti\tEnable tracing of unhooked internal THUMB functions. (HIGHLY EXPERIMENTAL)\n");
    printf("\t--trace-function|-tf <function>\t\tTrace the specified function.\n");
    printf("\t--trace-all|-ta\t\t\t\tTrace all functions.\n");
    printf("\t--use-dvm <path/to/android>\t\tUse the dalvikvm instead of our fake vm.\n");
    printf("\t--help|-h\t\t\t\tPrint this help.\n");
    
    exit(1);
}

static const char *
apk_basename(const char *filename)
{
    char *fn = strrchr(filename, '/');
    if (fn != NULL) {
        return fn + 1;
    }
    return filename;
}

static void
operation(const char *operation, const char *filename)
{
    const char *install_directory = global.platform->get_path(PLATFORM_PATH_INSTALL_DIRECTORY);
    const char *data_directory = global.platform->get_path(PLATFORM_PATH_DATA_DIRECTORY);

    if (strcmp(operation, "--install") == 0 && install_directory != NULL) {
        char apkenv_absolute[PATH_MAX];
        char apk_absolute[PATH_MAX];

        readlink("/proc/self/exe", apkenv_absolute, sizeof(apkenv_absolute));
        /* On Linux, realpath(3) always returns an absolute path */
        assert(realpath(filename, apk_absolute) != NULL);

        const char *app_name = apk_basename(filename);

        char icon_filename[PATH_MAX];
        sprintf(icon_filename, "%s%s.png", data_directory, app_name);

        struct stat st;
        if (stat(icon_filename, &st) != 0) {
            // Extract icon from .apk here
            AndroidApk *apk = apk_open(filename);
            assert(apk != NULL);

            char *icon_buffer;
            size_t icon_size;
            /**
             * FIXME: The icon isn't always called like that, we should look
             * into the Android manifest and get the icon filename and also
             * the "nice" application name (for the .desktop file below) from
             * there (see also: http://stackoverflow.com/questions/2097813/)
             **/
            enum ApkResult result = apk_read_file(apk,
                    "res/drawable-hdpi/icon.png", &icon_buffer, &icon_size);
            apk_close(apk);

            if (result == APK_OK) {
                FILE *fp = fopen(icon_filename, "wb");
                assert(fp != NULL);
                fwrite(icon_buffer, icon_size, 1, fp);
                fclose(fp);
                printf("Extracted icon: %s\n", icon_filename);
                free(icon_buffer);
            } else {
                printf("Cannot extract icon from %s\n", filename);
                exit(2);
            }
        }

        char desktop_filename[PATH_MAX];
        recursive_mkdir(install_directory);
        sprintf(desktop_filename, "%s/%s.desktop", install_directory, app_name);

        FILE *desktop = fopen(desktop_filename, "w");
        assert(desktop != NULL);
        fprintf(desktop,
                "[Desktop Entry]\n"
                "Name=%s\n"
                "Exec=invoker --single-instance --type=e %s %s\n"
                "Icon=%s\n"
                "Terminal=false\n"
                "Type=Application\n"
                "Categories=Game;\n",
                app_name, apkenv_absolute, apk_absolute, icon_filename);
        fclose(desktop);
        printf("Installed: %s\n", desktop_filename);
    }
}


static int
system_init(int gles_version)
{
    if (!(global.platform->init(gles_version))) {
        printf("Platform initialization failed.\n");
        return 0;
    }

    gles_extensions_init();
    gles1_init();
    gles2_init();

    /* SDL loads some libs.. */
    apkenv_notify_gdb_of_libraries();

    return 1;
}

int init_dvm(struct GlobalState *global, void *libdvm_handle)
{
    printf("initializing dalvikvm\n");

    int (*CreateJavaVM)(JavaVM *vm, JNIEnv *env, const char *apk_filename);
    CreateJavaVM = apkenv_android_dlsym(libdvm_handle, "compat_CreateJavaVM");
    if(NULL == CreateJavaVM) {
        return 0; // failed
    }
    if(0 == CreateJavaVM((const struct JNIInvokeInterface**)&(global->_vm), (const struct JNINativeInterface**)&(global->_env), global->apk_filename)) return 0; // failed

    global->vm = global->_vm->functions;
    global->env = global->_env->functions;

    memcpy(&global->dalvik_copy_env, global->env, sizeof(struct JNINativeInterface));

    // some must overrides
    install_default_dalvik_overrides();

    return 1; // success
}

/* Provided by one of the support modules in "platform/" */
extern struct PlatformSupport platform_support;

static void
parse_config_line(char *line)
{
    if (line[0] == '#') {
        // Comment line
        return;
    }

    char *eq = strchr(line, '=');
    if (eq) {
        *eq = '\0';

        char *key = line;
        char *value = eq + 1;

        if (strlen(key) == 0 || strlen(value) == 0) {
            // Empty key or value
            return;
        }

        if (global.config_count == global.config_size) {
            global.config_size += 4;
            size_t size = global.config_size * sizeof(struct ConfigOption);
            global.config = realloc(global.config, size);
        }

        global.config[global.config_count].key = strdup(key);
        global.config[global.config_count].value = strdup(value);
        global.config_count++;

        *eq = '=';
    }
}

static void
read_config(const char *filename)
{
    FILE *config_file = fopen(filename, "r");
    if (!config_file) {
        return;
    }

    char tmp[1024];
    while (!feof(config_file)) {
        if (fgets(tmp, sizeof(tmp), config_file) != NULL) {
            size_t end = strlen(tmp) - 1;
            while (isspace(tmp[end])) {
                tmp[end--] = '\0';
            }
            parse_config_line(tmp);
        }
    }

#ifdef APKENV_DEBUG
    int i;
    for (i=0; i < global.config_count; i++) {
        printf("Config option %s=%s\n",
                global.config[i].key,
                global.config[i].value);
    }
#endif
}

char *
get_config(char *name)
{
    if (!global.config) {
        return NULL;
    }

    int i;
    for (i=0; i<global.config_count; i++) {
        if (strcmp(global.config[i].key, name) == 0) {
            return global.config[i].value;
        }
    }

    return 0;
}

int
get_config_int(char *name, int fallback)
{
    int result = fallback;
    char *value = get_config(name);

    if (value != NULL) {
        result = atoi(value);
    }

    return result;
}

int main(int argc, char **argv)
{
    global.platform = &platform_support;

    debug_init();

    char **tmp;
    void *libdvm_handle = NULL;
    char android_sopath[PATH_MAX];

    const char *main_data_dir = global.platform->get_path(PLATFORM_PATH_DATA_DIRECTORY);
    recursive_mkdir(main_data_dir);

    char *config_path;
    asprintf(&config_path, "%s/config", main_data_dir);
    read_config(config_path);
    free(config_path);

    global.apkenv_executable = argv[0];
    global.apkenv_headline = APKENV_HEADLINE;
    global.apkenv_copyright = APKENV_COPYRIGHT;

    printf("%s\n%s\n\n", global.apkenv_headline, global.apkenv_copyright);
   
    global.trace_all = 0;
    global.trace_latehooked = 0;
    global.trace_unhooked = 0;
    global.trace_dynhooked = 0;
    global.trace_arm_injection = 0;
    global.trace_thumb_injection = 0; 
    global.functions_to_trace = NULL;
    
    global.use_dvm = 0;

    if(argc <= 1) {
        printf("ERROR: too few arguments\n");
        usage();
    }
    
    if(argc >= 2) {
        int i;
        // parse argv, last argument has to be the apk
        for(i=1;i<argc;i++) {
            if(0 == strcmp(argv[i], "--install")) {
                i++;
                if(i >= argc) {
                    printf("missing argument to --install\n");
                    return -1;
                }
                operation(argv[i-1],argv[i]);
            }
            else if(0 == strcmp(argv[i], "--help")
                 || 0 == strcmp(argv[i], "-h")) {
                usage();
            }
            else if(0 == strcmp(argv[i], "--enable-trace-latehooked")
                 || 0 == strcmp(argv[i], "-l")) {
                global.trace_latehooked = 1;
            }
            else if(0 == strcmp(argv[i], "--enable-trace-unhooked")
                 || 0 == strcmp(argv[i], "-u")) {
                global.trace_unhooked = 1;
            }
            else if(0 == strcmp(argv[i], "--enable-trace-dynhooked")
                 || 0 == strcmp(argv[i], "-d")) {
                global.trace_dynhooked = 1;
            }
            else if(0 == strcmp(argv[i], "--enable-trace-arm-injection")
                 || 0 == strcmp(argv[i], "-ai")) {
                global.trace_arm_injection = 1;
            }
            else if(0 == strcmp(argv[i], "--enable-trace-thumb-injection")
                 || 0 == strcmp(argv[i], "-ti")) {
                global.trace_thumb_injection = 1;
            }
            else if(0 == strcmp(argv[i], "--trace-all")
                 || 0 == strcmp(argv[i], "-ta")) {
                global.trace_all = 1;
            }
            else if(0 == strcmp(argv[i], "--trace-function")
                 || 0 == strcmp(argv[i], "-tf")) {
                i++;
                if(i >= argc) {
                    printf("ERROR: missing argument to %s", argv[i-1]);
                    usage();
                }
                if(NULL == global.functions_to_trace) {
                    global.functions_to_trace = (struct trace_list*)malloc(sizeof(struct trace_list));
                    global.functions_to_trace->name = argv[i];
                    global.functions_to_trace->next = NULL;
                }
                else {
                    struct trace_list *it = global.functions_to_trace;
                    struct trace_list *last = NULL;
                    while(NULL != it) {
                        last = it;
                        it = it->next;
                    }
                    last->next = (struct trace_list*)malloc(sizeof(struct trace_list));
                    last->next->name = argv[i];
                    last->next->next = NULL;
                }
            }
            else if(0 == strcmp(argv[i], "--use-dvm"))
            {
                i++;
                if(i >= argc) {
                    printf("ERROR: missing argument to --use-dvm\n");
                    usage();
                }
                global.use_dvm = 1;
                global.android_path = argv[i];
            }
        }
    }

    if((global.trace_all || NULL != global.functions_to_trace)
    && !global.trace_latehooked && !global.trace_unhooked
    && !global.trace_dynhooked && !global.trace_arm_injection
    && !global.trace_thumb_injection) {
        printf("WARNING: specified --trace-all or --trace-function but no type of tracing is enabled\n");
        usage();
    }

    memset(&global_module_hacks,0,sizeof(global_module_hacks));

    global.lookup_symbol = lookup_symbol_impl;
    global.lookup_lib_symbol = lookup_lib_symbol_impl;
    global.foreach_file = foreach_file_impl;
    global.read_file = read_file_impl;
    global.read_file_to_jni_array = read_file_to_jni_array_impl;
    global.recursive_mkdir = recursive_mkdir;
    global.lookup_resource = lookup_resource_impl;
    global.image_loader = imagelib_load_from_mem;
    global.module_hacks = &global_module_hacks;

    global.apk_filename = strdup(argv[argc-1]);
    
    hooks_init();
    
    if(global.use_dvm) {
        strcpy(android_sopath, global.android_path);
        strcat(android_sopath, "/system/lib/");
        apkenv_add_sopath(android_sopath);
        libdvm_handle = apkenv_android_dlopen("libdvm_compat.so", RTLD_LAZY);
        if(NULL == libdvm_handle) {
            printf("ERROR: could not load libdvm_compat.so.\n");
            libdvm_handle = NULL;
            exit(3);
        }
        else if(!init_dvm(&global, libdvm_handle)) {
            printf("ERROR: failed to initialize the dalvikvm.\n");
            apkenv_android_dlclose(libdvm_handle);
            libdvm_handle = NULL;
            exit(3);
        }
    }
    else {
        jnienv_init(&global);
        javavm_init(&global);
    }
    
    global.apklib_handle = apk_open(global.apk_filename);
    global.support_modules = NULL;

    static const char* libdir[] = {
#ifdef FREMANTLE
        // On Fremantle, only use non-v7a armeabi libs
        "assets/libs/armeabi",
        "lib/armeabi",
#else
        "assets/libs/armeabi-v7a",
        "assets/libs/armeabi",
        "lib/armeabi-v7a",
        "lib/armeabi",
#endif
        0
    };
    static const char* libblacklist[] = {
        "libs3eflurry_ext.so",
        0
    };
    int ilib = 0;
    struct SharedLibrary *shlibs = 0;
    while (libdir[ilib]!=0) {
        shlibs = apk_get_shared_libraries(global.apklib_handle,libdir[ilib],main_data_dir);
        if (shlibs!=0)
            break;
        ilib ++;
    }
    if (shlibs==0) {
        printf("Not a native APK.\n");
        return 0;
    }

    struct JniLibrary *lib = malloc(sizeof(struct JniLibrary));
    struct JniLibrary *head = lib;
    lib->next = 0;

    struct SharedLibrary *shlib = shlibs;
    struct JniLibrary *prev = NULL;

    while (shlib!=0) {
        const char *basename = strrchr(shlib->filename, '/');
        if (basename++ == NULL)
            basename = shlib->filename;
        for (ilib = 0; libblacklist[ilib] != NULL; ilib++)
            if (strcmp(basename, libblacklist[ilib]) == 0)
                break;
        if (libblacklist[ilib] != NULL) {
            shlib = shlib->next;
            if(shlib == 0) {
                if(prev != NULL) {
                    prev->next = 0;
                }
                free(lib);
                break;
            }
            continue;
        }
        apkenv_add_sopath(shlib->dirname);
        lib->lib = apkenv_android_dlopen(shlib->filename,RTLD_LAZY);
        if (!(lib->lib)) {
            printf("Missing library dependencies.\n");
            return 0;
        }
        lib->method_table = jni_shlib_find_methods(shlib->filename);
        lib->name = strdup(shlib->filename);
    // unlink(shlib->filename);
        shlib = shlib->next;
        if (shlib!=0) {
            lib->next = malloc(sizeof(struct JniLibrary));
            prev = lib;
            lib = lib->next;
            lib->next = 0;
        }
    }

    global.libraries = head;

    load_modules(".");
    load_modules(global.platform->get_path(PLATFORM_PATH_MODULE_DIRECTORY));
    apkenv_notify_gdb_of_libraries();

    if (global.support_modules == NULL) {
        printf("No support modules found.\n");
    }

    /* mmap apk for direct asset access (avoids tempfile extraction) */
    struct stat st;
    global.apk_fd = open(global.apk_filename, O_RDONLY);
    if (global.apk_fd == -1) {
        perror("open");
        exit(1);
    }
    if (fstat(global.apk_fd, &st)) {
        perror("fstat");
        exit(1);
    }
    global.apk_size = st.st_size;
    global.apk_in_mem = mmap(NULL, global.apk_size, PROT_READ,
        MAP_SHARED, global.apk_fd, 0);
    if (global.apk_in_mem == MAP_FAILED) {
        perror("warning: mmap'ing apk failed");
        global.apk_in_mem = NULL;
    }

    /* Search for a suitable module to handle the library */
    struct SupportModule *module = global.support_modules;
    while (module != NULL) {
        if (module->try_init(module)) {
            break;
        }
        module = module->next;
    }

    if (module == NULL) {
        printf("Not supported yet, but found JNI methods:\n");

        struct JniLibrary *lib = global.libraries;
        while (lib!=0) {
            tmp = lib->method_table;
            while (*tmp) {
                printf("    %s\n", *tmp);
                tmp++;
            }
            lib = lib->next;
        }
        goto finish;
    }

    /* figure out GLES version to use */
    int gles_version = 1;

    if (global.loader_seen_glesv1 && global.loader_seen_glesv2) {
        printf("Warning: app uses both GLESv1 and GLESv2 libs\n");
        /* TODO: perhaps consult the module what to use here? */
    } else if (global.loader_seen_glesv2) {
        gles_version = 2;
    }
#if !defined(APKENV_GLES)
    gles_version = 2;
#endif
    printf("Using GLES version %d\n", gles_version);

    if (!system_init(gles_version)) {
        return 0;
    }

    global.active_module = module;
    printf("Using module: %s\n", module->filename);
    if(global.use_dvm) install_dalvik_overrides(module);
    else install_overrides(module);

    char data_directory[PATH_MAX];
    strcpy(data_directory, main_data_dir);
    strcat(data_directory, apk_basename(global.apk_filename));
    strcat(data_directory, "/");
    recursive_mkdir(data_directory);

    apk_read_resources(global.apklib_handle, &global.resource_strings);

    int width, height;
    global.platform->get_size(&width, &height);
    module->init(module, width, height, data_directory);

    apkenv_notify_gdb_of_libraries();

    while (1) {

        if (module->requests_exit(module)) {
            break;
        }

        if (global.platform->input_update(module)) {
            break;
        }

        module->update(module);
        global.platform->update();
    }

finish:
    module->deinit(module);

    lib = global.libraries;
    while (lib!=0) {
        tmp = lib->method_table;
        while (*tmp) {
            free(*tmp++);
        }
        free(lib->method_table);
        unlink(lib->name);
        struct JniLibrary *next = lib->next;
        free(lib);
        lib = next;
    }

    if (global.apk_in_mem)
        munmap((void *)global.apk_in_mem, global.apk_size);
    close(global.apk_fd);
    apk_close(global.apklib_handle);
    global.platform->exit();

    if(NULL != libdvm_handle) {
        apkenv_android_dlclose(libdvm_handle);
        libdvm_handle = NULL;
    }
    
    release_all_wrappers();
    if(NULL != global.functions_to_trace) {
        struct trace_list *it = global.functions_to_trace;
        struct trace_list *next;
        while(it != NULL) {
            next = it->next;
            free(it);
            it = next;
        }
    }
    
    return 0;
}

