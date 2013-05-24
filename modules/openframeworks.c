
/**
 * openFrameworks module
 * Copyright (c) 2013, Gražvydas "notaz" Ignotas <notasas@gmail.com>
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <SDL/SDL_mixer.h>

#include "common.h"

/* from OF 0.7.4 source */
typedef void (*openframeworks_setAppDataDir_t)(JNIEnv *env, jobject thiz,
                        jstring data_dir, jstring app_name) SOFTFP;
typedef void (*openframeworks_init_t)(JNIEnv *env, jobject thiz) SOFTFP;
typedef void (*openframeworks_setup_t)(JNIEnv *env, jclass thiz, jint w, jint h) SOFTFP;
typedef void (*openframeworks_render_t)(JNIEnv *env, jclass thiz) SOFTFP;
typedef void (*openframeworks_update_t)(JNIEnv *env, jclass thiz) SOFTFP;
typedef void (*openframeworks_onSurfaceCreated_t)(JNIEnv *env, jclass thiz) SOFTFP;
typedef void (*openframeworks_onSurfaceDestroyed_t)(JNIEnv *env, jclass thiz) SOFTFP;
typedef void (*openframeworks_resize_t)(JNIEnv *env, jclass thiz, jint w, jint h) SOFTFP;
typedef void (*openframeworks_onPause_t)(JNIEnv *env, jobject thiz) SOFTFP;
typedef void (*openframeworks_onResume_t)(JNIEnv *env, jobject thiz) SOFTFP;
typedef void (*openframeworks_onTouchDown_t)(JNIEnv *env, jclass thiz, jint id,
                        jfloat x, jfloat y, jfloat pressure) SOFTFP;
typedef void (*openframeworks_onTouchUp_t)(JNIEnv *env, jclass thiz, jint id,
                        jfloat x, jfloat y, jfloat pressure) SOFTFP;
typedef void (*openframeworks_onTouchMoved_t)(JNIEnv *env, jclass thiz, jint id,
                        jfloat x, jfloat y, jfloat pressure) SOFTFP;
typedef void (*openframeworks_onKeyDown_t)(JNIEnv *env, jclass thiz, jint keyCode);
typedef void (*openframeworks_onKeyUp_t)(JNIEnv *env, jclass thiz, jint keyCode);

struct SupportModulePriv {
    jni_onload_t JNI_OnLoad;
    openframeworks_setAppDataDir_t setAppDataDir;
    openframeworks_init_t init;
    openframeworks_setup_t setup;
    openframeworks_render_t render;
    openframeworks_onSurfaceCreated_t onSurfaceCreated;
    openframeworks_onSurfaceDestroyed_t onSurfaceDestroyed;
    openframeworks_resize_t resize;
    openframeworks_onPause_t onPause;
    openframeworks_onResume_t onResume;
    openframeworks_onTouchDown_t onTouchDown;
    openframeworks_onTouchUp_t onTouchUp;
    openframeworks_onTouchMoved_t onTouchMoved;
    openframeworks_onKeyDown_t onKeyDown;
    openframeworks_onKeyUp_t onKeyUp;
    openframeworks_update_t update; // SuperHexagon specific
    const char *app_name;
    char *home;
    int w, h;
};
static struct SupportModulePriv openframeworks_priv;

static void publish_superhexagon_score(int level, int score);

struct player_sound {
    Mix_Music *music;
    Mix_Chunk *chunk;
    int loop;
    int music_playing;
    int chunk_channel;
};

struct _jobject {
    struct dummy_jclass *class;
    void *priv;
};

jobject
JNIEnv_NewObjectV(JNIEnv *env, jclass p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("module_NewObjectV(%p, %s)\n", p1, p2->name);
    struct _jobject *object = malloc(sizeof(*object));
    object->class = p1;
    object->priv = NULL;
    return (jobject)object;
}

void
JNIEnv_CallVoidMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("module_JNIEnv_CallVoidMethodV(%x, %s, %s)\n", p1, p2->name, p2->sig);

    if (strcmp(p2->name, "reportScore") == 0
       && strcmp(openframeworks_priv.app_name, "Super Hexagon") == 0)
    {
        int level = va_arg(p3, int);
        int score = va_arg(p3, int); // in 1/60 sec (frame?) units
        printf("level %d hiscore: %d:%02d\n", level, score / 60, score % 60);
        publish_superhexagon_score(level, score);
        return;
    }

    /* handle OFAndroidSoundPlayer class */
    if (p1 == NULL || p1 == GLOBAL_J(env))
        return;

    struct _jobject *obj = p1;

    if (strcmp(obj->class->name, "cc/openframeworks/OFAndroidSoundPlayer") != 0)
        return;

    struct player_sound *player = obj->priv;

    if (strcmp(p2->name, "loadSound") == 0)
    {
        struct dummy_jstring *arg = va_arg(p3, struct dummy_jstring *);
        MODULE_DEBUG_PRINTF("loadSound %s, obj %p\n", arg->data, obj);
        struct player_sound *player = calloc(1, sizeof(*player));
        const char *ext = arg->data + strlen(arg->data) - 3;
        if (strcasecmp(ext, "wav") == 0)
            player->chunk = Mix_LoadWAV(arg->data);
        else
            player->music = Mix_LoadMUS(arg->data);
        player->loop = 0;
        player->chunk_channel = -1;
        obj->priv = player;
    }
    else if (strcmp(p2->name, "setLoop") == 0)
    {
        int arg = va_arg(p3, int);
        MODULE_DEBUG_PRINTF("setLoop %d, obj %p\n", arg, obj);
        player->loop = arg;
    }
    else if (strcmp(p2->name, "play") == 0)
    {
        MODULE_DEBUG_PRINTF("play obj %p\n", obj);
        if (player->chunk != NULL)
            player->chunk_channel =
                Mix_PlayChannel(-1, player->chunk, player->loop);
        else if (player->music != NULL) {
            Mix_PlayMusic(player->music, player->loop ? -1 : 1);
            player->music_playing = 1;
        } else
            fprintf(stderr, "play issued with nothing set to play\n");
    }
    else if (strcmp(p2->name, "stop") == 0)
    {
        MODULE_DEBUG_PRINTF("stop obj %p\n", obj);
        if (player->chunk != NULL) {
            if (player->chunk_channel >= 0) {
                Mix_HaltChannel(player->chunk_channel);
                player->chunk_channel = -1;
            }
        } else {
            Mix_HaltMusic();
            player->music_playing = 0;
        }
    }
    else if (strcmp(p2->name, "setPositionMS") == 0)
    {
        jint arg = va_arg(p3, jint);
        MODULE_DEBUG_PRINTF("setPositionMS %d, obj %p\n", arg, obj);
        if (player->music != NULL && player->music_playing) {
            // note: SDL_mixer has seek broken when compiled with tremor
            // http://bugzilla.libsdl.org/show_bug.cgi?id=1807
            Mix_SetMusicPosition((double)arg / 1000.0);
        }
    }
    else if (strcmp(p2->name, "setVolume") == 0)
    {
        double arg = va_arg(p3, double);
        MODULE_DEBUG_PRINTF("setVolume %.3f, obj %p\n", arg, obj);
        int sdl_volume = (int)(arg * MIX_MAX_VOLUME);
        if (player->chunk != NULL)
            Mix_VolumeChunk(player->chunk, sdl_volume);
        else if (player->music != NULL && player->music_playing)
            Mix_VolumeMusic(sdl_volume);
    }
}

/* SuperHexagon specific */
static void
hexagon_extract(const char *filename, char *buffer, size_t size)
{
    char cmd[1024];
    FILE *f;

    // XXX: could use internal unzip instead..
    snprintf(cmd, sizeof(cmd),
        "t=$(tempfile); cat > $t; unzip $t -d \"%s\"; rm -f $t",
        openframeworks_priv.home);
    f = popen(cmd, "w");
    if (f == NULL) {
        fprintf(stderr, "zip extraction failed!\n");
        return;
    }

    fwrite(buffer, 1, size, f);
    fclose(f);
}

#ifdef PANDORA
#include <sys/wait.h>

static void
publish_superhexagon_score(int level, int score)
{
    static int sent_scores[6], prev_level, prev_score;
    static pid_t pid = -1;
    int status;

    if ((unsigned int)level >= 6)
        return;

    if (pid != -1) {
        waitpid(pid, &status, 0);
        pid = -1;
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            sent_scores[prev_level] = prev_score;
        else
            printf("last WEXITSTATUS %d\n", WEXITSTATUS(status));
    }

    /* We always get highest score, not current, so only send once */
    if (sent_scores[level] == score)
        return;

    prev_level = level;
    prev_score = score;

    pid = fork();
    if (pid == 0) {
        char name[16] = "superhexagon\0";
        char sscore[16];
        if (level > 0)
            name[12] = '0' + level + 1;
        snprintf(sscore, sizeof(sscore), "%u", score);
        printf("sending score..\n");
        execl("./sc", "./sc", "so", "push", name, "pandora",
              sscore, (char *)NULL);
        perror("execl");
        _exit(1);
    }
}
#else
static void publish_superhexagon_score(int level, int score) {}
#endif

#define LOOKUP_OF_SYM(var) \
    self->priv->var = (openframeworks_##var##_t)LOOKUP_M( \
        "Java_cc_openframeworks_OFAndroid_" #var); \
    if (self->priv->var == NULL) \
        failed_syms = 1

static int
openframeworks_try_init(struct SupportModule *self)
{
    int failed_syms = 0;

    self->priv->JNI_OnLoad = (jni_onload_t)LOOKUP_M("JNI_OnLoad");
    LOOKUP_OF_SYM(setAppDataDir);
    LOOKUP_OF_SYM(init);
    LOOKUP_OF_SYM(setup);
    LOOKUP_OF_SYM(render);
    LOOKUP_OF_SYM(onSurfaceCreated);
    LOOKUP_OF_SYM(onSurfaceDestroyed);
    LOOKUP_OF_SYM(resize);
    LOOKUP_OF_SYM(onPause);
    LOOKUP_OF_SYM(onResume);
    LOOKUP_OF_SYM(onTouchDown);
    LOOKUP_OF_SYM(onTouchUp);
    LOOKUP_OF_SYM(onTouchMoved);
    LOOKUP_OF_SYM(onKeyDown);
    LOOKUP_OF_SYM(onKeyUp);

    /* SuperHexagon specific */    
    self->priv->update = (openframeworks_update_t)LOOKUP_M(
        "Java_cc_openframeworks_OFAndroid_update");

    self->override_env.NewObjectV = JNIEnv_NewObjectV;
    self->override_env.CallVoidMethodV = JNIEnv_CallVoidMethodV;

    return (self->priv->JNI_OnLoad != NULL) && !failed_syms;
}

static void
openframeworks_resume(struct SupportModule *self);

static void
openframeworks_init(struct SupportModule *self, int width, int height, const char *home)
{
    char buf[1024];
    struct stat st;

    openframeworks_priv.app_name = GLOBAL_M->lookup_resource("app_name");
    openframeworks_priv.home = strdup(home);
    openframeworks_priv.w = width;
    openframeworks_priv.h = height;

    if (strcmp(openframeworks_priv.app_name, "Super Hexagon") == 0) {
        /* do SuperHexagonSplash's work */
        snprintf(buf, sizeof(buf), "%s/data", home);
        if (stat(buf, &st) != 0)
            GLOBAL_M->foreach_file("res/raw/", hexagon_extract);
    }

    /* init sound */
    Mix_Init(MIX_INIT_OGG);

    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) < 0)
    {
        fprintf(stderr, "Mix_OpenAudio failed: %s.\n", Mix_GetError());
        exit(-1);
    }

    /* note: order is important here */
    self->priv->JNI_OnLoad(VM_M, NULL);
    self->priv->setAppDataDir(ENV_M, GLOBAL_M,
        GLOBAL_M->env->NewStringUTF(ENV_M, home),
        GLOBAL_M->env->NewStringUTF(ENV_M, openframeworks_priv.app_name));
    self->priv->init(ENV_M, GLOBAL_M);
    self->priv->setup(ENV_M, GLOBAL_M, width, height);
    self->priv->onPause(ENV_M, GLOBAL_M); // must pause for resume to work
    openframeworks_resume(self);
}

static void
openframeworks_input(struct SupportModule *self, int event, int x, int y, int finger)
{
    jfloat jx = (jfloat)x;
    jfloat jy = (jfloat)y;

    switch (event) {
    case ACTION_DOWN:
        self->priv->onTouchDown(ENV_M, GLOBAL_M, finger, jx, jy, 1.0f);
        break;
    case ACTION_UP:
        self->priv->onTouchUp(ENV_M, GLOBAL_M, finger, jx, jy, 0.0f);
        break;
    case ACTION_MOVE:
        self->priv->onTouchMoved(ENV_M, GLOBAL_M, finger, jx, jy, 1.0f);
        break;
    }
}

static void
openframeworks_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
    switch (event) {
    case ACTION_DOWN:
        self->priv->onKeyDown(ENV_M, GLOBAL_M, keycode);
        break;
    case ACTION_UP:
        self->priv->onKeyUp(ENV_M, GLOBAL_M, keycode);
        break;
    }
}

static void
openframeworks_update(struct SupportModule *self)
{
    if (self->priv->update)
        self->priv->update(ENV_M, GLOBAL_M);

    self->priv->render(ENV_M, GLOBAL_M);
}

static void
openframeworks_pause(struct SupportModule *self)
{
    self->priv->onPause(ENV_M, GLOBAL_M);
    self->priv->onSurfaceDestroyed(ENV_M, GLOBAL_M);
}

static void
openframeworks_resume(struct SupportModule *self)
{
    self->priv->onSurfaceCreated(ENV_M, GLOBAL_M);
    self->priv->resize(ENV_M, GLOBAL_M, self->priv->w, self->priv->h);
    self->priv->onResume(ENV_M, GLOBAL_M);
}

static void
openframeworks_deinit(struct SupportModule *self)
{
    /* onStop(), onDestroy(), exit() never called? */
    openframeworks_pause(self);

    Mix_CloseAudio();
}

static int
openframeworks_requests_exit(struct SupportModule *self)
{
    return 0;
}

APKENV_MODULE(openframeworks, MODULE_PRIORITY_ENGINE)

// vim:shiftwidth=4:expandtab
