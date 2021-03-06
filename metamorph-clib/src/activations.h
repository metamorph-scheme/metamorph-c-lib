#ifndef ACTIVATIONS_GLOB
#define ACTIVATIONS_GLOB
#include "dyntypes.h"

#define ACTIVATION_BASE 0
#define ACTIVATION_EXTENSION 1

typedef struct auxillary_stack_struct_t {
    dyntype_t value;
    struct auxillary_stack_struct_t* next;
} auxillary_stack_t;

typedef struct activation_struct_t {
    int return_address;
    //Counts the number of objects, which can access data directly, i.e. lambdas
    int n_captures;
    //Count how many activations have the ability to make activation current activation 
    //Basically one called activation and all continuations
    int n_computations;
    struct activation_struct_t* previous_activation;
    struct activation_struct_t* parent_activation;
    struct activation_struct_t* extension_activation;
    int number_parameters;
    dyntype_t* formal_parameters;
    dyntype_t last_pop;
    auxillary_stack_t* stack;
    int activation_type;
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
auxillary_stack_t* copy_stack(auxillary_stack_t* src);
activation_t* add_extension(activation_t* activation, int);
activation_t* remove_extension(activation_t* activation);
void discard_computation(activation_t* activation);
int count_references_activation(activation_t* src, activation_t* target);
void release_activation(activation_t* activation);

#endif