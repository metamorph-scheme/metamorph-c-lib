#ifndef ACTIVATIONS_GLOB
#define ACTIVATIONS_GLOB
#include "dyntypes.h"


typedef struct dyntype_stack_struct_t {
    dyntype_t value;
    struct dyntype_stack_struct_t* next;
} dyntype_stack_t;

typedef struct activation_struct_t {
    int return_address;
    //Counts the number of objects, which can access data directly, i.e. lambdas
    int n_captures;
    //Count how many activations have the ability to make activation current activation 
    //Basically one called activation and all continuations
    int n_computations;
    struct activation_struct_t* previous_activation;
    struct activation_struct_t* parent_activation;
    int number_parameters;
    dyntype_t* formal_parameters;
    dyntype_stack_t* stack;
    dyntype_stack_t* stack_garbage;
} activation_t;

activation_t* create_activation(int parameters);
activation_t* copy_activation(activation_t* src);
activation_t* capture_activation(activation_t* activation);
//free as opposite to capture
void free_activation(activation_t* activation);
activation_t* add_to_current_computation(activation_t* activation);
void remove_from_current_computation(activation_t* activation);
activation_t* create_computation(activation_t* activation);
void discard_computation(activation_t* activation);
void stack_push(activation_t*, dyntype_t);
void stack_push_literal(activation_t*, dyntype_t);
dyntype_t stack_pop(activation_t*);
dyntype_stack_t* copy_stack(dyntype_stack_t* src);
activation_t* add_extension(activation_t* activation, int);
activation_t* remove_extension(activation_t* activation);
void discard_computation(activation_t* activation);
int count_references_activation(activation_t* src, activation_t* target);
void gc_activation(activation_t*);
void release_activation(activation_t* activation);
dyntype_t stack_peek(activation_t* activation, int num);

#endif