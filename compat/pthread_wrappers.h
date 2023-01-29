
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

/* pthread_once_t: int in glibc, volatile int in Bionic */
/* pthread_mutexattr_t: union{char[4],int} in glibc, long in Bionic */
/* pthread_condattr_t: union{char[4],int} in glibc, long in Bionic */

/* pthread_mutex_t: big struct in glibc, struct{int} in Bionic -> wrap */
/* pthread_cond_t: big struct in glibc, struct{int} in Bionic -> wrap */
/* pthread_attr_t: big struct in glibc, small struct in Bionic -> wrap */

int apkenv_my_pthread_create(pthread_t *thread, const pthread_attr_t *__attr,
        void *(*start_routine)(void*), void *arg);
int apkenv_my_pthread_attr_init(pthread_attr_t *__attr);
int apkenv_my_pthread_attr_destroy(pthread_attr_t *__attr);
int apkenv_my_pthread_attr_setdetachstate(pthread_attr_t *__attr, int state);
int apkenv_my_pthread_attr_getdetachstate(pthread_attr_t const *__attr, int *state);
int apkenv_my_pthread_attr_setschedpolicy(pthread_attr_t *__attr, int policy);
int apkenv_my_pthread_attr_getschedpolicy(pthread_attr_t const *__attr, int *policy);
int apkenv_my_pthread_attr_setschedparam(pthread_attr_t *__attr, struct sched_param const *param);
int apkenv_my_pthread_attr_getschedparam(pthread_attr_t const *__attr, struct sched_param *param);
int apkenv_my_pthread_attr_setstacksize(pthread_attr_t *__attr, size_t stack_size);
int apkenv_my_pthread_attr_getstacksize(pthread_attr_t const *__attr, size_t *stack_size);
int apkenv_my_pthread_attr_setstackaddr(pthread_attr_t *__attr, void *stack_addr);
int apkenv_my_pthread_attr_getstackaddr(pthread_attr_t const *__attr, void **stack_addr);
int apkenv_my_pthread_attr_setstack(pthread_attr_t *__attr, void *stack_base, size_t stack_size);
int apkenv_my_pthread_attr_getstack(pthread_attr_t const *__attr, void **stack_base, size_t *stack_size);
int apkenv_my_pthread_attr_setguardsize(pthread_attr_t *__attr, size_t guard_size);
int apkenv_my_pthread_attr_getguardsize(pthread_attr_t const *__attr, size_t *guard_size);
int apkenv_my_pthread_attr_setscope(pthread_attr_t *__attr, int scope);
int apkenv_my_pthread_attr_getscope(pthread_attr_t const *__attr);
int apkenv_my_pthread_getattr_np(pthread_t thid, pthread_attr_t *__attr);
int apkenv_my_pthread_mutex_init(pthread_mutex_t *__mutex,
                          __const pthread_mutexattr_t *__mutexattr);
int apkenv_my_pthread_mutex_destroy(pthread_mutex_t *__mutex);
int apkenv_my_pthread_mutex_lock(pthread_mutex_t *__mutex);
int apkenv_my_pthread_mutex_trylock(pthread_mutex_t *__mutex);
int apkenv_my_pthread_mutex_unlock(pthread_mutex_t *__mutex);
int apkenv_my_pthread_mutex_lock_timeout_np(pthread_mutex_t *__mutex, unsigned __msecs);
int apkenv_my_pthread_mutexattr_setpshared(pthread_mutexattr_t *__attr,
                                           int pshared);
int apkenv_my_pthread_cond_init(pthread_cond_t *cond,
                                const pthread_condattr_t *attr);
int apkenv_my_pthread_cond_destroy(pthread_cond_t *cond);
int apkenv_my_pthread_cond_broadcast(pthread_cond_t *cond);
int apkenv_my_pthread_cond_signal(pthread_cond_t *cond);
int apkenv_my_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int apkenv_my_pthread_cond_timedwait(pthread_cond_t *cond,
                pthread_mutex_t *mutex, const struct timespec *abstime);
int apkenv_my_pthread_cond_timedwait_relative_np(pthread_cond_t *cond,
                pthread_mutex_t *mutex, const struct timespec *abstime);
int apkenv_my_pthread_rwlockattr_init(pthread_rwlockattr_t *__attr);
int apkenv_my_pthread_rwlockattr_destroy(pthread_rwlockattr_t *__attr);
int apkenv_my_pthread_rwlockattr_setpshared(pthread_rwlockattr_t *__attr,
                                            int pshared);
int apkenv_my_pthread_rwlockattr_getpshared(pthread_rwlockattr_t *__attr,
                                            int *pshared);
int apkenv_my_pthread_rwlock_init(pthread_rwlock_t *__rwlock,
                                  __const pthread_rwlockattr_t *__attr);
int apkenv_my_pthread_rwlock_destroy(pthread_rwlock_t *__rwlock);
int apkenv_my_pthread_rwlock_rdlock(pthread_rwlock_t *__rwlock);
int apkenv_my_pthread_rwlock_tryrdlock(pthread_rwlock_t *__rwlock);
int apkenv_my_pthread_rwlock_timedrdlock(pthread_rwlock_t *__rwlock,
                                         __const struct timespec *abs_timeout);
int apkenv_my_pthread_rwlock_wrlock(pthread_rwlock_t *__rwlock);
int apkenv_my_pthread_rwlock_trywrlock(pthread_rwlock_t *__rwlock);
int apkenv_my_pthread_rwlock_timedwrlock(pthread_rwlock_t *__rwlock,
                                         __const struct timespec *abs_timeout);
int apkenv_my_pthread_rwlock_unlock(pthread_rwlock_t *__rwlock);
int apkenv_my_pthread_setname_np(pthread_t thid, const char *thname);

