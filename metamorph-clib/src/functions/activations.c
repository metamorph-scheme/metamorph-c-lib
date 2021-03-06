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
    //0 as being part of a computation is not handled via references
    new_activation->references = 0;
    //1 as every activation is created as part of the current computation
    new_activation->computations = 1;
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
    activation_t* dest = REQUEST(activation_t);

    //Debug
    balance++;

    //New activation is known by nobody
    dest->references = 0;

    dest->parent_activation = src->parent_activation;
    dest->parent_activation->references++;

    dest->activation_type = src->activation_type;

    //Weak reference to previous activation
    dest->previous_activation = src->previous_activation;

    //New activation is generally not a part of any computation
    dest->return_address = src->return_address;
    dest->computations = 0;

    //Copy parameters
    dest->number_parameters = src->number_parameters;
    dest->formal_parameters = REQUEST_ARRAY(dyntype_t, dest->number_parameters);
    int i = 0;
    for (i = 0; i < src->number_parameters; i++) {
        dest->formal_parameters[i] = copy_dyntype(src->formal_parameters[i]);
    }

    dest->stack = copy_stack(src->stack);

    return dest;
}

void stack_push_literal(activation_t* activation, dyntype_t value) {
    auxillary_stack_t* elem = REQUEST(auxillary_stack_t);
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
    while (activation->stack != NULL) {
        release_dyntype(stack_pop(activation));
    }
}

//Does not release previous activation, because previous_activation is a weak reference
void release_activation(activation_t* activation) {
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

    //Save parent activation
    activation_t* parent_activation = activation->parent_activation;

    //Just in case, stack should be empty
    release_stack(activation);
    RELEASE_ARRAY(dyntype_t, activation->number_parameters, activation->formal_parameters);
    RELEASE(activation_t, activation);

    //Tail call
    release_activation(parent_activation);
}

//void body(int defines)
//{
//    create_activation(defines);
//    temporary_activation->parent_activation = current_activation;
//    temporary_activation->parent_activation->references++;
//
//    temporary_activation->activation_type = ACTIVATION_EXTENSION;
//
//    //previous activation for later release
//    temporary_activation->previous_activation = current_activation;
//    temporary_activation->return_address = 0;
//    //activation and extension SHARE stack
//    temporary_activation->last_pop = current_activation->last_pop;    
//    temporary_activation->stack = current_activation->stack;
//    current_activation->stack = NULL;
//
//    current_activation = temporary_activation;
//}
//
//void body_close()
//{
//    temporary_activation = current_activation;
//    current_activation = temporary_activation->parent_activation;
//    //synchronize stack
//    current_activation->stack = temporary_activation->stack;
//    current_activation->last_pop = temporary_activation->last_pop;
//    temporary_activation->stack = NULL;
//    //body activation no longer part of this computation
//    temporary_activation->computations--;
//
//    if (temporary_activation->computations)
//        current_activation->computations++;
//
//    release_activation(temporary_activation);
//}