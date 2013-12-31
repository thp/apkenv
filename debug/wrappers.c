
/**
 * Copyright (c) 2013, Franz-Josef Haider <f_haider@gmx.at>
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

#include "wrappers.h"
#include "../compat/hooks.h"
#include "../apkenv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <stdint.h>
#include <assert.h>

struct wrapper {
    char *name;
    void *ptr;
    unsigned int size;
    int type;
    struct wrapper *next;
};

struct wrapper *wrappers = NULL;

int wrapper_cmp(void *a, void *b) {
    struct wrapper *key = (struct wrapper*)a;
    struct wrapper *elem = (struct wrapper*)b;
    int c = strcmp(key->name, elem->name);
    if(0 == c) {
        return (key->type - elem->type);
    }
    return c;
}

void register_wrapper(void *wrapper_ptr, unsigned int size, char *name, int type)
{
    struct wrapper *it = wrappers;
    struct wrapper *last = NULL;
    while(NULL != it)
    {
        last = it;
        it = it->next;
    }
    struct wrapper *w = (struct wrapper*)malloc(sizeof(struct wrapper));
    w->name = name;
    w->ptr = wrapper_ptr;
    w->type = type;
    w->size = size;
    w->next = NULL;
    if(NULL == last)
    {
        wrappers = w;
    }
    else
    {
        last->next = w;
    }
}

extern struct GlobalState global;

int is_traceable(char *symbol)
{
    if(global.trace_all) return 1;

    struct trace_list *it = global.functions_to_trace;
    while(it != NULL)
    {
        if(0 == strcmp(it->name, symbol)) return 1;
        it = it->next;
    }
    return 0;
}

void trace_callback(void *function, char *name, char *msg)
{
    printf("%s: %s@%p\n", msg, name, function);
}

const char *msg_latehook = "calling late hooked method";
const char *msg_unhooked = "calling unhooked method";
const char *msg_dynhook = "calling dynamically loaded method";
const char *msg_arm_injection = "called injected ARM wrapper";
const char *msg_thumb_injection = "called injected THUMB wrapper";

// the following definition is taken from linux-2.6/arch/arm/mm/alignment.c
/* Thumb-2 32 bit format per ARMv7 DDI0406A A6.3, either f800h,e800h,f800h */
#define IS_T32(hi16) \
            (((hi16) & 0xe000) == 0xe000 && ((hi16) & 0x1800))

void *create_wrapper(char *symbol, void *function, int wrapper_type)
{   
    int helper = 0;
    int wrapper_size = 0;
    void *wrapper_addr = NULL;

    const char *msg = NULL;
    
    switch(wrapper_type)
    {
        case WRAPPER_LATEHOOK:
            if(!global.trace_latehooked) return function;
            break;
        case WRAPPER_UNHOOKED:
            if(!global.trace_unhooked) return function;
            break;
        case WRAPPER_DYNHOOK:
            if(!global.trace_dynhooked) return function;
            break;
        case WRAPPER_ARM_INJECTION:
            if(!global.trace_arm_injection) return function;
            break;
        case WRAPPER_THUMB_INJECTION:
            if(!global.trace_thumb_injection) return function;
            break;
        default:
            assert(NULL == "ERROR: invalid wrapper type!\n");
    };
    
    if(!is_traceable(symbol))
        return function;
    
    // get wrapper size and setup msg
    switch(wrapper_type)
    {
        case WRAPPER_LATEHOOK:
            wrapper_size = 
#include "wrappers/wrapper_GENERIC.size"
                * sizeof(uint32_t);
            msg = msg_latehook;
            break;
        case WRAPPER_UNHOOKED:
            wrapper_size = 
#include "wrappers/wrapper_GENERIC.size"
                * sizeof(uint32_t);
            msg = msg_unhooked;
            break;
        case WRAPPER_DYNHOOK:
            wrapper_size = 
#include "wrappers/wrapper_GENERIC.size"
                * sizeof(uint32_t);
            msg = msg_dynhook;
            break;
        case WRAPPER_ARM_INJECTION:
            wrapper_size = 
#include "wrappers/wrapper_ARM.size"
                * sizeof(uint32_t); // 32-Bit
            msg = msg_arm_injection;
            break;
        case WRAPPER_THUMB_INJECTION:
            wrapper_size = 
#include "wrappers/wrapper_THUMB.size"
                * sizeof(uint16_t); // 16-Bit
            msg = msg_thumb_injection;
            break;
        default:
            assert(NULL == "ERROR: invalid wrapper type!\n");
    }
    
    // 4 additional longs for data storage, see below
    wrapper_size += 4 * sizeof(uint32_t);
    
    // reserve memory for the generated wrapper
    wrapper_addr = mmap(NULL, wrapper_size,
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_ANONYMOUS | MAP_PRIVATE,
        0, 0);
    
    if(MAP_FAILED == wrapper_addr)
    {
        printf("ERROR: failed to create wrapper for %s@%x (mmap failed).\n", symbol, function);
        return function;
    }
    
    // this variable is used to determine how many operations we need to copy from the thumb code
    int thumb_fifth_nop = 0;
    
    switch(wrapper_type)
    {
        case WRAPPER_LATEHOOK:
        case WRAPPER_UNHOOKED:
        case WRAPPER_DYNHOOK:
#include "wrappers/wrapper_GENERIC.instructions"
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)symbol;
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)function;
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)trace_callback;
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)msg;
            break;
        case WRAPPER_ARM_INJECTION:
#include "wrappers/wrapper_ARM.instructions"
            // relocate the first 2 instructions
            // this is EXPERIMENTAL! it works in many cases because of how
            // ARM methods are layout in ASM.
            // adjust these indexes if you change the wrapper code
            ((int32_t*)wrapper_addr)[helper-3] = *((int32_t*)function);
            ((int32_t*)wrapper_addr)[helper-2] = *((int32_t*)function + 1);
            
            // patch code
            ((int32_t*)function)[0] = 0xe51ff004; // ldr pc, [pc, -#4] (load wrapper_addr into pc)
            ((int32_t*)function)[1] = (uint32_t)wrapper_addr;
            __clear_cache((int32_t*)function, (int32_t*)function + 2);
            
            // store name of the wrapped function
            // store the pointer to the 3rd instruction of the wrapped function
            // store the pointer to trace_callback, so that the wrapper can call it
            // store the msg to be displayed by trace_callback
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)symbol;
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)((int32_t*)function + 2);
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)trace_callback;
            ((int32_t*)wrapper_addr)[helper++] = (uint32_t)msg;
            break;
        case WRAPPER_THUMB_INJECTION:
            printf("WARNING: injecting THUMB code is very experimental. expect crashes!\n");
            // get the actual address from the THUMB entry point
            function = (void*)((char*)function - 1);
            // fix wrapper addr (tell the processor this is THUMB code)
            wrapper_addr = (void*)((char*)wrapper_addr + 1);
#include "wrappers/wrapper_THUMB.instructions"
            thumb_fifth_nop = IS_T32(*((int16_t*)function + 4)) && (
                        (!IS_T32(*((int16_t*)function)) && !IS_T32(*((int16_t*)function + 1)) && !IS_T32(*((int16_t*)function + 3))) ||
                        (!IS_T32(*((int16_t*)function)) &&  IS_T32(*((int16_t*)function + 1))                                      ) ||
                        ( IS_T32(*((int16_t*)function))                                       && !IS_T32(*((int16_t*)function + 3)))
                                             );
            // relocate code. This is very EXPERIMENTAL and will FAIL in most
            // cases due to the layout of THUMB functions in ASM.
            // adjust these indexes if you change the wrapper code
            if(!thumb_fifth_nop)
            {
                ((int16_t*)wrapper_addr)[helper-6] = ((uint16_t*)function)[0];
                ((int16_t*)wrapper_addr)[helper-5] = ((uint16_t*)function)[1];
                ((int16_t*)wrapper_addr)[helper-4] = ((uint16_t*)function)[2];
                ((int16_t*)wrapper_addr)[helper-3] = ((uint16_t*)function)[3];
            }
            else
            {
                ((int16_t*)wrapper_addr)[helper-6] = ((uint16_t*)function)[0];
                ((int16_t*)wrapper_addr)[helper-5] = ((uint16_t*)function)[1];
                ((int16_t*)wrapper_addr)[helper-4] = ((uint16_t*)function)[2];
                ((int16_t*)wrapper_addr)[helper-3] = ((uint16_t*)function)[3];
                ((int16_t*)wrapper_addr)[helper-2] = ((uint16_t*)function)[4];
            }
            
            // patch code
            ((int16_t*)function)[0] = 0xf8df; // ldr pc, [pc] (load wrapper_addr into pc)
            ((int16_t*)function)[1] = 0xf000;
            // store wrapper address so the modified code
            // will jump into it via the instruction above
            ((int16_t*)function)[2] = (uint32_t)wrapper_addr & 0x0000FFFF;
            ((int16_t*)function)[3] = (uint32_t)wrapper_addr >> 16;
            __clear_cache((int16_t*)function, (int16_t*)function + 4);
            
            // store name of the wrapped function
            // store the pointer to the 3rd instruction of the wrapped function
            // store the pointer to trace_callback, so that the wrapper can call it
            // store a pointer to the message which trace_callback should display
            ((int16_t*)wrapper_addr)[helper++] = ((uint32_t)symbol) & 0x0000FFFF;
            ((int16_t*)wrapper_addr)[helper++] = ((uint32_t)symbol) >> 16;
            ((int16_t*)wrapper_addr)[helper++] = ((uint32_t)((int32_t*)function + 2)) & 0x0000FFFF;
            ((int16_t*)wrapper_addr)[helper++] = ((uint32_t)((int32_t*)function + 2)) >> 16;
            ((int16_t*)wrapper_addr)[helper++] = ((uint32_t)trace_callback) & 0x0000FFFF;
            ((int16_t*)wrapper_addr)[helper++] = ((uint32_t)trace_callback) >> 16;
            ((int16_t*)wrapper_addr)[helper++] = ((uint32_t)msg) & 0x0000FFFF;
            ((int16_t*)wrapper_addr)[helper++] = ((uint32_t)msg) >> 16;
            break;
        default:
            break;
    };
    
    register_wrapper(wrapper_addr, wrapper_size, symbol, wrapper_type);
    
    return wrapper_addr;
}

void release_all_wrappers()
{
    struct wrapper *it = wrappers;
    while(NULL != it)
    {
        struct wrapper *next = it->next;
        munmap(it->ptr,it->size);
        free(it);
        it = next;
    }
}

