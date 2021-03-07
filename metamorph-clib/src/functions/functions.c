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
int return_address = 0;
int balance=0;
exception_t global_exception;

void initprog(int globals){
    initalize_allocator();
    root_activation = create_activation(globals);
    root_activation->previous_activation = NULL;
    root_activation = add_to_current_computation(root_activation);
    current_activation=root_activation;
}

void applicate(int n_params, int id)
{
    dyntype_t proc = POP_LITERAL;
    activation_t* new_activation = create_activation(n_params);
    for (int i = 0; i < n_params; i++)
        new_activation->formal_parameters[i] = POP_LITERAL;
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
    release_dyntype(proc);
}


void prereturn() {
    dyntype_t return_value = POP_LITERAL;

    finalize_call();

    return_address = current_activation->return_address;

    activation_t* returning_activation = current_activation;
    current_activation = returning_activation->previous_activation;

    remove_from_current_computation(returning_activation);

    //set return value
    PUSH_LITERAL(return_value);
}


void cleanup(){
    root_activation->previous_activation = root_activation;
    release_activation(root_activation);
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
    activation_t* body = create_activation(defines);
    body->parent_activation = capture_activation(current_activation);

    body->previous_activation = current_activation;
    body->return_address = -1;
    body->stack = NULL;

    current_activation=add_to_current_computation(body);
}

int close_body()
{
    if (current_activation->return_address != -1)
        return 1;
    activation_t* body = current_activation;
    current_activation = body->parent_activation;
    remove_from_current_computation(body);
    return 0;
}


void finalize_call() {
    while (!close_body());
}