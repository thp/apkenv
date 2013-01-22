
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

#include "common.h"
#include "../imagelib/imagelib.h"
#include <linux/limits.h>


#ifdef APKENV_DEBUG
#  define MODULE_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define MODULE_DEBUG_PRINTF(...)
#endif

#include <SDL/SDL.h>

typedef void (*cuttherope_init_t)(JNIEnv *env, jobject obj, jobject resourceLoader, jobject soundManager, jobject preferences,
            jobject saveManager, jobject flurry, jobject videoPlayer, jobject scorer,
            jobject billingInterface, jobject remoteDataManager, jint x) SOFTFP;
typedef void (*cuttherope_resize_t)(JNIEnv *env, jobject object, jint width, jint height) SOFTFP;
typedef void (*cuttherope_tick_t)(JNIEnv *env, jobject object, jlong tick) SOFTFP;
typedef void (*cuttherope_render_t)(JNIEnv *env, jobject object) SOFTFP;
typedef void (*cuttherope_videobannerfinished_t)(JNIEnv *env, jobject object) SOFTFP;
typedef void (*cuttherope_resume_t)(JNIEnv*,jobject) SOFTFP;
typedef void (*cuttherope_imageloaded_t)(JNIEnv*,jobject,jint p1, jbyteArray p2, jint p3, jint p4) SOFTFP;
typedef void (*cuttherope_nativetouchadd_t)(JNIEnv *env, jobject p0, jint p1, jint p2, jfloat p3, jfloat p4) SOFTFP;
typedef void (*cuttherope_nativetouchprocess_t)(JNIEnv *env, jobject p0) SOFTFP;
typedef void (*cuttherope_nativeplaybackfinished_t)(JNIEnv *env, jobject p0, jint p1);


struct SupportModulePriv {
    jni_onload_t JNI_OnLoad;
    cuttherope_init_t nativeInit;
    cuttherope_resize_t nativeResize;
    cuttherope_tick_t nativeTick;
    cuttherope_render_t nativeRender;
    cuttherope_videobannerfinished_t videoBannerFinished;
    cuttherope_resume_t nativeResume;
    cuttherope_imageloaded_t imageLoaded;
    cuttherope_nativetouchadd_t nativeTouchAdd;
    cuttherope_nativetouchprocess_t nativeTouchProcess;
    cuttherope_nativeplaybackfinished_t nativePlaybackFinished;
    struct GlobalState *global;
    const char* home;
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

    if (strcmp(p2->name, "loadData") == 0) {
        struct dummy_jstring *arg;
        arg = va_arg(p3, struct dummy_jstring*);
        jboolean flag = va_arg(p3, jint);
        (void)flag;

        char *filename = malloc(strlen(arg->data) + strlen("assets/") + 1);
        sprintf(filename, "assets/%s", arg->data);

        MODULE_DEBUG_PRINTF("   data=%s\n",arg->data);

        char *buf;
        size_t buf_size;
        if (GLOBAL_J(env)->read_file(filename, &buf, &buf_size)) {
            struct dummy_byte_array *array;
            array = malloc(sizeof(struct dummy_byte_array));
            array->data = buf;
            array->size = buf_size;

            MODULE_DEBUG_PRINTF("   data=%s size=%d\n",arg->data, buf_size);

	    // Poll for events here to avoid "not responding" messages
            SDL_Event e;
            while (SDL_PollEvent(&e));

            return array;
        } else {
            MODULE_DEBUG_PRINTF("   data=%s not found\n",arg->data);
            return GLOBAL_J(env);
        }
    }
    else if (strcmp(p2->name, "getBytesOfBitmap") == 0) {

        char *buf;
        size_t buf_size;

        if (GLOBAL_J(env)->read_file("assets/zeptolab.png", &buf, &buf_size)) {
            struct dummy_byte_array *array;
            array = malloc(sizeof(struct dummy_byte_array));
            array->data = buf;
            array->size = buf_size;
            return array;
        }
    }
    else if (strcmp(p2->name, "getQuadsOfBitmap")==0) {
        struct dummy_float_array *array;
        array = malloc(sizeof(struct dummy_float_array));
        array->data = malloc(4*sizeof(jfloat)); memset(array->data,0,sizeof(jfloat)*4);
        array->size = 4;
        return array;

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

static void
cuttherope_CallVoidMethodV(JNIEnv* env, jobject p1, jmethodID p2, va_list p3)
{
    if (!p2)
        return;

    MODULE_DEBUG_PRINTF("cuttherope_CallVoidMethodV %s\n",p2->name);

    if (strcmp(p2->name,"loadImage")==0) {//public void loadImage(String paramString, int paramInt)

        imageloadersettings_t loadsettings =
        {
            .swapy = 0,
            .forcergba = 1,
            .alpha = 255,
            .swaprb = 1,
        };

        struct dummy_jstring *filename = va_arg(p3, struct dummy_jstring*);
        jint fileId =  va_arg(p3, jint);

        MODULE_DEBUG_PRINTF("CallVoidMethodV(%s): filename=%s fileId=%d\n",p2->name,filename->data,fileId);

        char *buf;
        size_t buf_size;

        char filepath[PATH_MAX]; sprintf(filepath,"assets/%s",filename->data);

        if (GLOBAL_J(env)->read_file(filepath, &buf, &buf_size)) {

            MODULE_DEBUG_PRINTF("CallVoidMethodV(%s): filename=%s ok\n",p2->name,filename->data);

            sprintf(filepath,"%s/%s",cuttherope_priv.home,filename->data);
            FILE *fp =fopen(filepath,"wb");
            if (fp!=0) {
                fwrite(buf, buf_size, 1, fp);
                fclose(fp);

                image_t* image = 0;
                if (strstr(filepath,".png")!=0) image = loadpng(filepath,loadsettings);
                if (strstr(filepath,".jpeg")!=0) image = loadjpeg(filepath,loadsettings);
                if (image!=0) {

                    MODULE_DEBUG_PRINTF("CallVoidMethodV(%s): imageLoaded=%s (%d,%d,%d) (%x)\n",p2->name,filename->data,image->width,image->height,image->bpp,image);

                    cuttherope_priv.imageLoaded(ENV(cuttherope_priv.global),cuttherope_priv.global,fileId,image,image->width,image->height);
                }
            }
        }
    }
    else
    if (strcmp(p2->name,"loadSound")==0) {  //public void loadSound(String paramString, int paramInt)
        struct dummy_jstring *filename = va_arg(p3, struct dummy_jstring*);
        jint paramInt =  va_arg(p3, jint);

        MODULE_DEBUG_PRINTF("cuttherope_CallVoidMethodV(%s): filename=%s param=%d\n",p2->name,filename->data,paramInt);

    }
    else
    if ( strcmp(p2->name, "showBanner") == 0) {
        cuttherope_video_banner_finished();
    }
    else
    if (strcmp(p2->name,"playVideo")==0) {

        struct dummy_jstring *filename = va_arg(p3, struct dummy_jstring*);
        jint paramInt =  va_arg(p3, jint);

        if (cuttherope_priv.nativePlaybackFinished)
            cuttherope_priv.nativePlaybackFinished(ENV(cuttherope_priv.global),
                    cuttherope_priv.global,paramInt);
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
    //MODULE_DEBUG_PRINTF("JNIEnv_CallIntMethodV(%s)\n",p2->name);
    if (strcmp(p2->name,"getIntForKey")==0) {
        struct dummy_jstring *arg;
        arg = va_arg(p3, struct dummy_jstring*);
        MODULE_DEBUG_PRINTF("   key=%s\n",arg->data);

        if (strstr(arg->data,"UNLOCKED_")!=0)
            return 1;
        if (strstr(arg->data,"STARS_")!=0)
            return 3;
        if (strstr(arg->data,"SCORE_")!=0)
            return 1;
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

    return (self->priv->JNI_OnLoad != NULL &&
            self->priv->nativeInit != NULL &&
            self->priv->nativeResize != NULL &&
            self->priv->nativeTick != NULL &&
            self->priv->nativeRender != NULL &&
            //self->priv->imageLoaded != NULL &&
            self->priv->nativeTouchAdd != NULL &&
            self->priv->nativeTouchProcess != NULL &&
            self->priv->nativePlaybackFinished != NULL
            );
}

static void
cuttherope_init(struct SupportModule *self, int width, int height, const char *home)
{
    void *resourceLoader = (void*)0xF00;
    void *soundManager = (void*)0xF01;
    void *preferences = (void*)0xF02;
    void *saveManager = (void*)0xF03;
    void *flurry = (void*)0xF04;
    void *videoPlayer = (void*)0xF05;
    void *scorer = (void*)0xF06;
    void *billingInterface = (void*)0xF07;
    void *remoteDataManager = (void*)0xF08;

    self->priv->home = strdup(home);

    self->priv->JNI_OnLoad(VM_M, NULL);

#ifdef PANDORA
    self->priv->nativeResize(ENV_M, GLOBAL_M,  height, width); //$$$ landscape-to-portrait pandora fix
#else
    self->priv->nativeResize(ENV_M, GLOBAL_M,  width, height); //$$$ landscape-to-portrait pandora fix
#endif

    self->priv->nativeInit(ENV_M, GLOBAL_M, resourceLoader, soundManager, preferences,
            saveManager, flurry, videoPlayer, scorer,
            billingInterface, remoteDataManager, 0);

    self->priv->nativeResume(ENV_M, GLOBAL_M);
}

static void
cuttherope_input(struct SupportModule *self, int event, int x, int y, int finger)
{
#ifdef PANDORA
     //$$$ landscape-to-portrait pandora fix
    x = 800-x;
    y = y;
    int tmp = x; x = y; y = tmp;
#endif

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
cuttherope_update(struct SupportModule *self)
{
    self->priv->nativeTick(ENV_M, GLOBAL_M, 16);
    self->priv->nativeRender(ENV_M, GLOBAL_M);
}

static void
cuttherope_deinit(struct SupportModule *self)
{
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
    return 0;
}

APKENV_MODULE(cuttherope, MODULE_PRIORITY_GAME)

