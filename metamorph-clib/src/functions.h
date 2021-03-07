#ifndef FUNCTIONS_GLOB
#define FUNCTIONS_GLOB

#include "dyntypes.h"
#include "cache.h"
#include "activations.h"
#include <setjmp.h>

extern activation_t* current_activation;
extern activation_t* root_activation;
extern dyntype_t return_value;
extern dyntype_t bound_value;
extern jmp_buf error_buffer;
extern exception_t global_exception;

extern int return_address;
void prog();
void initprog(int);
void prereturn(dyntype_t );
void prereturn_literal(dyntype_t );
void cleanup();
void applicate(int n_params, dyntype_t params[], dyntype_t proc, int id);
void applicate_literal(int n_params, dyntype_t params[], dyntype_t proc, int id);
void error(int);
void body(int);
int close_body();
void finalize_call();


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
#define ARGUMENT(number)    (current_activation->formal_parameters[number])
#define RETURN(VALUE)   {\
                            prereturn(VALUE);\
                            goto table;\
                        }

#define RETURN_LITERAL(VALUE) {\
                            prereturn_literal(VALUE);\
                            goto table;\
                        }

#define CALL(PARAMETERS) applicate(PARAMETERS, (dyntype_t[PARAMETERS]){
#define CALL_LITERAL(PARAMETERS) applicate_literal(PARAMETERS, (dyntype_t[PARAMETERS]){

#define PARAMETER(VALUE) copy_dyntype(VALUE),

#define PARAMETER_LITERAL(VALUE)   VALUE,

#define APPLICATE(PROC, ID)  }, PROC, ID);\
                    goto table;\
                    case(ID):;

#define TAIL_APPLICATE(PROC)  }, PROC, -1);\
                    goto table;


#define PUSH(dyntype)           stack_push(current_activation, dyntype);
#define PUSH_LITERAL(dyntype)   stack_push_literal(current_activation, dyntype);
#define POP                     stack_pop(current_activation)

#define BODY(NUMBER_OF_DEFINES)   body(NUMBER_OF_DEFINES);
#define BODY_CLOSE                close_body();

//ACHTUNG JUMP allokiert keine Parentactivation kann zu SEGFAULT führen
/*
#define JUMP(FUNCTION_ID,ID)   prejump(FUNCTION_ID, ID);\
                    goto table;\
                    case(ID):\
                    postjump();}
*/
                
#define CRASH(CODE) longjmp(error_buffer,CODE);

#endif