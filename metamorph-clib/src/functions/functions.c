#include "functions_internal.h"
#include "../functions.h"
#include "../continuation.h"
#include "../lambda.h"
#include "../activations.h"
#include <stdio.h>
#include <stdlib.h>

activation_t* current_activation;

jmp_buf error_buffer;
int balance=0;
exception_t global_exception;
target_id(*current_function)(int);

void init(int globals, activation_t** root_activation){
    initalize_allocator();
    *root_activation = create_activation(globals);
    (*root_activation)->previous_activation = NULL;
    *root_activation = add_to_current_computation(*root_activation);
    current_activation=*root_activation;
}

void init_library(int globals, activation_t** root_activation) {
    *root_activation = create_activation(globals);
    (*root_activation)->previous_activation = NULL;
}

void control_loop() {
    target_id next_jump = {0, &prog};
    while (current_function = next_jump.target_function) {
        next_jump = (*current_function)(next_jump.marker);

    }
}

target_id applicate(int n_params, int id)
{
    target_id next_jump;
    dyntype_t proc = POP_LITERAL;
    switch (proc.type)
    {
    case(SCHEME_TYPE_PROCEDURE): {
        activation_t* new_activation = create_activation(n_params);
        for (int i = 0; i < n_params; i++)
            new_activation->formal_parameters[i] = POP_LITERAL;
        next_jump = applicate_lambda(proc, id, new_activation);
        break;
    }
    case(SCHEME_TYPE_CONTINUATION): {
        next_jump = applicate_continuation(proc, n_params);
        break;
    }
    default:
        CRASH(APPLICATION_OF_NON_PROCEDURE_TYPE)
        break;
    }
    release_dyntype(proc);
    return next_jump;
}


target_id prereturn() {
    dyntype_t return_value = POP_LITERAL;

    finalize_call();

    target_id return_jump;
    return_jump.marker = current_activation->return_marker;
    return_jump.target_function = current_activation->return_function;

    activation_t* returning_activation = current_activation;
    current_activation = returning_activation->previous_activation;

    remove_from_current_computation(returning_activation);

    //set return value
    PUSH_LITERAL(return_value);

    return return_jump;
}


void cleanup(){
    exit(0);
}

int popif() {
    dyntype_t tmp = POP_LITERAL;
    REQUIRE_SCHEME_BOOLEAN(tmp, 0);
    release_dyntype(tmp);
    return c_tmp;
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
    body->return_marker = -1;
    body->return_function = NULL;
    body->stack = NULL;

    current_activation=add_to_current_computation(body);
}

int close_body()
{
    if (current_activation->return_marker != -1)
        return 1;
    activation_t* body = current_activation;
    current_activation = body->parent_activation;
    remove_from_current_computation(body);
    return 0;
}


void finalize_call() {
    while (!close_body());
}