
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
 * Angry Birds Space support module 0.6 - By: Arto Rusanen
 *
 **/

#include "common.h"

#include <string.h>
#include <limits.h>
#include "SDL/SDL.h"
#include <assert.h>

// Typedefs. Got these from classes.dex (http://stackoverflow.com/questions/1249973/decompiling-dex-into-java-sourcecode)
typedef jboolean (*angrybirds_init_t)(JNIEnv *env, jobject obj, jint paramInt1, jint paramInt2, jstring paramString) SOFTFP;
typedef jboolean (*angrybirds_resize_t)(JNIEnv *env, jobject obj, jint width, jint height) SOFTFP;
typedef void (*angrybirds_input_t)(JNIEnv *env, jobject obj, jint action, jfloat x, jfloat y, jint finger) SOFTFP;
typedef void (*angrybirds_keyinput_t)(JNIEnv *env, jobject obj, jint keycode, jint is_down) SOFTFP;
typedef jboolean (*angrybirds_update_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*angrybirds_pause_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*angrybirds_resume_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*angrybirds_mixdata_t)(JNIEnv *env, jobject obj, jlong paramLong, jbyteArray paramArrayOfByte, jint paramInt) SOFTFP;
typedef void (*angrybirds_deinit_t)(JNIEnv *env, jobject obj) SOFTFP;


struct SupportModulePriv {
    jni_onload_t JNI_OnLoad;
    angrybirds_init_t native_init;
    angrybirds_resize_t native_resize;
    angrybirds_update_t native_update;
    angrybirds_input_t native_input;
    angrybirds_keyinput_t native_keyinput;
    angrybirds_pause_t native_pause;
    angrybirds_resume_t native_resume;
    angrybirds_mixdata_t native_mixdata;
    angrybirds_deinit_t native_deinit;
    const char *myHome;
    int want_exit;
};
static struct SupportModulePriv angrybirds_priv;

/* Audio specs and handle */
SDL_AudioSpec *desired, *obtained;
jlong audioHandle;

/* Global application state so we can call this from override thingie */
struct GlobalState *global;

/* Fill audio buffer */
void my_audio_callback(void *ud, Uint8 *stream, int len)
{
    angrybirds_priv.native_mixdata(ENV(global), VM(global), audioHandle, stream, len);
}


/* CallVoidMethodV override. Signal when to start or stop audio */
void
JNIEnv_CallVoidMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("module_JNIEnv_CallVoidMethodV(%x, %s, %s)\n", p1, p2->name, p2->sig);
    if (strcmp(p2->name, "startOutput") == 0)
    {
        MODULE_DEBUG_PRINTF("Start audio Output\n");
        SDL_PauseAudio(0);
    }
    if (strcmp(p2->name, "stopOutput") == 0)
    {
        // Stop output :)
        MODULE_DEBUG_PRINTF("Stop audio Output\n");
        SDL_PauseAudio(1);
    }
}

/* NewObjectV override. Initialize audio output */
jobject
JNIEnv_NewObjectV(JNIEnv *env, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *clazz = p1;
    MODULE_DEBUG_PRINTF("module_JNIEnv_NewObjectV(%x, %s, %s)\n", p1, p2->name, clazz->name);
    if (strcmp(clazz->name, "com/rovio/ka3d/AudioOutput") == 0)
    {
        /* Open the audio device */
        desired = malloc(sizeof(SDL_AudioSpec));
        obtained = malloc(sizeof(SDL_AudioSpec));

        audioHandle = va_arg(p3, jlong);

        desired->freq = va_arg(p3, int);
        desired->channels = va_arg(p3, int);
        desired->format = AUDIO_S16SYS;
        jint bitrate = va_arg(p3, int);
        desired->samples = va_arg(p3, int) / 8;
        desired->callback=my_audio_callback;
        desired->userdata=NULL;
        MODULE_DEBUG_PRINTF("Module: Handle: %lld, freq: %i, channels: %i, bitrate: %i, samples: %i\n",
            audioHandle,desired->freq,desired->channels,bitrate,desired->samples);

        assert( SDL_OpenAudio(desired, obtained) == 0 );
        free(desired);
    }

    return GLOBAL_J(env);
}

/* CallObjectMethodV override. AB calls readFile to read data from apk */
jobject
JNIEnv_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("module_JNIEnv_CallObjectMethodV(%x, %s, %s, ...)\n", p1, p2->name, p2->sig);
    if (strcmp(p2->name, "readFile") == 0)
    {
	SDL_Event e; 
	SDL_PollEvent(&e); //fast hack to prevent "not responding" message when game starts
        struct dummy_jstring *str = va_arg(p3,struct dummy_jstring*);
        char tmp[PATH_MAX];
        strcpy(tmp, "assets/");
        strcat(tmp, str->data);

#ifdef PANDORA
        if (strcmp(str->data,"data/bundleIndex.idx")==0) //ignore this file, it segfaults but the games still work afterwards
            return NULL;
#endif

        size_t file_size;
        struct dummy_byte_array *result = malloc(sizeof(struct dummy_byte_array));

        global->read_file(tmp, &result->data, &file_size);

        result->size = file_size;
        MODULE_DEBUG_PRINTF("angrybirds_readFile: %s -> %x\n", str->data, result);
        return result;
    }
    return NULL;
}

/* DeleteLocalRef override. Free some memory :) */
void
JNIEnv_DeleteLocalRef(JNIEnv* p0, jobject p1)
{
    MODULE_DEBUG_PRINTF("JNIEnv_DeleteLocalRef(%x)\n", p1);
    if (p1 == GLOBAL_J(p0) || p1 == NULL) {
        MODULE_DEBUG_PRINTF("WARNING: DeleteLocalRef on global\n");
        return;
    }
    free(p1);
}


jsize
JNIEnv_GetArrayLength(JNIEnv* env, jarray p1)
{
    MODULE_DEBUG_PRINTF("JNIEnv_GetArrayLength(%x)\n", p1);

    if (p1 != GLOBAL_J(env)) {

        //if(access_ok())

        struct dummy_byte_array *array = p1;
        MODULE_DEBUG_PRINTF("JNIEnv_GetArrayLength(%x) -> %d\n", p1, array->size);
        return array->size;
    }
    return 0;
}


static int
angrybirds_try_init(struct SupportModule *self)
{
    self->priv->native_init = (angrybirds_init_t)LOOKUP_M("ka3d_MyRenderer_nativeInit");
    self->priv->native_resize = (angrybirds_resize_t)LOOKUP_M("ka3d_MyRenderer_nativeResize");
    self->priv->native_input = (angrybirds_input_t)LOOKUP_M("ka3d_MyRenderer_nativeInput");
    self->priv->native_keyinput = (angrybirds_keyinput_t)LOOKUP_M("ka3d_MyRenderer_nativeKeyInput");
    self->priv->native_update = (angrybirds_update_t)LOOKUP_M("ka3d_MyRenderer_nativeUpdate");
    self->priv->native_pause = (angrybirds_pause_t)LOOKUP_M("ka3d_MyRenderer_nativePause");
    self->priv->native_resume = (angrybirds_resume_t)LOOKUP_M("ka3d_MyRenderer_nativeResume");
    self->priv->native_mixdata = (angrybirds_mixdata_t)LOOKUP_M("ka3d_AudioOutput_nativeMixData");
    self->priv->native_deinit = (angrybirds_deinit_t)LOOKUP_M("ka3d_MyRenderer_nativeDeinit");

    /* Overrides for JNIEnv_ */
    self->override_env.CallObjectMethodV = JNIEnv_CallObjectMethodV;
    self->override_env.DeleteLocalRef = JNIEnv_DeleteLocalRef;
    self->override_env.CallVoidMethodV = JNIEnv_CallVoidMethodV;
    self->override_env.NewObjectV = JNIEnv_NewObjectV;
    self->override_env.GetArrayLength = JNIEnv_GetArrayLength;

    return (self->priv->native_init != NULL &&
            self->priv->native_resize != NULL &&
            self->priv->native_input != NULL &&
            self->priv->native_keyinput != NULL &&
            self->priv->native_update != NULL &&
            self->priv->native_pause != NULL &&
            self->priv->native_resume != NULL &&
            self->priv->native_mixdata != NULL &&
            self->priv->native_deinit != NULL);
}

static void
angrybirds_init(struct SupportModule *self, int width, int height, const char *home)
{
    MODULE_DEBUG_PRINTF("Module: Init(%i,%i,%s)\n",width,height,home);

    self->priv->myHome = strdup(home);
    global = GLOBAL_M;
    self->priv->native_init(ENV_M, GLOBAL_M, width, height, GLOBAL_M->env->NewStringUTF(ENV_M, home));
}

static void
angrybirds_input(struct SupportModule *self, int event, int x, int y, int finger)
{
    self->priv->native_input(ENV_M, GLOBAL_M, event, x, y, finger);
}

static void
angrybirds_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
    self->priv->native_keyinput(ENV_M, GLOBAL_M, keycode, event == ACTION_DOWN);
}

static void
angrybirds_update(struct SupportModule *self)
{
    if (self->priv->native_update(ENV_M, GLOBAL_M) == 0)
        self->priv->want_exit = 1;
}

static void
angrybirds_deinit(struct SupportModule *self)
{
    self->priv->native_deinit(ENV_M, GLOBAL_M);
    SDL_CloseAudio();
}

static void
angrybirds_pause(struct SupportModule *self)
{
    self->priv->native_pause(ENV_M, GLOBAL_M);
}

static void
angrybirds_resume(struct SupportModule *self)
{
    self->priv->native_resume(ENV_M, GLOBAL_M);
}

static int
angrybirds_requests_exit(struct SupportModule *self)
{
    return self->priv->want_exit;
}

APKENV_MODULE(angrybirds, MODULE_PRIORITY_GAME)

