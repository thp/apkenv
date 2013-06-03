
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
#include <string.h>
#include <stdlib.h>

#include "jnienv.h"
#include "../apkenv.h"

#ifdef APKENV_DEBUG
#  define JNIENV_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define JNIENV_DEBUG_PRINTF(...)
#endif

static int
check_obj(JNIEnv *env, const void *obj, const char *varname, const char *func)
{
    if (obj == NULL) {
        JNIENV_DEBUG_PRINTF("%s: Warning: %s is NULL\n", func, varname);
        return 0;
    }
    if (obj == GLOBAL_J(env)) {
        JNIENV_DEBUG_PRINTF("%s: Warning: %s is GLOBAL\n", func, varname);
        return 0;
    }
    return 1;
}
#define CHECK_OBJ(obj) check_obj(env, obj, #obj, __FUNCTION__)


jint
JNIEnv_GetVersion(JNIEnv * p0)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetVersion()\n");
    return 0;
}


jclass
JNIEnv_DefineClass(JNIEnv* p0, const char* p1, jobject p2, const jbyte* p3, jsize p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_DefineClass()\n");
    return NULL;
}


jclass
JNIEnv_FindClass(JNIEnv* p0, const char* p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_FindClass('%s')\n", p1);
    struct dummy_jclass *class = malloc(sizeof(struct dummy_jclass));
    class->name = strdup(p1);
    return class;
}


jmethodID
JNIEnv_FromReflectedMethod(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_FromReflectedMethod()\n");
    return NULL;
}


jfieldID
JNIEnv_FromReflectedField(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_FromReflectedField()\n");
    return NULL;
}


jobject
JNIEnv_ToReflectedMethod(JNIEnv* p0, jclass p1, jmethodID p2, jboolean p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ToReflectedMethod()\n");
    return NULL;
}


jclass
JNIEnv_GetSuperclass(JNIEnv* p0, jclass p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetSuperclass()\n");
    return NULL;
}


jboolean
JNIEnv_IsAssignableFrom(JNIEnv* p0, jclass p1, jclass p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_IsAssignableFrom()\n");
    return 0;
}


jobject
JNIEnv_ToReflectedField(JNIEnv* p0, jclass p1, jfieldID p2, jboolean p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ToReflectedField()\n");
    return NULL;
}


jint
JNIEnv_Throw(JNIEnv* p0, jthrowable p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_Throw()\n");
    return 0;
}


jint
JNIEnv_ThrowNew(JNIEnv * p0, jclass p1, const char * p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ThrowNew()\n");
    return 0;
}


jthrowable
JNIEnv_ExceptionOccurred(JNIEnv* p0)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ExceptionOccurred()\n");
    return NULL;
}


void
JNIEnv_ExceptionDescribe(JNIEnv* p0)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ExceptionDescribe()\n");
}


void
JNIEnv_ExceptionClear(JNIEnv* p0)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ExceptionClear()\n");
}


void
JNIEnv_FatalError(JNIEnv* p0, const char* p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_FatalError()\n");
}


jint
JNIEnv_PushLocalFrame(JNIEnv* p0, jint p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_PushLocalFrame()\n");
    return 0;
}


jobject
JNIEnv_PopLocalFrame(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_PopLocalFrame()\n");
    return NULL;
}


jobject
JNIEnv_NewGlobalRef(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewGlobalRef(%x)\n", p1);
    return p1;
}


void
JNIEnv_DeleteGlobalRef(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_DeleteGlobalRef()\n");
}


void
JNIEnv_DeleteLocalRef(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_DeleteLocalRef(%x)\n", p1);
}


jboolean
JNIEnv_IsSameObject(JNIEnv* p0, jobject p1, jobject p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_IsSameObject()\n");
    return 0;
}


jobject
JNIEnv_NewLocalRef(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewLocalRef()\n");
    return NULL;
}


jint
JNIEnv_EnsureLocalCapacity(JNIEnv* p0, jint p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_EnsureLocalCapacity()\n");
    return 0;
}


jobject
JNIEnv_AllocObject(JNIEnv* p0, jclass p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_AllocObject()\n");
    return NULL;
}


jobject
JNIEnv_NewObject(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewObject(%x, %s)\n", p1, p2->name);
    return NULL;
}


jobject
JNIEnv_NewObjectV(JNIEnv *env, jclass p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewObjectV(%x, %s)\n", p1, p2->name);
    return GLOBAL_J(env);
}


jobject
JNIEnv_NewObjectA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewObjectA()\n");
    return NULL;
}


jclass
JNIEnv_GetObjectClass(JNIEnv* env, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetObjectClass(%x, %x)\n", env, p1);
    return GLOBAL_J(env);
}


jboolean
JNIEnv_IsInstanceOf(JNIEnv* p0, jobject p1, jclass p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_IsInstanceOf()\n");
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

jmethodID
JNIEnv_GetMethodID(JNIEnv* p0, jclass clazz, const char* name, const char* sig)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetMethodID(%x, '%s', '%s')\n", clazz, name, sig);
    return jnienv_make_method(clazz, name, sig);
}


jobject
JNIEnv_CallObjectMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallObjectMethod(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return NULL;
}


jobject
JNIEnv_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallObjectMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return GLOBAL_J(env);
}


jobject
JNIEnv_CallObjectMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallObjectMethodA(%p, %s/%s, %p)\n", p1, p2->name, p2->sig, p3);
    return NULL;
}


jboolean
JNIEnv_CallBooleanMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallBooleanMethod(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jboolean
JNIEnv_CallBooleanMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallBooleanMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jboolean
JNIEnv_CallBooleanMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallBooleanMethodA(%p, %s/%s, %p)\n", p1, p2->name, p2->sig, p3);
    return 0;
}


jbyte
JNIEnv_CallByteMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallByteMethod(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jbyte
JNIEnv_CallByteMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallByteMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jbyte
JNIEnv_CallByteMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallByteMethodA(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jchar
JNIEnv_CallCharMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallCharMethod(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jchar
JNIEnv_CallCharMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallCharMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jchar
JNIEnv_CallCharMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallCharMethodA(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jshort
JNIEnv_CallShortMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallShortMethod(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jshort
JNIEnv_CallShortMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallShortMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jshort
JNIEnv_CallShortMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallShortMethodA(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jint
JNIEnv_CallIntMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallIntMethod(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jint
JNIEnv_CallIntMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallIntMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jint
JNIEnv_CallIntMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallIntMethodA(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jlong
JNIEnv_CallLongMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallLongMethod(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jlong
JNIEnv_CallLongMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallLongMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jlong
JNIEnv_CallLongMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallLongMethodA(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0;
}


jfloat
JNIEnv_CallFloatMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallFloatMethod(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0.f;
}


jfloat
JNIEnv_CallFloatMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallFloatMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0.f;
}


jfloat
JNIEnv_CallFloatMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallFloatMethodA(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0.f;
}


jdouble
JNIEnv_CallDoubleMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallDoubleMethod(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0.;
}


jdouble
JNIEnv_CallDoubleMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallDoubleMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0.;
}


jdouble
JNIEnv_CallDoubleMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallDoubleMethodA(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
    return 0.;
}


void
JNIEnv_CallVoidMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallVoidMethod(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
}


void
JNIEnv_CallVoidMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallVoidMethodV(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
}


void
JNIEnv_CallVoidMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_CallVoidMethodA(%p, %s/%s, ...)\n", p1, p2->name, p2->sig);
}


jobject
JNIEnv_CallNonvirtualObjectMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualObjectMethod(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return NULL;
}


jobject
JNIEnv_CallNonvirtualObjectMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualObjectMethodV(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return NULL;
}


jobject
JNIEnv_CallNonvirtualObjectMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualObjectMethodA(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return NULL;
}


jboolean
JNIEnv_CallNonvirtualBooleanMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualBooleanMethod(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jboolean
JNIEnv_CallNonvirtualBooleanMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualBooleanMethodV(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jboolean
JNIEnv_CallNonvirtualBooleanMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualBooleanMethodA(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jbyte
JNIEnv_CallNonvirtualByteMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualByteMethod(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jbyte
JNIEnv_CallNonvirtualByteMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualByteMethodV(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jbyte
JNIEnv_CallNonvirtualByteMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualByteMethodA(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jchar
JNIEnv_CallNonvirtualCharMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualCharMethod(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jchar
JNIEnv_CallNonvirtualCharMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualCharMethodV(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jchar
JNIEnv_CallNonvirtualCharMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualCharMethodA(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jshort
JNIEnv_CallNonvirtualShortMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualShortMethod(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jshort
JNIEnv_CallNonvirtualShortMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualShortMethodV(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jshort
JNIEnv_CallNonvirtualShortMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualShortMethodA(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jint
JNIEnv_CallNonvirtualIntMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualIntMethod(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jint
JNIEnv_CallNonvirtualIntMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualIntMethodV(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jint
JNIEnv_CallNonvirtualIntMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualIntMethodA(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jlong
JNIEnv_CallNonvirtualLongMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualLongMethod(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jlong
JNIEnv_CallNonvirtualLongMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualLongMethodV(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jlong
JNIEnv_CallNonvirtualLongMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualLongMethodA(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0;
}


jfloat
JNIEnv_CallNonvirtualFloatMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualFloatMethod(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0.f;
}


jfloat
JNIEnv_CallNonvirtualFloatMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualFloatMethodV(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0.f;
}


jfloat
JNIEnv_CallNonvirtualFloatMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualFloatMethodA(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0.f;
}


jdouble
JNIEnv_CallNonvirtualDoubleMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualDoubleMethod(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0.;
}


jdouble
JNIEnv_CallNonvirtualDoubleMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualDoubleMethodV(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0.;
}


jdouble
JNIEnv_CallNonvirtualDoubleMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualDoubleMethodA(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
    return 0.;
}


void
JNIEnv_CallNonvirtualVoidMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualVoidMethod(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
}


void
JNIEnv_CallNonvirtualVoidMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualVoidMethodV(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
}


void
JNIEnv_CallNonvirtualVoidMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
    struct dummy_jclass *jcl = (struct dummy_jclass*)p2;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallNonvirtualVoidMethodA(%p, %s, %s/%s, ...)\n", p1, jcl->name, p3->name, p3->sig);
}


jfieldID
jnienv_make_fieldid(jclass clazz, const char *name, const char *sig)
{
    return (jfieldID)jnienv_make_method(clazz,name,sig);
}


jfieldID
JNIEnv_GetFieldID(JNIEnv* p0, jclass clazz, const char* name, const char* sig)
{
    struct dummy_jclass *class = (struct dummy_jclass*)clazz;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetFieldID(%s, '%s', '%s')\n", class->name, name, sig);
    return jnienv_make_fieldid(clazz, name, sig);
}


jobject
JNIEnv_GetObjectField(JNIEnv* p0, jobject p1, jfieldID p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetObjectField()\n");
    return NULL;
}


jboolean
JNIEnv_GetBooleanField(JNIEnv* p0, jobject p1, jfieldID p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetBooleanField()\n");
    return 0;
}


jbyte
JNIEnv_GetByteField(JNIEnv* p0, jobject p1, jfieldID p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetByteField()\n");
    return 0;
}


jchar
JNIEnv_GetCharField(JNIEnv* p0, jobject p1, jfieldID p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetCharField()\n");
    return 0;
}


jshort
JNIEnv_GetShortField(JNIEnv* p0, jobject p1, jfieldID p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetShortField()\n");
    return 0;
}


jint
JNIEnv_GetIntField(JNIEnv* p0, jobject p1, jfieldID p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetIntField()\n");
    return 0;
}


jlong
JNIEnv_GetLongField(JNIEnv* p0, jobject p1, jfieldID p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetLongField()\n");
    return 0;
}


jfloat
JNIEnv_GetFloatField(JNIEnv* p0, jobject p1, jfieldID p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetFloatField()\n");
    return 0.f;
}


jdouble
JNIEnv_GetDoubleField(JNIEnv* p0, jobject p1, jfieldID p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetDoubleField()\n");
    return 0.;
}


void
JNIEnv_SetObjectField(JNIEnv* p0, jobject p1, jfieldID p2, jobject p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetObjectField()\n");
}


void
JNIEnv_SetBooleanField(JNIEnv* p0, jobject p1, jfieldID p2, jboolean p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetBooleanField()\n");
}


void
JNIEnv_SetByteField(JNIEnv* p0, jobject p1, jfieldID p2, jbyte p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetByteField()\n");
}


void
JNIEnv_SetCharField(JNIEnv* p0, jobject p1, jfieldID p2, jchar p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetCharField()\n");
}


void
JNIEnv_SetShortField(JNIEnv* p0, jobject p1, jfieldID p2, jshort p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetShortField()\n");
}


void
JNIEnv_SetIntField(JNIEnv* p0, jobject p1, jfieldID p2, jint p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetIntField()\n");
}


void
JNIEnv_SetLongField(JNIEnv* p0, jobject p1, jfieldID p2, jlong p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetLongField()\n");
}


void
JNIEnv_SetFloatField(JNIEnv* p0, jobject p1, jfieldID p2, jfloat p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetFloatField()\n");
}


void
JNIEnv_SetDoubleField(JNIEnv* p0, jobject p1, jfieldID p2, jdouble p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetDoubleField()\n");
}


jmethodID
JNIEnv_GetStaticMethodID(JNIEnv* p0, jclass clazz, const char* name, const char* sig)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticMethodID(%x, %s, %s)\n",
            clazz, name, sig);
    return jnienv_make_method(clazz, name, sig);
}


jobject
JNIEnv_CallStaticObjectMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticObjectMethod(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return NULL;
}


jobject
JNIEnv_CallStaticObjectMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticObjectMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return NULL;
}


jobject
JNIEnv_CallStaticObjectMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticObjectMethodA(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return NULL;
}


jboolean
JNIEnv_CallStaticBooleanMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticBooleanMethod(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jboolean
JNIEnv_CallStaticBooleanMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticBooleanMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jboolean
JNIEnv_CallStaticBooleanMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticBooleanMethodA(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jbyte
JNIEnv_CallStaticByteMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticByteMethod(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jbyte
JNIEnv_CallStaticByteMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticByteMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jbyte
JNIEnv_CallStaticByteMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticByteMethodA(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jchar
JNIEnv_CallStaticCharMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticCharMethod(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jchar
JNIEnv_CallStaticCharMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticCharMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jchar
JNIEnv_CallStaticCharMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticCharMethodA(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jshort
JNIEnv_CallStaticShortMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticShortMethod(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jshort
JNIEnv_CallStaticShortMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticShortMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jshort
JNIEnv_CallStaticShortMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticShortMethodA(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jint
JNIEnv_CallStaticIntMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticIntMethod(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jint
JNIEnv_CallStaticIntMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticIntMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jint
JNIEnv_CallStaticIntMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticIntMethodA(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jlong
JNIEnv_CallStaticLongMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticLongMethod(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jlong
JNIEnv_CallStaticLongMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticLongMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jlong
JNIEnv_CallStaticLongMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticLongMethodA(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jfloat
JNIEnv_CallStaticFloatMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticFloatMethod(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0.f;
}


jfloat
JNIEnv_CallStaticFloatMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticFloatMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0.f;
}


jfloat
JNIEnv_CallStaticFloatMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticFloatMethodA(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0.f;
}


jdouble
JNIEnv_CallStaticDoubleMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticDoubleMethod(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0.;
}


jdouble
JNIEnv_CallStaticDoubleMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticDoubleMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0.;
}


jdouble
JNIEnv_CallStaticDoubleMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticDoubleMethodA(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
    return 0.;
}


void
JNIEnv_CallStaticVoidMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticVoidMethod(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
}


void
JNIEnv_CallStaticVoidMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticVoidMethodV(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
}


void
JNIEnv_CallStaticVoidMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_CallStaticVoidMethodA(%s, %s/%s, ...)\n",
            jcl->name, p2->name, p2->sig);
}


jfieldID
JNIEnv_GetStaticFieldID(JNIEnv* p0, jclass clazz, const char* name, const char* sig)
{
    struct dummy_jclass *class = (struct dummy_jclass*)clazz;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticFieldID(%s, '%s', '%s')\n", class->name, name, sig);
    return jnienv_make_fieldid(clazz, name, sig);
}


jobject
JNIEnv_GetStaticObjectField(JNIEnv* p0, jclass p1, jfieldID p2)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticObjectField(%s, %s/%s)\n",
            jcl->name, p2->name, p2->sig);
    return NULL;
}


jboolean
JNIEnv_GetStaticBooleanField(JNIEnv* p0, jclass p1, jfieldID p2)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticBooleanField(%s, %s/%s)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jbyte
JNIEnv_GetStaticByteField(JNIEnv* p0, jclass p1, jfieldID p2)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticByteField(%s, %s/%s)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jchar
JNIEnv_GetStaticCharField(JNIEnv* p0, jclass p1, jfieldID p2)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticCharField(%s, %s/%s)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jshort
JNIEnv_GetStaticShortField(JNIEnv* p0, jclass p1, jfieldID p2)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticShortField(%s, %s/%s)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jint
JNIEnv_GetStaticIntField(JNIEnv* p0, jclass p1, jfieldID p2)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticIntField(%s, %s/%s)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jlong
JNIEnv_GetStaticLongField(JNIEnv* p0, jclass p1, jfieldID p2)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticLongField(%s, %s/%s)\n",
            jcl->name, p2->name, p2->sig);
    return 0;
}


jfloat
JNIEnv_GetStaticFloatField(JNIEnv* p0, jclass p1, jfieldID p2)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticFloatField(%s, %s/%s)\n",
            jcl->name, p2->name, p2->sig);
    return 0.f;
}


jdouble
JNIEnv_GetStaticDoubleField(JNIEnv* p0, jclass p1, jfieldID p2)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStaticDoubleField(%s, %s/%s)\n",
            jcl->name, p2->name, p2->sig);
    return 0.;
}


void
JNIEnv_SetStaticObjectField(JNIEnv* p0, jclass p1, jfieldID p2, jobject p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_SetStaticObjectField(%s, %s/%s, %p)\n",
            jcl->name, p2->name, p2->sig, p3);
}


void
JNIEnv_SetStaticBooleanField(JNIEnv* p0, jclass p1, jfieldID p2, jboolean p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_SetStaticBooleanField(%s, %s/%s, %d)\n",
            jcl->name, p2->name, p2->sig, p3);
}


void
JNIEnv_SetStaticByteField(JNIEnv* p0, jclass p1, jfieldID p2, jbyte p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_SetStaticByteField(%s, %s/%s, %02x)\n",
            jcl->name, p2->name, p2->sig, p3);
}


void
JNIEnv_SetStaticCharField(JNIEnv* p0, jclass p1, jfieldID p2, jchar p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_SetStaticCharField(%s, %s/%s, '%c')\n",
            jcl->name, p2->name, p2->sig, p3);
}


void
JNIEnv_SetStaticShortField(JNIEnv* p0, jclass p1, jfieldID p2, jshort p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_SetStaticShortField(%s, %s/%s, %d)\n",
            jcl->name, p2->name, p2->sig, p3);
}


void
JNIEnv_SetStaticIntField(JNIEnv* p0, jclass p1, jfieldID p2, jint p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_SetStaticIntField(%s, %s/%s, %d)\n",
            jcl->name, p2->name, p2->sig, p3);
}


void
JNIEnv_SetStaticLongField(JNIEnv* p0, jclass p1, jfieldID p2, jlong p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_SetStaticLongField(%s, %s/%s, %ld)\n",
            jcl->name, p2->name, p2->sig, (long)p3);
}


void
JNIEnv_SetStaticFloatField(JNIEnv* p0, jclass p1, jfieldID p2, jfloat p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_SetStaticFloatField(%s, %s/%s, %.3f)\n",
            jcl->name, p2->name, p2->sig, p3);
}


void
JNIEnv_SetStaticDoubleField(JNIEnv* p0, jclass p1, jfieldID p2, jdouble p3)
{
    struct dummy_jclass *jcl = p1;
    JNIENV_DEBUG_PRINTF("JNIEnv_SetStaticDoubleField(%s, %s/%s, %.3f)\n",
            jcl->name, p2->name, p2->sig, p3);
}


jstring
JNIEnv_NewString(JNIEnv* p0, const jchar* p1, jsize p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewString(%s, %d)\n", p1, p2);
    char *str = malloc(p2+1);
    int i;
    for (i=0; i<p2; i++) {
        str[i] = p1[i];
    }
    str[i] = '\0';
    JNIENV_DEBUG_PRINTF(" \\-> %s\n", str);
    struct dummy_jstring *result = malloc(sizeof(struct dummy_jstring));
    result->data = str;
    return result;
}


jsize
JNIEnv_GetStringLength(JNIEnv* p0, jstring p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStringLength()\n");
    return 0;
}


const jchar*
JNIEnv_GetStringChars(JNIEnv* p0, jstring p1, jboolean* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStringChars()\n");
    return NULL;
}


void
JNIEnv_ReleaseStringChars(JNIEnv* p0, jstring p1, const jchar* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseStringChars()\n");
}


jstring
JNIEnv_NewStringUTF(JNIEnv* p0, const char* p1)
{
    struct dummy_jstring *result = malloc(sizeof(struct dummy_jstring));
    JNIENV_DEBUG_PRINTF("JNIEnv_NewStringUTF('%s') -> %x\n", p1, result);
    result->data = strdup(p1);
    return result;
}


jsize
JNIEnv_GetStringUTFLength(JNIEnv* p0, jstring p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStringUTFLength()\n");
    struct dummy_jstring *result = p1;
    return strlen(result->data);
}


jsize
JNIEnv_GetArrayLength(JNIEnv* env, jarray p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetArrayLength(%p)\n", p1);
    jsize ret = 0;
    if (CHECK_OBJ(p1)) {
        struct dummy_array *array = p1;
        ret = array->length;
    }
    JNIENV_DEBUG_PRINTF(" -> %d\n", ret);
    return ret;
}


jobjectArray
JNIEnv_NewObjectArray(JNIEnv* p0, jsize p1, jclass p2, jobject p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewObjectArray()\n");
    return NULL;
}


jobject
JNIEnv_GetObjectArrayElement(JNIEnv* p0, jobjectArray p1, jsize p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetObjectArrayElement()\n");
    return NULL;
}


void
JNIEnv_SetObjectArrayElement(JNIEnv* p0, jobjectArray p1, jsize p2, jobject p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetObjectArrayElement()\n");
}


static void *
new_array(jsize len, jsize element_size)
{
    struct dummy_array *array;
    if (len <= 0) {
        JNIENV_DEBUG_PRINTF("new_array: len is %d\n", len);
        return NULL;
    }

    array = malloc(sizeof(*array));
    if (array == NULL)
        return NULL;
    array->data = calloc(len, element_size);
    if (array->data == NULL) {
        free(array->data);
        return NULL;
    }
    array->element_size = element_size;
    array->length = len;
    return array;
}


jbooleanArray
JNIEnv_NewBooleanArray(JNIEnv* p0, jsize p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewBooleanArray(%d)\n", p1);
    return new_array(p1, sizeof(jboolean));
}


jbyteArray
JNIEnv_NewByteArray(JNIEnv* p0, jsize p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewByteArray(%d)\n", p1);
    return new_array(p1, sizeof(jbyte));
}


jcharArray
JNIEnv_NewCharArray(JNIEnv* p0, jsize p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewCharArray(%d)\n", p1);
    return new_array(p1, sizeof(jchar));
}


jshortArray
JNIEnv_NewShortArray(JNIEnv* p0, jsize p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewShortArray(%d)\n", p1);
    return new_array(p1, sizeof(jshort));
}


jintArray
JNIEnv_NewIntArray(JNIEnv* p0, jsize p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewIntArray(%d)\n", p1);
    return new_array(p1, sizeof(jint));
}


jlongArray
JNIEnv_NewLongArray(JNIEnv* p0, jsize p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewLongArray(%d)\n", p1);
    return new_array(p1, sizeof(jlong));
}


jfloatArray
JNIEnv_NewFloatArray(JNIEnv* p0, jsize p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewFloatArray(%d)\n", p1);
    return new_array(p1, sizeof(jfloat));
}


jdoubleArray
JNIEnv_NewDoubleArray(JNIEnv* p0, jsize p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewDoubleArray(%d)\n", p1);
    return new_array(p1, sizeof(jdouble));
}


static void *
get_array_elements(JNIEnv *env, void *array, jboolean *isCopy)
{
    void *ret = NULL;
    if (CHECK_OBJ(array)) {
        struct dummy_array *my_array = array;
        ret = my_array->data;
        if (isCopy)
            *isCopy = JNI_FALSE;
    }
    JNIENV_DEBUG_PRINTF(" -> %p\n", ret);
    return ret;
}

jboolean*
JNIEnv_GetBooleanArrayElements(JNIEnv* p0, jbooleanArray p1, jboolean* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetBooleanArrayElements(%p, %p)\n", p1, p2);
    return get_array_elements(p0, p1, p2);
}


jbyte*
JNIEnv_GetByteArrayElements(JNIEnv* p0, jbyteArray p1, jboolean* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetByteArrayElements(%p, %p)\n", p1, p2);
    return get_array_elements(p0, p1, p2);
}


jchar*
JNIEnv_GetCharArrayElements(JNIEnv* p0, jcharArray p1, jboolean* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetCharArrayElements(%p, %p)\n", p1, p2);
    return get_array_elements(p0, p1, p2);
}


jshort*
JNIEnv_GetShortArrayElements(JNIEnv* p0, jshortArray p1, jboolean* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetShortArrayElements(%p, %p)\n", p1, p2);
    return get_array_elements(p0, p1, p2);
}


jint*
JNIEnv_GetIntArrayElements(JNIEnv* p0, jintArray p1, jboolean* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetIntArrayElements(%p, %p)\n", p1, p2);
    return get_array_elements(p0, p1, p2);
}


jlong*
JNIEnv_GetLongArrayElements(JNIEnv* p0, jlongArray p1, jboolean* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetLongArrayElements(%p, %p)\n", p1, p2);
    return get_array_elements(p0, p1, p2);
}


jfloat*
JNIEnv_GetFloatArrayElements(JNIEnv* p0, jfloatArray p1, jboolean* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetFloatArrayElements(%p, %p)\n", p1, p2);
    return get_array_elements(p0, p1, p2);
}


jdouble*
JNIEnv_GetDoubleArrayElements(JNIEnv* p0, jdoubleArray p1, jboolean* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetDoubleArrayElements(%p, %p)\n", p1, p2);
    return get_array_elements(p0, p1, p2);
}


void
JNIEnv_ReleaseBooleanArrayElements(JNIEnv* p0, jbooleanArray p1, jboolean* p2, jint p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseBooleanArrayElements()\n");
}


void
JNIEnv_ReleaseByteArrayElements(JNIEnv* p0, jbyteArray p1, jbyte* p2, jint p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseByteArrayElements()\n");
}


void
JNIEnv_ReleaseCharArrayElements(JNIEnv* p0, jcharArray p1, jchar* p2, jint p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseCharArrayElements()\n");
}


void
JNIEnv_ReleaseShortArrayElements(JNIEnv* p0, jshortArray p1, jshort* p2, jint p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseShortArrayElements()\n");
}


void
JNIEnv_ReleaseIntArrayElements(JNIEnv* p0, jintArray p1, jint* p2, jint p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseIntArrayElements()\n");
}


void
JNIEnv_ReleaseLongArrayElements(JNIEnv* p0, jlongArray p1, jlong* p2, jint p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseLongArrayElements()\n");
}


void
JNIEnv_ReleaseFloatArrayElements(JNIEnv* p0, jfloatArray p1, jfloat* p2, jint p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseFloatArrayElements()\n");
}


void
JNIEnv_ReleaseDoubleArrayElements(JNIEnv* p0, jdoubleArray p1, jdouble* p2, jint p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseDoubleArrayElements()\n");
}


void
JNIEnv_GetBooleanArrayRegion(JNIEnv* p0, jbooleanArray p1, jsize p2, jsize p3, jboolean* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetBooleanArrayRegion()\n");
}


static void
get_array_region(JNIEnv *env, const void *arrayobj, jsize start, jsize len, void *buf)
{
    if (CHECK_OBJ(arrayobj)) {
        const struct dummy_array *array = arrayobj;
        if (start + len > array->length)
            fprintf(stderr, "set_array_region: ArrayIndexOutOfBounds\n");
        start *= array->element_size;
        len *= array->element_size;
        memcpy(buf, (char *)array->data + start, len);
    }
}

void
JNIEnv_GetByteArrayRegion(JNIEnv *env, jbyteArray arrayobj, jsize start, jsize len, jbyte* buf)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetByteArrayRegion(%x, %d, %d, %x)\n", arrayobj, start, len, buf);
    get_array_region(env, arrayobj, start, len, buf);
}


void
JNIEnv_GetCharArrayRegion(JNIEnv* p0, jcharArray p1, jsize p2, jsize p3, jchar* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetCharArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    get_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_GetShortArrayRegion(JNIEnv* p0, jshortArray p1, jsize p2, jsize p3, jshort* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetShortArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    get_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_GetIntArrayRegion(JNIEnv* p0, jintArray p1, jsize p2, jsize p3, jint* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetIntArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    get_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_GetLongArrayRegion(JNIEnv* p0, jlongArray p1, jsize p2, jsize p3, jlong* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetLongArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    get_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_GetFloatArrayRegion(JNIEnv* p0, jfloatArray p1, jsize p2, jsize p3, jfloat* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetFloatArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    get_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_GetDoubleArrayRegion(JNIEnv* p0, jdoubleArray p1, jsize p2, jsize p3, jdouble* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetDoubleArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    get_array_region(p0, p1, p2, p3, p4);
}


static void
set_array_region(JNIEnv *env, void *arrayobj, jsize start, jsize len, const void *buf)
{
    if (CHECK_OBJ(arrayobj)) {
        const struct dummy_array *array = arrayobj;
        if (start + len > array->length)
            fprintf(stderr, "set_array_region: ArrayIndexOutOfBounds\n");
        start *= array->element_size;
        len *= array->element_size;
        memcpy((char *)array->data + start, buf, len);
    }
}


void
JNIEnv_SetBooleanArrayRegion(JNIEnv* p0, jbooleanArray p1, jsize p2, jsize p3, const jboolean* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetBooleanArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    set_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_SetByteArrayRegion(JNIEnv* p0, jbyteArray p1, jsize p2, jsize p3, const jbyte* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetByteArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    set_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_SetCharArrayRegion(JNIEnv* p0, jcharArray p1, jsize p2, jsize p3, const jchar* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetCharArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    set_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_SetShortArrayRegion(JNIEnv* p0, jshortArray p1, jsize p2, jsize p3, const jshort* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetShortArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    set_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_SetIntArrayRegion(JNIEnv* p0, jintArray p1, jsize p2, jsize p3, const jint* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetIntArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    set_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_SetLongArrayRegion(JNIEnv* p0, jlongArray p1, jsize p2, jsize p3, const jlong* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetLongArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    set_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_SetFloatArrayRegion(JNIEnv* p0, jfloatArray p1, jsize p2, jsize p3, const jfloat* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetFloatArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    set_array_region(p0, p1, p2, p3, p4);
}


void
JNIEnv_SetDoubleArrayRegion(JNIEnv* p0, jdoubleArray p1, jsize p2, jsize p3, const jdouble* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_SetDoubleArrayRegion(%p, %d, %d, %p)\n", p1, p2, p3, p4);
    set_array_region(p0, p1, p2, p3, p4);
}


jint
JNIEnv_RegisterNatives(JNIEnv* p0, jclass p1, const JNINativeMethod* p2, jint p3)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_RegisterNatives()\n");

    struct dummy_jclass *clazz = (struct dummy_jclass*)p1;
    JNIENV_DEBUG_PRINTF("\n\tClass: %s\n", clazz->name);

    int i=0;
    const JNINativeMethod *method = p2;
    while (i<p3) {
        JNIENV_DEBUG_PRINTF("\tName: %-20s Sig: %-10s Addr: %x\n", method->name, method->signature, method->fnPtr);
        method++;
        i++;
    }
    JNIENV_DEBUG_PRINTF("\n");
    return 0;
}


jint
JNIEnv_UnregisterNatives(JNIEnv* p0, jclass p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_UnregisterNatives()\n");
    return 0;
}


jint
JNIEnv_MonitorEnter(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_MonitorEnter()\n");
    return 0;
}


jint
JNIEnv_MonitorExit(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_MonitorExit()\n");
    return 0;
}


jint
JNIEnv_GetJavaVM(JNIEnv* env, JavaVM** vm)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetJavaVM()\n");
    struct GlobalState *global = GLOBAL_J(env);
    *vm = VM(global);
    return 0;
}


void
JNIEnv_GetStringRegion(JNIEnv* p0, jstring p1, jsize p2, jsize p3, jchar* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStringRegion()\n");
}


void
JNIEnv_GetStringUTFRegion(JNIEnv* p0, jstring p1, jsize p2, jsize p3, char* p4)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStringUTFRegion()\n");
}


void*
JNIEnv_GetPrimitiveArrayCritical(JNIEnv *env, jarray array, jboolean *isCopy)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetPrimitiveArrayCritical(%p, %p)\n", array, isCopy);
    return get_array_elements(env, array, isCopy);
}


void
JNIEnv_ReleasePrimitiveArrayCritical(JNIEnv *env, jarray array, void *carray, jint mode)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleasePrimitiveArrayCritical(%p, %p, %x)\n", array, carray, mode);
}


const jchar*
JNIEnv_GetStringCritical(JNIEnv* p0, jstring p1, jboolean* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStringCritical()\n");
    return NULL;
}


void
JNIEnv_ReleaseStringCritical(JNIEnv* p0, jstring p1, const jchar* p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseStringCritical()\n");
}


jweak
JNIEnv_NewWeakGlobalRef(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewWeakGlobalRef()\n");
    return NULL;
}


void
JNIEnv_DeleteWeakGlobalRef(JNIEnv* p0, jweak p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_DeleteWeakGlobalRef()\n");
}


jboolean
JNIEnv_ExceptionCheck(JNIEnv* p0)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ExceptionCheck()\n");
    return 0;
}


jobject
JNIEnv_NewDirectByteBuffer(JNIEnv* p0, void* p1, jlong p2)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_NewDirectByteBuffer()\n");
    return NULL;
}


void*
JNIEnv_GetDirectBufferAddress(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetDirectBufferAddress()\n");
    return NULL;
}


jlong
JNIEnv_GetDirectBufferCapacity(JNIEnv* p0, jobject p1)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetDirectBufferCapacity()\n");
    return 0;
}

const char *
JNIEnv_GetStringUTFChars(JNIEnv *env, jstring string, jboolean *isCopy)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_GetStringUTFChars(%x)\n", string);
    if (string == GLOBAL_J(env)) {
        JNIENV_DEBUG_PRINTF("WARNING: GetStringUTFChars on global\n");
        return NULL;
    }
    if (string == NULL) {
        return strdup("");
    }
    struct dummy_jstring *str = (struct dummy_jstring*)string;
    JNIENV_DEBUG_PRINTF(" \\-> %s\n", str->data);
    return str->data;
}

void
JNIEnv_ReleaseStringUTFChars(JNIEnv *env, jstring string, const char *utf)
{
    JNIENV_DEBUG_PRINTF("JNIEnv_ReleaseStringUTFChars()\n");
    if (string == GLOBAL_J(env)) {
        JNIENV_DEBUG_PRINTF("WARNING: ReleaseStringUTFChars on global\n");
        return;
    }
    free((void*)utf);
}

void
jnienv_init(struct GlobalState *global)
{
    global->fake_env.reserved0 = global;
    global->fake_env.GetStringUTFChars = JNIEnv_GetStringUTFChars;
    global->fake_env.ReleaseStringUTFChars = JNIEnv_ReleaseStringUTFChars;
    global->fake_env.GetVersion = JNIEnv_GetVersion;
    global->fake_env.DefineClass = JNIEnv_DefineClass;
    global->fake_env.FindClass = JNIEnv_FindClass;
    global->fake_env.FromReflectedMethod = JNIEnv_FromReflectedMethod;
    global->fake_env.FromReflectedField = JNIEnv_FromReflectedField;
    global->fake_env.ToReflectedMethod = JNIEnv_ToReflectedMethod;
    global->fake_env.GetSuperclass = JNIEnv_GetSuperclass;
    global->fake_env.IsAssignableFrom = JNIEnv_IsAssignableFrom;
    global->fake_env.ToReflectedField = JNIEnv_ToReflectedField;
    global->fake_env.Throw = JNIEnv_Throw;
    global->fake_env.ThrowNew = JNIEnv_ThrowNew;
    global->fake_env.ExceptionOccurred = JNIEnv_ExceptionOccurred;
    global->fake_env.ExceptionDescribe = JNIEnv_ExceptionDescribe;
    global->fake_env.ExceptionClear = JNIEnv_ExceptionClear;
    global->fake_env.FatalError = JNIEnv_FatalError;
    global->fake_env.PushLocalFrame = JNIEnv_PushLocalFrame;
    global->fake_env.PopLocalFrame = JNIEnv_PopLocalFrame;
    global->fake_env.NewGlobalRef = JNIEnv_NewGlobalRef;
    global->fake_env.DeleteGlobalRef = JNIEnv_DeleteGlobalRef;
    global->fake_env.DeleteLocalRef = JNIEnv_DeleteLocalRef;
    global->fake_env.IsSameObject = JNIEnv_IsSameObject;
    global->fake_env.NewLocalRef = JNIEnv_NewLocalRef;
    global->fake_env.EnsureLocalCapacity = JNIEnv_EnsureLocalCapacity;
    global->fake_env.AllocObject = JNIEnv_AllocObject;
    global->fake_env.NewObject = JNIEnv_NewObject;
    global->fake_env.NewObjectV = JNIEnv_NewObjectV;
    global->fake_env.NewObjectA = JNIEnv_NewObjectA;
    global->fake_env.GetObjectClass = JNIEnv_GetObjectClass;
    global->fake_env.IsInstanceOf = JNIEnv_IsInstanceOf;
    global->fake_env.GetMethodID = JNIEnv_GetMethodID;
    global->fake_env.CallObjectMethod = JNIEnv_CallObjectMethod;
    global->fake_env.CallObjectMethodV = JNIEnv_CallObjectMethodV;
    global->fake_env.CallObjectMethodA = JNIEnv_CallObjectMethodA;
    global->fake_env.CallBooleanMethod = JNIEnv_CallBooleanMethod;
    global->fake_env.CallBooleanMethodV = JNIEnv_CallBooleanMethodV;
    global->fake_env.CallBooleanMethodA = JNIEnv_CallBooleanMethodA;
    global->fake_env.CallByteMethod = JNIEnv_CallByteMethod;
    global->fake_env.CallByteMethodV = JNIEnv_CallByteMethodV;
    global->fake_env.CallByteMethodA = JNIEnv_CallByteMethodA;
    global->fake_env.CallCharMethod = JNIEnv_CallCharMethod;
    global->fake_env.CallCharMethodV = JNIEnv_CallCharMethodV;
    global->fake_env.CallCharMethodA = JNIEnv_CallCharMethodA;
    global->fake_env.CallShortMethod = JNIEnv_CallShortMethod;
    global->fake_env.CallShortMethodV = JNIEnv_CallShortMethodV;
    global->fake_env.CallShortMethodA = JNIEnv_CallShortMethodA;
    global->fake_env.CallIntMethod = JNIEnv_CallIntMethod;
    global->fake_env.CallIntMethodV = JNIEnv_CallIntMethodV;
    global->fake_env.CallIntMethodA = JNIEnv_CallIntMethodA;
    global->fake_env.CallLongMethod = JNIEnv_CallLongMethod;
    global->fake_env.CallLongMethodV = JNIEnv_CallLongMethodV;
    global->fake_env.CallLongMethodA = JNIEnv_CallLongMethodA;
    global->fake_env.CallFloatMethod = JNIEnv_CallFloatMethod;
    global->fake_env.CallFloatMethodV = JNIEnv_CallFloatMethodV;
    global->fake_env.CallFloatMethodA = JNIEnv_CallFloatMethodA;
    global->fake_env.CallDoubleMethod = JNIEnv_CallDoubleMethod;
    global->fake_env.CallDoubleMethodV = JNIEnv_CallDoubleMethodV;
    global->fake_env.CallDoubleMethodA = JNIEnv_CallDoubleMethodA;
    global->fake_env.CallVoidMethod = JNIEnv_CallVoidMethod;
    global->fake_env.CallVoidMethodV = JNIEnv_CallVoidMethodV;
    global->fake_env.CallVoidMethodA = JNIEnv_CallVoidMethodA;
    global->fake_env.CallNonvirtualObjectMethod = JNIEnv_CallNonvirtualObjectMethod;
    global->fake_env.CallNonvirtualObjectMethodV = JNIEnv_CallNonvirtualObjectMethodV;
    global->fake_env.CallNonvirtualObjectMethodA = JNIEnv_CallNonvirtualObjectMethodA;
    global->fake_env.CallNonvirtualBooleanMethod = JNIEnv_CallNonvirtualBooleanMethod;
    global->fake_env.CallNonvirtualBooleanMethodV = JNIEnv_CallNonvirtualBooleanMethodV;
    global->fake_env.CallNonvirtualBooleanMethodA = JNIEnv_CallNonvirtualBooleanMethodA;
    global->fake_env.CallNonvirtualByteMethod = JNIEnv_CallNonvirtualByteMethod;
    global->fake_env.CallNonvirtualByteMethodV = JNIEnv_CallNonvirtualByteMethodV;
    global->fake_env.CallNonvirtualByteMethodA = JNIEnv_CallNonvirtualByteMethodA;
    global->fake_env.CallNonvirtualCharMethod = JNIEnv_CallNonvirtualCharMethod;
    global->fake_env.CallNonvirtualCharMethodV = JNIEnv_CallNonvirtualCharMethodV;
    global->fake_env.CallNonvirtualCharMethodA = JNIEnv_CallNonvirtualCharMethodA;
    global->fake_env.CallNonvirtualShortMethod = JNIEnv_CallNonvirtualShortMethod;
    global->fake_env.CallNonvirtualShortMethodV = JNIEnv_CallNonvirtualShortMethodV;
    global->fake_env.CallNonvirtualShortMethodA = JNIEnv_CallNonvirtualShortMethodA;
    global->fake_env.CallNonvirtualIntMethod = JNIEnv_CallNonvirtualIntMethod;
    global->fake_env.CallNonvirtualIntMethodV = JNIEnv_CallNonvirtualIntMethodV;
    global->fake_env.CallNonvirtualIntMethodA = JNIEnv_CallNonvirtualIntMethodA;
    global->fake_env.CallNonvirtualLongMethod = JNIEnv_CallNonvirtualLongMethod;
    global->fake_env.CallNonvirtualLongMethodV = JNIEnv_CallNonvirtualLongMethodV;
    global->fake_env.CallNonvirtualLongMethodA = JNIEnv_CallNonvirtualLongMethodA;
    global->fake_env.CallNonvirtualFloatMethod = JNIEnv_CallNonvirtualFloatMethod;
    global->fake_env.CallNonvirtualFloatMethodV = JNIEnv_CallNonvirtualFloatMethodV;
    global->fake_env.CallNonvirtualFloatMethodA = JNIEnv_CallNonvirtualFloatMethodA;
    global->fake_env.CallNonvirtualDoubleMethod = JNIEnv_CallNonvirtualDoubleMethod;
    global->fake_env.CallNonvirtualDoubleMethodV = JNIEnv_CallNonvirtualDoubleMethodV;
    global->fake_env.CallNonvirtualDoubleMethodA = JNIEnv_CallNonvirtualDoubleMethodA;
    global->fake_env.CallNonvirtualVoidMethod = JNIEnv_CallNonvirtualVoidMethod;
    global->fake_env.CallNonvirtualVoidMethodV = JNIEnv_CallNonvirtualVoidMethodV;
    global->fake_env.CallNonvirtualVoidMethodA = JNIEnv_CallNonvirtualVoidMethodA;
    global->fake_env.GetFieldID = JNIEnv_GetFieldID;
    global->fake_env.GetObjectField = JNIEnv_GetObjectField;
    global->fake_env.GetBooleanField = JNIEnv_GetBooleanField;
    global->fake_env.GetByteField = JNIEnv_GetByteField;
    global->fake_env.GetCharField = JNIEnv_GetCharField;
    global->fake_env.GetShortField = JNIEnv_GetShortField;
    global->fake_env.GetIntField = JNIEnv_GetIntField;
    global->fake_env.GetLongField = JNIEnv_GetLongField;
    global->fake_env.GetFloatField = JNIEnv_GetFloatField;
    global->fake_env.GetDoubleField = JNIEnv_GetDoubleField;
    global->fake_env.SetObjectField = JNIEnv_SetObjectField;
    global->fake_env.SetBooleanField = JNIEnv_SetBooleanField;
    global->fake_env.SetByteField = JNIEnv_SetByteField;
    global->fake_env.SetCharField = JNIEnv_SetCharField;
    global->fake_env.SetShortField = JNIEnv_SetShortField;
    global->fake_env.SetIntField = JNIEnv_SetIntField;
    global->fake_env.SetLongField = JNIEnv_SetLongField;
    global->fake_env.SetFloatField = JNIEnv_SetFloatField;
    global->fake_env.SetDoubleField = JNIEnv_SetDoubleField;
    global->fake_env.GetStaticMethodID = JNIEnv_GetStaticMethodID;
    global->fake_env.CallStaticObjectMethod = JNIEnv_CallStaticObjectMethod;
    global->fake_env.CallStaticObjectMethodV = JNIEnv_CallStaticObjectMethodV;
    global->fake_env.CallStaticObjectMethodA = JNIEnv_CallStaticObjectMethodA;
    global->fake_env.CallStaticBooleanMethod = JNIEnv_CallStaticBooleanMethod;
    global->fake_env.CallStaticBooleanMethodV = JNIEnv_CallStaticBooleanMethodV;
    global->fake_env.CallStaticBooleanMethodA = JNIEnv_CallStaticBooleanMethodA;
    global->fake_env.CallStaticByteMethod = JNIEnv_CallStaticByteMethod;
    global->fake_env.CallStaticByteMethodV = JNIEnv_CallStaticByteMethodV;
    global->fake_env.CallStaticByteMethodA = JNIEnv_CallStaticByteMethodA;
    global->fake_env.CallStaticCharMethod = JNIEnv_CallStaticCharMethod;
    global->fake_env.CallStaticCharMethodV = JNIEnv_CallStaticCharMethodV;
    global->fake_env.CallStaticCharMethodA = JNIEnv_CallStaticCharMethodA;
    global->fake_env.CallStaticShortMethod = JNIEnv_CallStaticShortMethod;
    global->fake_env.CallStaticShortMethodV = JNIEnv_CallStaticShortMethodV;
    global->fake_env.CallStaticShortMethodA = JNIEnv_CallStaticShortMethodA;
    global->fake_env.CallStaticIntMethod = JNIEnv_CallStaticIntMethod;
    global->fake_env.CallStaticIntMethodV = JNIEnv_CallStaticIntMethodV;
    global->fake_env.CallStaticIntMethodA = JNIEnv_CallStaticIntMethodA;
    global->fake_env.CallStaticLongMethod = JNIEnv_CallStaticLongMethod;
    global->fake_env.CallStaticLongMethodV = JNIEnv_CallStaticLongMethodV;
    global->fake_env.CallStaticLongMethodA = JNIEnv_CallStaticLongMethodA;
    global->fake_env.CallStaticFloatMethod = JNIEnv_CallStaticFloatMethod;
    global->fake_env.CallStaticFloatMethodV = JNIEnv_CallStaticFloatMethodV;
    global->fake_env.CallStaticFloatMethodA = JNIEnv_CallStaticFloatMethodA;
    global->fake_env.CallStaticDoubleMethod = JNIEnv_CallStaticDoubleMethod;
    global->fake_env.CallStaticDoubleMethodV = JNIEnv_CallStaticDoubleMethodV;
    global->fake_env.CallStaticDoubleMethodA = JNIEnv_CallStaticDoubleMethodA;
    global->fake_env.CallStaticVoidMethod = JNIEnv_CallStaticVoidMethod;
    global->fake_env.CallStaticVoidMethodV = JNIEnv_CallStaticVoidMethodV;
    global->fake_env.CallStaticVoidMethodA = JNIEnv_CallStaticVoidMethodA;
    global->fake_env.GetStaticFieldID = JNIEnv_GetStaticFieldID;
    global->fake_env.GetStaticObjectField = JNIEnv_GetStaticObjectField;
    global->fake_env.GetStaticBooleanField = JNIEnv_GetStaticBooleanField;
    global->fake_env.GetStaticByteField = JNIEnv_GetStaticByteField;
    global->fake_env.GetStaticCharField = JNIEnv_GetStaticCharField;
    global->fake_env.GetStaticShortField = JNIEnv_GetStaticShortField;
    global->fake_env.GetStaticIntField = JNIEnv_GetStaticIntField;
    global->fake_env.GetStaticLongField = JNIEnv_GetStaticLongField;
    global->fake_env.GetStaticFloatField = JNIEnv_GetStaticFloatField;
    global->fake_env.GetStaticDoubleField = JNIEnv_GetStaticDoubleField;
    global->fake_env.SetStaticObjectField = JNIEnv_SetStaticObjectField;
    global->fake_env.SetStaticBooleanField = JNIEnv_SetStaticBooleanField;
    global->fake_env.SetStaticByteField = JNIEnv_SetStaticByteField;
    global->fake_env.SetStaticCharField = JNIEnv_SetStaticCharField;
    global->fake_env.SetStaticShortField = JNIEnv_SetStaticShortField;
    global->fake_env.SetStaticIntField = JNIEnv_SetStaticIntField;
    global->fake_env.SetStaticLongField = JNIEnv_SetStaticLongField;
    global->fake_env.SetStaticFloatField = JNIEnv_SetStaticFloatField;
    global->fake_env.SetStaticDoubleField = JNIEnv_SetStaticDoubleField;
    global->fake_env.NewString = JNIEnv_NewString;
    global->fake_env.GetStringLength = JNIEnv_GetStringLength;
    global->fake_env.GetStringChars = JNIEnv_GetStringChars;
    global->fake_env.ReleaseStringChars = JNIEnv_ReleaseStringChars;
    global->fake_env.NewStringUTF = JNIEnv_NewStringUTF;
    global->fake_env.GetStringUTFLength = JNIEnv_GetStringUTFLength;
    global->fake_env.GetArrayLength = JNIEnv_GetArrayLength;
    global->fake_env.NewObjectArray = JNIEnv_NewObjectArray;
    global->fake_env.GetObjectArrayElement = JNIEnv_GetObjectArrayElement;
    global->fake_env.SetObjectArrayElement = JNIEnv_SetObjectArrayElement;
    global->fake_env.NewBooleanArray = JNIEnv_NewBooleanArray;
    global->fake_env.NewByteArray = JNIEnv_NewByteArray;
    global->fake_env.NewCharArray = JNIEnv_NewCharArray;
    global->fake_env.NewShortArray = JNIEnv_NewShortArray;
    global->fake_env.NewIntArray = JNIEnv_NewIntArray;
    global->fake_env.NewLongArray = JNIEnv_NewLongArray;
    global->fake_env.NewFloatArray = JNIEnv_NewFloatArray;
    global->fake_env.NewDoubleArray = JNIEnv_NewDoubleArray;
    global->fake_env.GetBooleanArrayElements = JNIEnv_GetBooleanArrayElements;
    global->fake_env.GetByteArrayElements = JNIEnv_GetByteArrayElements;
    global->fake_env.GetCharArrayElements = JNIEnv_GetCharArrayElements;
    global->fake_env.GetShortArrayElements = JNIEnv_GetShortArrayElements;
    global->fake_env.GetIntArrayElements = JNIEnv_GetIntArrayElements;
    global->fake_env.GetLongArrayElements = JNIEnv_GetLongArrayElements;
    global->fake_env.GetFloatArrayElements = JNIEnv_GetFloatArrayElements;
    global->fake_env.GetDoubleArrayElements = JNIEnv_GetDoubleArrayElements;
    global->fake_env.ReleaseBooleanArrayElements = JNIEnv_ReleaseBooleanArrayElements;
    global->fake_env.ReleaseByteArrayElements = JNIEnv_ReleaseByteArrayElements;
    global->fake_env.ReleaseCharArrayElements = JNIEnv_ReleaseCharArrayElements;
    global->fake_env.ReleaseShortArrayElements = JNIEnv_ReleaseShortArrayElements;
    global->fake_env.ReleaseIntArrayElements = JNIEnv_ReleaseIntArrayElements;
    global->fake_env.ReleaseLongArrayElements = JNIEnv_ReleaseLongArrayElements;
    global->fake_env.ReleaseFloatArrayElements = JNIEnv_ReleaseFloatArrayElements;
    global->fake_env.ReleaseDoubleArrayElements = JNIEnv_ReleaseDoubleArrayElements;
    global->fake_env.GetBooleanArrayRegion = JNIEnv_GetBooleanArrayRegion;
    global->fake_env.GetByteArrayRegion = JNIEnv_GetByteArrayRegion;
    global->fake_env.GetCharArrayRegion = JNIEnv_GetCharArrayRegion;
    global->fake_env.GetShortArrayRegion = JNIEnv_GetShortArrayRegion;
    global->fake_env.GetIntArrayRegion = JNIEnv_GetIntArrayRegion;
    global->fake_env.GetLongArrayRegion = JNIEnv_GetLongArrayRegion;
    global->fake_env.GetFloatArrayRegion = JNIEnv_GetFloatArrayRegion;
    global->fake_env.GetDoubleArrayRegion = JNIEnv_GetDoubleArrayRegion;
    global->fake_env.SetBooleanArrayRegion = JNIEnv_SetBooleanArrayRegion;
    global->fake_env.SetByteArrayRegion = JNIEnv_SetByteArrayRegion;
    global->fake_env.SetCharArrayRegion = JNIEnv_SetCharArrayRegion;
    global->fake_env.SetShortArrayRegion = JNIEnv_SetShortArrayRegion;
    global->fake_env.SetIntArrayRegion = JNIEnv_SetIntArrayRegion;
    global->fake_env.SetLongArrayRegion = JNIEnv_SetLongArrayRegion;
    global->fake_env.SetFloatArrayRegion = JNIEnv_SetFloatArrayRegion;
    global->fake_env.SetDoubleArrayRegion = JNIEnv_SetDoubleArrayRegion;
    global->fake_env.RegisterNatives = JNIEnv_RegisterNatives;
    global->fake_env.UnregisterNatives = JNIEnv_UnregisterNatives;
    global->fake_env.MonitorEnter = JNIEnv_MonitorEnter;
    global->fake_env.MonitorExit = JNIEnv_MonitorExit;
    global->fake_env.GetJavaVM = JNIEnv_GetJavaVM;
    global->fake_env.GetStringRegion = JNIEnv_GetStringRegion;
    global->fake_env.GetStringUTFRegion = JNIEnv_GetStringUTFRegion;
    global->fake_env.GetPrimitiveArrayCritical = JNIEnv_GetPrimitiveArrayCritical;
    global->fake_env.ReleasePrimitiveArrayCritical = JNIEnv_ReleasePrimitiveArrayCritical;
    global->fake_env.GetStringCritical = JNIEnv_GetStringCritical;
    global->fake_env.ReleaseStringCritical = JNIEnv_ReleaseStringCritical;
    global->fake_env.NewWeakGlobalRef = JNIEnv_NewWeakGlobalRef;
    global->fake_env.DeleteWeakGlobalRef = JNIEnv_DeleteWeakGlobalRef;
    global->fake_env.ExceptionCheck = JNIEnv_ExceptionCheck;
    global->fake_env.NewDirectByteBuffer = JNIEnv_NewDirectByteBuffer;
    global->fake_env.GetDirectBufferAddress = JNIEnv_GetDirectBufferAddress;
    global->fake_env.GetDirectBufferCapacity = JNIEnv_GetDirectBufferCapacity;
    global->env = &(global->fake_env);
}

jint
JavaVM_DestroyJavaVM(JavaVM *vm)
{
    JNIENV_DEBUG_PRINTF("JavaVM_DestroyJavaVM()\n");
    return 0;
}

jint
JavaVM_AttachCurrentThread(JavaVM *vm, JNIEnv **env, void *args)
{
    JNIENV_DEBUG_PRINTF("JavaVM_AttachCurrentThread()\n");
    struct GlobalState *global = (*vm)->reserved0;
    *env = ENV(global);
    return 0;
}

jint
JavaVM_DetachCurrentThread(JavaVM *vm)
{
    JNIENV_DEBUG_PRINTF("JavaVM_DetachCurrentThread()\n");
    return 0;
}

jint
JavaVM_GetEnv(JavaVM *vm, void **env, jint version)
{
    JNIENV_DEBUG_PRINTF("JavaVM_GetEnv(%x, %d)\n", env, version);
    struct GlobalState *global = (*vm)->reserved0;
    *env = ENV(global);
    return 0;
}

jint
JavaVM_AttachCurrentThreadAsDaemon(JavaVM *vm, JNIEnv **env, void *args)
{
    JNIENV_DEBUG_PRINTF("JavaVM_AttachCurrentThreadAsDaemon()\n");
    struct GlobalState *global = (*vm)->reserved0;
    *env = ENV(global);
    return 0;
}

void
javavm_init(struct GlobalState *global)
{
    global->fake_vm.reserved0 = global;
    global->fake_vm.DestroyJavaVM = JavaVM_DestroyJavaVM;
    global->fake_vm.AttachCurrentThread = JavaVM_AttachCurrentThread;
    global->fake_vm.DetachCurrentThread = JavaVM_DetachCurrentThread;
    global->fake_vm.GetEnv = JavaVM_GetEnv;
    global->fake_vm.AttachCurrentThreadAsDaemon = JavaVM_AttachCurrentThreadAsDaemon;
    global->vm = &(global->fake_vm);
}


