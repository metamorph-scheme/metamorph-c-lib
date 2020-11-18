#ifndef FUNCTIONS_GLOB
#define FUNCTIONS_GLOB

#include "dyntypes.h"
#include "cache.h"
#include <setjmp.h>

typedef struct activation_struct_t{
    int return_address;
    int references;
    int number_parameters;
    struct activation_struct_t* previous_activation;
    struct activation_struct_t* parent_activation;
    dyntype_t* formal_parameters;
} activation_t;

extern activation_t* current_activation;
extern activation_t* temporary_activation;
extern activation_t* root_activation;
extern dyntype_t return_value;
extern dyntype_t bound_value;
extern jmp_buf error_buffer;
extern exception_t global_exception;

extern int return_address;
void prog();
void initprog(int);
void prereturn(dyntype_t );
void bind(int, dyntype_t);
void bind_literal(int, dyntype_t);
void prereturn_literal(dyntype_t );
void create_activation(int parameters);
int count_references_activation(activation_t* src, activation_t* target);
void cleanup();
void error(int);
void release_activation(activation_t*);
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

#define FUNCTION(ID)  CRASH(1)\
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

#define CALL(PARAMETERS) {\
                        int i=0;\
                        create_activation(PARAMETERS);
#define PARAMETER(VALUE)    bind(i, VALUE);\
                            i++;

#define PARAMETER_LITERAL(VALUE)    bind_literal(i, VALUE);\
                                    i++;
                                
//ACHTUNG JUMP allokiert keine Parentactivation kann zu SEGFAULT führen
/*
#define JUMP(FUNCTION_ID,ID)   prejump(FUNCTION_ID, ID);\
                    goto table;\
                    case(ID):\
                    postjump();}
*/
                  
#define CRASH(CODE) longjmp(error_buffer,CODE);

#endif