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
    root_activation->references=1;
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
}

void create_activation(int parameters) {
    balance++;
    temporary_activation = REQUEST(activation_t);
    //1 as temporary activation will be current activation
    temporary_activation->references = 1;
    temporary_activation->number_parameters = parameters;
    temporary_activation->formal_parameters = REQUEST_ARRAY(dyntype_t, parameters);
    temporary_activation->stack = NULL;
}

activation_t* copy_activation(activation_t* src) {
    activation_t* dest = REQUEST(activation_t);
    dest->references = 0;
    
    dest->parent_activation = src->parent_activation;
    dest->parent_activation->references++;

    dest->previous_activation = src->previous_activation;
    dest->previous_activation++;
    
    dest->return_address = src->return_address;

    //Copy Parameters
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

void release_activation(activation_t* activation){
    if (!activation->previous_activation)
        return;

    if (activation->references) {
        int ref = 0;
        ref = count_cycle_references(activation);
        if (ref < activation->references)
            return;
    }
    balance--;

    int i = 0;
    for (i = 1; i <= activation->number_parameters; i++) {
        release_dyntype(activation->formal_parameters[i - 1]);
    }

    //As previous activation was current activation when activation was created
    activation->previous_activation->references--;
    //In lambda application lambda activation became parent activation
    activation->parent_activation->references--;
    activation_t* previous_activation = activation->previous_activation;
    activation->previous_activation = NULL;
    if(activation->parent_activation != previous_activation)
        release_activation(activation->parent_activation);
    release_activation(previous_activation);

    //Just in case, stack should be empty
    release_stack(activation);
    RELEASE_ARRAY(dyntype_t, activation->number_parameters, activation->formal_parameters);
    RELEASE(activation_t, activation);
}

void release_root_activation(activation_t* activation) {
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
    if (src->previous_activation == target)
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