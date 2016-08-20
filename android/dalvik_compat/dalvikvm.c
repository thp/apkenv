/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "jni.h"

#include <stdlib.h>
#include <stdio.h>

int compat_CreateJavaVM(JavaVM **vm, JNIEnv **env, char *apk_filename)
{
    struct JavaVMInitArgs vm_args;
    struct JavaVMOption *options;
    char option_string[1024];
    
    options = (struct JavaVMOption*)malloc(sizeof(struct JavaVMOption));
    strcpy(option_string, "-Djava.class.path=");
    strcat(option_string, apk_filename);

    options->optionString = option_string;
    
    vm_args.version = JNI_VERSION_1_4;
    vm_args.options = options;
    vm_args.nOptions = 1;
    vm_args.ignoreUnrecognized = JNI_FALSE; 
    if(JNI_CreateJavaVM(vm, env, &vm_args) < 0) {
        printf("ERROR: JNI_CreateJavaVM failed.\n");
        return 0; // failed
    }
    return 1; // success
}

