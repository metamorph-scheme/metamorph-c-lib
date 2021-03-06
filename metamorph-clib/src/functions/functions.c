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
    root_activation=REQUEST(activation_t);

    //Same as normal activation for consistency but should not matter
    root_activation->references=0;
    root_activation->computations = 1;

    root_activation->activation_type = ACTIVATION_ROOT;

    //Semantically correct, but additionaly protects from accidental release
    root_activation->previous_activation = NULL;

    root_activation->number_parameters=globals;
    root_activation->formal_parameters=REQUEST_ARRAY(dyntype_t, globals);
    //Initalize parameters to UNSPECIFIED, that GC can collect them, even if not used
    int i;
    for (i = 0; i < root_activation->number_parameters; i++)
        root_activation->formal_parameters[i] = (dyntype_t){ SCHEME_TYPE_UNSPECIFIED, 0, 0 };
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
    release_dyntype(return_value);
    return_value = value;
    return_address = current_activation->return_address;

    activation_t* returning_activation = current_activation;
    current_activation = returning_activation->previous_activation;

    //Temporary activation is no longer part of current computation
    returning_activation->computations--;

    //If temporary activation is part of computations other than current, then current activation is part of ONE additional computation (correct number of computations would be difficult to handle)
    //Basically a lazy update through all activations of the previous_activation chain
    if (returning_activation->computations)
        current_activation->computations++;

    release_activation(returning_activation);
}


int count_cycle_references(activation_t* activation) {
    return count_references_activation(activation, activation);
}

int count_references_activation(activation_t* src, activation_t* target) {
    int i,sum=0;
    for (i = 1; i <= src->number_parameters; i++) {
        sum+=count_references_dyntype(src->formal_parameters[i - 1],target);
    }
    if (src->parent_activation == target)
        sum++;
    return sum;
}

void cleanup(){
    release_dyntype(return_value);
    root_activation->references--;
    release_root_activation(root_activation);

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

//Should only be performed on an activation which is the head of a computation (current activation, activations of continuation)
void discard_computation(activation_t* activation)
{
    activation_t* next;
    while (activation) {
        next = activation->previous_activation;

        //Activation is no longer part of discarded computation
        activation->computations--;

        //Check if activation (and previous activations) is part of different computation
        //because of lazy update only the first activation is aware of the correct number of computations
        if (activation->computations)
            return;

        release_activation(activation);
        activation = next;
    }
}

void release_root_activation(activation_t* activation) {

    int i = 0;
    for (i = 1; i <= activation->number_parameters; i++) {
        release_dyntype(activation->formal_parameters[i - 1]);
    }
    RELEASE_ARRAY(dyntype_t, activation->number_parameters, activation->formal_parameters);
    RELEASE(activation_t, activation);
}