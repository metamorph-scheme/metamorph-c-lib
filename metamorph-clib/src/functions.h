#ifndef FUNCTIONS_GLOB
#define FUNCTIONS_GLOB

#include "dyntypes.h"
#include "cache.h"
#include "activations.h"
#include <setjmp.h>

extern activation_t* current_activation;
extern activation_t* root_activation;
extern dyntype_t bound_value;
extern jmp_buf error_buffer;
extern exception_t global_exception;

extern int return_address;
void prog();
void initprog(int);
void prereturn();
void cleanup();
void applicate(int n_params, int id);
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

#define START(GLOBALS)  int main(){int code;\
                if(!(code=setjmp(error_buffer))){prog(); cleanup();}\
                error(code);\
                }\
                void prog(){\
                initprog(GLOBALS);\
                table:\
                    switch(return_address){\
                    case(0):{
#define END     }}}
#define EXIT    return;

#define FUNCTION(ID)  CRASH(METAMORPH_C_SYNTAX_VIOLATION)\
                    }case(ID):{
#define RETURN                  prereturn(); goto table;
#define APPLICATE(PARAMS, ID)   applicate(PARAMS, ID); goto table; case(ID):;
#define TAIL_APPLICATE(PARAMS)  applicate(PARAMS, -1); if(return_address!=-1){goto table;};

#define PARAMETER(NAME)         dyntype_t NAME = POP_LITERAL; n_ellipsis--;
#define ELLIPSIS                dyntype_t* ellipsis=stack_ellipsis(current_activation, n_ellipsis);
#define DESTROY_PARAM(NAME)     release_dyntype(NAME);
#define DESTROY_ELLIPSIS        release_dyntypes(ellipsis, n_ellipsis); 
#define PUSH(DYNTYPE)           stack_push(current_activation, DYNTYPE);
#define PUSH_UNSPECIFIED        stack_push_literal(current_activation, (dyntype_t){SCHEME_TYPE_UNSPECIFIED,0,0});
#define PUSH_LITERAL(DYNTYPE)   stack_push_literal(current_activation, DYNTYPE);
#define POP                     stack_pop(current_activation)
#define POP_LITERAL             stack_pop_literal(current_activation)
#define BODY(NUMBER_OF_DEFINES)   body(NUMBER_OF_DEFINES);
#define BODY_CLOSE                close_body();
#define BASE_FUNCTION(FUNCTION)   void FUNCTION(int n_ellipsis)
                
#define CRASH(CODE) longjmp(error_buffer,CODE);

#endif