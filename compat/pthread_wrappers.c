
/**
 * apkenv
 * Copyright (c) 2012, Thomas Perl <m@thp.io>
 * Based on code from libhybris: Copyright (c) 2012 Carsten Munk
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

#include "pthread_wrappers.h"
#include <sys/time.h>
#include <errno.h>

#ifdef APKENV_DEBUG
#  define WRAPPERS_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define WRAPPERS_DEBUG_PRINTF(...)
#endif

/* Base address to check for Android specifics */
#define ANDROID_TOP_ADDR_VALUE_MUTEX  0xFFFF
#define ANDROID_TOP_ADDR_VALUE_COND   0xFFFF
#define ANDROID_TOP_ADDR_VALUE_RWLOCK 0xFFFF

#define ANDROID_MUTEX_SHARED_MASK      0x2000
#define ANDROID_COND_SHARED_MASK       0x0001
#define ANDROID_RWLOCKATTR_SHARED_MASK 0x0010

/* For the static initializer types */
#define ANDROID_PTHREAD_MUTEX_INITIALIZER            0
#define ANDROID_PTHREAD_RECURSIVE_MUTEX_INITIALIZER  0x4000
#define ANDROID_PTHREAD_ERRORCHECK_MUTEX_INITIALIZER 0x8000
#define ANDROID_PTHREAD_COND_INITIALIZER             0
#define ANDROID_PTHREAD_RWLOCK_INITIALIZER           0

/* Debug */
#define LOGD(message, ...) WRAPPERS_DEBUG_PRINTF(message, ##__VA_ARGS__)

#include "hooks_shm.h"

/* Helpers */
int apkenv_check_android_shared_mutex(unsigned int mutex_addr)
{
    /* If not initialized or initialized by Android, it should contain a low
     * address, which is basically just the int values for Android's own
     * pthread_mutex_t */
    if ((mutex_addr <= ANDROID_TOP_ADDR_VALUE_MUTEX) &&
                    (mutex_addr & ANDROID_MUTEX_SHARED_MASK))
        return 1;

    return 0;
}

int apkenv_check_android_shared_cond(unsigned int cond_addr)
{
    /* If not initialized or initialized by Android, it should contain a low
     * address, which is basically just the int values for Android's own
     * pthread_cond_t */
    if ((cond_addr <= ANDROID_TOP_ADDR_VALUE_COND) &&
                    (cond_addr & ANDROID_COND_SHARED_MASK))
        return 1;

    return 0;
}

void apkenv_set_mutex_attr(unsigned int android_value, pthread_mutexattr_t *attr)
{
    /* Init already sets as PTHREAD_MUTEX_NORMAL */
    pthread_mutexattr_init(attr);

    if (android_value & ANDROID_PTHREAD_RECURSIVE_MUTEX_INITIALIZER) {
        pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE);
    } else if (android_value & ANDROID_PTHREAD_ERRORCHECK_MUTEX_INITIALIZER) {
        pthread_mutexattr_settype(attr, PTHREAD_MUTEX_ERRORCHECK);
    }
}

pthread_mutex_t* apkenv_alloc_init_mutex(unsigned int android_mutex)
{
    pthread_mutex_t *realmutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutexattr_t attr;
    apkenv_set_mutex_attr(android_mutex, &attr);
    pthread_mutex_init(realmutex, &attr);
    return realmutex;
}

pthread_cond_t* apkenv_alloc_init_cond(void)
{
    pthread_cond_t *realcond = malloc(sizeof(pthread_cond_t));
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    pthread_cond_init(realcond, &attr);
    return realcond;
}

pthread_rwlock_t* apkenv_alloc_init_rwlock(void)
{
    pthread_rwlock_t *realrwlock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlock_init(realrwlock, &attr);
    return realrwlock;
}

/*
 * Main pthread functions
 *
 * Custom implementations to workaround difference between Bionic and Glibc.
 * Our own pthread_create helps avoiding direct handling of TLS.
 *
 * */

int apkenv_my_pthread_create(pthread_t *thread, const pthread_attr_t *__attr,
                             void *(*start_routine)(void*), void *arg)
{
    pthread_attr_t *realattr = NULL;

    if (__attr != NULL)
        realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    printf("%x %x %x\n",thread,__attr, arg);
    return pthread_create(thread, realattr, start_routine, arg);
}

/*
 * pthread_attr_* functions
 *
 * Specific implementations to workaround the differences between at the
 * pthread_attr_t struct differences between Bionic and Glibc.
 *
 * */

int apkenv_my_pthread_attr_init(pthread_attr_t *__attr)
{
    pthread_attr_t *realattr;

    realattr = malloc(sizeof(pthread_attr_t));
    *((unsigned int *)__attr) = (unsigned int) realattr;

    return pthread_attr_init(realattr);
}

int apkenv_my_pthread_attr_destroy(pthread_attr_t *__attr)
{
    int ret;
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;

    ret = pthread_attr_destroy(realattr);
    /* We need to release the memory allocated at apkenv_my_pthread_attr_init
     * Possible side effects if destroy is called without our init */
    free(realattr);

    return ret;
}

int apkenv_my_pthread_attr_setdetachstate(pthread_attr_t *__attr, int state)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_setdetachstate(realattr, state);
}

int apkenv_my_pthread_attr_getdetachstate(pthread_attr_t const *__attr, int *state)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_getdetachstate(realattr, state);
}

int apkenv_my_pthread_attr_setschedpolicy(pthread_attr_t *__attr, int policy)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_setschedpolicy(realattr, policy);
}

int apkenv_my_pthread_attr_getschedpolicy(pthread_attr_t const *__attr, int *policy)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_getschedpolicy(realattr, policy);
}

int apkenv_my_pthread_attr_setschedparam(pthread_attr_t *__attr, struct sched_param const *param)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_setschedparam(realattr, param);
}

int apkenv_my_pthread_attr_getschedparam(pthread_attr_t const *__attr, struct sched_param *param)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_getschedparam(realattr, param);
}

int apkenv_my_pthread_attr_setstacksize(pthread_attr_t *__attr, size_t stack_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_setstacksize(realattr, stack_size);
}

int apkenv_my_pthread_attr_getstacksize(pthread_attr_t const *__attr, size_t *stack_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_getstacksize(realattr, stack_size);
}

int apkenv_my_pthread_attr_setstackaddr(pthread_attr_t *__attr, void *stack_addr)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_setstackaddr(realattr, stack_addr);
}

int apkenv_my_pthread_attr_getstackaddr(pthread_attr_t const *__attr, void **stack_addr)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_getstackaddr(realattr, stack_addr);
}

int apkenv_my_pthread_attr_setstack(pthread_attr_t *__attr, void *stack_base, size_t stack_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_setstack(realattr, stack_base, stack_size);
}

int apkenv_my_pthread_attr_getstack(pthread_attr_t const *__attr, void **stack_base, size_t *stack_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_getstack(realattr, stack_base, stack_size);
}

int apkenv_my_pthread_attr_setguardsize(pthread_attr_t *__attr, size_t guard_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_setguardsize(realattr, guard_size);
}

int apkenv_my_pthread_attr_getguardsize(pthread_attr_t const *__attr, size_t *guard_size)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_getguardsize(realattr, guard_size);
}

int apkenv_my_pthread_attr_setscope(pthread_attr_t *__attr, int scope)
{
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;
    return pthread_attr_setscope(realattr, scope);
}

int apkenv_my_pthread_attr_getscope(pthread_attr_t const *__attr)
{
    int scope;
    pthread_attr_t *realattr = (pthread_attr_t *) *(unsigned int *) __attr;

    /* Android doesn't have the scope attribute because it always
     * returns PTHREAD_SCOPE_SYSTEM */
    pthread_attr_getscope(realattr, &scope);

    return scope;
}

int apkenv_my_pthread_getattr_np(pthread_t thid, pthread_attr_t *__attr)
{
    pthread_attr_t *realattr;

    realattr = malloc(sizeof(pthread_attr_t));
    *((unsigned int *)__attr) = (unsigned int) realattr;

    return pthread_getattr_np(thid, realattr);
}

/*
 * pthread_mutex* functions
 *
 * Specific implementations to workaround the differences between at the
 * pthread_mutex_t struct differences between Bionic and Glibc.
 *
 * */

int apkenv_my_pthread_mutex_init(pthread_mutex_t *__mutex,
                          __const pthread_mutexattr_t *__mutexattr)
{
    pthread_mutex_t *realmutex = NULL;

    int pshared = 0;
    if (__mutexattr)
        pthread_mutexattr_getpshared(__mutexattr, &pshared);

    if (!pshared) {
        /* non shared, standard mutex: use malloc */
        realmutex = malloc(sizeof(pthread_mutex_t));

        *((unsigned int *)__mutex) = (unsigned int) realmutex;
    }
    else {
        /* process-shared mutex: use the shared memory segment */
        apkenv_shm_pointer_t handle = apkenv_shm_alloc(sizeof(pthread_mutex_t));

        *((apkenv_shm_pointer_t *)__mutex) = handle;

        if (handle)
            realmutex = (pthread_mutex_t *)apkenv_get_shmpointer(handle);
    }

    return pthread_mutex_init(realmutex, __mutexattr);
}

int apkenv_my_pthread_mutex_destroy(pthread_mutex_t *__mutex)
{
    int ret;

    if (!__mutex)
        return EINVAL;

    pthread_mutex_t *realmutex = (pthread_mutex_t *) *(unsigned int *) __mutex;

    if (!realmutex)
        return EINVAL;

    if (!apkenv_is_pointer_in_shm((void*)realmutex)) {
        ret = pthread_mutex_destroy(realmutex);
        free(realmutex);
    }
    else {
        realmutex = (pthread_mutex_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)realmutex);
        ret = pthread_mutex_destroy(realmutex);
    }

    *((unsigned int *)__mutex) = 0;

    return ret;
}

int apkenv_my_pthread_mutex_lock(pthread_mutex_t *__mutex)
{
    if (!__mutex) {
        LOGD("Null mutex lock, not locking.");
        return 0;
    }

    unsigned int value = (*(unsigned int *) __mutex);
    if (apkenv_check_android_shared_mutex(value)) {
        LOGD("Shared mutex with Android, not locking.");
        return 0;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) value;
    if (apkenv_is_pointer_in_shm((void*)value))
        realmutex = (pthread_mutex_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)value);

    if (value <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = apkenv_alloc_init_mutex(value);
        *((unsigned int *)__mutex) = (unsigned int) realmutex;
    }

    return pthread_mutex_lock(realmutex);
}

int apkenv_my_pthread_mutex_trylock(pthread_mutex_t *__mutex)
{
    unsigned int value = (*(unsigned int *) __mutex);

    if (apkenv_check_android_shared_mutex(value)) {
        LOGD("Shared mutex with Android, not try locking.");
        return 0;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) value;
    if (apkenv_is_pointer_in_shm((void*)value))
        realmutex = (pthread_mutex_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)value);

    if (value <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = apkenv_alloc_init_mutex(value);
        *((unsigned int *)__mutex) = (unsigned int) realmutex;
    }

    return pthread_mutex_trylock(realmutex);
}

int apkenv_my_pthread_mutex_unlock(pthread_mutex_t *__mutex)
{
    if (!__mutex) {
        LOGD("Null mutex lock, not unlocking.");
        return 0;
    }

    unsigned int value = (*(unsigned int *) __mutex);
    if (apkenv_check_android_shared_mutex(value)) {
        LOGD("Shared mutex with Android, not unlocking.");
        return 0;
    }

    if (value <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        LOGD("Trying to unlock a lock that's not locked/initialized"
               " by Hybris, not unlocking.");
        return 0;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) value;
    if (apkenv_is_pointer_in_shm((void*)value))
        realmutex = (pthread_mutex_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)value);

    return pthread_mutex_unlock(realmutex);
}

int apkenv_my_pthread_mutex_lock_timeout_np(pthread_mutex_t *__mutex, unsigned __msecs)
{
    struct timespec tv;
    pthread_mutex_t *realmutex;
    unsigned int value = (*(unsigned int *) __mutex);

    if (apkenv_check_android_shared_mutex(value)) {
        LOGD("Shared mutex with Android, not lock timeout np.");
        return 0;
    }

    realmutex = (pthread_mutex_t *) value;

    if (value <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = apkenv_alloc_init_mutex(value);
        *((int *)__mutex) = (int) realmutex;
    }

    /* TODO: Android uses CLOCK_MONOTONIC here but I am not sure which one to use */
    clock_gettime(CLOCK_REALTIME, &tv);
    tv.tv_sec += __msecs/1000;
    tv.tv_nsec += (__msecs % 1000) * 1000000;
    if (tv.tv_nsec >= 1000000000) {
      tv.tv_sec++;
      tv.tv_nsec -= 1000000000;
    }

    return pthread_mutex_timedlock(realmutex, &tv);
}

int apkenv_my_pthread_mutexattr_setpshared(pthread_mutexattr_t *__attr,
                                           int pshared)
{
    return pthread_mutexattr_setpshared(__attr, pshared);
}

/*
 * pthread_cond* functions
 *
 * Specific implementations to workaround the differences between at the
 * pthread_cond_t struct differences between Bionic and Glibc.
 *
 * */

int apkenv_my_pthread_cond_init(pthread_cond_t *cond,
                                const pthread_condattr_t *attr)
{
    pthread_cond_t *realcond = NULL;

    int pshared = 0;

    if (attr)
        pthread_condattr_getpshared(attr, &pshared);

    if (!pshared) {
        /* non shared, standard cond: use malloc */
        realcond = malloc(sizeof(pthread_cond_t));

        *((unsigned int *) cond) = (unsigned int) realcond;
    }
    else {
        /* process-shared condition: use the shared memory segment */
        apkenv_shm_pointer_t handle = apkenv_shm_alloc(sizeof(pthread_cond_t));

        *((unsigned int *)cond) = (unsigned int) handle;

        if (handle)
            realcond = (pthread_cond_t *)apkenv_get_shmpointer(handle);
    }

    return pthread_cond_init(realcond, attr);
}

int apkenv_my_pthread_cond_destroy(pthread_cond_t *cond)
{
    int ret;
    pthread_cond_t *realcond = (pthread_cond_t *) *(unsigned int *) cond;

    if (!realcond) {
      return EINVAL;
    }

    if (!apkenv_is_pointer_in_shm((void*)realcond)) {
        ret = pthread_cond_destroy(realcond);
        free(realcond);
    }
    else {
        realcond = (pthread_cond_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)realcond);
        ret = pthread_cond_destroy(realcond);
    }

    *((unsigned int *)cond) = 0;

    return ret;
}

int apkenv_my_pthread_cond_broadcast(pthread_cond_t *cond)
{
    unsigned int value = (*(unsigned int *) cond);
    if (apkenv_check_android_shared_cond(value)) {
        LOGD("shared condition with Android, not broadcasting.");
        return 0;
    }

    pthread_cond_t *realcond = (pthread_cond_t *) value;
    if (apkenv_is_pointer_in_shm((void*)value))
        realcond = (pthread_cond_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)value);

    if (value <= ANDROID_TOP_ADDR_VALUE_COND) {
        realcond = apkenv_alloc_init_cond();
        *((unsigned int *) cond) = (unsigned int) realcond;
    }

    return pthread_cond_broadcast(realcond);
}

int apkenv_my_pthread_cond_signal(pthread_cond_t *cond)
{
    unsigned int value = (*(unsigned int *) cond);

    if (apkenv_check_android_shared_cond(value)) {
        LOGD("Shared condition with Android, not signaling.");
        return 0;
    }

    pthread_cond_t *realcond = (pthread_cond_t *) value;
    if (apkenv_is_pointer_in_shm((void*)value))
        realcond = (pthread_cond_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)value);

    if (value <= ANDROID_TOP_ADDR_VALUE_COND) {
        realcond = apkenv_alloc_init_cond();
        *((unsigned int *) cond) = (unsigned int) realcond;
    }

    return pthread_cond_signal(realcond);
}

int apkenv_my_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    /* Both cond and mutex can be statically initialized, check for both */
    unsigned int cvalue = (*(unsigned int *) cond);
    unsigned int mvalue = (*(unsigned int *) mutex);

    if (apkenv_check_android_shared_cond(cvalue) ||
        apkenv_check_android_shared_mutex(mvalue)) {
        LOGD("Shared condition/mutex with Android, not waiting.");
        return 0;
    }

    pthread_cond_t *realcond = (pthread_cond_t *) cvalue;
    if (apkenv_is_pointer_in_shm((void*)cvalue))
        realcond = (pthread_cond_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)cvalue);

    if (cvalue <= ANDROID_TOP_ADDR_VALUE_COND) {
        realcond = apkenv_alloc_init_cond();
        *((unsigned int *) cond) = (unsigned int) realcond;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) mvalue;
    if (apkenv_is_pointer_in_shm((void*)mvalue))
        realmutex = (pthread_mutex_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)mvalue);

    if (mvalue <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = apkenv_alloc_init_mutex(mvalue);
        *((unsigned int *) mutex) = (unsigned int) realmutex;
    }

    return pthread_cond_wait(realcond, realmutex);
}

int apkenv_my_pthread_cond_timedwait(pthread_cond_t *cond,
                pthread_mutex_t *mutex, const struct timespec *abstime)
{
    /* Both cond and mutex can be statically initialized, check for both */
    unsigned int cvalue = (*(unsigned int *) cond);
    unsigned int mvalue = (*(unsigned int *) mutex);

    if (apkenv_check_android_shared_cond(cvalue) ||
         apkenv_check_android_shared_mutex(mvalue)) {
        LOGD("Shared condition/mutex with Android, not waiting.");
        return 0;
    }

    pthread_cond_t *realcond = (pthread_cond_t *) cvalue;
    if (apkenv_is_pointer_in_shm((void*)cvalue))
        realcond = (pthread_cond_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)cvalue);

    if (cvalue <= ANDROID_TOP_ADDR_VALUE_COND) {
        realcond = apkenv_alloc_init_cond();
        *((unsigned int *) cond) = (unsigned int) realcond;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) mvalue;
    if (apkenv_is_pointer_in_shm((void*)mvalue))
        realmutex = (pthread_mutex_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)mvalue);

    if (mvalue <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = apkenv_alloc_init_mutex(mvalue);
        *((unsigned int *) mutex) = (unsigned int) realmutex;
    }

    return pthread_cond_timedwait(realcond, realmutex, abstime);
}

int apkenv_my_pthread_cond_timedwait_relative_np(pthread_cond_t *cond,
                pthread_mutex_t *mutex, const struct timespec *reltime)
{
    /* Both cond and mutex can be statically initialized, check for both */
    unsigned int cvalue = (*(unsigned int *) cond);
    unsigned int mvalue = (*(unsigned int *) mutex);

    if (apkenv_check_android_shared_cond(cvalue) ||
         apkenv_check_android_shared_mutex(mvalue)) {
        LOGD("Shared condition/mutex with Android, not waiting.");
        return 0;
    }

    pthread_cond_t *realcond = (pthread_cond_t *) cvalue;
    if( apkenv_is_pointer_in_shm((void*)cvalue) )
        realcond = (pthread_cond_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)cvalue);

    if (cvalue <= ANDROID_TOP_ADDR_VALUE_COND) {
        realcond = apkenv_alloc_init_cond();
        *((unsigned int *) cond) = (unsigned int) realcond;
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t *) mvalue;
    if (apkenv_is_pointer_in_shm((void*)mvalue))
        realmutex = (pthread_mutex_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)mvalue);

    if (mvalue <= ANDROID_TOP_ADDR_VALUE_MUTEX) {
        realmutex = apkenv_alloc_init_mutex(mvalue);
        *((unsigned int *) mutex) = (unsigned int) realmutex;
    }

    /* TODO: Android uses CLOCK_MONOTONIC here but I am not sure which one to use */
    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);
    tv.tv_sec += reltime->tv_sec;
    tv.tv_nsec += reltime->tv_nsec;
    if (tv.tv_nsec >= 1000000000) {
      tv.tv_sec++;
      tv.tv_nsec -= 1000000000;
    }
    return pthread_cond_timedwait(realcond, realmutex, &tv);
}

/*
 * pthread_rwlockattr_* functions
 *
 * Specific implementations to workaround the differences between at the
 * pthread_rwlockattr_t struct differences between Bionic and Glibc.
 *
 * */

int apkenv_my_pthread_rwlockattr_init(pthread_rwlockattr_t *__attr)
{
    pthread_rwlockattr_t *realattr;

    realattr = malloc(sizeof(pthread_rwlockattr_t));
    *((unsigned int *)__attr) = (unsigned int) realattr;

    return pthread_rwlockattr_init(realattr);
}

int apkenv_my_pthread_rwlockattr_destroy(pthread_rwlockattr_t *__attr)
{
    int ret;
    pthread_rwlockattr_t *realattr = (pthread_rwlockattr_t *) *(unsigned int *) __attr;

    ret = pthread_rwlockattr_destroy(realattr);
    free(realattr);

    return ret;
}

int apkenv_my_pthread_rwlockattr_setpshared(pthread_rwlockattr_t *__attr,
                                            int pshared)
{
    pthread_rwlockattr_t *realattr = (pthread_rwlockattr_t *) *(unsigned int *) __attr;
    return pthread_rwlockattr_setpshared(realattr, pshared);
}

int apkenv_my_pthread_rwlockattr_getpshared(pthread_rwlockattr_t *__attr,
                                            int *pshared)
{
    pthread_rwlockattr_t *realattr = (pthread_rwlockattr_t *) *(unsigned int *) __attr;
    return pthread_rwlockattr_getpshared(realattr, pshared);
}

/*
 * pthread_rwlock_* functions
 *
 * Specific implementations to workaround the differences between at the
 * pthread_rwlock_t struct differences between Bionic and Glibc.
 *
 * */

int apkenv_my_pthread_rwlock_init(pthread_rwlock_t *__rwlock,
                                  __const pthread_rwlockattr_t *__attr)
{
    pthread_rwlock_t *realrwlock = NULL;
    pthread_rwlockattr_t *realattr = NULL;
    int pshared = 0;

    if (__attr != NULL)
        realattr = (pthread_rwlockattr_t *) *(unsigned int *) __attr;

    if (realattr)
        pthread_rwlockattr_getpshared(realattr, &pshared);

    if (!pshared) {
        /* non shared, standard rwlock: use malloc */
        realrwlock = malloc(sizeof(pthread_rwlock_t));

        *((unsigned int *) __rwlock) = (unsigned int) realrwlock;
    }
    else {
        /* process-shared condition: use the shared memory segment */
        apkenv_shm_pointer_t handle = apkenv_shm_alloc(sizeof(pthread_rwlock_t));

        *((unsigned int *)__rwlock) = (unsigned int) handle;

        if (handle)
            realrwlock = (pthread_rwlock_t *)apkenv_get_shmpointer(handle);
    }

    return pthread_rwlock_init(realrwlock, realattr);
}

int apkenv_my_pthread_rwlock_destroy(pthread_rwlock_t *__rwlock)
{
    int ret;
    pthread_rwlock_t *realrwlock = (pthread_rwlock_t *) *(unsigned int *) __rwlock;

    if (!apkenv_is_pointer_in_shm((void*)realrwlock)) {
        ret = pthread_rwlock_destroy(realrwlock);
        free(realrwlock);
    }
    else {
        ret = pthread_rwlock_destroy(realrwlock);
        realrwlock = (pthread_rwlock_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)realrwlock);
    }

    return ret;
}

pthread_rwlock_t* apkenv_set_realrwlock(pthread_rwlock_t *rwlock)
{
    unsigned int value = (*(unsigned int *) rwlock);
    pthread_rwlock_t *realrwlock = (pthread_rwlock_t *) value;
    if (apkenv_is_pointer_in_shm((void*)value))
        realrwlock = (pthread_rwlock_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)value);

    if (realrwlock <= ANDROID_TOP_ADDR_VALUE_RWLOCK) {
        realrwlock = apkenv_alloc_init_rwlock();
        *((unsigned int *)rwlock) = (unsigned int) realrwlock;
    }
    return realrwlock;
}

int apkenv_my_pthread_rwlock_rdlock(pthread_rwlock_t *__rwlock)
{
    pthread_rwlock_t *realrwlock = apkenv_set_realrwlock(__rwlock);
    return pthread_rwlock_rdlock(realrwlock);
}

int apkenv_my_pthread_rwlock_tryrdlock(pthread_rwlock_t *__rwlock)
{
    pthread_rwlock_t *realrwlock = apkenv_set_realrwlock(__rwlock);
    return pthread_rwlock_tryrdlock(realrwlock);
}

int apkenv_my_pthread_rwlock_timedrdlock(pthread_rwlock_t *__rwlock,
                                         __const struct timespec *abs_timeout)
{
    pthread_rwlock_t *realrwlock = apkenv_set_realrwlock(__rwlock);
    return pthread_rwlock_timedrdlock(realrwlock, abs_timeout);
}

int apkenv_my_pthread_rwlock_wrlock(pthread_rwlock_t *__rwlock)
{
    pthread_rwlock_t *realrwlock = apkenv_set_realrwlock(__rwlock);
    return pthread_rwlock_wrlock(realrwlock);
}

int apkenv_my_pthread_rwlock_trywrlock(pthread_rwlock_t *__rwlock)
{
    pthread_rwlock_t *realrwlock = apkenv_set_realrwlock(__rwlock);
    return pthread_rwlock_trywrlock(realrwlock);
}

int apkenv_my_pthread_rwlock_timedwrlock(pthread_rwlock_t *__rwlock,
                                         __const struct timespec *abs_timeout)
{
    pthread_rwlock_t *realrwlock = apkenv_set_realrwlock(__rwlock);
    return pthread_rwlock_timedwrlock(realrwlock, abs_timeout);
}

int apkenv_my_pthread_rwlock_unlock(pthread_rwlock_t *__rwlock)
{
    unsigned int value = (*(unsigned int *) __rwlock);
    if (value <= ANDROID_TOP_ADDR_VALUE_RWLOCK) {
        LOGD("Trying to unlock a rwlock that's not locked/initialized"
               " by Hybris, not unlocking.");
        return 0;
    }
    pthread_rwlock_t *realrwlock = (pthread_rwlock_t *) value;
    if (apkenv_is_pointer_in_shm((void*)value))
        realrwlock = (pthread_rwlock_t *)apkenv_get_shmpointer((apkenv_shm_pointer_t)value);

    return pthread_rwlock_unlock(realrwlock);
}

int apkenv_my_pthread_setname_np(pthread_t thid, const char *thname)
{
    WRAPPERS_DEBUG_PRINTF("pthread_setname_np(%ld, %s)\n", thid, thname);
    return 0;
}

#if 0
/**
 * This will take care of doing the right thing for mutexes that are not
 * yet initialized correctly (e.g. because they were statically initialized
 * on the Bionic-using side). This function should be called at the start
 * of every function that takes a pthread_mutex_t * as parameter.
 *
 * Returns the "real" glibc pthread_mutex_t *
 **/
pthread_mutex_t *
late_init_pthread_mutex(pthread_mutex_t *mutex)
{
    if (mutex == NULL) {
        printf("Mutex is NULL in %s\n", __func__);
        exit(1);
    }

    pthread_mutex_t *realmutex = (pthread_mutex_t*) (*((int*)mutex));
    if (realmutex == NULL) {
        WRAPPERS_DEBUG_PRINTF("late-initializing normal mutex\n");
        realmutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(realmutex, NULL);
        *((int *)mutex) = (int)realmutex;
    } else if (realmutex == BIONIC_PTHREAD_RECURSIVE_MUTEX) {
        WRAPPERS_DEBUG_PRINTF("late-initializing recursive mutex\n");
        realmutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_t tmp = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
        memcpy(realmutex, &tmp, sizeof(pthread_mutex_t));
        *((int *)mutex) = (int)realmutex;
    } else if (realmutex == BIONIC_PTHREAD_ERRORCHECK_MUTEX) {
        WRAPPERS_DEBUG_PRINTF("late-initializing errorcheck mutex\n");
        realmutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_t tmp = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
        memcpy(realmutex, &tmp, sizeof(pthread_mutex_t));
        *((int *)mutex) = (int)realmutex;
    }

    return realmutex;
}

/**
 * Same thing as above, but for pthread_cond_t * variables.
 *
 * Returns the "real" glibc pthread_cond_t *
 **/
pthread_cond_t *
late_init_pthread_cond(pthread_cond_t *cond)
{
    if (cond == NULL) {
        printf("Cond iS NULL in %s\n", __func__);
        exit(1);
    }

    pthread_cond_t *realcond = (pthread_cond_t *) *(int *) cond;
    if (realcond == NULL) {
        WRAPPERS_DEBUG_PRINTF("late-initializing cond\n");
        realcond = malloc(sizeof(pthread_cond_t));
        pthread_cond_t tmp = PTHREAD_COND_INITIALIZER;
        memcpy(realcond, &tmp, sizeof(pthread_cond_t));
        *((int *) cond) = (int)realcond;
    }

    return realcond;
}

int
apkenv_my_pthread_mutex_init (pthread_mutex_t *__mutex,
        __const pthread_mutexattr_t *__mutexattr)
{
    pthread_mutex_t *realmutex = malloc(sizeof(pthread_mutex_t));
    *((int *)__mutex) = (int) realmutex;
    return pthread_mutex_init(realmutex, __mutexattr);
}

int
apkenv_my_pthread_mutex_lock (pthread_mutex_t *__mutex)
{
    pthread_mutex_t *realmutex = late_init_pthread_mutex(__mutex);
    return pthread_mutex_lock(realmutex);
}

int
apkenv_my_pthread_mutex_trylock (pthread_mutex_t *__mutex)
{
    pthread_mutex_t *realmutex = late_init_pthread_mutex(__mutex);
    return pthread_mutex_trylock(realmutex);
}

int
apkenv_my_pthread_mutex_unlock (pthread_mutex_t *__mutex)
{
    pthread_mutex_t *realmutex = late_init_pthread_mutex(__mutex);
    return pthread_mutex_unlock(realmutex);
}

int
apkenv_my_pthread_mutex_destroy (pthread_mutex_t *__mutex)
{
    pthread_mutex_t *realmutex = late_init_pthread_mutex(__mutex);
    int ret = pthread_mutex_destroy(realmutex);
    free(realmutex);
    return ret;
}

int
apkenv_my_pthread_cond_init(pthread_cond_t *cond,
        const pthread_condattr_t *attr)
{
    assert(cond != NULL);
    pthread_cond_t *realcond = malloc(sizeof(pthread_cond_t));
    *((int *) cond) = (int) realcond;
    return pthread_cond_init(realcond, attr);
}

int
apkenv_my_pthread_cond_destroy(pthread_cond_t *cond)
{
    pthread_cond_t *realcond = late_init_pthread_cond(cond);
    int ret = pthread_cond_destroy(realcond);
    free(realcond);
    return ret;
}

int
apkenv_my_pthread_cond_broadcast(pthread_cond_t *cond)
{
    pthread_cond_t *realcond = late_init_pthread_cond(cond);
    return pthread_cond_broadcast(realcond);
}

int
apkenv_my_pthread_cond_signal(pthread_cond_t *cond)
{
    pthread_cond_t *realcond = late_init_pthread_cond(cond);
    return pthread_cond_signal(realcond);
}

int
apkenv_my_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    pthread_cond_t *realcond = late_init_pthread_cond(cond);
    pthread_mutex_t *realmutex = late_init_pthread_mutex(mutex);
    return pthread_cond_wait(realcond, realmutex);
}

int
apkenv_my_pthread_cond_timedwait(pthread_cond_t *cond,
        pthread_mutex_t *mutex, const struct timespec *abstime)
{
    pthread_cond_t *realcond = late_init_pthread_cond(cond);
    pthread_mutex_t *realmutex = late_init_pthread_mutex(mutex);
    return pthread_cond_timedwait(realcond, realmutex, abstime);
}

int
apkenv_my_pthread_cond_timeout_np(pthread_cond_t *cond,
        pthread_mutex_t * mutex, unsigned msecs)
{
    pthread_cond_t *realcond = late_init_pthread_cond(cond);
    pthread_mutex_t *realmutex = late_init_pthread_mutex(mutex);
    struct timespec ts;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec + msecs / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + (msecs % 1000) * 1000000;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec++;
        ts.tv_nsec -= 1000000000;
    }
    return pthread_cond_timedwait(realcond, realmutex, &ts);
}

int
apkenv_my_pthread_attr_init(pthread_attr_t *__attr)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;

    realattr = malloc(sizeof(pthread_attr_t));
    *((int *)__attr) = (int) realattr;

    return pthread_attr_init(realattr);
}

int
apkenv_my_pthread_attr_destroy(pthread_attr_t *__attr)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_attr_destroy(realattr);
}

int
apkenv_my_pthread_attr_setdetachstate(pthread_attr_t *__attr, int detachstate)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_attr_setdetachstate(realattr, detachstate);
}


int
apkenv_my_pthread_attr_getdetachstate(pthread_attr_t *__attr, int* detachstate)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_attr_getdetachstate(realattr, detachstate);
}

int
apkenv_my_pthread_attr_setschedparam (pthread_attr_t * __attr,struct sched_param * __param)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_attr_setschedparam(realattr,__param);
}

int
apkenv_my_pthread_attr_getschedparam(pthread_attr_t *__attr, struct sched_param * __param)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_attr_getschedparam(realattr, __param);
}

int
apkenv_my_pthread_attr_setstacksize(pthread_attr_t *__attr, size_t stacksize)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_attr_setstacksize (realattr,stacksize);
}

int
apkenv_my_pthread_attr_getstacksize(pthread_attr_t *__attr, size_t *stacksize)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_attr_getstacksize(realattr, stacksize);
}

int
apkenv_my_pthread_attr_getstack(pthread_attr_t *__attr, void** stackaddr, size_t* stacksize)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_attr_getstack(realattr,stackaddr,stacksize);
}

int
apkenv_my_pthread_attr_setstack(pthread_attr_t *__attr, void* stackaddr, size_t stacksize)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_attr_setstack(realattr,stackaddr,stacksize);
}

int
apkenv_my_pthread_attr_setschedpolicy(pthread_attr_t *__attr, int __policy)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_attr_setschedpolicy(realattr, __policy);
}

int
apkenv_my_pthread_getattr_np(pthread_t __th, pthread_attr_t *__attr)
{
    assert(__attr != NULL);
    pthread_attr_t *realattr = (pthread_attr_t *) *(int *) __attr;
    assert(realattr != NULL);
    return pthread_getattr_np(__th,realattr);
}

int
apkenv_my_pthread_setname_np(pthread_t thid, const char *thname)
{
    // TODO?
    WRAPPERS_DEBUG_PRINTF("pthread_setname_np(%ld, %s)\n", thid, thname);
    return 0;
}

void
apkenv_my_pthread_cleanup_pop(int execute)
{
    printf("UNIMPLEMENTED: pthread_cleanup_pop\n");
}
void
apkenv_my_pthread_cleanup_push(void (*routine) (void *), void *arg)
{
    printf("UNIMPLEMENTED: pthread_cleanup_push\n");

}

void *
start_wrapped_thread(void *arg)
{
    struct WrappedThread *wrapped = arg;
    //printf("XXX THREAD START XXX\n");
    void *result = wrapped->start_routine(wrapped->arg);
    //printf("XXX THREAD END XXX\n");
    free(wrapped);
    return result;
}

int apkenv_my_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        void *(*start_routine)(void*), void *arg)
{
    struct WrappedThread *wrapped = malloc(
            sizeof(struct WrappedThread));
    wrapped->start_routine = start_routine;
    wrapped->arg = arg;

    //printf("XXX PTHREAD CREATE XXX\n");
    return pthread_create(thread, attr, start_wrapped_thread, (void*)wrapped);
}
#endif

