#ifndef JNIENV_H
#define JNIENV_H

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

#include "../apkenv.h"

#include "jni.h"

#define GLOBAL_J(env) ((struct GlobalState*)((*env)->reserved0))

struct dummy_jstring {
    char *data;
};

struct dummy_byte_array {
    char *data;
    long size;
};

struct dummy_short_array {
    short *data;
    long size;
};

struct dummy_int_array {
    int *data;
    long size;
};

struct dummy_float_array {
    float *data;
    long size;
};

struct dummy_jclass {
    char *name;
};

struct _jmethodID {
    jclass clazz;
    char *name;
    char *sig;
};

#define _jfieldID _jmethodID

jint JNIEnv_GetVersion(JNIEnv *p0) SOFTFP;
jclass JNIEnv_DefineClass(JNIEnv *p0, const char *p1, jobject p2, const jbyte *p3, jsize p4) SOFTFP;
jclass JNIEnv_FindClass(JNIEnv *p0, const char *p1) SOFTFP;
jmethodID JNIEnv_FromReflectedMethod(JNIEnv *p0, jobject p1) SOFTFP;
jfieldID JNIEnv_FromReflectedField(JNIEnv *p0, jobject p1) SOFTFP;
jobject JNIEnv_ToReflectedMethod(JNIEnv *p0, jclass p1, jmethodID p2, jboolean p3) SOFTFP;
jclass JNIEnv_GetSuperclass(JNIEnv *p0, jclass p1) SOFTFP;
jboolean JNIEnv_IsAssignableFrom(JNIEnv *p0, jclass p1, jclass p2) SOFTFP;
jobject JNIEnv_ToReflectedField(JNIEnv *p0, jclass p1, jfieldID p2, jboolean p3) SOFTFP;
jint JNIEnv_Throw(JNIEnv *p0, jthrowable p1) SOFTFP;
jint JNIEnv_ThrowNew(JNIEnv *p0, jclass p1, const char *p2) SOFTFP;
jthrowable JNIEnv_ExceptionOccurred(JNIEnv *p0) SOFTFP;
void JNIEnv_ExceptionDescribe(JNIEnv *p0) SOFTFP;
void JNIEnv_ExceptionClear(JNIEnv *p0) SOFTFP;
void JNIEnv_FatalError(JNIEnv *p0, const char *p1) SOFTFP;
jint JNIEnv_PushLocalFrame(JNIEnv *p0, jint p1) SOFTFP;
jobject JNIEnv_PopLocalFrame(JNIEnv *p0, jobject p1) SOFTFP;
jobject JNIEnv_NewGlobalRef(JNIEnv *p0, jobject p1) SOFTFP;
void JNIEnv_DeleteGlobalRef(JNIEnv *p0, jobject p1) SOFTFP;
void JNIEnv_DeleteLocalRef(JNIEnv *p0, jobject p1) SOFTFP;
jboolean JNIEnv_IsSameObject(JNIEnv *p0, jobject p1, jobject p2) SOFTFP;
jobject JNIEnv_NewLocalRef(JNIEnv *p0, jobject p1) SOFTFP;
jint JNIEnv_EnsureLocalCapacity(JNIEnv *p0, jint p1) SOFTFP;
jobject JNIEnv_AllocObject(JNIEnv *p0, jclass p1) SOFTFP;
jobject JNIEnv_NewObject(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
jobject JNIEnv_NewObjectV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
jobject JNIEnv_NewObjectA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
jclass JNIEnv_GetObjectClass(JNIEnv *p0, jobject p1) SOFTFP;
jboolean JNIEnv_IsInstanceOf(JNIEnv *p0, jobject p1, jclass p2) SOFTFP;
jmethodID JNIEnv_GetMethodID(JNIEnv *p0, jclass clazz, const char *name, const char *sig) SOFTFP;
jobject JNIEnv_CallObjectMethod(JNIEnv *p0, jobject p1, jmethodID p2, ...) SOFTFP;
jobject JNIEnv_CallObjectMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jobject JNIEnv_CallObjectMethodA(JNIEnv *p0, jobject p1, jmethodID p2, jvalue *p3) SOFTFP;
jboolean JNIEnv_CallBooleanMethod(JNIEnv *p0, jobject p1, jmethodID p2, ...) SOFTFP;
jboolean JNIEnv_CallBooleanMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jboolean JNIEnv_CallBooleanMethodA(JNIEnv *p0, jobject p1, jmethodID p2, jvalue *p3) SOFTFP;
jbyte JNIEnv_CallByteMethod(JNIEnv *p0, jobject p1, jmethodID p2, ...) SOFTFP;
jbyte JNIEnv_CallByteMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jbyte JNIEnv_CallByteMethodA(JNIEnv *p0, jobject p1, jmethodID p2, jvalue *p3) SOFTFP;
jchar JNIEnv_CallCharMethod(JNIEnv *p0, jobject p1, jmethodID p2, ...) SOFTFP;
jchar JNIEnv_CallCharMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jchar JNIEnv_CallCharMethodA(JNIEnv *p0, jobject p1, jmethodID p2, jvalue *p3) SOFTFP;
jshort JNIEnv_CallShortMethod(JNIEnv *p0, jobject p1, jmethodID p2, ...) SOFTFP;
jshort JNIEnv_CallShortMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jshort JNIEnv_CallShortMethodA(JNIEnv *p0, jobject p1, jmethodID p2, jvalue *p3) SOFTFP;
jint JNIEnv_CallIntMethod(JNIEnv *p0, jobject p1, jmethodID p2, ...) SOFTFP;
jint JNIEnv_CallIntMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jint JNIEnv_CallIntMethodA(JNIEnv *p0, jobject p1, jmethodID p2, jvalue *p3) SOFTFP;
jlong JNIEnv_CallLongMethod(JNIEnv *p0, jobject p1, jmethodID p2, ...) SOFTFP;
jlong JNIEnv_CallLongMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jlong JNIEnv_CallLongMethodA(JNIEnv *p0, jobject p1, jmethodID p2, jvalue *p3) SOFTFP;
jfloat JNIEnv_CallFloatMethod(JNIEnv *p0, jobject p1, jmethodID p2, ...) SOFTFP;
jfloat JNIEnv_CallFloatMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jfloat JNIEnv_CallFloatMethodA(JNIEnv *p0, jobject p1, jmethodID p2, jvalue *p3) SOFTFP;
jdouble JNIEnv_CallDoubleMethod(JNIEnv *p0, jobject p1, jmethodID p2, ...) SOFTFP;
jdouble JNIEnv_CallDoubleMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;
jdouble JNIEnv_CallDoubleMethodA(JNIEnv *p0, jobject p1, jmethodID p2, jvalue *p3) SOFTFP;
void JNIEnv_CallVoidMethod(JNIEnv *p0, jobject p1, jmethodID p2, ...) SOFTFP;
void JNIEnv_CallVoidMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;
void JNIEnv_CallVoidMethodA(JNIEnv *p0, jobject p1, jmethodID p2, jvalue *p3) SOFTFP;
jobject JNIEnv_CallNonvirtualObjectMethod(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, ...) SOFTFP;
jobject JNIEnv_CallNonvirtualObjectMethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, va_list p4) SOFTFP;
jobject JNIEnv_CallNonvirtualObjectMethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, jvalue *p4) SOFTFP;
jboolean JNIEnv_CallNonvirtualBooleanMethod(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, ...) SOFTFP;
jboolean JNIEnv_CallNonvirtualBooleanMethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, va_list p4) SOFTFP;
jboolean JNIEnv_CallNonvirtualBooleanMethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, jvalue *p4) SOFTFP;
jbyte JNIEnv_CallNonvirtualByteMethod(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, ...) SOFTFP;
jbyte JNIEnv_CallNonvirtualByteMethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, va_list p4) SOFTFP;
jbyte JNIEnv_CallNonvirtualByteMethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, jvalue *p4) SOFTFP;
jchar JNIEnv_CallNonvirtualCharMethod(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, ...) SOFTFP;
jchar JNIEnv_CallNonvirtualCharMethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, va_list p4) SOFTFP;
jchar JNIEnv_CallNonvirtualCharMethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, jvalue *p4) SOFTFP;
jshort JNIEnv_CallNonvirtualShortMethod(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, ...) SOFTFP;
jshort JNIEnv_CallNonvirtualShortMethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, va_list p4) SOFTFP;
jshort JNIEnv_CallNonvirtualShortMethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, jvalue *p4) SOFTFP;
jint JNIEnv_CallNonvirtualIntMethod(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, ...) SOFTFP;
jint JNIEnv_CallNonvirtualIntMethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, va_list p4) SOFTFP;
jint JNIEnv_CallNonvirtualIntMethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, jvalue *p4) SOFTFP;
jlong JNIEnv_CallNonvirtualLongMethod(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, ...) SOFTFP;
jlong JNIEnv_CallNonvirtualLongMethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, va_list p4) SOFTFP;
jlong JNIEnv_CallNonvirtualLongMethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, jvalue *p4) SOFTFP;
jfloat JNIEnv_CallNonvirtualFloatMethod(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, ...) SOFTFP;
jfloat JNIEnv_CallNonvirtualFloatMethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, va_list p4) SOFTFP;
jfloat JNIEnv_CallNonvirtualFloatMethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, jvalue *p4) SOFTFP;
jdouble JNIEnv_CallNonvirtualDoubleMethod(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, ...) SOFTFP;
jdouble JNIEnv_CallNonvirtualDoubleMethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, va_list p4) SOFTFP;
jdouble JNIEnv_CallNonvirtualDoubleMethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, jvalue *p4) SOFTFP;
void JNIEnv_CallNonvirtualVoidMethod(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, ...) SOFTFP;
void JNIEnv_CallNonvirtualVoidMethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, va_list p4) SOFTFP;
void JNIEnv_CallNonvirtualVoidMethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID p3, jvalue *p4) SOFTFP;
jfieldID JNIEnv_GetFieldID(JNIEnv *p0, jclass p1, const char *p2, const char *p3) SOFTFP;
jobject JNIEnv_GetObjectField(JNIEnv *p0, jobject p1, jfieldID p2) SOFTFP;
jboolean JNIEnv_GetBooleanField(JNIEnv *p0, jobject p1, jfieldID p2) SOFTFP;
jbyte JNIEnv_GetByteField(JNIEnv *p0, jobject p1, jfieldID p2) SOFTFP;
jchar JNIEnv_GetCharField(JNIEnv *p0, jobject p1, jfieldID p2) SOFTFP;
jshort JNIEnv_GetShortField(JNIEnv *p0, jobject p1, jfieldID p2) SOFTFP;
jint JNIEnv_GetIntField(JNIEnv *p0, jobject p1, jfieldID p2) SOFTFP;
jlong JNIEnv_GetLongField(JNIEnv *p0, jobject p1, jfieldID p2) SOFTFP;
jfloat JNIEnv_GetFloatField(JNIEnv *p0, jobject p1, jfieldID p2) SOFTFP;
jdouble JNIEnv_GetDoubleField(JNIEnv *p0, jobject p1, jfieldID p2) SOFTFP;
void JNIEnv_SetObjectField(JNIEnv *p0, jobject p1, jfieldID p2, jobject p3) SOFTFP;
void JNIEnv_SetBooleanField(JNIEnv *p0, jobject p1, jfieldID p2, jboolean p3) SOFTFP;
void JNIEnv_SetByteField(JNIEnv *p0, jobject p1, jfieldID p2, jbyte p3) SOFTFP;
void JNIEnv_SetCharField(JNIEnv *p0, jobject p1, jfieldID p2, jchar p3) SOFTFP;
void JNIEnv_SetShortField(JNIEnv *p0, jobject p1, jfieldID p2, jshort p3) SOFTFP;
void JNIEnv_SetIntField(JNIEnv *p0, jobject p1, jfieldID p2, jint p3) SOFTFP;
void JNIEnv_SetLongField(JNIEnv *p0, jobject p1, jfieldID p2, jlong p3) SOFTFP;
void JNIEnv_SetFloatField(JNIEnv *p0, jobject p1, jfieldID p2, jfloat p3) SOFTFP;
void JNIEnv_SetDoubleField(JNIEnv *p0, jobject p1, jfieldID p2, jdouble p3) SOFTFP;
jmethodID JNIEnv_GetStaticMethodID(JNIEnv *p0, jclass p1, const char *p2, const char *p3) SOFTFP;
jobject JNIEnv_CallStaticObjectMethod(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
jobject JNIEnv_CallStaticObjectMethodV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
jobject JNIEnv_CallStaticObjectMethodA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
jboolean JNIEnv_CallStaticBooleanMethod(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
jboolean JNIEnv_CallStaticBooleanMethodV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
jboolean JNIEnv_CallStaticBooleanMethodA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
jbyte JNIEnv_CallStaticByteMethod(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
jbyte JNIEnv_CallStaticByteMethodV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
jbyte JNIEnv_CallStaticByteMethodA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
jchar JNIEnv_CallStaticCharMethod(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
jchar JNIEnv_CallStaticCharMethodV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
jchar JNIEnv_CallStaticCharMethodA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
jshort JNIEnv_CallStaticShortMethod(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
jshort JNIEnv_CallStaticShortMethodV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
jshort JNIEnv_CallStaticShortMethodA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
jint JNIEnv_CallStaticIntMethod(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
jint JNIEnv_CallStaticIntMethodV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
jint JNIEnv_CallStaticIntMethodA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
jlong JNIEnv_CallStaticLongMethod(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
jlong JNIEnv_CallStaticLongMethodV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
jlong JNIEnv_CallStaticLongMethodA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
jfloat JNIEnv_CallStaticFloatMethod(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
jfloat JNIEnv_CallStaticFloatMethodV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
jfloat JNIEnv_CallStaticFloatMethodA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
jdouble JNIEnv_CallStaticDoubleMethod(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
jdouble JNIEnv_CallStaticDoubleMethodV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
jdouble JNIEnv_CallStaticDoubleMethodA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
void JNIEnv_CallStaticVoidMethod(JNIEnv *p0, jclass p1, jmethodID p2, ...) SOFTFP;
void JNIEnv_CallStaticVoidMethodV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3) SOFTFP;
void JNIEnv_CallStaticVoidMethodA(JNIEnv *p0, jclass p1, jmethodID p2, jvalue *p3) SOFTFP;
jfieldID JNIEnv_GetStaticFieldID(JNIEnv *p0, jclass p1, const char *p2, const char *p3) SOFTFP;
jobject JNIEnv_GetStaticObjectField(JNIEnv *p0, jclass p1, jfieldID p2) SOFTFP;
jboolean JNIEnv_GetStaticBooleanField(JNIEnv *p0, jclass p1, jfieldID p2) SOFTFP;
jbyte JNIEnv_GetStaticByteField(JNIEnv *p0, jclass p1, jfieldID p2) SOFTFP;
jchar JNIEnv_GetStaticCharField(JNIEnv *p0, jclass p1, jfieldID p2) SOFTFP;
jshort JNIEnv_GetStaticShortField(JNIEnv *p0, jclass p1, jfieldID p2) SOFTFP;
jint JNIEnv_GetStaticIntField(JNIEnv *p0, jclass p1, jfieldID p2) SOFTFP;
jlong JNIEnv_GetStaticLongField(JNIEnv *p0, jclass p1, jfieldID p2) SOFTFP;
jfloat JNIEnv_GetStaticFloatField(JNIEnv *p0, jclass p1, jfieldID p2) SOFTFP;
jdouble JNIEnv_GetStaticDoubleField(JNIEnv *p0, jclass p1, jfieldID p2) SOFTFP;
void JNIEnv_SetStaticObjectField(JNIEnv *p0, jclass p1, jfieldID p2, jobject p3) SOFTFP;
void JNIEnv_SetStaticBooleanField(JNIEnv *p0, jclass p1, jfieldID p2, jboolean p3) SOFTFP;
void JNIEnv_SetStaticByteField(JNIEnv *p0, jclass p1, jfieldID p2, jbyte p3) SOFTFP;
void JNIEnv_SetStaticCharField(JNIEnv *p0, jclass p1, jfieldID p2, jchar p3) SOFTFP;
void JNIEnv_SetStaticShortField(JNIEnv *p0, jclass p1, jfieldID p2, jshort p3) SOFTFP;
void JNIEnv_SetStaticIntField(JNIEnv *p0, jclass p1, jfieldID p2, jint p3) SOFTFP;
void JNIEnv_SetStaticLongField(JNIEnv *p0, jclass p1, jfieldID p2, jlong p3) SOFTFP;
void JNIEnv_SetStaticFloatField(JNIEnv *p0, jclass p1, jfieldID p2, jfloat p3) SOFTFP;
void JNIEnv_SetStaticDoubleField(JNIEnv *p0, jclass p1, jfieldID p2, jdouble p3) SOFTFP;
jstring JNIEnv_NewString(JNIEnv *p0, const jchar *p1, jsize p2) SOFTFP;
jsize JNIEnv_GetStringLength(JNIEnv *p0, jstring p1) SOFTFP;
const jchar *JNIEnv_GetStringChars(JNIEnv *p0, jstring p1, jboolean *p2) SOFTFP;
void JNIEnv_ReleaseStringChars(JNIEnv *p0, jstring p1, const jchar *p2) SOFTFP;
jstring JNIEnv_NewStringUTF(JNIEnv *p0, const char *p1) SOFTFP;
jsize JNIEnv_GetStringUTFLength(JNIEnv *p0, jstring p1) SOFTFP;
jsize JNIEnv_GetArrayLength(JNIEnv *p0, jarray p1) SOFTFP;
jobjectArray JNIEnv_NewObjectArray(JNIEnv *p0, jsize p1, jclass p2, jobject p3) SOFTFP;
jobject JNIEnv_GetObjectArrayElement(JNIEnv *p0, jobjectArray p1, jsize p2) SOFTFP;
void JNIEnv_SetObjectArrayElement(JNIEnv *p0, jobjectArray p1, jsize p2, jobject p3) SOFTFP;
jbooleanArray JNIEnv_NewBooleanArray(JNIEnv *p0, jsize p1) SOFTFP;
jbyteArray JNIEnv_NewByteArray(JNIEnv *p0, jsize p1) SOFTFP;
jcharArray JNIEnv_NewCharArray(JNIEnv *p0, jsize p1) SOFTFP;
jshortArray JNIEnv_NewShortArray(JNIEnv *p0, jsize p1) SOFTFP;
jintArray JNIEnv_NewIntArray(JNIEnv *p0, jsize p1) SOFTFP;
jlongArray JNIEnv_NewLongArray(JNIEnv *p0, jsize p1) SOFTFP;
jfloatArray JNIEnv_NewFloatArray(JNIEnv *p0, jsize p1) SOFTFP;
jdoubleArray JNIEnv_NewDoubleArray(JNIEnv *p0, jsize p1) SOFTFP;
jboolean *JNIEnv_GetBooleanArrayElements(JNIEnv *p0, jbooleanArray p1, jboolean *p2) SOFTFP;
jbyte *JNIEnv_GetByteArrayElements(JNIEnv *p0, jbyteArray p1, jboolean *p2) SOFTFP;
jchar *JNIEnv_GetCharArrayElements(JNIEnv *p0, jcharArray p1, jboolean *p2) SOFTFP;
jshort *JNIEnv_GetShortArrayElements(JNIEnv *p0, jshortArray p1, jboolean *p2) SOFTFP;
jint *JNIEnv_GetIntArrayElements(JNIEnv *p0, jintArray p1, jboolean *p2) SOFTFP;
jlong *JNIEnv_GetLongArrayElements(JNIEnv *p0, jlongArray p1, jboolean *p2) SOFTFP;
jfloat *JNIEnv_GetFloatArrayElements(JNIEnv *p0, jfloatArray p1, jboolean *p2) SOFTFP;
jdouble *JNIEnv_GetDoubleArrayElements(JNIEnv *p0, jdoubleArray p1, jboolean *p2) SOFTFP;
void JNIEnv_ReleaseBooleanArrayElements(JNIEnv *p0, jbooleanArray p1, jboolean *p2, jint p3) SOFTFP;
void JNIEnv_ReleaseByteArrayElements(JNIEnv *p0, jbyteArray p1, jbyte *p2, jint p3) SOFTFP;
void JNIEnv_ReleaseCharArrayElements(JNIEnv *p0, jcharArray p1, jchar *p2, jint p3) SOFTFP;
void JNIEnv_ReleaseShortArrayElements(JNIEnv *p0, jshortArray p1, jshort *p2, jint p3) SOFTFP;
void JNIEnv_ReleaseIntArrayElements(JNIEnv *p0, jintArray p1, jint *p2, jint p3) SOFTFP;
void JNIEnv_ReleaseLongArrayElements(JNIEnv *p0, jlongArray p1, jlong *p2, jint p3) SOFTFP;
void JNIEnv_ReleaseFloatArrayElements(JNIEnv *p0, jfloatArray p1, jfloat *p2, jint p3) SOFTFP;
void JNIEnv_ReleaseDoubleArrayElements(JNIEnv *p0, jdoubleArray p1, jdouble *p2, jint p3) SOFTFP;
void JNIEnv_GetBooleanArrayRegion(JNIEnv *p0, jbooleanArray p1, jsize p2, jsize p3, jboolean *p4) SOFTFP;
void JNIEnv_GetByteArrayRegion(JNIEnv *p0, jbyteArray arrayobj, jsize start, jsize len, jbyte *buf) SOFTFP;
void JNIEnv_GetCharArrayRegion(JNIEnv *p0, jcharArray p1, jsize p2, jsize p3, jchar *p4) SOFTFP;
void JNIEnv_GetShortArrayRegion(JNIEnv *p0, jshortArray p1, jsize p2, jsize p3, jshort *p4) SOFTFP;
void JNIEnv_GetIntArrayRegion(JNIEnv *p0, jintArray p1, jsize p2, jsize p3, jint *p4) SOFTFP;
void JNIEnv_GetLongArrayRegion(JNIEnv *p0, jlongArray p1, jsize p2, jsize p3, jlong *p4) SOFTFP;
void JNIEnv_GetFloatArrayRegion(JNIEnv *p0, jfloatArray p1, jsize p2, jsize p3, jfloat *p4) SOFTFP;
void JNIEnv_GetDoubleArrayRegion(JNIEnv *p0, jdoubleArray p1, jsize p2, jsize p3, jdouble *p4) SOFTFP;
void JNIEnv_SetBooleanArrayRegion(JNIEnv *p0, jbooleanArray p1, jsize p2, jsize p3, const jboolean *p4) SOFTFP;
void JNIEnv_SetByteArrayRegion(JNIEnv *p0, jbyteArray p1, jsize p2, jsize p3, const jbyte *p4) SOFTFP;
void JNIEnv_SetCharArrayRegion(JNIEnv *p0, jcharArray p1, jsize p2, jsize p3, const jchar *p4) SOFTFP;
void JNIEnv_SetShortArrayRegion(JNIEnv *p0, jshortArray p1, jsize p2, jsize p3, const jshort *p4) SOFTFP;
void JNIEnv_SetIntArrayRegion(JNIEnv *p0, jintArray p1, jsize p2, jsize p3, const jint *p4) SOFTFP;
void JNIEnv_SetLongArrayRegion(JNIEnv *p0, jlongArray p1, jsize p2, jsize p3, const jlong *p4) SOFTFP;
void JNIEnv_SetFloatArrayRegion(JNIEnv *p0, jfloatArray p1, jsize p2, jsize p3, const jfloat *p4) SOFTFP;
void JNIEnv_SetDoubleArrayRegion(JNIEnv *p0, jdoubleArray p1, jsize p2, jsize p3, const jdouble *p4) SOFTFP;
jint JNIEnv_RegisterNatives(JNIEnv *p0, jclass p1, const JNINativeMethod *p2, jint p3) SOFTFP;
jint JNIEnv_UnregisterNatives(JNIEnv *p0, jclass p1) SOFTFP;
jint JNIEnv_MonitorEnter(JNIEnv *p0, jobject p1) SOFTFP;
jint JNIEnv_MonitorExit(JNIEnv *p0, jobject p1) SOFTFP;
jint JNIEnv_GetJavaVM(JNIEnv *p0, JavaVM **p1) SOFTFP;
void JNIEnv_GetStringRegion(JNIEnv *p0, jstring p1, jsize p2, jsize p3, jchar *p4) SOFTFP;
void JNIEnv_GetStringUTFRegion(JNIEnv *p0, jstring p1, jsize p2, jsize p3, char *p4) SOFTFP;
void *JNIEnv_GetPrimitiveArrayCritical(JNIEnv *p0, jarray p1, jboolean *p2) SOFTFP;
void JNIEnv_ReleasePrimitiveArrayCritical(JNIEnv *p0, jarray p1, void *p2, jint p3) SOFTFP;
const jchar *JNIEnv_GetStringCritical(JNIEnv *p0, jstring p1, jboolean *p2) SOFTFP;
void JNIEnv_ReleaseStringCritical(JNIEnv *p0, jstring p1, const jchar *p2) SOFTFP;
jweak JNIEnv_NewWeakGlobalRef(JNIEnv *p0, jobject p1) SOFTFP;
void JNIEnv_DeleteWeakGlobalRef(JNIEnv *p0, jweak p1) SOFTFP;
jboolean JNIEnv_ExceptionCheck(JNIEnv *p0) SOFTFP;
jobject JNIEnv_NewDirectByteBuffer(JNIEnv *p0, void *p1, jlong p2) SOFTFP;
void *JNIEnv_GetDirectBufferAddress(JNIEnv *p0, jobject p1) SOFTFP;
jlong JNIEnv_GetDirectBufferCapacity(JNIEnv *p0, jobject p1) SOFTFP;
const char *JNIEnv_GetStringUTFChars(JNIEnv *env, jstring string, jboolean *isCopy) SOFTFP;
void JNIEnv_ReleaseStringUTFChars(JNIEnv *env, jstring string, const char *utf) SOFTFP;

void jnienv_init(struct GlobalState *global) SOFTFP;

jint JavaVM_DestroyJavaVM(JavaVM *vm) SOFTFP;
jint JavaVM_AttachCurrentThread(JavaVM *vm, JNIEnv **env, void *args) SOFTFP;
jint JavaVM_DetachCurrentThread(JavaVM *vm) SOFTFP;
jint JavaVM_GetEnv(JavaVM *vm, void **env, jint version) SOFTFP;
jint JavaVM_AttachCurrentThreadAsDaemon(JavaVM *vm, JNIEnv **env, void *args) SOFTFP;

void javavm_init(struct GlobalState *global) SOFTFP;

#endif /* JNIENV_H */
