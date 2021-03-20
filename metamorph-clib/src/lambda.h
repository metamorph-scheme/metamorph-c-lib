#ifndef LAMBDAHEADER
#define LAMBDAHEADER

#include "dyntypes.h"

dyntype_t* bound(int, int);
void set_bound(dyntype_t*, dyntype_t);
void set_bound_literal(dyntype_t*, dyntype_t);
dyntype_t create_lambda(activation_t* activation, target_id(*)(int),int, int);
dyntype_t copy_procedure(dyntype_t);
target_id applicate_lambda(dyntype_t, int, activation_t*);
int count_references_procedure(scheme_procedure_t procedure, activation_t* activation);
void release_procedure(scheme_procedure_t);

#define LAMBDA(FUNCTION_NAME, FORMAL_PARAMETERS)  create_lambda(current_activation,&FUNCTION_NAME, FORMAL_PARAMETERS, 0)
#define LAMBDA_VARIADIC(FUNCTION_NAME, FORMAL_PARAMETERS)  create_lambda(current_activation,&FUNCTION_NAME, FORMAL_PARAMETERS, 1)
#define LAMBDA_BASE(FUNCTION_NAME)  create_lambda(current_activation,&FUNCTION_NAME, -1, 0)

#define BOUND(PARENT, NUMBER) (*(bound(PARENT, NUMBER)))
#define GLOBAL_BOUND(NAME, NUMBER) (global_##NAME##_activation->formal_parameters[NUMBER])

#define SET_BOUND(PARENT, NUMBER) set_bound(bound(PARENT, NUMBER), POP_LITERAL); PUSH_UNSPECIFIED;
#define SET_GLOBAL_BOUND(NAME, NUMBER) set_bound(&(global_##NAME##_activation->formal_parameters[NUMBER]), POP_LITERAL); PUSH_UNSPECIFIED;

#endif