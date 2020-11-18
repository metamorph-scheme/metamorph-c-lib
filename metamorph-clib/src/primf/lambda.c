#include "../functions.h"
#include "../dyntypes.h"
#include "../lambda.h"
#include <stdlib.h>

dyntype_t* bound(int parent, int number){
    int i;
    activation_t* activation = current_activation;
    for(i=parent;i!=0;i--)
        activation = current_activation->parent_activation;
    return (activation->formal_parameters+number);
}

void set_bound(dyntype_t* target, dyntype_t src) {
    set_bound_literal(target, copy_dyntype(src));
}

void set_bound_literal(dyntype_t* target, dyntype_t src) {
    release_dyntype(*target);
    *target = src;
}

dyntype_t create_lambda(int function){
    current_activation->references++;
    return (scheme_literal_procedure((scheme_procedure_t) {
                                function,
                                current_activation}));
}

dyntype_t copy_procedure(dyntype_t lambda){
    REQUIRE_SCHEME_PROCEDURE(lambda, 0);
    c_lambda.activation->references++;
    return (scheme_literal_procedure(c_lambda));
}

void preapplication(dyntype_t lambda, int id){
    temporary_activation->return_address=id;
    REQUIRE_SCHEME_PROCEDURE(lambda, id);
    temporary_activation->parent_activation=c_lambda.activation;
    current_activation = temporary_activation;
    //Lambda activation is now referenced by temporary activation
    temporary_activation->parent_activation->references++; 
    return_address=c_lambda.function_id;
}

void preapplication_literal(dyntype_t lambda, int id) {
    preapplication(lambda, id);
    release_dyntype(lambda);
}

void postapplication(){
    temporary_activation = current_activation;
    current_activation = temporary_activation->previous_activation;
    //Temporary activation is no longer current activation
    temporary_activation->references--; 
    release_activation(temporary_activation);
}

int count_references_procedure(scheme_procedure_t procedure, 
                                activation_t* activation) {
    if (procedure.activation == activation)
        return 1;
    return count_references_activation(procedure.activation, activation);
}

void release_procedure(scheme_procedure_t procedure){
    
    procedure.activation->references--;
    release_activation(procedure.activation);
}