
#ifdef DEBUG_TRACE_INJ_THUMB
static void stub_f() __attribute__((naked,noinline));
static void stub_f()
{
    // the thumb wrapper does not work yet
    asm volatile(
        // don't use any labels within the code because our *.o to code converter will stop at the first label
        // preserve the registers
        "push {r0-r11, lr}\n"
        
        // now that everything is secure we can call a function
        "ldrd r0, fun\n" // load the function pointer to r0
        "sub r0, r0, #2\n" // correct the pointer (TODO: check if this is correct)
        "ldrd r1, name\n" // load the address of the functions name to r1
        "mov r2, #1\n" // is_thumb = 1
        "ldrd r4, pfn\n" // load the address of print_fun_name to r4
        "blx r4\n" // call print_info

        // the stack should be restored once print_info returned

        // restore the registers
        "pop {r0-r11, lr}\n"
        "mov pc, lr\n" // test
        "mov r0, r0\n"

        // reserved space for 2 up to 5 thumb instructions (16-bit vs 32-Bit)
        "mov r0, r0\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        "ldr pc, fun\n" // jump to the 3rd instruction of the wrapped function

        // the following is only needed because of the labels
        // we could rewrite this and omit the following but it's not
        // necessary and this is easier

        // dummy instructions, this is where we locate our pointers
        "name: mov r0, r0\n" // name of function to call
        "mov r0, r0\n"
        "fun: mov r0, r0\n" // function to call (actually the pointer to the third instruction)
        "mov r0, r0\n"
        "pfn: mov r0, r0\n" // address of print_fun_name
        "mov r0, r0\n"
        "wrp: mov r0, r0\n" // wrapper base address
        "mov r0, r0\n"
    );
}

#endif

