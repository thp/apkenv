
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

#include <string.h>
#include "common.h"
#include <pthread.h>
#include <signal.h>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#include "../platform.h"

#define ORIENTATION_LANDSCAPE 2
#define ORIENTATION_PORTRAIT 1

#define AUDIO_RATE 22050
#define AUDIO_CHUKSIZE 1024
#define AUDIO_CHANNELS 2

// TODO: to jni/jnienv.h?
typedef struct
{
    jclass clazz;
    jfieldID field;

} dummy_jobject;

// LoaderThread
typedef void (*marmalade_initNative_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*marmalade_shutdownNative_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*marmalade_suspendAppThreads_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*marmalade_resumeAppThreads_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*marmalade_onAccelNative_t)(JNIEnv *env, jobject p0, jfloat f1, jfloat f2, jfloat f3) SOFTFP;
typedef void (*marmalade_onCompassNative_t)(JNIEnv *env, jobject p0, jint i1, jfloat f2, jfloat f3, jfloat f4) SOFTFP;
typedef void (*marmalade_onMotionEvent_t)(JNIEnv *env, jobject p0, jint i1, jint i2, jint i3, jint i4) SOFTFP;
typedef void (*marmalade_setViewNative_t)(JNIEnv *env, jobject p0, jobject loaderview_o) SOFTFP;
typedef void (*marmalade_runNative_t)(JNIEnv* env, jobject p0, jstring s1, jstring s2) SOFTFP;
typedef void (*marmalade_audioStoppedNotify_t)(JNIEnv *env, jobject p0, jint i1) SOFTFP;
typedef void (*marmalade_chargerStateChanged_t)(JNIEnv *env, jobject p0, jboolean b1) SOFTFP;
typedef void (*marmalade_networkCheckChanged_t)(JNIEnv *env, jobject p0, jboolean b1) SOFTFP;
typedef void (*marmalade_runOnOSThreadNative_t)(JNIEnv *env, jobject p0, jobject runnable_o) SOFTFP;
typedef void (*marmalade_runOnOSTickNative_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*marmalade_signalSuspend_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*marmalade_signalResume_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*marmalade_setSuspended_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*marmalade_setResumed_t)(JNIEnv *env, jobject p0) SOFTFP;

// LoaderView
typedef void (*marmalade_setPixelsNative_t)(JNIEnv *env, jobject p0, jint i1, jint i2, jarray a3 /* of jint */, jboolean b5) SOFTFP;
typedef void (*marmalade_videoStoppedNotify_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*marmalade_setInputText_t)(JNIEnv *env, jobject p0, jstring s1) SOFTFP;

// LoaderKeyboard
typedef jboolean (*marmalade_onKeyEventNative_t)(JNIEnv *env, jobject p0, jint i1, jint i2, jint i3) SOFTFP;
typedef void (*marmalade_setCharInputEnabledNative_t)(JNIEnv *env, jobject p0, jboolean b1) SOFTFP;

// SoundPlayer
typedef void (*marmalade_generateAudio_t)(JNIEnv *env, jobject p0, jshortArray a1, jint length) SOFTFP;

// SoundRecord
typedef void (*marmalade_recordAudio_t)(JNIEnv *env, jobject p0, jarray a1 /* of jstring */, jint i3, jint i4) SOFTFP;

// LoaderSMSReceiver
typedef void (*marmalade_onReceiveCallback_t)(JNIEnv *env, jobject p0, jstring s1, jstring s2) SOFTFP;

// LoaderAPI
typedef void (*marmalade_s3eDebugTraceLine_t)(JNIEnv *env, jobject p0, jstring s1) SOFTFP;
typedef void (*marmalade_s3eDeviceYield_t)(JNIEnv *env, jobject p0, jint i1) SOFTFP;
typedef jint (*marmalade_s3eConfigGet_t)(JNIEnv *env, jobject p0, jstring s1, jint i2) SOFTFP;
typedef jint (*marmalade_s3eConfigGetInt_t)(JNIEnv *env, jobject p0, jstring s1, jstring s2, jarray a3 /* of jint*/) SOFTFP;

// LoaderLocation
typedef void (*marmalade_locationUpdate_t)(JNIEnv *env, jobject p0, jint i1, jlong j2, jdouble d3, jdouble d4, jdouble d5, jfloat f6, jfloat f7, jfloat f8) SOFTFP;
typedef void (*marmalade_locationSatellite_t)(JNIEnv *env, jobject p0, jint i1, jfloat f3, jfloat f4, jint i5, jfloat f6, jboolean b8) SOFTFP;

struct marmalade_loaderthread_t
{
    marmalade_shutdownNative_t shutdownNative;
    marmalade_initNative_t initNative;
    marmalade_suspendAppThreads_t suspendAppThreads;
    marmalade_resumeAppThreads_t resumeAppThreads;
    marmalade_onAccelNative_t onAccelNative;
    marmalade_onCompassNative_t onCompassNative;
    marmalade_onMotionEvent_t onMotionEvent;
    marmalade_setViewNative_t setViewNative;
    marmalade_runNative_t runNative;
    marmalade_audioStoppedNotify_t audioStoppedNotify;
    marmalade_chargerStateChanged_t chargerStateChanged;
    marmalade_networkCheckChanged_t networkCheckChanged;
    marmalade_runOnOSThreadNative_t runOnOSThreadNative;
    marmalade_runOnOSTickNative_t runOnOSTickNative;
    marmalade_signalSuspend_t signalSuspend;
    marmalade_signalResume_t signalResume;
    marmalade_setSuspended_t setSuspended;
    marmalade_setResumed_t setResumed;
};

struct marmalade_loaderview_t
{
    marmalade_setPixelsNative_t setPixelsNative;
    marmalade_videoStoppedNotify_t videoStoppedNotify;
    marmalade_setInputText_t setInputText;
};

struct marmalade_loaderkeyboard_t
{
    marmalade_onKeyEventNative_t onKeyEventNative;
    marmalade_setCharInputEnabledNative_t setCharInputEnabledNative;
};

struct marmalade_soundplayer_t
{
    marmalade_generateAudio_t generateAudio;
};

struct marmalade_soundrecord_t
{
    marmalade_recordAudio_t recordAudio;
};

struct marmalade_loadersmsreceiver_t
{
    marmalade_onReceiveCallback_t onReceiveCallback;
};

struct marmalade_loaderapi_t
{
    marmalade_s3eDebugTraceLine_t s3eDebugTraceLine;
    marmalade_s3eDeviceYield_t s3eDeviceYield;
    marmalade_s3eConfigGet_t s3eConfigGet;
    marmalade_s3eConfigGetInt_t s3eConfigGetInt;
};

struct marmalade_loaderlocation_t
{
    marmalade_locationUpdate_t locationUpdate;
    marmalade_locationSatellite_t locationSatellite;
};

struct SupportModulePriv {
    jni_onload_t JNI_OnLoad;

    struct marmalade_loaderthread_t loaderthread;
    struct marmalade_loaderview_t loaderview;
    struct marmalade_loaderkeyboard_t loaderkeyboard;
    struct marmalade_soundplayer_t soundplayer;
    struct marmalade_soundrecord_t soundrecord;
    struct marmalade_loadersmsreceiver_t loadersmsreceiver;
    struct marmalade_loaderapi_t loaderapi;
    struct marmalade_loaderlocation_t loaderlocation;

    struct GlobalState *global;
    struct SupportModule *module;
   
    char *home;
    
    /* LoaderThread is singleton */
    dummy_jobject *theloaderthread;
    
    /* LoaderView and LoaderView::m_Pixels */
    /* hopefully we only need one view so this should be ok */
    dummy_jobject *theview;
    jintArray *pixels;
    
    int marmalade_found;
};

static struct SupportModulePriv marmalade_priv;

#define MARMALADE_LOADERTHREAD "com/ideaworks3d/marmalade/LoaderThread"
#define MARMALADE_LOADERVIEW "com/ideaworks3d/marmalade/LoaderView"
#define MARMALADE_LOADERKEYBOARD "com/ideaworks3d/marmalade/LoaderKeyboard"
#define MARMALADE_SOUNDPLAYER "com/ideaworks3d/marmalade/SoundPlayer"
#define MARMALADE_SOUNDRECORD "com/ideaworks3d/marmalade/SoundRecord"
#define MARMALADE_LOADERSMSRECEIVER "com/ideaworks3d/marmalade/LoaderSMSReceiver"
#define MARMALADE_LOADERAPI "com/ideaworks3d/marmalade/LoaderAPI"
#define MARMALADE_LOADERLOCATION "com/ideaworks3d/marmalade/LoaderLocation"

#define AIRPLAY_LOADERTHREAD "com/ideaworks3d/airplay/AirplayThread"
#define AIRPLAY_LOADERVIEW "com/ideaworks3d/airplay/AirplayView"
#define AIRPLAY_SOUNDPLAYER "com/ideaworks3d/airplay/SoundPlayer"
#define AIRPLAY_SOUNDRECORD "com/ideaworks3d/airplay/SoundRecord"
#define AIRPLAY_LOADERSMSRECEIVER "com/ideaworks3d/airplay/AirplaySMSReceiver"
#define AIRPLAY_LOADERAPI "com/ideaworks3d/airplay/AirplayAPI"

jclass marmalade_FindClass(JNIEnv* p0, const char* p1) SOFTFP;
jthrowable marmalade_ExceptionOccurred(JNIEnv* p0) SOFTFP;
jint marmalade_RegisterNatives(JNIEnv* p0, jclass p1, const JNINativeMethod* p2, jint p3) SOFTFP;
jint marmalade_CallIntMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jfieldID marmalade_GetFieldID(JNIEnv* p0, jclass clazz, const char* name, const char* sig) SOFTFP;
jobject marmalade_GetObjectField(JNIEnv* p0, jobject p1, jfieldID p2) SOFTFP;
jobject marmalade_NewGlobalRef(JNIEnv* p0, jobject p1) SOFTFP;
jint* marmalade_GetIntArrayElements(JNIEnv* p0, jintArray p1, jboolean* p2) SOFTFP;
void marmalade_CallVoidMethodV(JNIEnv* env, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jfieldID marmalade_GetStaticFieldID(JNIEnv *p0, jclass p1, const char *p2, const char *p3) SOFTFP;
jobject marmalade_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jint marmalade_GetStaticIntField(JNIEnv* p0, jclass p1, jfieldID p2) SOFTFP;
jboolean marmalade_CallBooleanMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;

jthrowable
marmalade_ExceptionOccurred(JNIEnv* p0)
{
    // ignore
    return NULL;
}

#define method_is(met) (0 == strcmp(method->name,#met))
#define class_is(claxx) (strcmp(clazz->name,claxx) == 0)

jint
marmalade_RegisterNatives(JNIEnv* p0, jclass p1, const JNINativeMethod* p2, jint p3)
{
    MODULE_DEBUG_PRINTF("marmalade_RegisterNatives()\n");

    struct dummy_jclass *clazz = (struct dummy_jclass*)p1;
    MODULE_DEBUG_PRINTF("\n\tClass: %s\n", clazz->name);

    int is_loaderthread = class_is(MARMALADE_LOADERTHREAD) || class_is(AIRPLAY_LOADERTHREAD);
    int is_loaderview = class_is(MARMALADE_LOADERVIEW) || class_is(AIRPLAY_LOADERVIEW);
    int is_loaderkeyboard = class_is(MARMALADE_LOADERKEYBOARD); /* airplay has no loaderkeyboard */
    int is_soundplayer = class_is(MARMALADE_SOUNDPLAYER) || class_is(AIRPLAY_SOUNDPLAYER);
    int is_soundrecord = class_is(MARMALADE_SOUNDRECORD) || class_is(AIRPLAY_SOUNDRECORD);
    int is_loadersmsreceiver = class_is(MARMALADE_LOADERSMSRECEIVER) || class_is(AIRPLAY_LOADERSMSRECEIVER);
    int is_loaderapi = class_is(MARMALADE_LOADERAPI) || class_is(AIRPLAY_LOADERAPI);
    int is_loaderlocation = class_is(MARMALADE_LOADERLOCATION); /* airplay has no loaderlocation */
    
    int i=0;
    const JNINativeMethod *method = p2;
    while (i<p3) {
        MODULE_DEBUG_PRINTF("\tName: %-20s Sig: %-10s Addr: %x\n", method->name, method->signature, method->fnPtr);
#define REG_IF_MATCH(theclass, themethod) if(method_is(themethod)) marmalade_priv.theclass.themethod = (marmalade_ ## themethod ## _t)method->fnPtr; else
        if(is_loaderthread)
        {
            REG_IF_MATCH(loaderthread,shutdownNative)
            REG_IF_MATCH(loaderthread,initNative)
            REG_IF_MATCH(loaderthread,suspendAppThreads)
            REG_IF_MATCH(loaderthread,resumeAppThreads)
            REG_IF_MATCH(loaderthread,onAccelNative)
            REG_IF_MATCH(loaderthread,onCompassNative)
            REG_IF_MATCH(loaderthread,onMotionEvent)
            REG_IF_MATCH(loaderthread,setViewNative)
            REG_IF_MATCH(loaderthread,runNative)
            REG_IF_MATCH(loaderthread,audioStoppedNotify)
            REG_IF_MATCH(loaderthread,chargerStateChanged)
            REG_IF_MATCH(loaderthread,networkCheckChanged)
            REG_IF_MATCH(loaderthread,runOnOSThreadNative)
            REG_IF_MATCH(loaderthread,runOnOSTickNative)
            REG_IF_MATCH(loaderthread,signalSuspend)
            REG_IF_MATCH(loaderthread,signalResume)
            REG_IF_MATCH(loaderthread,setSuspended)
            REG_IF_MATCH(loaderthread,setResumed)
            ;
        }
        else if(is_loaderview)
        {
            REG_IF_MATCH(loaderview,setPixelsNative)
            REG_IF_MATCH(loaderview,videoStoppedNotify)
            REG_IF_MATCH(loaderview,setInputText)
            ;
        }
        else if(is_loaderkeyboard)
        {
            REG_IF_MATCH(loaderkeyboard,onKeyEventNative)
            REG_IF_MATCH(loaderkeyboard,setCharInputEnabledNative)
            ;
        }
        else if(is_soundplayer)
        {
            REG_IF_MATCH(soundplayer,generateAudio)
            ;
        }
        else if(is_soundrecord)
        {
            REG_IF_MATCH(soundrecord,recordAudio)
            ;
        }
        else if(is_loadersmsreceiver)
        {
            REG_IF_MATCH(loadersmsreceiver,onReceiveCallback)
            ;
        }
        else if(is_loaderapi)
        {
            REG_IF_MATCH(loaderapi,s3eDebugTraceLine)
            REG_IF_MATCH(loaderapi,s3eDeviceYield)
            REG_IF_MATCH(loaderapi,s3eConfigGet)
            REG_IF_MATCH(loaderapi,s3eConfigGetInt)
            ;
        }
        else if(is_loaderlocation)
        {
            REG_IF_MATCH(loaderlocation,locationUpdate)
            REG_IF_MATCH(loaderlocation,locationSatellite)
            ;
        }

        method++;
        i++;
    }
    MODULE_DEBUG_PRINTF("\n");

    return 0;
}

/* mix audio buffer */
static void my_audio_mixer(void *udata, Uint8 *stream, int len)
{
    static short soundbuf[AUDIO_CHUKSIZE * AUDIO_CHANNELS];
    struct dummy_short_array arr;
    short *mixbuf = (short *)stream;
    int i;

    if (len > sizeof(soundbuf)) {
        fprintf(stderr, "audio setup broken\n");
        exit(1);
    }
    len /= 2;
    arr.data = soundbuf;
    arr.size = len;
    marmalade_priv.soundplayer.generateAudio(ENV(marmalade_priv.global),
        VM(marmalade_priv.global), &arr, len);
    for (i = 0; i < len; i++)
        mixbuf[i] += soundbuf[i];
}

static int my_soundInit(void)
{
    MODULE_DEBUG_PRINTF("marmalade initializing audio\n");
    
    // marmalade gives useless values
    int audio_rate = AUDIO_RATE;
    
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = AUDIO_CHANNELS;
    int audio_buffers = AUDIO_CHUKSIZE;

    int act_audio_rate;
    int act_audio_channels;
    Uint16 act_audio_format;

    Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);

    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        MODULE_DEBUG_PRINTF("marmalade: Unable to initialize audio: %s\n", Mix_GetError());
        exit(1);
    }
        
    Mix_AllocateChannels(16);

    if(Mix_QuerySpec(&act_audio_rate, &act_audio_format, &act_audio_channels) != 0)
    {
        if(act_audio_rate != audio_rate || act_audio_format != audio_format || act_audio_channels != audio_channels)
        {
            MODULE_DEBUG_PRINTF("marmalade actual audio settings differ from set: set/act [rate,format,channels]: [%d/%d,%d/%d,%d/%d]\n",audio_rate,act_audio_rate,audio_format,act_audio_format,audio_channels,act_audio_channels);
        }

        audio_rate = act_audio_rate;
    }
    else
    {
        MODULE_DEBUG_PRINTF("Mix_QuerySpec failed.\n");
    }

    Mix_SetPostMix(my_audio_mixer, NULL);

    MODULE_DEBUG_PRINTF("marmalade initializing audio done.\n");
    return audio_rate;
}

jint
marmalade_CallIntMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    jmethodID method = p2;

    MODULE_DEBUG_PRINTF("marmalade_CallIntMethodV(%s)\n",p2->name);

    if(method_is(getOrientation))
    {
        return ORIENTATION_LANDSCAPE;
    }
    else if(method_is(getNetworkType))
    {
        return 8; // apparently HSDPA
    }
    else if(method_is(audioPlay))
    {
        // (Ljava/lang/String;IJJI)I
        struct dummy_jstring *filename = va_arg(p3, struct dummy_jstring *);
        int repeats = va_arg(p3, int);
        long file_offset = va_arg(p3, long);
        long arg3 = va_arg(p3, long); // unused?
        int file_size = va_arg(p3, int);
        static SDL_RWops *rw;
        static Mix_Music *music;
        const void *mem;

        MODULE_DEBUG_PRINTF("audioPlay '%s', %d, %ld, %ld, %d\n",
            filename->data, repeats, file_offset, arg3, file_size);
        if (music != NULL) {
            Mix_FreeMusic(music);
            music = NULL;
        }
        if (rw != NULL) {
            SDL_RWclose(rw);
            rw = NULL;
        }
        mem = (const char *)marmalade_priv.global->apk_in_mem + file_offset;
        rw = SDL_RWFromMem((void *)mem, file_size);
        music = Mix_LoadMUS_RW(rw);
        Mix_PlayMusic(music, repeats > 0 ? repeats : -1);
        return 0;
    }
    else if(method_is(soundInit))
    {
        //(IZI)I
        
        // marmalade gives usesless variables:
        //jint audio_rate = va_arg(p3,int);
        //jboolean stereo = va_arg(p3,int); // bool
        //jint volume = va_arg(p3,int);

        my_soundInit();
        
        return AUDIO_RATE;
    }
    else if(method_is(audioGetNumChannels))
    {
        // TODO: actually get number of channels
        return AUDIO_CHANNELS;
    }
    else if(method_is(getBatteryLevel))
    {
        return 100;
    }

    return 0;
}

jmethodID
jnienv_make_method(jclass clazz, const char *name, const char *sig)
{
    jmethodID id = malloc(sizeof(struct _jmethodID));
    id->clazz = clazz;
    id->name = strdup(name);
    id->sig = strdup(sig);
    return id;
}

jfieldID
jnienv_make_field(jclass clazz, const char *name, const char *sig)
{
    return (jfieldID)jnienv_make_method(clazz,name,sig);
}

/* TODO: to jni/jnienv.c? */
jfieldID
marmalade_GetFieldID(JNIEnv* p0, jclass clazz, const char* name, const char* sig)
{
    struct dummy_jclass *class = (struct dummy_jclass*)clazz;
    MODULE_DEBUG_PRINTF("marmalade_GetFieldID(%s, '%s', '%s')\n", class->name, name, sig);
    return jnienv_make_field(clazz, name, sig);
}

/* TODO: to jni/jnienv.c? */
jclass
marmalade_GetObjectClass(JNIEnv *p0, jobject p1)
{
    MODULE_DEBUG_PRINTF("marmalade_GetObjectClass(%x,%x)\n",p0,p1);
    if(NULL != p1)
    {
        dummy_jobject *obj = p1;
        return obj->clazz;
    }
    return NULL;
}

/* is this correct? */
jobject
marmalade_GetObjectField(JNIEnv* p0, jobject p1, jfieldID p2)
{
    if(NULL != p1)
    {
        //if(0 == strcmp(((jmethodID)p2)->name,"m_GL"))
        //{
        //TODO
        //}
        /*
        MODULE_DEBUG_PRINTF("marmalade_GetObjectField(%s)\n",((jmethodID)p2)->name);
        struct dummy_jclass *cls = malloc(sizeof(struct dummy_jclass));
        cls->name = ((struct dummy_jclass*)((jmethodID)p2)->clazz)->name;

        dummy_jobject* obj = malloc(sizeof(dummy_jobject));
        obj->clazz = cls;
        obj->field = p2;

        return obj;*/
    }
    else
    {
        MODULE_DEBUG_PRINTF("marmalade_GetObjectField(%s) -> NULL\n",((jmethodID)p2)->name);
    }

    return NULL;
}

jobject
marmalade_NewGlobalRef(JNIEnv* p0, jobject p1)
{
    if (p1 == NULL)
    {
        // from other module, isn't actually needed here (at least in PvZ not)
        struct dummy_jclass* cls = malloc(sizeof(struct dummy_jclass));
        cls->name = "null";

        dummy_jobject* obj = malloc(sizeof(dummy_jobject));
        obj->clazz = cls;
        obj->field = NULL;
        MODULE_DEBUG_PRINTF("marmalade_NewGlobalRef(%x) -> %x\n", p1, obj);
        return obj;
    }
    else
    {
        dummy_jobject *obj = p1;
        MODULE_DEBUG_PRINTF("marmalade_NewGlobalRef(%x)\n", ((jmethodID)obj->field));
        return p1; // is this correct?
    }
}

/* TODO: shouldn't that be in jni/jnienv.c? */
jint*
marmalade_GetIntArrayElements(JNIEnv* p0, jintArray p1, jboolean* p2)
{
    struct dummy_int_array *intarr = p1;
    if(NULL != p1)
    {
        MODULE_DEBUG_PRINTF("marmalade_GetIntArrayElements(%x) -> %x\n",p1,intarr->data);
        return (jint*)intarr->data;
    }
    
    MODULE_DEBUG_PRINTF("marmalade_GetIntArrayElements(NULL) -> NULL\n");
    return NULL;
}

static void
marmalade_SetShortArrayRegion(JNIEnv* p0, jshortArray p1, jsize start, jsize len, const jshort* p4)
{
    struct dummy_short_array *arr = p1;
    MODULE_DEBUG_PRINTF("marmalade_SetShortArrayRegion(%p) -> %p\n", arr, arr ? arr->data : NULL);
    if (arr != NULL && arr->data != NULL)
        memcpy(arr->data + start, p4, len * sizeof(arr->data[0]));
}

static void
marmalade_input_handler(struct SupportModule *self);

void
marmalade_CallVoidMethodV(JNIEnv* env, jobject p1, jmethodID p2, va_list p3)
{
    jmethodID method = p2;
    
    if(method_is(glReInit))
    {
        MODULE_DEBUG_PRINTF("TODO: implement glReInit\n");
    }
    else if(method_is(glInit))
    {
        MODULE_DEBUG_PRINTF("TODO: implement glInit\n");
    }
    else if(method_is(glSwapBuffers))
    {
        marmalade_input_handler(marmalade_priv.module);
        marmalade_priv.global->module_hacks->system_update();
    }
    else if(method_is(videoStop))
    {
        // for now
        MODULE_DEBUG_PRINTF("videoStoppedNotify.\n");
        marmalade_priv.loaderview.videoStoppedNotify(ENV(marmalade_priv.global),marmalade_priv.theloaderthread);
        MODULE_DEBUG_PRINTF("videoStoppedNotify done.\n");
    }
    else if(method_is(deviceYield))
    {
    }
    else if(method_is(deviceUnYield))
    {
    }
    else if(method_is(fixOrientation))
    {
        int orientation = va_arg(p3,int);
        switch(orientation)
        {
            case ORIENTATION_LANDSCAPE:
               MODULE_DEBUG_PRINTF("TODO: fixOrientation: LANDSCAPE\n");
               break;
           case ORIENTATION_PORTRAIT:
               MODULE_DEBUG_PRINTF("TODO: fixOrientation: PORTRAIT\n");
               break;
           default:
               MODULE_DEBUG_PRINTF("TODO: fixOrientation, unknown orientation: %d\n", orientation);
        }
    }
    else if(method_is(soundSetVolume))
    {
        jint volume = va_arg(p3, int);
        MODULE_DEBUG_PRINTF("soundSetVolume(%d) obj=%p\n", volume, p1);
    }
    else if(method_is(audioSetVolume))
    {
        jint volume = va_arg(p3, int);
        jint which = va_arg(p3, int);
        MODULE_DEBUG_PRINTF("audioSetVolume(%d, %d) obj=%p\n", volume, which, p1);
        Mix_VolumeMusic(volume * MIX_MAX_VOLUME / 100);
    }
    else if(method_is(doDraw))
    {
        // TODO (draw whatever is in LoaderThread::m_Pixels)
        /*
        GLuint doDraw_tex;
        glGenTextures(1,&doDraw_tex);
        glBindTexture(GL_TEXTURE_2D, doDraw_tex);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, marmalade_priv.w, marmalade_priv.h, 0, GL_RGB, GL_UNSIGNED_INT, marmalade_priv.pixels);
        GLfloat vertices[] = {-1, -1, 0,
                              -1,  1, 0,
                               1,  1, 0,
                               1, -1, 0};
        GLubyte indices[] = {0,1,2,
                             0,2,3};
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
        glDeleteTextures(1,&doDraw_tex);
        */
        marmalade_priv.global->module_hacks->system_update();
    }
    else
    {
        MODULE_DEBUG_PRINTF("marmalade_CallVoidMethodV(%s)\n",p2->name);
    }
}

// TODO: shouldn't that be in jni/jnienv.c ?
jfieldID
marmalade_GetStaticFieldID(JNIEnv *p0, jclass p1, const char *p2, const char *p3)
{
    struct dummy_jclass* cls = (struct dummy_jclass*)p1;
    
    if(!strcmp(p2, "SDK_INT") == 0)
    {
        MODULE_DEBUG_PRINTF("marmalade_GetStaticFieldID %s %s %s\n", cls->name, p2, p3);
    }

    struct _jfieldID* field = malloc(sizeof(struct _jfieldID));

    field->clazz = p1;
    field->name = strdup(p2);
    field->sig = strdup(p3);

    return (jfieldID)field;
}

jobject
marmalade_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    jmethodID method = p2;
    MODULE_DEBUG_PRINTF("marmalade_CallObjectMethodV %x %s\n",p1,p2->name);
    
    if(method_is(getLocale))
    {
        // TODO: actually get the locale
        return (*env)->NewStringUTF(env, "en");
    }
    else if(method_is(getDeviceModel))
    {
        // honesty
        return (*env)->NewStringUTF(env,"Nokia N9");
    }
    else if(method_is(getCardRoot))
    {
        return (*env)->NewStringUTF(env, marmalade_priv.home); // or "/"?
    }
    else if(method_is(getDeviceNumber))
    {
        // TODO: find out what Marmalade expects from this function
        return (*env)->NewStringUTF(env, "1234");
    }
    else
    {
        MODULE_DEBUG_PRINTF("TODO: %s\n", p2->name);
    }
    return NULL;
}

//#define ICE_CREAM_SANDWICH 14

jint
marmalade_GetStaticIntField(JNIEnv* p0, jclass p1, jfieldID p2)
{
    struct _jfieldID *fld = (struct _jfieldID*)p2;

    if(strcmp(fld->name,"SDK_INT") == 0)
    {
        MODULE_DEBUG_PRINTF("Get SDK_INT\n");
        // TODO: look further into that
        return 0x8; // froyo, return whatever, doesn't seem to make much change
    }
    else
    {
        MODULE_DEBUG_PRINTF("marmalade_GetStaticIntField(%s)\n",fld->name);
    }
    
    return 0;
}

jboolean
marmalade_CallBooleanMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("marmalade_CallBooleanMethodV: %s\n",p2->name);

    if(strcmp(p2->name,"networkCheckStop") == 0)
    {
        MODULE_DEBUG_PRINTF("marmalade_CallBooleanMethodV: networkCheckStop default 0\n");
        // TODO: actually check network
        return 0;
    }
    else if(strcmp(p2->name,"hasMultitouch") == 0)
    {
        return HAVE_MULTITOUCH;
    }
    else if(strcmp(p2->name,"chargerIsConnected") == 0)
    {
        // TODO: actually check charger
        return 1;
    }
    
    return 0;
}

extern void *memmem (__const void *__haystack, size_t __haystacklen,
                     __const void *__needle, size_t __needlelen);

void
check_marmalade(const char *filename, char *buffer, size_t size)
{
    char *marmalade_str = "Marmalade";
    char *airplay_str = "Airplay";
    if(0 != memmem(buffer,size,marmalade_str,strlen(marmalade_str)) || 0 != memmem(buffer,size,airplay_str,strlen(airplay_str)))
    {
        marmalade_priv.marmalade_found = 1;
    }
    else
    {
        marmalade_priv.marmalade_found = 0;
    }
}

static int
marmalade_try_init(struct SupportModule *self)
{
    self->priv->JNI_OnLoad = (jni_onload_t)LOOKUP_M("JNI_OnLoad");
    
    GLOBAL_M->foreach_file("classes.dex",check_marmalade);
    
    self->override_env.RegisterNatives = marmalade_RegisterNatives;
    self->override_env.CallObjectMethodV = marmalade_CallObjectMethodV;
    self->override_env.CallVoidMethodV = marmalade_CallVoidMethodV;
    self->override_env.CallIntMethodV = marmalade_CallIntMethodV;
    self->override_env.CallBooleanMethodV = marmalade_CallBooleanMethodV;
    self->override_env.GetStaticIntField = marmalade_GetStaticIntField;
    self->override_env.GetStaticFieldID = marmalade_GetStaticFieldID;
    self->override_env.ExceptionOccurred = marmalade_ExceptionOccurred;
    self->override_env.GetIntArrayElements = marmalade_GetIntArrayElements;
    self->override_env.SetShortArrayRegion = marmalade_SetShortArrayRegion;
    self->override_env.NewGlobalRef = marmalade_NewGlobalRef;
    self->override_env.GetFieldID = marmalade_GetFieldID;
    self->override_env.GetObjectField = marmalade_GetObjectField;
    self->override_env.GetObjectClass = marmalade_GetObjectClass;
    
    return (self->priv->JNI_OnLoad != NULL) && marmalade_priv.marmalade_found;
}

static void
marmalade_init(struct SupportModule *self, int width, int height, const char *home)
{
    self->priv->global = GLOBAL_M;
    self->priv->module = self;
    self->priv->home = strdup(home);

    GLOBAL_M->module_hacks->handle_update = 1;
    
    MODULE_DEBUG_PRINTF("JNI_OnLoad\n");
    self->priv->JNI_OnLoad(VM_M, NULL);
    MODULE_DEBUG_PRINTF("JNI_OnLoad done.\n");
   
    self->priv->theloaderthread = malloc(sizeof(dummy_jobject));
    
    MODULE_DEBUG_PRINTF("initNative\n");
    self->priv->loaderthread.initNative(ENV_M,self->priv->theloaderthread);
    MODULE_DEBUG_PRINTF("initNative done.\n");
    
    // TODO: extract files (implement dzip algorithm)
    
    // initialize LoaderView and LoaderView::m_Pixels 
    
    self->priv->theview = (dummy_jobject*)malloc(sizeof(dummy_jobject));
    self->priv->theview->clazz = malloc(sizeof(struct dummy_jclass));
    ((struct dummy_jclass*)(self->priv->theview->clazz))->name = MARMALADE_LOADERVIEW;
    self->priv->theview->field = jnienv_make_field(self->priv->theview->clazz, "theview", "L" MARMALADE_LOADERVIEW ";"); 
 
    self->priv->pixels = malloc(sizeof(struct dummy_int_array));
    ((struct dummy_int_array*)self->priv->pixels)->data = (int*)malloc(width*height*sizeof(int));
    ((struct dummy_int_array*)self->priv->pixels)->size = width*height;
    
    MODULE_DEBUG_PRINTF("setViewNative\n");
    self->priv->loaderthread.setViewNative(ENV_M,self->priv->theloaderthread,self->priv->theview);
    MODULE_DEBUG_PRINTF("setViewNative done.\n");
    
    MODULE_DEBUG_PRINTF("setPixelsNative\n");
    self->priv->loaderview.setPixelsNative(ENV_M,self->priv->theview,width,height,self->priv->pixels, 1 /* == newly created */ );
    MODULE_DEBUG_PRINTF("setPixelsNative done.\n");

    /* needed ?
    MODULE_DEBUG_PRINTF("resumeAppThreads\n");
    self->priv->loaderthread.resumeAppThreads(ENV_M,self->priv->theloaderthread);
    MODULE_DEBUG_PRINTF("resumeAppThreads done.\n");
    */
    
    MODULE_DEBUG_PRINTF("runNative\n");
    self->priv->loaderthread.runNative(ENV_M,self->priv->theloaderthread,
            GLOBAL_M->env->NewStringUTF(ENV_M,self->priv->home),
            GLOBAL_M->env->NewStringUTF(ENV_M,GLOBAL_M->apk_filename));
    MODULE_DEBUG_PRINTF("runNative done.\n");
}

#define ACTION_POINTER_1_DOWN   5
#define ACTION_POINTER_1_UP     6

static void
marmalade_input(struct SupportModule *self, int event, int x, int y, int finger)
{
   if(self->priv->loaderthread.onMotionEvent)
   {
       int action = 0;
       if(ACTION_DOWN == event)
       {
           action = ACTION_POINTER_1_DOWN;
       }
       else if(ACTION_UP == event)
       {
           action = ACTION_POINTER_1_UP;
       }
       else if(ACTION_MOVE == event)
       {
           return; // TODO
       }

       MODULE_DEBUG_PRINTF("onMotionEvent: %s\n", action == ACTION_POINTER_1_UP ? "up" : "down");
       self->priv->loaderthread.onMotionEvent(ENV_M,self->priv->theloaderthread,finger,action-1 /* duh? */, x,y);
       MODULE_DEBUG_PRINTF("onMotionEvent done.\n");
   }
}

static void
marmalade_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
}



/* this function is never called */
static void
marmalade_update(struct SupportModule *self)
{
}

static void
marmalade_deinit(struct SupportModule *self)
{
    MODULE_DEBUG_PRINTF("marmalade_deinit\n");
    MODULE_DEBUG_PRINTF("shutdownNative\n");
    marmalade_priv.loaderthread.shutdownNative(ENV(marmalade_priv.global),marmalade_priv.theloaderthread);
    MODULE_DEBUG_PRINTF("shutdownNative done.\n");
}

static void
marmalade_pause(struct SupportModule *self)
{
}

static void
marmalade_resume(struct SupportModule *self)
{
}

static int
marmalade_requests_exit(struct SupportModule *self)
{
    return 0;
}

void
marmalade_input_handler(struct SupportModule *self)
{
    int emulate_multitouch = 0;
    const int emulate_finger_id = 2;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
#ifdef PANDORA
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym==SDLK_ESCAPE) {
                marmalade_deinit(self);
                goto finish;
            }
            else if (e.key.keysym.sym==SDLK_RSHIFT) {
                emulate_multitouch = 1;
                marmalade_input(self,ACTION_DOWN, platform_getscreenwidth()>>1, platform_getscreenheight()>>1,emulate_finger_id);
            }
        }
        else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym==SDLK_RSHIFT) {
                emulate_multitouch = 0;
                marmalade_input(self,ACTION_UP, platform_getscreenwidth()>>1, platform_getscreenheight()>>1,emulate_finger_id);
            }
        } else
#endif
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            marmalade_input(self, ACTION_DOWN, e.button.x, e.button.y, e.button.which);
            if (emulate_multitouch) {
                marmalade_input(self,ACTION_DOWN, platform_getscreenwidth()-e.button.x, platform_getscreenheight()-e.button.y,emulate_finger_id);
            }
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            marmalade_input(self, ACTION_UP, e.button.x, e.button.y, e.button.which);
            if (emulate_multitouch) {
                marmalade_input(self,ACTION_UP, platform_getscreenwidth()-e.button.x, platform_getscreenheight()-e.button.y,emulate_finger_id);
            }
        } else if (e.type == SDL_MOUSEMOTION) {
            marmalade_input(self, ACTION_MOVE, e.motion.x, e.motion.y, e.motion.which);
            if (emulate_multitouch) {
                marmalade_input(self,ACTION_MOVE, platform_getscreenwidth()-e.button.x, platform_getscreenheight()-e.button.y,emulate_finger_id);
            }
        } else if (e.type == SDL_QUIT) {
            marmalade_deinit(self);
            goto finish;
        } else if (e.type == SDL_ACTIVEEVENT) {
            if (e.active.state == SDL_APPACTIVE && e.active.gain == 0) {
                marmalade_pause(self);
                while (1) {
                    SDL_WaitEvent(&e);
                    if (e.type == SDL_ACTIVEEVENT) {
                        if (e.active.state == SDL_APPACTIVE &&
                                e.active.gain == 1) {
                            break;
                        }
                    } else if (e.type == SDL_QUIT) {
                        goto finish;
                    }
                }
                marmalade_resume(self);
            }
        }
    }
    finish:
    return;
}

APKENV_MODULE(marmalade, MODULE_PRIORITY_GENERIC)

