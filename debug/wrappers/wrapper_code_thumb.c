#include "wrapper_code.h"
    
void
wrapper_code_thumb()
{
    // the thumb wrapper does not work yet
    asm volatile(
        // preserve the registers
        "push {r0-r11, lr}\n"
        
        // now that everything is secure we can call a function
        "ldrd r0, fun\n" // load the function pointer to r0
        "sub r0, r0, #8\n" // correct the pointer (TODO: this is slightly of for different cases, but it is only a printed value, not actually used)
        "ldrd r1, name\n" // load the address of the functions name to r1
        "ldrd r2, str\n" // load the string to print
        "ldrd r4, tc\n" // load the address of print_fun_name to r4
        "blx r4\n" // call print_info

        // the stack should be restored once print_info returned

        // restore the registers
        "pop {r0-r11, lr}\n"
        // reserved space for thumb instructions
        "mov r0, r0\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        // load the address to which we should jump
        "push {r0}\n"
        "ldr r0, fun\n"
        "mov ip, r0\n"
        "pop {r0}\n"
        "bx ip\n" // jump to the first uncopied instruction of the wrapped function

        // the following is only needed because of the labels
        // we could rewrite this and omit the following but it's not
        // necessary and this is easier

        // dummy instructions, this is where we locate our pointers
        "name: .word 0xFFFFFFFF\n" // name of function to call
        "fun: .word 0xFFFFFFFF\n" // function to call (actually the pointer to the first uncopied instruction)
        "tc: .word 0xFFFFFFFF\n" // address of print_fun_name
        "str: .word 0xFFFFFFFF\n" // the string being printed in trace_callback
    );
}
