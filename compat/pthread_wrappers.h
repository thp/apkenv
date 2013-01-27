
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

#define _GNU_SOURCE
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

/* Static initializer values for pthread_mutex_t in Bionic */
#define BIONIC_PTHREAD_RECURSIVE_MUTEX (void*)0x4000
#define BIONIC_PTHREAD_ERRORCHECK_MUTEX (void*)0x8000

pthread_mutex_t *
late_init_pthread_mutex(pthread_mutex_t *mutex);
pthread_cond_t *
late_init_pthread_cond(pthread_cond_t *cond);
int
my_pthread_mutex_init (pthread_mutex_t *__mutex,
        __const pthread_mutexattr_t *__mutexattr);
int
my_pthread_mutex_lock (pthread_mutex_t *__mutex);
int
my_pthread_mutex_trylock (pthread_mutex_t *__mutex);
int
my_pthread_mutex_unlock (pthread_mutex_t *__mutex);
int
my_pthread_mutex_destroy (pthread_mutex_t *__mutex);
int
my_pthread_cond_init(pthread_cond_t *cond,
        const pthread_condattr_t *attr);
int
my_pthread_cond_destroy(pthread_cond_t *cond);
int
my_pthread_cond_broadcast(pthread_cond_t *cond);
int
my_pthread_cond_signal(pthread_cond_t *cond);
int
my_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int
my_pthread_cond_timedwait(pthread_cond_t *cond,
        pthread_mutex_t *mutex, const struct timespec *abstime);
int
my_pthread_attr_init(pthread_attr_t *__attr);
int
my_pthread_attr_destroy(pthread_attr_t *__attr);
int
my_pthread_attr_setdetachstate(pthread_attr_t *__attr,
        int detachstate);
int
my_pthread_attr_getdetachstate(pthread_attr_t *__attr,
        int* detachstate);
int
my_pthread_attr_setstacksize(pthread_attr_t *__attr, size_t stacksize);
int
my_pthread_attr_getstack(pthread_attr_t *__attr, void** stackaddr, size_t* stacksize);
int
my_pthread_attr_setschedpolicy(pthread_attr_t *__attr, int __policy);
int
my_pthread_attr_setschedparam (pthread_attr_t * __attr,struct sched_param * __param);
int
my_pthread_getattr_np(pthread_t __th, pthread_attr_t *__attr);

void
my_pthread_cleanup_pop(int execute);
void
my_pthread_cleanup_push(void (*routine) (void *), void *arg);


struct WrappedThread {
    void *(*start_routine)(void *);
    void *arg;
};

void *
start_wrapped_thread(void *arg);
int
my_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        void *(*start_routine)(void*), void *arg);

