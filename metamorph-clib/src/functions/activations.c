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
    new_activation->activation_type = ACTIVATION_BASE;
    new_activation->number_parameters = parameters;
    new_activation->formal_parameters = REQUEST_ARRAY(dyntype_t, parameters);
    //Initalize parameters to UNSPECIFIED, so GC can collect them even if not set
    int i;
    for (i = 0; i < new_activation->number_parameters; i++)
        new_activation->formal_parameters[i] = (dyntype_t){ SCHEME_TYPE_UNSPECIFIED, 0, 0 };
    new_activation->stack = NULL;
    return new_activation;
}


activation_t* copy_activation(activation_t* src) {
    activation_t* dest = create_activation(src->number_parameters);

    if (src->activation_type == ACTIVATION_EXTENSION) {
        dest->previous_activation = copy_activation(src->previous_activation);
        dest->parent_activation = capture_activation(dest->previous_activation);
    }
    else {
        dest->parent_activation = capture_activation(src->parent_activation);
        dest->previous_activation = src->previous_activation;
    }

    dest->activation_type = src->activation_type;
    dest->return_address = src->return_address;

    int i = 0;
    for (i = 0; i < src->number_parameters; i++) {
        dest->formal_parameters[i] = copy_dyntype(src->formal_parameters[i]);
    }
    dest->stack = copy_stack(src->stack);
    dest->stack_garbage = NULL;
    return dest;
}

activation_t* base_activation(activation_t* activation) {
    while (activation->activation_type == ACTIVATION_EXTENSION)
        activation = activation->previous_activation;
    return activation;
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

void stack_push_literal(activation_t* activation, dyntype_t value) {
    activation = base_activation(activation);
    dyntype_stack_t* elem = REQUEST(dyntype_stack_t);
    elem->next = activation->stack;
    activation->stack = elem;
    activation->stack->value = value;

}

void stack_push(activation_t* activation, dyntype_t value) {
    stack_push_literal(activation, copy_dyntype(value));
}

dyntype_t stack_pop(activation_t* activation) {
    activation = base_activation(activation);
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

dyntype_stack_t* copy_stack(dyntype_stack_t* src) {
    if (!src)
        return NULL;
    dyntype_stack_t* dest = REQUEST(dyntype_stack_t);
    dest->value = copy_dyntype(src->value);

    //Not a tailcall, stack is usually small
    dest->next = copy_stack(src->next);
    return dest;
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
    if(parent_activation)
        free_activation(parent_activation);
}

void release_activation(activation_t* activation) {
    //Debug
    balance--;

    //Release parameters
    int i = 0;
    for (i = 1; i <= activation->number_parameters; i++)
        release_dyntype(activation->formal_parameters[i - 1]);

    release_stack(activation);
    RELEASE_ARRAY(dyntype_t, activation->number_parameters, activation->formal_parameters);
    RELEASE(activation_t, activation);
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

activation_t* add_extension(activation_t* activation, int defines)
{
    activation_t* new_extension = create_activation(defines);
    new_extension->parent_activation = capture_activation(activation);
    new_extension->activation_type = ACTIVATION_EXTENSION;

    new_extension->previous_activation = activation;
    new_extension->return_address = 0;
    new_extension->stack = NULL;

    return new_extension;
}

activation_t* remove_extension(activation_t* activation)
{
    if (activation->activation_type == ACTIVATION_BASE)
        return NULL;
    activation_t* extension = activation;
    activation = extension->parent_activation;
    gc_activation(extension);
    return activation;
}