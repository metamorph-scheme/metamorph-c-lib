#ifndef LAMBDAHEADER
#define LAMBDAHEADER

#include "dyntypes.h"

dyntype_t* bound(int, int);
void set_bound(dyntype_t*, dyntype_t);
void set_bound_literal(dyntype_t*, dyntype_t);
dyntype_t create_lambda(int);
dyntype_t copy_procedure(dyntype_t);
void preapplication(dyntype_t, int);
void preapplication_literal(dyntype_t lambda, int id);
void postapplication();
int count_references_procedure(scheme_procedure_t procedure, activation_t* activation);
void release_procedure(scheme_procedure_t);

#define LAMBDA(FUNCTION_ID)  create_lambda(FUNCTION_ID)
                   
//ACHTUNG Activation werden nicht deallokiert
#define APPLICATE(LAMBDA, ID)  preapplication(LAMBDA, ID);\
                    goto table;\
                    case(ID):\
                    postapplication();}

#define APPLICATE_LITERAL(LAMBDA, ID) preapplication_literal(LAMBDA, ID);\
                    goto table;\
                    case(ID):\
                    postapplication();}

#define BOUND(PARENT, NUMBER) (*(bound(PARENT, NUMBER)))
#define GLOBAL_BOUND(NUMBER) (root_activation->formal_parameters[NUMBER])

#define SET_BOUND(PARENT, NUMBER, DYNTYPE) set_bound(bound(PARENT, NUMBER), DYNTYPE);
#define SET_GLOBAL_BOUND(NUMBER, DYNTYPE) set_bound(&root_activation->formal_parameters[NUMBER], DYNTYPE);

#define SET_BOUND_LITERAL(PARENT, NUMBER, DYNTYPE) set_bound_literal(bound(PARENT, NUMBER), DYNTYPE);
#define SET_GLOBAL_BOUND_LITERAL(NUMBER, DYNTYPE) set_bound_literal(&root_activation->formal_parameters[NUMBER], DYNTYPE);


#endif