#include "functions_internal.h"
#include "../functions.h"
#include <stdio.h>
#include <stdlib.h>

activation_t* current_activation;
activation_t* temporary_activation;
activation_t* root_activation;
jmp_buf error_buffer;
dyntype_t return_value;
int return_address = 0;
int balance=0;
int act_total=0;
exception_t global_exception;

void initprog(int globals){
    initalize_allocator();
    //DEBUG
    balance++;
    act_total++;
    return_value.type = SCHEME_TYPE_UNSPECIFIED;
    root_activation=REQUEST(activation_t);

    //Same as normal activation for consistency but should not matter
    root_activation->references=0;
    root_activation->computations = 1;

    //Semantically correct, but additionaly protects from accidental release
    root_activation->previous_activation = NULL;

    root_activation->number_parameters=globals;
    root_activation->formal_parameters=REQUEST_ARRAY(dyntype_t, globals);
    current_activation=root_activation;
}

void prereturn(dyntype_t value){
    prereturn_literal(copy_dyntype(value));
}

void prereturn_literal(dyntype_t value) {
    release_dyntype(return_value);
    return_value = value;
    return_address = current_activation->return_address;

    temporary_activation = current_activation;
    current_activation = temporary_activation->previous_activation;

    //Temporary activation is no longer part of current computation
    temporary_activation->computations--;

    //If temporary activation is part of computations other than current, then current activation is part of ONE additional computation
    //Basically a lazy update through all activations of the previous_activation chain
    if (temporary_activation->computations)
        current_activation->computations++;

    release_activation(temporary_activation);
}

void create_activation(int parameters) {
    balance++;
    temporary_activation = REQUEST(activation_t);
    //0 as being part of a computation is not handled via references
    temporary_activation->references = 0;
    //1 as every activation is created as part of the current computation
    temporary_activation->computations = 1;
    temporary_activation->number_parameters = parameters;
    temporary_activation->formal_parameters = REQUEST_ARRAY(dyntype_t, parameters);
    temporary_activation->stack = NULL;
}

activation_t* copy_activation(activation_t* src) {
    activation_t* dest = REQUEST(activation_t);

    //Debug
    balance++;

    //New activation is known by nobody
    dest->references = 0;
    
    dest->parent_activation = src->parent_activation;
    dest->parent_activation->references++;

    //Additional new head of compuation
    dest->previous_activation = src->previous_activation;
    
    //New activation is generally not a part of any computation
    dest->return_address = src->return_address;
    dest->computations = 0;

    //Copy parameters
    dest->number_parameters = src->number_parameters;
    dest->formal_parameters = REQUEST_ARRAY(dyntype_t, dest->number_parameters);
    int i=0;
    for (i = 0; i < src->number_parameters; i++) {
        dest->formal_parameters[i] = copy_dyntype(src->formal_parameters[i]);
    }

    dest->stack = copy_stack(src->stack);

    dest->last_pop = copy_dyntype(src->last_pop);

    return dest;
}

void bind(int number, dyntype_t src) {
    bind_literal(number, copy_dyntype(src));
}

void bind_literal(int number, dyntype_t src) {
    temporary_activation->formal_parameters[number] = src;
}

/*
void prejump(int function, int id){
    temporary_activation->return_address=id;
    current_activation=temporary_activation;
    return_address=function;
}

void postjump(){
    temporary_activation = current_activation;
    temporary_activation->references--;
    current_activation = temporary_activation->previous_activation;
    release_activation(temporary_activation);
}
*/

void stack_push(activation_t* activation, dyntype_t value) {
    stack_push_literal(activation, copy_dyntype(value));
}
void stack_push_literal(activation_t* activation, dyntype_t value) {
    auxillary_stack_t* elem = REQUEST(auxillary_stack_t);
    elem->next = activation->stack;
    activation->stack = elem;
    activation->stack->value = value;

}
dyntype_t stack_pop(activation_t* activation) {
    if (!activation->stack) {
        CRASH(POP_EMPTY_STACK)
    }
    dyntype_t value = activation->stack->value;
    activation->last_pop = value;
    auxillary_stack_t* tmp = activation->stack;
    activation->stack = activation->stack->next;
    RELEASE(auxillary_stack_t, tmp);
    return value;
}

auxillary_stack_t* copy_stack(auxillary_stack_t* src) {
    if (!src)
        return NULL;
    auxillary_stack_t* dest = REQUEST(auxillary_stack_t);
    dest->value = copy_dyntype(src->value);

    //Not a tailcall, stack is usually small
    dest->next = copy_stack(src->next);
    return dest;
}

void release_stack(activation_t* activation) {
    while ( activation->stack != NULL) {
        release_dyntype(stack_pop(activation));
    }
}

//Does not release previous activation, because previous_activation is a weak reference
void release_activation(activation_t* activation){
    //Never release activation when in computation or in ongoing release 
    if (activation->computations || !activation->previous_activation)
        return;

    //Release activation only if all references to it are cycle refernces
    if (activation->references) {
        int ref = 0;
        ref = count_cycle_references(activation);
        if (ref < activation->references)
            return;
    }

    //Debug
    balance--;

    //Release parameters
    int i = 0;
    for (i = 1; i <= activation->number_parameters; i++) {
        release_dyntype(activation->formal_parameters[i - 1]);
    }

    //In lambda application lambda activation became parent activation
    activation->parent_activation->references--;

    //Mark as ongoing release
    activation->previous_activation = NULL;

    //Just in case, stack should be empty
    release_stack(activation);
    RELEASE_ARRAY(dyntype_t, activation->number_parameters, activation->formal_parameters);
    RELEASE(activation_t, activation);

    //Tail call
    release_activation(activation->parent_activation);
}

void release_root_activation(activation_t* activation) {
    //Debug
    balance--;

    int i = 0;
    for (i = 1; i <= activation->number_parameters; i++) {
        release_dyntype(activation->formal_parameters[i - 1]);
    }
    RELEASE_ARRAY(dyntype_t, activation->number_parameters, activation->formal_parameters);
    RELEASE(activation_t, activation);
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


    exit(0);
}

void error(int code){
    switch (code)
    {
    case(POP_EMPTY_STACK): 
        printf("Attempted to perfrom a pop operation on an empty stack");
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
        if (activation->computations)
            return;
        release_activation(activation);
        activation = next;
    }
}
