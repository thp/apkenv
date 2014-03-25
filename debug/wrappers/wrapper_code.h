#ifndef WRAPPER_CODE_H
#define WRAPPER_CODE_H

void wrapper_code_generic() __attribute__((naked,noinline));
void wrapper_code_arm() __attribute__((naked,noinline));
void wrapper_code_thumb() __attribute__((naked,noinline));

#endif /* WRAPPER_CODE_H */
