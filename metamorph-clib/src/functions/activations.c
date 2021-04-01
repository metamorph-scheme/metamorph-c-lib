#include "functions_internal.h"
#include "../functions.h"
#include "../continuation.h"
#include "../lambda.h"
#include "../activations.h"
#include <stdio.h>
#include <stdlib.h>



activation_t* create_activation(int parameters) {
    balance++;
    activation_t* new_activation = REQUEST(activation_t);
    //0 as a new activation is not captures
    new_activation->n_captures = 0;
    //0 as a new activation is not part of any computation
    new_activation->n_computations = 0;
    new_activation->number_parameters = parameters;
    new_activation->formal_parameters = REQUEST_ARRAY(dyntype_t, parameters);
    //Initalize parameters to UNSPECIFIED, so GC can collect them even if not set
    int i;
    for (i = 0; i < new_activation->number_parameters; i++)
        new_activation->formal_parameters[i] = (dyntype_t){ SCHEME_TYPE_UNSPECIFIED, 0, 0 };
    new_activation->stack = NULL;
    return new_activation;
}

activation_t* capture_activation(activation_t* activation)
{
    activation->n_captures++;
    return activation;
}

void free_activation(activation_t* activation)
{
    activation->n_captures--;
    //GC from top down via parent
    gc_activation(activation);
}

activation_t* add_to_current_computation(activation_t* activation)
{
    activation->n_computations++;
    return activation;
}

void remove_from_current_computation(activation_t* activation)
{
    activation->n_computations--;
    gc_activation(activation);
}

activation_t* create_computation(activation_t* activation)
{
    activation_t* head_of_comp=activation;
    while (activation) {
        //Activation is now part of the new computation
        activation->n_computations++;

        activation = activation->previous_activation;
    }
    return head_of_comp;
}

void discard_computation(activation_t* activation)
{
    while (activation) {
        //Activation is no longer part of discarded computation
        activation->n_computations--;
        activation_t* next = activation->previous_activation;
        gc_activation(activation); 
        activation = next;
    }
}

dyntype_t* stack_ellipsis(activation_t* activation, int n) {
    dyntype_t* ellipsis = REQUEST_ARRAY(dyntype_t, n);
    for (int i = 0; i < n; i++)
        ellipsis[i] = POP_LITERAL;
    return ellipsis;
}

void stack_push_literal(activation_t* activation, dyntype_t value) {
    dyntype_stack_t* elem = REQUEST(dyntype_stack_t);
    elem->next = activation->stack;
    activation->stack = elem;
    activation->stack->value = value;

}

void stack_push(activation_t* activation, dyntype_t value) {
    stack_push_literal(activation, copy_dyntype(value));
}

dyntype_t stack_pop(activation_t* activation) {
    if (!activation->stack) {
        CRASH(POP_EMPTY_STACK)
    }
    dyntype_stack_t* tmp = activation->stack;
    dyntype_t value = tmp->value;
    activation->stack = activation->stack->next;

    tmp->next = activation->stack_garbage;
    activation->stack_garbage = tmp;

    return value;
}

dyntype_t stack_pop_literal(activation_t* activation) {
    if (!activation->stack) {
        CRASH(POP_EMPTY_STACK)
    }
    dyntype_stack_t* tmp = activation->stack;
    dyntype_t value = tmp->value;
    activation->stack = tmp->next;
    RELEASE(dyntype_stack_t, tmp);
    return value;
}


void release_stack(activation_t* activation) {
    while (activation->stack != NULL) {
        stack_pop(activation);
    }
    while (activation->stack_garbage != NULL) {
        dyntype_stack_t* tmp = activation->stack_garbage;
        release_dyntype(activation->stack_garbage->value);
        activation->stack_garbage = tmp->next;
        RELEASE(dyntype_stack_t,tmp)
    }
}

//Does not release previous activation, because previous_activation is a weak reference
void gc_activation(activation_t* activation) {
    //Never release activation when in computation or in ongoing release 
    if (activation->n_computations || !activation->previous_activation)
        return;

    //Release activation only if all references to it are cycle refernces
    if (activation->n_captures) {
        int ref = 0;
        ref = count_references_activation(activation, activation);
        if (ref < activation->n_captures)
            return;
    }

    //Mark as ongoing release
    activation->previous_activation = NULL;
    //Save parent activation
    activation_t* parent_activation = activation->parent_activation;
    release_activation(activation);
    //Parent activation is no longer captured
    if(parent_activation){
        parent_activation->n_captures--;
        gc_activation(parent_activation);
    }
}

void release_activation(activation_t* activation) {
    //Debug
    balance--;

    //Release parameters
    release_dyntypes(activation->formal_parameters, activation->number_parameters);

    release_stack(activation);
    RELEASE(activation_t, activation);
}

void release_dyntypes(dyntype_t* dyn, int n) {
    for (int i = 0; i < n; i++)
        release_dyntype(dyn[i]);
    RELEASE_ARRAY(dyntype_t, n, dyn);
}

int count_references_activation(activation_t* src, activation_t* target) {
    int i, sum = 0;
    for (i = 1; i <= src->number_parameters; i++) {
        sum += count_references_dyntype(src->formal_parameters[i - 1], target);
    }
    if (src->parent_activation == target)
        sum++;
    return sum;
}
