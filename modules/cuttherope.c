
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
 * Cut the Rope Lite - 0.1 crow_riot based on the works of thp
 **/

#include "../imagelib/imagelib.h"
#include "../imagelib/loadjpeg.c"
#include "../imagelib/loadpng.c"
#include "../apklib/keycodes.h"
#include <linux/limits.h>
#include <sys/stat.h>
#include <unistd.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "common.h"

typedef void (*cuttherope_init_t)(JNIEnv *env, jobject obj, jobject resourceLoader, jobject soundManager, jobject preferences,
            jobject saveManager, jobject flurry, jobject videoPlayer, jobject scorer,
            jobject billingInterface, jobject remoteDataManager, jint x) SOFTFP;
typedef void (*cuttherope_resize_t)(JNIEnv *env, jobject object, jint width, jint height) SOFTFP;
typedef void (*cuttherope_tick_t)(JNIEnv *env, jobject object, jlong tick) SOFTFP;
typedef void (*cuttherope_render_t)(JNIEnv *env, jobject object) SOFTFP;
typedef void (*cuttherope_videobannerfinished_t)(JNIEnv *env, jobject object) SOFTFP;
typedef void (*cuttherope_resume_t)(JNIEnv*,jobject) SOFTFP;
typedef void (*cuttherope_pause_t)(JNIEnv*,jobject) SOFTFP;
typedef jboolean (*cuttherope_backpressed_t)(JNIEnv*, jobject) SOFTFP;
typedef jboolean (*cuttherope_menupressed_t)(JNIEnv*, jobject) SOFTFP;
typedef void (*cuttherope_imageloaded_t)(JNIEnv*,jobject,jint p1, jbyteArray p2, jint p3, jint p4) SOFTFP;
typedef void (*cuttherope_nativetouchadd_t)(JNIEnv *env, jobject p0, jint p1, jint p2, jfloat p3, jfloat p4) SOFTFP;
typedef void (*cuttherope_nativetouchprocess_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*cuttherope_nativeplaybackfinished_t)(JNIEnv *env, jobject p0, jint p1) SOFTFP;

#define MAX_SOUNDS 256
typedef struct {
    char* name;
    Mix_Chunk* sound;
} Sound;

#define MAX_IMAGES 200
typedef struct {
    char filename[PATH_MAX];
    jint id;
    image_t* image;
} Image;


#define KEY_SIZE 64
#define MAX_KEYVALUES 1024
typedef struct {
    char key[KEY_SIZE];
    int value;
} KeyValue;


struct SupportModulePriv {
    jni_onload_t JNI_OnLoad;
    cuttherope_init_t nativeInit;
    cuttherope_resize_t nativeResize;
    cuttherope_tick_t nativeTick;
    cuttherope_render_t nativeRender;
    cuttherope_videobannerfinished_t videoBannerFinished;
    cuttherope_resume_t nativeResume;
    cuttherope_pause_t nativePause;
    cuttherope_backpressed_t nativeBackPressed;
    cuttherope_menupressed_t nativeMenuPressed;
    cuttherope_imageloaded_t imageLoaded;
    cuttherope_nativetouchadd_t nativeTouchAdd;
    cuttherope_nativetouchprocess_t nativeTouchProcess;
    cuttherope_nativeplaybackfinished_t nativePlaybackFinished;
    struct GlobalState *global;
    const char* home;
    Sound sounds[MAX_SOUNDS];
    Image images[MAX_IMAGES];
    Mix_Music* music;
    char musicpath[PATH_MAX];
    KeyValue keyvalues[MAX_KEYVALUES];
    int want_exit;
};
static struct SupportModulePriv cuttherope_priv;

static void
cuttherope_video_banner_finished()
{
    if (cuttherope_priv.videoBannerFinished!=0) {
        cuttherope_priv.videoBannerFinished(ENV(cuttherope_priv.global),
                cuttherope_priv.global);
    }
}

static KeyValue*
get_preference(const char* key, int create) {
    int i;
    for (i=0; i<MAX_KEYVALUES; i++) {
        KeyValue* kv = &cuttherope_priv.keyvalues[i];
        if (kv->key[0]==0)
            break;
        if (strcmp(kv->key,key)==0) {
            return kv;
        }
    }
    if (create!=0) {
        if (i<MAX_KEYVALUES) {
            KeyValue* kv = &cuttherope_priv.keyvalues[i];
            strcpy(kv->key,key);
            kv->value = 0;
            return kv;
        }
    }
    return NULL;
}

#define PREFERENCES_FILENAME "preferences.dat"

static void
load_preferences()
{
    int i;
    char tmp[PATH_MAX];
    strcpy(tmp,cuttherope_priv.home);
    strcat(tmp,PREFERENCES_FILENAME);

    memset(cuttherope_priv.keyvalues,0,sizeof(cuttherope_priv.keyvalues));

    FILE* fp = fopen(tmp,"rb");
    if (!fp)
        return;

    for (i=0; i<MAX_KEYVALUES;i++) {
        KeyValue kv;
        if(fread(&kv,sizeof(KeyValue),1,fp)!=1)
            break;
        cuttherope_priv.keyvalues[i] = kv;
        MODULE_DEBUG_PRINTF("Loaded key=%s value=%d\n",kv.key,kv.value);
    }
    fclose(fp);
}

static void
save_preferences()
{
    int i;
    char tmp[PATH_MAX];
    strcpy(tmp,cuttherope_priv.home);
    strcat(tmp,PREFERENCES_FILENAME);

    FILE* fp = fopen(tmp,"wb");
    if (!fp)
        return;
    for (i=0; i<MAX_KEYVALUES;i++) {
        KeyValue *kv = &cuttherope_priv.keyvalues[i];
        if(kv->key[0]==0)
            break;
        fwrite(kv,sizeof(KeyValue),1,fp);
    }
    fclose(fp);
    sync();
}

static jobject
cuttherope_CallObjectMethodV(JNIEnv *, jobject, jmethodID, va_list) SOFTFP;

static void
cuttherope_CallVoidMethodV(JNIEnv *, jobject, jmethodID, va_list) SOFTFP;

static jobject
cuttherope_CallStaticObjectMethodV(JNIEnv *, jclass, jmethodID, va_list) SOFTFP;

static jobject
cuttherope_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("cuttherope_CallObjectMethodV %s (%s)\n",p2->name,p2->sig);
    struct dummy_array *array;

    if (strcmp(p2->name, "loadData") == 0) {
        struct dummy_jstring *arg;
        arg = va_arg(p3, struct dummy_jstring*);
        jboolean flag = va_arg(p3, jint);
        (void)flag;

        char *filename = malloc(strlen(arg->data) + strlen("assets/") + 1);
        sprintf(filename, "assets/%s", arg->data);

        MODULE_DEBUG_PRINTF("   data=%s\n",arg->data);

        if ((array = GLOBAL_J(env)->read_file_to_jni_array(filename))) {

            MODULE_DEBUG_PRINTF("   data=%s size=%d\n", arg->data, array->length);

            // Process input to prevent "not responding" message
            cuttherope_priv.global->platform->input_update(cuttherope_priv.global->active_module);

            return array;
        } else {
            MODULE_DEBUG_PRINTF("   data=%s not found\n",arg->data);
            return GLOBAL_J(env);
        }
    }
    else if (strcmp(p2->name, "getBytesOfBitmap") == 0) {

        if ((array = GLOBAL_J(env)->read_file_to_jni_array("assets/zeptolab.png"))) {
            return array;
        }
    }
    else if (strcmp(p2->name, "getQuadsOfBitmap")==0) {
        return (*env)->NewFloatArray(env, 4);

    }
    else if (strcmp(p2->name, "getFontGenerator")==0) {
        return (void*)0xF09;
    }
    else if (strcmp(p2->name, "getBanner")==0) {
        return 0;
    }
    else if (strcmp(p2->name, "getName")==0) {
        return (*env)->NewStringUTF(env,"");
    }
    return GLOBAL_J(env);
}

static image_t*
cuttherope_loadImage(JNIEnv *env, const char *filename )
{
    int i;
    Image* img = 0;
    for (i=0; i<MAX_IMAGES;i++) {
        img = &cuttherope_priv.images[i];
        if (img->filename==0 || img->filename[0]==0) {
            break;
        }
        if (strcmp(img->filename,filename)==0) {
            MODULE_DEBUG_PRINTF("cuttherope_loadImage: returning cached image %s\n",filename);
            return img->image;
        }
    }

    Image nocache;
    if (i>=MAX_IMAGES) {
        MODULE_DEBUG_PRINTF("cuttherope_loadImage: Images exceeding limits: %s\n",filename);
        //return NULL;
        img = &nocache; //dont cache
    }

    strcpy(img->filename,filename);
    img->image = NULL;

    char *buf;
    size_t buf_size;
    char filepath[PATH_MAX]; sprintf(filepath,"assets/%s",filename);

    if (GLOBAL_J(env)->read_file(filepath, &buf, &buf_size)) {

        imageloadersettings_t loadsettings =
        {
            .swapy = 0,
            .forcergba = 1,
            .alpha = 255,
            .swaprb = 1,
        };

        if (strstr(filepath,".png")!=0) {
            img->image = loadpng_mem(buf,buf_size,loadsettings);
        }
        else
        if (strstr(filepath,".jpeg")!=0) {
            img->image = loadjpeg_mem(buf,buf_size,loadsettings);
        }
    }

    return img->image;
}

static void
cuttherope_CallVoidMethodV(JNIEnv* env, jobject p1, jmethodID p2, va_list p3)
{
    if (!p2)
        return;

    MODULE_DEBUG_PRINTF("cuttherope_CallVoidMethodV %s\n",p2->name);

    if (strcmp(p2->name,"loadImage")==0) {//public void loadImage(String paramString, int paramInt)

        struct dummy_jstring *filename = va_arg(p3, struct dummy_jstring*);
        jint fileId =  va_arg(p3, jint);

        MODULE_DEBUG_PRINTF("cuttherope_CallVoidMethodV(%s): filename=%s fileId=%d\n",p2->name,filename->data,fileId);

        image_t *image = cuttherope_loadImage(env,filename->data);
        if (image!=NULL) {
            MODULE_DEBUG_PRINTF("cuttherope_CallVoidMethodV(%s): imageLoaded=%s (%d,%d,%d) (%x)\n",p2->name,filename->data,image->width,image->height,image->bpp,image);
            cuttherope_priv.imageLoaded(ENV(cuttherope_priv.global),cuttherope_priv.global,fileId,image,image->width,image->height);
        }
    }
    else
    if (strcmp(p2->name,"loadSound")==0) {
        char filepath[PATH_MAX];
        struct dummy_jstring *filename = va_arg(p3, struct dummy_jstring*);
        jint soundId =  va_arg(p3, jint);

        MODULE_DEBUG_PRINTF("cuttherope_CallVoidMethodV(%s): filename=%s id=%d\n",p2->name,filename->data,soundId);

        if (soundId>=MAX_SOUNDS) {
            MODULE_DEBUG_PRINTF("loadSound id=%d exceeding limits \n",soundId,MAX_SOUNDS);
        }
        else
        if (cuttherope_priv.sounds[soundId].sound==0) {
            sprintf(filepath,"assets/%s",filename->data);

            char* buffer;
            size_t size;
            if (GLOBAL_J(env)->read_file(filepath,&buffer,&size)) {
                SDL_RWops *rw = SDL_RWFromMem(buffer, size);
                Mix_Chunk *sound = Mix_LoadWAV_RW(rw, 1);
                cuttherope_priv.sounds[soundId].sound = sound;
                MODULE_DEBUG_PRINTF("loadSound id=%d %s.\n",soundId,sound?"ok":"failed");
            }
        }
    }
    else
    if (strcmp(p2->name, "showBanner")==0) {
        cuttherope_video_banner_finished();
        save_preferences(); //fast hack to save prefences in harmattan
    }
    else
    if (strcmp(p2->name,"playVideo")==0) {

        struct dummy_jstring *filename = va_arg(p3, struct dummy_jstring*);
        jint paramInt =  va_arg(p3, jint);
        MODULE_DEBUG_PRINTF("playVideo: filename=%s, param=%d\n", filename->data, paramInt);

        if (cuttherope_priv.nativePlaybackFinished)
            cuttherope_priv.nativePlaybackFinished(ENV(cuttherope_priv.global),
                    cuttherope_priv.global,paramInt);
    }
    else
    if (strcmp(p2->name,"playSoundLooped")==0) {
        //playSoundLooped(int paramInt, boolean paramBoolean)
        jint soundId = va_arg(p3,jint);
        jboolean loop = va_arg(p3,jint);

        if (soundId>=0 && soundId<MAX_SOUNDS && cuttherope_priv.sounds[soundId].sound!=0) {
            MODULE_DEBUG_PRINTF("playSoundLooped id=%d loop=%d\n",soundId,loop);

            Mix_PlayChannel(-1,cuttherope_priv.sounds[soundId].sound,loop);
        }
    }
    else
    if (strcmp(p2->name,"playMusic")==0) {
        struct dummy_jstring *filename = va_arg(p3,struct dummy_jstring*);
        MODULE_DEBUG_PRINTF("playMusic: filename=%s\n", filename->data);

        char musicpath[PATH_MAX];
        strcpy(musicpath,cuttherope_priv.home);
        strcat(musicpath,filename->data);

        if (strcmp(cuttherope_priv.musicpath,musicpath)!=0) {
            if (cuttherope_priv.music!=NULL) {
                Mix_HaltMusic();
                Mix_FreeMusic(cuttherope_priv.music);
            }
            cuttherope_priv.music = Mix_LoadMUS(musicpath);
            Mix_PlayMusic(cuttherope_priv.music,-1);
        }
    }
    else
    if (strcmp(p2->name,"stopMusic")==0) {
        if(cuttherope_priv.music!=NULL) {
            Mix_HaltMusic();
        }
    }
    else
    if (strcmp(p2->name,"setIntforKey")==0 || strcmp(p2->name,"setBooleanforKey")==0) {
        struct dummy_jstring *key = va_arg(p3,struct dummy_jstring*);
        jint value = va_arg(p3,jint);
        KeyValue* kv = get_preference(key->data,1);
        if (kv!=0) {
            kv->value = value;
        }
        MODULE_DEBUG_PRINTF("set*forKey=%s value=%d\n",key->data, value);
    }
    else
    if (strcmp(p2->name,"exitApp")==0) {
        cuttherope_priv.want_exit = 1;
    }
}

static jobject
cuttherope_CallStaticObjectMethodV(JNIEnv *env, jclass p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("cuttherope_CallStaticObjectMethodV %s\n",p2->name);

    if (strcmp(p2->name, "getAppVersion") == 0) {
        return (*env)->NewStringUTF(env, "1.1.0");
    }
    else
    if (strcmp(p2->name,"getAsString") == 0) {
        jint param = va_arg(p3, jint);
        MODULE_DEBUG_PRINTF("   param=%d\n",param);
        return (*env)->NewStringUTF(env,"en");
        //MODULE_DEBUG_PRINTF("   string=%s\n",arg->data);
        //return arg;
    }
    return NULL;
}

static jint
cuttherope_CallIntMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("cuttherope_CallIntMethodV(%s)\n",p2->name);

    if (strcmp(p2->name,"getIntForKey")==0) {
        struct dummy_jstring *arg;
        arg = va_arg(p3, struct dummy_jstring*);
        MODULE_DEBUG_PRINTF("getIntForKey: key=%s\n",arg->data);

        KeyValue *kv = get_preference(arg->data,0);
        if (kv!=0) {
            MODULE_DEBUG_PRINTF("getIntForKey: key=%s -> %d\n",arg->data,kv->value);
            return kv->value;
        }
    }
    return 0;
}
static jboolean
cuttherope_CallBooleanMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("cuttherope_CallBooleanMethodV(%s)\n",p2->name);

    if (strcmp(p2->name,"getBooleanForKey")==0) {
        struct dummy_jstring *arg;
        arg = va_arg(p3, struct dummy_jstring*);
        MODULE_DEBUG_PRINTF("getBooleanForKey: key=%s\n",arg->data);

        KeyValue *kv = get_preference(arg->data,0);
        if (kv!=0) {
            return kv->value;
        }
    }

    return 0;
}

void*
cuttherope_GetDirectBufferAddress(JNIEnv* p0, jobject p1)
{
    MODULE_DEBUG_PRINTF("cuttherope_GetDirectBufferAddress(%x)\n",p1);
    image_t *img = p1;
    return img->data;
}

static void
extract_music_callback(const char *filename, char *buffer, size_t size)
{
    if (strstr(filename,"music")==0)
        return;

    MODULE_DEBUG_PRINTF("extract_music_callback: filename=%s\n", filename);

    char* fname = strrchr(filename,'/')+1;

    char tmp[PATH_MAX];
    strcpy(tmp, cuttherope_priv.home);
    strcat(tmp, fname);

    struct stat st;
    if (stat(tmp,&st)<0) { //dont write of not necessary ... be nice the NAND/SD card :)
        MODULE_DEBUG_PRINTF("extract_music_callback: Extract file to: %s\n", tmp);
        FILE *my_file = fopen(tmp, "wb");
        fwrite(buffer, size, 1, my_file);
        fclose(my_file);
    }
}



static int
cuttherope_try_init(struct SupportModule *self)
{
    /*
    Java_com_zeptolab_ctr_CtrRenderer_nativeResize
    Java_com_zeptolab_ctr_CtrRenderer_freeDirect
    Java_com_zeptolab_ctr_CtrRenderer_nativeMenuPressed
    Java_com_zeptolab_ctr_CtrRenderer_nativeBackPressed
    Java_com_zeptolab_ctr_CtrRenderer_nativeDrawFps
    Java_com_zeptolab_ctr_CtrRenderer_nativeCpuIndex
    Java_com_zeptolab_ctr_CtrRenderer_nativeTouchProcess
    Java_com_zeptolab_ctr_CtrRenderer_nativeTouchAdd
    Java_com_zeptolab_ctr_CtrRenderer_nativeRender
    Java_com_zeptolab_ctr_CtrRenderer_nativeTick
    Java_com_zeptolab_ctr_CtrRenderer_nativePause
    Java_com_zeptolab_ctr_CtrRenderer_nativeResume
    Java_com_zeptolab_ctr_CtrRenderer_nativeDestroy
    Java_com_zeptolab_ctr_CtrRenderer_allocDirect
    Java_com_zeptolab_ctr_CtrRenderer_nativeInit
    JNI_OnLoad
    JNI_OnUnload
    Java_com_zeptolab_ctr_CtrRenderer_nativePlaybackFinished
    Java_com_zeptolab_ctr_CtrResourceLoader_imageLoaded
    */

    /*
    Java_com_zeptolab_ctr_CtrRenderer_nativePlaybackFinished
    JNI_OnLoad
    JNI_OnUnload
    Java_com_zeptolab_ctr_billing_CtrBillingManager_purchaseTransactionStarted
    Java_com_zeptolab_ctr_billing_CtrBillingManager_purchaseCanceled
    Java_com_zeptolab_ctr_billing_CtrBillingManager_purchased
    Java_com_zeptolab_ctr_CtrRenderer_nativeResize
    Java_com_zeptolab_ctr_CtrRenderer_videoBannerFinished
    Java_com_zeptolab_ctr_CtrRenderer_freeDirect
    Java_com_zeptolab_ctr_CtrRenderer_allocDirect
    Java_com_zeptolab_ctr_CtrRenderer_nativeMenuPressed
    Java_com_zeptolab_ctr_CtrRenderer_nativeBackPressed
    Java_com_zeptolab_ctr_CtrRenderer_nativeDrawFps
    Java_com_zeptolab_ctr_CtrRenderer_nativeCpuIndex
    Java_com_zeptolab_ctr_CtrRenderer_nativeTouchProcess
    Java_com_zeptolab_ctr_CtrRenderer_nativeTouchAdd
    Java_com_zeptolab_ctr_CtrRenderer_nativeRender
    Java_com_zeptolab_ctr_CtrRenderer_nativeTick
    Java_com_zeptolab_ctr_CtrRenderer_nativePause
    Java_com_zeptolab_ctr_CtrRenderer_nativeResume
    Java_com_zeptolab_ctr_CtrRenderer_nativeDestroy
    Java_com_zeptolab_ctr_CtrRenderer_videoBannerRequestAdButton
    Java_com_zeptolab_ctr_CtrRenderer_nativeInit
    Java_com_zeptolab_ctr_CtrSaveManager_restoreBroken
    Java_com_zeptolab_ctr_CtrSaveManager_restoreSuccess
    Java_com_zeptolab_ctr_testing_ABTestingInterface_setInstance
    Java_com_zeptolab_ctr_testing_ABTestingInterface_callback
    Java_com_zeptolab_ctr_remotedata_cartoons_VideoDataManager_rebuildCartoonsSelect
    Java_com_zeptolab_ctr_CtrView_nativeInitScorer
    Java_com_zeptolab_ctr_CtrApp_nativeInitRemoteDataManager
    Java_com_zeptolab_ctr_CtrApp_nativeInitVideoDataManager
    Java_com_zeptolab_ctr_CtrResourceLoader_getLocalized
    */


    self->priv->JNI_OnLoad = (jni_onload_t)LOOKUP_M("JNI_OnLoad");
    self->priv->nativeInit = (cuttherope_init_t)LOOKUP_M("CtrRenderer_nativeInit");
    self->priv->nativeResize = (cuttherope_resize_t)LOOKUP_M("CtrRenderer_nativeResize");
    self->priv->nativeTick = (cuttherope_tick_t)LOOKUP_M("CtrRenderer_nativeTick");
    self->priv->nativeRender = (cuttherope_render_t)LOOKUP_M("CtrRenderer_nativeRender");
    self->priv->nativeResume = (cuttherope_resume_t)LOOKUP_M("CtrRenderer_nativeResume");
    self->priv->nativePause = (cuttherope_pause_t)LOOKUP_M("CtrRenderer_nativePause");
    self->priv->nativeBackPressed = (cuttherope_backpressed_t)LOOKUP_M("CtrRenderer_nativeBackPressed");
    self->priv->nativeMenuPressed = (cuttherope_menupressed_t)LOOKUP_M("CtrRenderer_nativeMenuPressed");
    self->priv->imageLoaded = (cuttherope_imageloaded_t)LOOKUP_M("CtrResourceLoader_imageLoaded");
    self->priv->nativeTouchAdd = (cuttherope_nativetouchadd_t)LOOKUP_M("CtrRenderer_nativeTouchAdd");
    self->priv->nativeTouchProcess = (cuttherope_nativetouchprocess_t)LOOKUP_M("CtrRenderer_nativeTouchProcess");
    self->priv->nativePlaybackFinished = (cuttherope_nativeplaybackfinished_t)LOOKUP_M("CtrRenderer_nativePlaybackFinished");

    self->priv->global = GLOBAL_M;

    self->override_env.CallObjectMethodV = cuttherope_CallObjectMethodV;
    self->override_env.CallVoidMethodV = cuttherope_CallVoidMethodV;
    self->override_env.CallStaticObjectMethodV = cuttherope_CallStaticObjectMethodV;
    self->override_env.CallIntMethodV = cuttherope_CallIntMethodV;
    self->override_env.GetDirectBufferAddress = cuttherope_GetDirectBufferAddress;
    self->override_env.CallBooleanMethodV = cuttherope_CallBooleanMethodV;

    return (self->priv->JNI_OnLoad != NULL &&
            self->priv->nativeInit != NULL &&
            self->priv->nativeResize != NULL &&
            self->priv->nativeTick != NULL &&
            self->priv->nativeRender != NULL &&
            self->priv->nativeBackPressed != NULL &&
            self->priv->nativeMenuPressed != NULL &&
            self->priv->imageLoaded != NULL &&
            self->priv->nativeTouchAdd != NULL &&
            self->priv->nativeTouchProcess != NULL &&
            self->priv->nativePlaybackFinished != NULL
            );
}

static void
cuttherope_init(struct SupportModule *self, int width, int height, const char *home)
{
    // home is req. in extract callback
    self->priv->home = strdup(home);

    // init sound stuff
    Mix_Init(MIX_INIT_OGG);

    int audio_rate = 22050;
    uint16_t audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 1024;

    if(Mix_OpenAudio(audio_rate,audio_format,audio_channels,audio_buffers)<0)
    {
        printf("Mix_OpenAudio failed %s.\n",Mix_GetError());
        exit(-1);
    }

    Mix_AllocateChannels(16);

    memset(self->priv->sounds,0,sizeof(self->priv->sounds));
    self->priv->music = NULL;
    self->priv->musicpath[0] = 0;

    GLOBAL_M->foreach_file("assets/", extract_music_callback);

    load_preferences();

    // init the game
    void *resourceLoader = (void*)0xF00;
    void *soundManager = (void*)0xF01;
    void *preferences = (void*)0xF02;
    void *saveManager = (void*)0xF03;
    void *flurry = (void*)0xF04;
    void *videoPlayer = (void*)0xF05;
    void *scorer = (void*)0xF06;
    void *billingInterface = (void*)0xF07;
    void *remoteDataManager = (void*)0xF08;

    memset(self->priv->images,0,sizeof(self->priv->images));

    self->priv->JNI_OnLoad(VM_M, NULL);

    self->priv->nativeResize(ENV_M, GLOBAL_M, height, width);
    self->priv->global->module_hacks->gles_landscape_to_portrait = 1;

#ifdef PANDORA
    self->priv->global->module_hacks->gles_downscale_images = 1;
    self->priv->global->module_hacks->gles_no_readpixels = 1;
#endif

    self->priv->nativeInit(ENV_M, GLOBAL_M, resourceLoader, soundManager, preferences,
            saveManager, flurry, videoPlayer, scorer,
            billingInterface, remoteDataManager, 0);

    self->priv->nativeResume(ENV_M, GLOBAL_M);
}

static void
cuttherope_input(struct SupportModule *self, int event, int x, int y, int finger)
{
    if(self->global->module_hacks->gles_landscape_to_portrait)
    {
        int tmpx = x;
        x = 480-y;
        y = tmpx;
    }

    if (event==ACTION_MOVE) {
        self->priv->nativeTouchAdd(ENV_M,GLOBAL_M,0,2,x,y);
    }
    else
    if (event==ACTION_UP) {
        self->priv->nativeTouchAdd(ENV_M,GLOBAL_M,0,0,x,y);
    }
    else
    if (event==ACTION_DOWN) {
        self->priv->nativeTouchAdd(ENV_M,GLOBAL_M,0,1,x,y);
    }

    self->priv->nativeTouchProcess(ENV_M,GLOBAL_M);
}

static void
cuttherope_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
    if (event == ACTION_DOWN) {
        switch (keycode) {
        case AKEYCODE_BACK:
            self->priv->nativeBackPressed(ENV_M, GLOBAL_M);
            break;

        case AKEYCODE_MENU:
            self->priv->nativeMenuPressed(ENV_M, GLOBAL_M);
            break;
        }
    }
}

static void
cuttherope_update(struct SupportModule *self)
{
    self->priv->nativeTick(ENV_M, GLOBAL_M, 16);
    self->priv->nativeRender(ENV_M, GLOBAL_M);
}

static void
cuttherope_deinit(struct SupportModule *self)
{
    Mix_CloseAudio();

    save_preferences(); //not saving here in harmattan
}

static void
cuttherope_pause(struct SupportModule *self)
{
}

static void
cuttherope_resume(struct SupportModule *self)
{
}

static int
cuttherope_requests_exit(struct SupportModule *self)
{
    return cuttherope_priv.want_exit;
}

APKENV_MODULE(cuttherope, MODULE_PRIORITY_GAME)

