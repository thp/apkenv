
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

/**
 * unity support module 0.1 - by crow_riot
 **/

#include "common.h"
#include <string.h>


#ifdef APKENV_DEBUG
#  define MODULE_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define MODULE_DEBUG_PRINTF(...)
#endif


struct GlobalState* global;

/**
 *   jni implementations
 **/

typedef struct
{
    jclass clazz;
    jfieldID field;

} dummy_jobject;


/* -------- */


jfieldID JNIEnv_GetStaticFieldID(JNIEnv *p0, jclass p1, const char *p2, const char *p3) SOFTFP;
jobject JNIEnv_GetStaticObjectField(JNIEnv *p0, jclass p1, jfieldID p2) SOFTFP;
jclass JNIEnv_GetObjectClass(JNIEnv *p0, jobject p1) SOFTFP;
jobject JNIEnv_CallObjectMethod(JNIEnv* env, jobject p1, jmethodID p2, ...) SOFTFP;
jobject JNIEnv_CallStaticObjectMethod(JNIEnv* env, jclass p1, jmethodID p2, ...) SOFTFP;
const char * JNIEnv_GetStringUTFChars(JNIEnv *env, jstring string, jboolean *isCopy) SOFTFP;


jfieldID JNIEnv_GetStaticFieldID(JNIEnv *p0, jclass p1, const char *p2, const char *p3)
{
    struct dummy_jclass* cls = (struct dummy_jclass*)p1;
    MODULE_DEBUG_PRINTF("GetStaticFieldID %s %s %s\n", cls->name, p2, p3);

    struct _jfieldID* field = malloc(sizeof(struct _jfieldID));

    field->clazz = p1;
    field->name = strdup(p2);
    field->sig = strdup(p3);

    return (jfieldID)field;
}

jobject JNIEnv_GetStaticObjectField(JNIEnv *p0, jclass p1, jfieldID p2)
{
    struct dummy_jclass* cls = p1;
    struct _jfieldID* fld = (void *)p2;

    MODULE_DEBUG_PRINTF("GetStaticObjectField %s, %s, %s\n", cls->name, fld->name, fld->sig);

    dummy_jobject* obj = malloc(sizeof(dummy_jobject));
    obj->clazz = cls;
    obj->field = p2;

    return obj;
}

jclass JNIEnv_GetObjectClass(JNIEnv *p0, jobject p1)
{
    MODULE_DEBUG_PRINTF("GetObjectClass %x\n",p1);
    if (p1!=NULL)
    {
        dummy_jobject* obj = p1;
        return obj->clazz;
    }
    return NULL;
}


jobject JNIEnv_CallObjectMethod(JNIEnv* env, jobject p1, jmethodID p2, ...)
{
    MODULE_DEBUG_PRINTF("CallObjectMethod %x %x\n",p1,p2);

    //dummy_jobject* obj = p1;
    jmethodID method = p2;

    if (strcmp(method->name,"getPackageCodePath")==0)
        return (*env)->NewStringUTF(env, GLOBAL_J(env)->apk_filename);

    return NULL;
}

jobject
JNIEnv_CallStaticObjectMethod(JNIEnv* env, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass* clazz = p1;
    jmethodID method = p2;

    MODULE_DEBUG_PRINTF("JNIEnv_CallStaticObjectMethod(%s,%s)\n",clazz->name,method->name);

    if (strcmp(method->name,"getProperty")==0) {
        //jstring property = va_arg(p3,jstring);
        //const char* prop = (*env)->
        //dummy_jobject* obj = malloc(sizeof(dummy_jobject));
        //return obj;
        return (*env)->NewStringUTF(env, GLOBAL_J(env)->apk_filename);
    }
    else
    if (strcmp(method->name,"getPackageCodePath")==0) {
        return (*env)->NewStringUTF(env, GLOBAL_J(env)->apk_filename);
    }

    return NULL;
}

jobject
JNIEnv_NewGlobalRef(JNIEnv* p0, jobject p1)
{
    MODULE_DEBUG_PRINTF("JNIEnv_NewGlobalRef(%x)\n", p1);
    if (p1==NULL) {
        struct dummy_jclass* cls = malloc(sizeof(struct dummy_jclass));
        cls->name = "null";

        dummy_jobject* obj = malloc(sizeof(dummy_jobject));
        obj->clazz = cls;
        obj->field = NULL;
        MODULE_DEBUG_PRINTF("JNIEnv_NewGlobalRef(%x) -> %x\n", p1, obj);
        return obj;
    }
    //dummy_jobject
    return NULL;
}




/**
    native calls
 */

typedef void (*unity_nativeInit_t)(JNIEnv* env, jobject p0, jint p1, jint p2);
typedef void (*unity_nativeFile_t)(JNIEnv* env, jobject p0, jstring p1);
typedef jboolean  (*unity_nativeRender_t)(JNIEnv* env, jobject p0);
typedef void (*unity_initJni_t)(JNIEnv* env, jobject p0);
typedef void (*unity_InitPlayerPrefs_t)(JNIEnv*, jobject p0);
typedef jboolean (*unity_androidinit_t)(JNIEnv*, jobject p0, jstring p1, jstring p2);
typedef void (*unity_androidpreparegameloop_t)(JNIEnv*, jobject);


/* -------- */

struct SupportModulePriv {
    jni_onload_t JNI_OnLoad_libunity;
    jni_onload_t JNI_OnLoad_libmono;
    unity_initJni_t initJni;
    unity_nativeInit_t nativeInit;
    unity_nativeFile_t nativeFile;
    unity_nativeRender_t nativeRender;
    unity_InitPlayerPrefs_t InitPlayerPrefs;
    unity_androidinit_t unityAndroidInit;
    unity_androidpreparegameloop_t unityAndroidPrepareGameLoop;
};
static struct SupportModulePriv unity_priv;

static int
unity_try_init(struct SupportModule *self)
{
    self->priv->JNI_OnLoad_libunity = (jni_onload_t)LOOKUP_LIBM("libunity","JNI_OnLoad");
    self->priv->JNI_OnLoad_libmono = (jni_onload_t)LOOKUP_LIBM("libmono","JNI_OnLoad");

    self->override_env.GetStaticFieldID = JNIEnv_GetStaticFieldID;
    self->override_env.GetStaticObjectField = JNIEnv_GetStaticObjectField;
    self->override_env.GetObjectClass = JNIEnv_GetObjectClass;
    self->override_env.CallObjectMethod = JNIEnv_CallObjectMethod;
    self->override_env.CallStaticObjectMethod = JNIEnv_CallStaticObjectMethod;
    self->override_env.RegisterNatives = JNIEnv_RegisterNatives;
    self->override_env.GetStringUTFChars = JNIEnv_GetStringUTFChars;
    self->override_env.NewGlobalRef = JNIEnv_NewGlobalRef;

    return (self->priv->JNI_OnLoad_libunity!=NULL);
}


#define UNITYPLAYER_CLASS_NAME "com/unity3d/player/UnityPlayer"
#define PLAYERPREFS_CLASS_NAME "com/unity3d/player/PlayerPrefs"
#define method_is(native) (strcmp(method->name,#native)==0)

jint
JNIEnv_RegisterNatives(JNIEnv* p0, jclass p1, const JNINativeMethod* p2, jint p3)
{
    MODULE_DEBUG_PRINTF("JNIEnv_RegisterNatives()\n");

    struct dummy_jclass *clazz = (struct dummy_jclass*)p1;
    MODULE_DEBUG_PRINTF("\n\tClass: %s\n", clazz->name);

    int is_unity_player = strcmp(clazz->name,UNITYPLAYER_CLASS_NAME)==0;
    int is_player_prefs = strcmp(clazz->name,PLAYERPREFS_CLASS_NAME)==0;

    int i=0;
    const JNINativeMethod *method = p2;
    while (i<p3) {
        MODULE_DEBUG_PRINTF("\tName: %-20s Sig: %-10s Addr: %x\n", method->name, method->signature, method->fnPtr);

        if (is_unity_player) {

            if (method_is(nativeInit)) { unity_priv.nativeInit = (unity_nativeInit_t)method->fnPtr;} else
            if (method_is(nativeFile)) { unity_priv.nativeFile = (unity_nativeFile_t)method->fnPtr;} else
            if (method_is(nativeRender)) { unity_priv.nativeRender = (unity_nativeRender_t)method->fnPtr;} else
            if (method_is(initJni)) { unity_priv.initJni = (unity_initJni_t)method->fnPtr;} else
            if (method_is(unityAndroidInit)) { unity_priv.unityAndroidInit = (unity_androidinit_t)method->fnPtr;} else
            if (method_is(unityAndroidPrepareGameLoop)) {unity_priv.unityAndroidPrepareGameLoop = (unity_androidpreparegameloop_t)method->fnPtr;} else
            ;
        } else if (is_player_prefs) {

            if (method_is(InitPlayerPrefs)) { unity_priv.InitPlayerPrefs = (unity_InitPlayerPrefs_t)method->fnPtr;} else
            ;
        }

        method++;
        i++;
    }
    MODULE_DEBUG_PRINTF("\n");

    return 0;
}

const char *
JNIEnv_GetStringUTFChars(JNIEnv *env, jstring string, jboolean *isCopy)
{
    MODULE_DEBUG_PRINTF("JNIEnv_GetStringUTFChars(%x)\n", string);
    if (string == GLOBAL_J(env)) {
        MODULE_DEBUG_PRINTF("WARNING: GetStringUTFChars on global\n");
        return NULL;
    }
    if (string == NULL) {
        return strdup("");
    }
    struct dummy_jstring *str = (struct dummy_jstring*)string;
    MODULE_DEBUG_PRINTF(" \\-> %s\n", str->data);
    return str->data;
}



static void
unity_init(struct SupportModule *self, int width, int height, const char *home)
{
    global = GLOBAL_M;

    system("export MALLOC_CHECK_=0"); //<unit? or who? does something weird on the pandora ...

    MODULE_DEBUG_PRINTF("JNI_OnLoad\n");
    self->priv->JNI_OnLoad_libunity(VM_M,0);
    MODULE_DEBUG_PRINTF("JNI_OnLoad done.\n");


    MODULE_DEBUG_PRINTF("nativeInit\n");
    self->priv->nativeInit(ENV_M,GLOBAL_M,width,height);
    MODULE_DEBUG_PRINTF("nativeInit done\n");

    MODULE_DEBUG_PRINTF("unityAndroidInit\n");
    jstring bin = GLOBAL_M->env->NewStringUTF(ENV_M,"assets/bin/");
    jstring lib = GLOBAL_M->env->NewStringUTF(ENV_M,"./lib/");//strcat(home,"lib/"));
    self->priv->unityAndroidInit(ENV_M,GLOBAL_M,bin,lib);
    MODULE_DEBUG_PRINTF("unityAndroidInitDone\n");


    MODULE_DEBUG_PRINTF("nativeFile\n");
    jstring file = GLOBAL_M->env->NewStringUTF(ENV_M,global->apk_filename);
    self->priv->nativeFile(ENV_M,GLOBAL_M,file);
    MODULE_DEBUG_PRINTF("nativeFile done\n");

}

static void
unity_input(struct SupportModule *self, int event, int x, int y, int finger)
{
}

static void
unity_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
}

static void
unity_update(struct SupportModule *self)
{
    if (self->priv->nativeRender) self->priv->nativeRender(ENV_M,GLOBAL_M);
}

static void
unity_deinit(struct SupportModule *self)
{
}

static void
unity_pause(struct SupportModule *self)
{
}

static void
unity_resume(struct SupportModule *self)
{
}

static int
unity_requests_exit(struct SupportModule *self)
{
    return 0;
}

APKENV_MODULE(unity, MODULE_PRIORITY_ENGINE)




