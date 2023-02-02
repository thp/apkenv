
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
 * Fruit Ninja support module 0.5 - By: Arto Rusanen
 *
 **/

#define MAX_SOUNDS  150

#include "common.h"
#include "../mixer/mixer.h"

#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>

// Typedefs. Got these from classes.dex (http://stackoverflow.com/questions/1249973/decompiling-dex-into-java-sourcecode)

typedef void (*fruitninja_init_t)(JNIEnv *env, jobject obj, jint paramInt1, jint paramInt2, jstring paramString) SOFTFP;
typedef void (*fruitninja_touchevent_t)(JNIEnv *env, jobject obj, jint action, jlong eventTime, jint pointerId, jfloat x, jfloat y, jfloat Pressure, jfloat size) SOFTFP;
typedef void (*fruitninja_keyevent_t)(JNIEnv *env, jobject obj, jint keycode, jboolean isDown, jboolean cancelled) SOFTFP;
typedef jboolean (*fruitninja_step_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*fruitninja_pause_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*fruitninja_resume_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*fruitninja_saveonexit_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef jboolean (*fruitninja_gamerequestedquit_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*fruitninja_setapplicensed_t)(JNIEnv *env, jobject obj, jboolean value) SOFTFP;
typedef void (*fruitninja_initfilemanager_t)(JNIEnv *env, jobject obj, jstring paramString1, jstring paramString2, jboolean paramBoolean) SOFTFP;
typedef void (*fruitninja_initjavasoundmanager_t)(JNIEnv *env, jobject obj) SOFTFP;

struct SupportModulePriv {
    jni_onload_t JNI_OnLoad;
    fruitninja_init_t native_init;
    fruitninja_touchevent_t native_touchevent;
    fruitninja_keyevent_t native_keyevent;
    fruitninja_step_t native_step;
    fruitninja_pause_t native_pause;
    fruitninja_resume_t native_resume;
    fruitninja_saveonexit_t native_saveonexit;
    fruitninja_gamerequestedquit_t native_gamerequestedquit;
    fruitninja_setapplicensed_t native_setapplicensed;
    fruitninja_initfilemanager_t native_initfilemanager;
    fruitninja_initjavasoundmanager_t native_initjavasoundmanager;
    jmethodID isNetworkConnected;
    const char *myHome;
};
static struct SupportModulePriv fruitninja_priv;

/* Global application state so we can call this from override thingie */
struct GlobalState *global;

typedef struct
{
    char* name;
    struct MixerSound *sound;
} SFXInfo;

SFXInfo SFX[MAX_SOUNDS+1]; //<bugfix, MAX_SOUNDS loops are <= (crow_riot)
int SoundCount = 0;

//The music that will be played
struct MixerMusic *music = NULL;
char Music_Path[PATH_MAX];

/* Load all SFX into memory */
void
load_sound_callback(const char *filename, char *buffer, size_t size)
{
    char* fname = strrchr (filename, '/') + 1;
    MODULE_DEBUG_PRINTF("module: Load sound: %s\n", fname);
    SFX[SoundCount].name = strdup(fname);

    SFX[SoundCount].sound = apkenv_mixer_load_sound_buffer(buffer, size);
    SoundCount++;
}

jobject
fruitninja_jnienv_CallStaticObjectMethodV(JNIEnv*env, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;

    if( strcmp( p2->name, "SFXPlayInternal" ) == 0 )
    {
        char *soundname = NULL;

        soundname = dup_jstring(global, va_arg(p3, jstring*));

        int i;
        for(i = 0; soundname[i]; i++) {
            soundname[i] = tolower(soundname[i]); // LowerCase soundname
        }

        for(i=0; i <= MAX_SOUNDS; i++)
        {
            if ( SFX[i].sound == NULL ) break;
            if ( memcmp(SFX[i].name, soundname, strlen(soundname)) == 0 ) {
                /* Play some sweet sounds? */
                MODULE_DEBUG_PRINTF("module: Play sound: %s\n", SFX[i].name);
                apkenv_mixer_play_sound(SFX[i].sound, 0);
                free(soundname);
                return NULL;
            }
        }
        MODULE_DEBUG_PRINTF("module: Sound not found: %s\n", soundname);

        free(soundname);

    }
    else
    if (strcmp(p2->name,"GetPackageName")==0){
        return (*env)->NewStringUTF(env,global->apk_filename);
    }
    else
    {
        MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_CallStaticObjectMethodV(%s, %s, %s)\n",
            jcl->name, p2->name, p2->sig);
    }

    return NULL;
}

/* Backround music support. I didn't find documentation about playing
   music from memory buffer. So extract music and play it from there */
void
extract_files_callback(const char *filename, char *buffer, size_t size)
{
    char* fname = strrchr (filename, '/') + 1;

    char tmp[PATH_MAX];
    strcpy(tmp, fruitninja_priv.myHome);
    strcat(tmp, fname);

    MODULE_DEBUG_PRINTF("module: Extract file to: %s\n", tmp);

    global->recursive_mkdir(tmp);

    FILE *my_file = fopen(tmp, "wb");
    fwrite(buffer, size, 1, my_file);
    fclose(my_file);
}

void
fruitninja_jnienv_CallStaticVoidMethodV(JNIEnv* env, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;

    MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_CallStaticVoidMethodV(%x, %s, %s)\n", jcl->name, p2->name, p2->sig);


    if( strcmp( p2->name, "SongPlay" ) == 0 ) // Play some sweet backround music?
    {
        char *musicname = NULL;

        musicname = dup_jstring(global, va_arg(p3, jstring*));

        if (music) {
            apkenv_mixer_stop_music(music);
            apkenv_mixer_free_music(music);
        }

        int i;
        for(i = 0; musicname[i]; i++) {
            musicname[i] = tolower(musicname[i]); // LowerCase musicname
        }

        strcpy(Music_Path, fruitninja_priv.myHome);
        strcat(Music_Path, musicname);
        strcat(Music_Path, ".ogg");

        MODULE_DEBUG_PRINTF("module: Play music: %s\n", Music_Path);

        music = apkenv_mixer_load_music(Music_Path);
        apkenv_mixer_play_music(music, 1);

        free(musicname);
    } else if( strcmp( p2->name, "SetMusicVolume" ) == 0 ){
        double musicvol = va_arg(p3, double);
        MODULE_DEBUG_PRINTF("module: SetMusicVolume: %.2f\n", musicvol);
        apkenv_mixer_volume_music(music, musicvol);
    } else if( strcmp( p2->name, "SetSFXVolume" ) == 0 ){
        double soundvol = va_arg(p3, double);
        int i;
        MODULE_DEBUG_PRINTF("module: SetSFXVolume: %.2f\n", soundvol);
        for(i=0; i <= MAX_SOUNDS; i++)
        {
            if ( SFX[i].sound == NULL ) break;
            apkenv_mixer_volume_sound(SFX[i].sound, soundvol);
        }
    } else {
        MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_CallStaticVoidMethodV(%x, %s, %s)\n", jcl->name, p2->name, p2->sig);
    }
}

/* Overrides to fix SegFault on end of classic mode (dirty hack?) */
jclass
fruitninja_jnienv_GetObjectClass(JNIEnv* env, jobject p1)
{
    MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_GetObjectClass(%x, %x)\n", env, p1);
    struct dummy_jclass *class = malloc(sizeof(struct dummy_jclass));
    class->name = strdup(p1);
    return class;
}
jobject
fruitninja_jnienv_NewObjectV(JNIEnv *env, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *clazz = p1;
    MODULE_DEBUG_PRINTF("fruitninja_jnienv_NewObjectV(%x, %s, %s)\n", p1, p2->name, clazz->name);
    return NULL;
}
jfieldID
fruitninja_jnienv_GetFieldID(JNIEnv* p0, jclass p1, const char* p2, const char* p3)
{
    struct dummy_jclass *jcl = p1;
    MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_GetFieldID(%s, %s, %s)\n", jcl->name, p2, p3);
    return 0;
}

/* Following overrides are just to suppress some not important debug messages... */
jclass
fruitninja_jnienv_FindClass(JNIEnv* p0, const char* p1)
{
    if( !strcmp( p1, "com/openfeint/api/OpenFeint" ) == 0 )
        MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_FindClass('%s')\n", p1);
    struct dummy_jclass *class = malloc(sizeof(struct dummy_jclass));
    class->name = strdup(p1);
    return class;
}
jmethodID
fruitninja_jnienv_GetStaticMethodID(JNIEnv* p0, jclass clazz, const char* name, const char* sig)
{
    if (strcmp(name,"isNetworkConnected")==0) {
        if (fruitninja_priv.isNetworkConnected==0) {
            fruitninja_priv.isNetworkConnected = malloc(sizeof(struct _jmethodID));
            fruitninja_priv.isNetworkConnected->clazz = clazz;
            fruitninja_priv.isNetworkConnected->name = strdup(name);
            fruitninja_priv.isNetworkConnected->sig = strdup(sig);
        }
        return fruitninja_priv.isNetworkConnected;
    }
    MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_GetStaticMethodID(%x, %s, %s)\n", clazz, name, sig);
    jmethodID id = malloc(sizeof(struct _jmethodID));
    id->clazz = clazz;
    id->name = strdup(name);
    id->sig = strdup(sig);
    return id;
}
jboolean
fruitninja_jnienv_CallStaticBooleanMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    //struct dummy_jclass *clazz = (struct dummy_jclass*)p1;
    if( strcmp( p2->name, "isNetworkConnected" ) != 0 ) {
        MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_CallStaticBooleanMethodV(%x, %s, %s)\n", p2->clazz, p2->name, p2->sig);
    }
    return 0;
}
jint
fruitninja_jnienv_CallStaticIntMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_CallStaticIntMethodMethodV(%x, %s, %s)\n", p2->clazz, p2->name, p2->sig);
    if (strcmp(p2->name,"GetWifi")==0) return 0;
    if (strcmp(p2->name,"GetTouchscreenCapabilities")==0) return 0;
    return 0;
}
void
fruitninja_jnienv_ExceptionClear(JNIEnv* p0)
{
    //MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_ExceptionClear()\n");
}

jthrowable
fruitninja_jnienv_ExceptionOccurred(JNIEnv* p0)
{
    //MODULE_DEBUG_PRINTF("module_fruitninja_jnienv_ExceptionOccurred()\n");
    return NULL;
}

static int
fruitninja_try_init(struct SupportModule *self)
{
    // Java_com_halfbrick_fruitninja_ < prefix in 1.5.4
    // Java_com_halfbrick_mortar_ < prefix in 1.7.6

    //self->priv->JNI_OnLoad = (jni_onload_t)LOOKUP_M("JNI_OnLoad"); //No JNI_Onload function in fruitninja
    self->priv->native_init = (fruitninja_init_t)LOOKUP_M("_NativeGameLib_native_1init");
    self->priv->native_touchevent = (fruitninja_touchevent_t)LOOKUP_M("_NativeGameLib_native_1touchEvent");
    self->priv->native_keyevent = (fruitninja_keyevent_t)LOOKUP_M("_NativeGameLib_native_1keyEvent");
    self->priv->native_step = (fruitninja_step_t)LOOKUP_M("_NativeGameLib_native_1step");
    self->priv->native_pause = (fruitninja_pause_t)LOOKUP_M("_NativeGameLib_native_1onPause");
    self->priv->native_resume = (fruitninja_resume_t)LOOKUP_M("_NativeGameLib_native_1onResume");
    self->priv->native_saveonexit = (fruitninja_saveonexit_t)LOOKUP_M("_NativeGameLib_native_1saveOnExit");
    self->priv->native_gamerequestedquit = (fruitninja_gamerequestedquit_t)LOOKUP_M("_NativeGameLib_native_1gameRequestedQuit");
    self->priv->native_setapplicensed = (fruitninja_setapplicensed_t)LOOKUP_M("_NativeGameLib_native_1SetAppLicensed");
    self->priv->native_initfilemanager = (fruitninja_initfilemanager_t)LOOKUP_M("_NativeGameLib_native_1InitFileManager");
    self->priv->native_initjavasoundmanager = (fruitninja_initjavasoundmanager_t)LOOKUP_M("_NativeGameLib_native_1InitJavaSoundManager");
    self->priv->isNetworkConnected = 0;

    /* override for fruitninja_jnienv_ */
    self->override_env.CallStaticObjectMethodV = fruitninja_jnienv_CallStaticObjectMethodV;
    self->override_env.GetObjectClass = fruitninja_jnienv_GetObjectClass;
    self->override_env.NewObjectV = fruitninja_jnienv_NewObjectV;
    self->override_env.GetFieldID = fruitninja_jnienv_GetFieldID;
    self->override_env.FindClass = fruitninja_jnienv_FindClass;
    self->override_env.GetStaticMethodID = fruitninja_jnienv_GetStaticMethodID;
    self->override_env.ExceptionClear = fruitninja_jnienv_ExceptionClear;
    self->override_env.CallStaticBooleanMethodV = fruitninja_jnienv_CallStaticBooleanMethodV;
    self->override_env.CallStaticVoidMethodV = fruitninja_jnienv_CallStaticVoidMethodV;
    self->override_env.ExceptionOccurred = fruitninja_jnienv_ExceptionOccurred;
    self->override_env.CallStaticIntMethodV = fruitninja_jnienv_CallStaticIntMethodV;


    return (//self->priv->JNI_OnLoad != NULL &&
            self->priv->native_init != NULL &&
            self->priv->native_touchevent != NULL &&
            self->priv->native_keyevent != NULL &&
            self->priv->native_step != NULL &&
            self->priv->native_pause != NULL &&
            self->priv->native_resume != NULL &&
            self->priv->native_saveonexit != NULL &&
            self->priv->native_gamerequestedquit != NULL &&
            self->priv->native_setapplicensed != NULL &&
            self->priv->native_initfilemanager != NULL);
}

static void
fruitninja_init(struct SupportModule *self, int width, int height, const char *home)
{
    global = GLOBAL_M;
    global->module_hacks->current_orientation = ORIENTATION_LANDSCAPE;

    self->priv->myHome = strdup(home);

    apkenv_mixer_open(22050, AudioFormat_S16SYS, 2, 1024);

    /* Load sounds */
    global->foreach_file("assets/sound", load_sound_callback);

    /* Extract files */
    global->foreach_file("assets/music", extract_files_callback);

#ifdef PANDORA
    sync();
#endif


    jstring jSource = GLOBAL_M->env->NewStringUTF(ENV_M, global->apk_filename); //APK path & filename
    jstring jHome = GLOBAL_M->env->NewStringUTF(ENV_M, home); // Save file location
    self->priv->native_initfilemanager(ENV_M, GLOBAL_M, jSource, jHome, 0);
    if (self->priv->native_initjavasoundmanager!=0)
        self->priv->native_initjavasoundmanager(ENV_M, GLOBAL_M);
    self->priv->native_init(ENV_M, GLOBAL_M, width, height, GLOBAL_M->env->NewStringUTF(ENV_M, "en"));
    self->priv->native_setapplicensed(ENV_M, GLOBAL_M, 1);
}

static void
fruitninja_input(struct SupportModule *self, int event, int x, int y, int finger)
{
    self->priv->native_touchevent(ENV_M, GLOBAL_M, event, 0, finger, x, y, 1, 0);
}

static void
fruitninja_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
    self->priv->native_keyevent(ENV_M, GLOBAL_M, keycode, event == ACTION_DOWN, 0);
}

static void
fruitninja_update(struct SupportModule *self)
{
    self->priv->native_step(ENV_M, GLOBAL_M);
}

static void
fruitninja_deinit(struct SupportModule *self)
{
    if (music) {
        apkenv_mixer_stop_music(music);
        apkenv_mixer_free_music(music);
    }

    int i;
    for(i=0; i <= MAX_SOUNDS; i++)
    {
        if ( SFX[i].sound == NULL ) break;
        apkenv_mixer_free_sound(SFX[i].sound);
    }
    apkenv_mixer_close();
}

static void
fruitninja_pause(struct SupportModule *self)
{
    self->priv->native_pause(ENV_M, GLOBAL_M);
}

static void
fruitninja_resume(struct SupportModule *self)
{
    self->priv->native_resume(ENV_M, GLOBAL_M);
}

static int
fruitninja_requests_exit(struct SupportModule *self)
{
    if (self->priv->native_gamerequestedquit(ENV_M,GLOBAL_M))
        return 1;
    return 0;
}

APKENV_MODULE(fruitninja, MODULE_PRIORITY_GAME)

