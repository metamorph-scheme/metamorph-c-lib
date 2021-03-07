#ifndef LAMBDAHEADER
#define LAMBDAHEADER

#include "dyntypes.h"

dyntype_t* bound(int, int);
void set_bound(dyntype_t*, dyntype_t);
void set_bound_literal(dyntype_t*, dyntype_t);
dyntype_t create_lambda(activation_t* activation,int,int, int);
dyntype_t copy_procedure(dyntype_t);
void applicate_lambda(dyntype_t, int, activation_t*);
int count_references_procedure(scheme_procedure_t procedure, activation_t* activation);
void release_procedure(scheme_procedure_t);

#define LAMBDA(FUNCTION_ID, FORMAL_PARAMETERS)  create_lambda(current_activation,FUNCTION_ID, FORMAL_PARAMETERS, 0)
#define LAMBDA_VARIADIC(FUNCTION_ID, FORMAL_PARAMETERS)  create_lambda(current_activation,FUNCTION_ID, FORMAL_PARAMETERS, 1)

#define BOUND(PARENT, NUMBER) (*(bound(PARENT, NUMBER)))
#define GLOBAL_BOUND(NUMBER) (root_activation->formal_parameters[NUMBER])

#define SET_BOUND(PARENT, NUMBER) set_bound(bound(PARENT, NUMBER), POP_LITERAL);
#define SET_GLOBAL_BOUND(NUMBER) set_bound(&root_activation->formal_parameters[NUMBER], POP_LITERAL);

#endif