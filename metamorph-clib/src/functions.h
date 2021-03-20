#ifndef FUNCTIONS_GLOB
#define FUNCTIONS_GLOB

#include "dyntypes.h"
#include "cache.h"
#include "activations.h"
#include <setjmp.h>

extern activation_t* current_activation;
extern dyntype_t bound_value;
extern jmp_buf error_buffer;
extern exception_t global_exception;

extern target_id (*current_function)(int);
target_id prog(int);
void init(int, activation_t**);
void init_library(int globals, activation_t** root_activation);
void control_loop();
target_id prereturn();
void cleanup();
target_id applicate(int n_params, int id);
void error(int);
void body(int);
int close_body();
void finalize_call();
int popif();

extern int balance;

#define POP_EMPTY_STACK 7
#define INVALID_NUMBER_ARGUMENTS 8
#define METAMORPH_C_SYNTAX_VIOLATION 9
#define APPLICATION_OF_NON_PROCEDURE_TYPE 10


/*
void prejump(int, int);
void postjump();
*/

#define START(GLOBALS) static activation_t*  global_0_activation = NULL;\
                int main(){int code;\
                if(!(code=setjmp(error_buffer))){init(GLOBALS, &global_0_activation); control_loop(); cleanup();}\
                error(code);\
                }\
                SIGNATUR(prog){\
                    switch(marker){\
                    case(0):
#define EXIT    return (target_id){0,NULL};}}
#define SIGNATUR(FUNCTION) target_id FUNCTION(int marker)
#define EXPORT(NAME) SIGNATUR(NAME){\
                        switch(marker){\
                        case(0):
#define FUNCTION(NAME) static SIGNATUR(NAME){\
                        switch(marker){\
                        case(0):
#define RETURN                  return prereturn(); 
#define APPLICATE(PARAMS, ID)   return applicate(PARAMS, ID); case(ID):;
#define TAIL_APPLICATE(PARAMS)  return applicate(PARAMS, -1);

#define INCLUDE_LIBRARY(NAME)   extern activation_t* global_##NAME##_activation; SIGNATUR(NAME);
#define LIBRARY(NAME, GLOBALS)   activation_t* global_##NAME##_activation = NULL;\
                        EXPORT(NAME)\
                        if ( global_##NAME##_activation != NULL){PUSH_UNSPECIFIED;return prereturn();};\
                        init_library(GLOBALS, &global_##NAME##_activation);

#define PUSH(DYNTYPE)           stack_push(current_activation, DYNTYPE);
#define PUSH_UNSPECIFIED        stack_push_literal(current_activation, (dyntype_t){SCHEME_TYPE_UNSPECIFIED,0,0});
#define PUSH_LITERAL(DYNTYPE)   stack_push_literal(current_activation, DYNTYPE);
#define POP                     stack_pop(current_activation)
#define POP_LITERAL             stack_pop_literal(current_activation)
#define BODY(NUMBER_OF_DEFINES)   body(NUMBER_OF_DEFINES);
#define BODY_CLOSE                close_body();
#define END                       CRASH(METAMORPH_C_SYNTAX_VIOLATION)}}     
#define CRASH(CODE) longjmp(error_buffer,CODE);

#define BASE_FUNCTION(NAME)     dyntype_t base_##NAME(
#define PARAMETER(NAME)             dyntype_t NAME,
#define ELLIPSIS                    dyntype_t* ellipsis, int n_ellipsis)

#define MAKE_ELLIPSIS(N)            current_activation->formal_parameters+N, current_activation->number_parameters-N

#endif