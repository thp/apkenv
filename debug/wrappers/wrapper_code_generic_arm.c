#include "wrapper_code.h"

void
wrapper_code_generic()
{
    // we can never use r0-r11, neither the stack
    asm volatile(
        // preserve the registers
        "push {r0-r11, lr}\n"
        
        "ldr r0, fun\n" // load the function pointer to r0
        "ldr r1, name\n" // load the address of the functions name to r1
        "ldr r2, str\n" // load the string to print
        "ldr r4, tc\n" // load the address of trace_callback to r4        
        "blx r4\n" // call trace_callback

        // restore the registers
        "pop {r0-r11, lr}\n"
        "ldr pc, fun\n"     // jump to function
        
        // the following is only needed because of the labels
        // we could rewrite this and omit the following but it's not
        // necessary and this is easier

        // dummy instructions, this is where we locate our pointers
        "name: .word 0xFFFFFFFF\n" // name of function to call
        "fun: .word 0xFFFFFFFF\n" // function to call
        "tc: .word 0xFFFFFFFF\n" // address of trace_callback
        "str: .word 0xFFFFFFFF\n" // the string being printed in trace_callback
    );
}
