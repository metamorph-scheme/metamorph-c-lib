#include "functions_internal.h"
#include "../functions.h"
#include "../continuation.h"
#include "../lambda.h"
#include "../activations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

activation_t* current_activation;
activation_t* root_activation;
jmp_buf error_buffer;
dyntype_t return_value;
int return_address = 0;
int balance=0;
exception_t global_exception;

void initprog(int globals){
    initalize_allocator();
    return_value.type = SCHEME_TYPE_UNSPECIFIED;
    root_activation = create_activation(globals);
    root_activation->previous_activation = NULL;
    root_activation = add_to_current_computation(root_activation);
    root_activation->activation_type = ACTIVATION_BASE;
    current_activation=root_activation;
}

void applicate(int n_params, dyntype_t params[], dyntype_t proc, int id)
{
    activation_t* new_activation = create_activation(n_params);
    memcpy(new_activation->formal_parameters, params, new_activation->number_parameters * sizeof(dyntype_t));
    switch (proc.type)
    {
    case(SCHEME_TYPE_PROCEDURE): {
        applicate_lambda(proc, id, new_activation);
        break;
    }
    case(SCHEME_TYPE_CONTINUATION): {
        applicate_continuation(proc, new_activation);
        break;
    }
    default:
        CRASH(APPLICATION_OF_NON_PROCEDURE_TYPE)
            break;
    }
}

void applicate_literal(int n_params, dyntype_t params[], dyntype_t proc, int id)
{
    applicate(n_params, params, proc, id);
    release_dyntype(proc);
}

void prereturn(dyntype_t value){
    prereturn_literal(copy_dyntype(value));
}

void prereturn_literal(dyntype_t value) {
    finalize_call();

    release_dyntype(return_value);
    return_value = value;

    return_address = current_activation->return_address;

    activation_t* returning_activation = current_activation;
    current_activation = returning_activation->previous_activation;

    remove_from_current_computation(returning_activation);
}


void cleanup(){
    release_dyntype(return_value);
    root_activation->previous_activation = root_activation;
    remove_from_current_computation(root_activation);
    //exit(0);

}

void error(int code){
    switch (code)
    {
    case(POP_EMPTY_STACK): 
        printf("Attempted to perform a pop operation on an empty stack");
        break;
    case(INVALID_NUMBER_ARGUMENTS):
        printf("Attempted to call function with incorrect number of arguments");
        break;
    case(METAMORPH_C_SYNTAX_VIOLATION):
        printf("Attempted execution of a non wellformed Metamorph C program");
        break;
    case(APPLICATION_OF_NON_PROCEDURE_TYPE):
        printf("Attempted to perfom function application on non procedure type");
        break;
    case(INTERNAL_GLOBAL_EXCEPTION):
        break;
    default:
        break;
    }
    exit(1);
}

void body(int defines)
{
    activation_t* extension = add_extension(current_activation, defines);
    current_activation=add_to_current_computation(extension);
}

int close_body()
{
    if (!remove_extension(current_activation))
        return 1;
    activation_t* extension = current_activation;
    current_activation = remove_extension(extension);
    remove_from_current_computation(extension);
    return 0;
}


void finalize_call() {
    while (!close_body());
}