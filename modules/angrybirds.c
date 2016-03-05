
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
#include "../audio/audio.h"

#include <string.h>
#include <limits.h>
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
jlong audioHandle;

/* Global application state so we can call this from override thingie */
struct GlobalState *global;

/* Fill audio buffer */
void my_audio_callback(void *ud, void *stream, int len)
{
    JNIEnv *thread_env;
    JNIEnv ref_env;

    jarray *array;
    (*VM(global))->AttachCurrentThread(VM(global), &thread_env, NULL);

    /* here we need the original NewGlobalRef */
    if(global->use_dvm)
    {
        ref_env = &global->dalvik_copy_env;
    }
    else ref_env = *thread_env;

    array = (*thread_env)->NewShortArray(thread_env, len / 2);

    jobject *ref = ref_env->NewGlobalRef(thread_env, array);
    angrybirds_priv.native_mixdata(ENV(global), VM(global), audioHandle, ref, len);
    ref_env->DeleteGlobalRef(thread_env, ref);

    jshort *elements = (*thread_env)->GetShortArrayElements(thread_env, array, 0);
    memcpy(stream, elements, len);
    (*thread_env)->ReleaseShortArrayElements(thread_env, array, elements, JNI_ABORT);

    (*thread_env)->DeleteLocalRef(thread_env, array);

    (*VM(global))->DetachCurrentThread(VM(global));
}


/* CallVoidMethodV override. Signal when to start or stop audio */
void
angrybirds_jnienv_CallVoidMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("module_angrybirds_jnienv_CallVoidMethodV(%x, %s, %s)\n", p1, p2->name, p2->sig);
    if (strcmp(p2->name, "startOutput") == 0)
    {
        MODULE_DEBUG_PRINTF("Start audio Output\n");
        apkenv_audio_play();
    }
    if (strcmp(p2->name, "stopOutput") == 0)
    {
        // Stop output :)
        MODULE_DEBUG_PRINTF("Stop audio Output\n");
        apkenv_audio_pause();
    }
}

/* NewObjectV override. Initialize audio output */
jobject
angrybirds_jnienv_NewObjectV(JNIEnv *env, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *clazz = p1;
    MODULE_DEBUG_PRINTF("module_angrybirds_jnienv_NewObjectV(%x, %s, %s)\n", p1, p2->name, clazz->name);
    if (strcmp(clazz->name, "com/rovio/ka3d/AudioOutput") == 0)
    {
        /* Open the audio device */
        audioHandle = va_arg(p3, jlong);

        int freq = va_arg(p3, int);
        enum AudioFormat format = AudioFormat_S16SYS;
        int channels = va_arg(p3, int);
        jint bitrate = va_arg(p3, int);
        int samples = va_arg(p3, int) / 8;

        apkenv_audio_open(freq, format, channels, samples, my_audio_callback, NULL);
    }

    if(global->use_dvm) return 0x1;
    return GLOBAL_J(env);
}

/* CallObjectMethodV override. AB calls readFile to read data from apk */
jobject
angrybirds_jnienv_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("module_angrybirds_jnienv_CallObjectMethodV(%x, %s, %s, ...)\n", p1, p2->name, p2->sig);
    if (strcmp(p2->name, "readFile") == 0)
    {
        // Process input to prevent "not responding" message when game starts
        global->platform->input_update(global->active_module);

        char *str_data = dup_jstring(global, va_arg(p3, jstring*));
        char tmp[PATH_MAX];
        strcpy(tmp, "assets/");
        strcat(tmp, str_data);

#ifdef PANDORA
        if (strcmp(str_data,"data/bundleIndex.idx")==0) //ignore this file, it segfaults but the games still work afterwards
            return NULL;
#endif

        jarray *result = global->read_file_to_jni_array(tmp);

        MODULE_DEBUG_PRINTF("angrybirds_readFile: %s -> %x\n", str_data, result);
        free(str_data);
        return result;
    }
    else if (strcmp(p2->name, "getUniqueIdHash") == 0)
    {
        return (*env)->NewStringUTF(env, "");
    }
    return NULL;
}

/* DeleteLocalRef override. Free some memory :) */
void
angrybirds_jnienv_DeleteLocalRef(JNIEnv* p0, jobject p1)
{
    if(!global->use_dvm)
    {
        MODULE_DEBUG_PRINTF("angrybirds_jnienv_DeleteLocalRef(%x)\n", p1);
        if (p1 == GLOBAL_J(p0) || p1 == NULL) {
            MODULE_DEBUG_PRINTF("WARNING: DeleteLocalRef on global\n");
            return;
        }
        free(p1);
    }
    else
    {
        global->dalvik_copy_env.DeleteLocalRef(p0, p1);
    }
}

jobjectArray
angrybirds_jnienv_NewObjectArray(JNIEnv* p0, jsize p1, jclass p2, jobject p3)
{
    MODULE_DEBUG_PRINTF("angrybirds_jnienv_NewObjectArray()\n");
    return NULL;
}

jobject
angrybirds_jnienv_CallStaticObjectMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    MODULE_DEBUG_PRINTF("angrybirds_jnienv__CallStaticObjectMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return NULL;
}

void
angrybirds_jnienv_CallStaticVoidMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    MODULE_DEBUG_PRINTF("angrybirds_jnienv__CallStaticVoidMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
}

jboolean
angrybirds_jnienv_CallBooleanMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("angrybirds_jnienv__CallBooleanMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
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

    /* Overrides for angrybirds_jnienv_ */
    self->override_env.CallObjectMethodV = angrybirds_jnienv_CallObjectMethodV;
    self->override_env.DeleteLocalRef = angrybirds_jnienv_DeleteLocalRef;
    self->override_env.CallBooleanMethodV = angrybirds_jnienv_CallBooleanMethodV;
    self->override_env.CallStaticObjectMethodV = angrybirds_jnienv_CallStaticObjectMethodV;
    self->override_env.CallStaticVoidMethodV = angrybirds_jnienv_CallStaticVoidMethodV;
    self->override_env.NewObjectArray = angrybirds_jnienv_NewObjectArray;
    self->override_env.CallVoidMethodV = angrybirds_jnienv_CallVoidMethodV;
    self->override_env.NewObjectV = angrybirds_jnienv_NewObjectV;

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

    global = GLOBAL_M;

    global->module_hacks->current_orientation = ORIENTATION_LANDSCAPE;
    global->module_hacks->glDrawArrays_rotation_hack = 1;
    global->module_hacks->gles_viewport_hack = 1;

    self->priv->myHome = strdup(home);

    if(GLOBAL_M->platform->get_orientation() == ORIENTATION_LANDSCAPE) {
        self->priv->native_init(ENV_M, GLOBAL_M, width, height, GLOBAL_M->env->NewStringUTF(ENV_M, home));
    }
    else {
        self->priv->native_init(ENV_M, GLOBAL_M, height, width, GLOBAL_M->env->NewStringUTF(ENV_M, home));
    }
}

static int first_finger = -1;

static void
angrybirds_input(struct SupportModule *self, int event, int x, int y, int finger)
{
    /* make sure first touch input is always finger == 0, seems to be required */
    if(event == ACTION_DOWN)
    {
        if(first_finger == -1)
        {
            first_finger = finger;
        }
    }

    self->priv->native_input(ENV_M, GLOBAL_M, event, x, y, (first_finger == finger) ? 0 : finger);

    if(event == ACTION_UP)
    {
        if(first_finger == finger)
        {
            first_finger = -1;
        }
    }
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
    apkenv_audio_close();
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

