#ifndef FUNCTIONS_GLOB
#define FUNCTIONS_GLOB

#include "dyntypes.h"
#include "cache.h"
#include <setjmp.h>

typedef struct auxillary_stack_struct_t {
    dyntype_t value;
    struct auxillary_stack_struct_t* next;
} auxillary_stack_t;

typedef struct activation_struct_t{
    int return_address;
    //References which can be used to access data directly
    int references;
    int number_parameters;
    //Count how many activations have the ability to make activation current activation 
    //Basically one called activation and all continuations
    int computations;
    struct activation_struct_t* previous_activation;
    struct activation_struct_t* parent_activation;
    dyntype_t* formal_parameters;
    dyntype_t last_pop;
    auxillary_stack_t* stack;
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
activation_t* copy_activation(activation_t* src);
int count_references_activation(activation_t* src, activation_t* target);
void cleanup();
void applicate(dyntype_t,int);
void applicate_literal(dyntype_t,int);
void error(int);
void discard_computation(activation_t* activation);
void release_activation(activation_t*);
int count_cycle_references(activation_t* activation);
void stack_push(activation_t*, dyntype_t);
void body_enter(int);
void body_exit();
void stack_push_literal(activation_t*, dyntype_t);
dyntype_t stack_pop(activation_t*);

auxillary_stack_t* copy_stack(auxillary_stack_t* src);
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

#define CALL(PARAMETERS) {\
                        int i=0;\
                        create_activation(PARAMETERS);
#define PARAMETER(VALUE)    bind(i, VALUE);\
                            i++;

#define PARAMETER_LITERAL(VALUE)    bind_literal(i, VALUE);\
                                    i++;

#define APPLICATE(PROC, ID)  applicate(PROC, ID);\
                    goto table;\
                    case(ID):;}

#define APPLICATE_LITERAL(PROC, ID) applicate_literal(PROC, ID);\
                    goto table;\
                    case(ID):;}

#define TAIL_APPLICATE(PROC)  applicate(PROC, -1);\
                    goto table;}

#define TAIL_APPLICATE_LITERAL(PROC) applicate_literal(PROC, -1);\
                    goto table;}

#define PUSH(dyntype)           stack_push(current_activation, dyntype);
#define PUSH_LITERAL(dyntype)   stack_push_literal(current_activation, dyntype);
#define POP                     stack_pop(current_activation)

#define BODY_ENTER(NUMBER_OF_DEFINES)   body_enter(NUMBER_OF_DEFINES);
#define BODY_EXIT                       body_exit();

//POP produces a literal, only if a manual destruction of said literal is necessary this directive is legal to use
#define POP_FORCE_GC            release_dyntype(current_activation->last_pop);

//ACHTUNG JUMP allokiert keine Parentactivation kann zu SEGFAULT führen
/*
#define JUMP(FUNCTION_ID,ID)   prejump(FUNCTION_ID, ID);\
                    goto table;\
                    case(ID):\
                    postjump();}
*/
                
#define CRASH(CODE) longjmp(error_buffer,CODE);

#endif