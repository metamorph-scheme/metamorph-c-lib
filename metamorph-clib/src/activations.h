#ifndef ACTIVATIONS_GLOB
#define ACTIVATIONS_GLOB
#include "dyntypes.h"

typedef struct auxillary_stack_struct_t {
    dyntype_t value;
    struct auxillary_stack_struct_t* next;
} auxillary_stack_t;

typedef struct activation_struct_t {
    int return_address;
    //References which can be used to access data directly
    int references;
    //Count how many activations have the ability to make activation current activation 
    //Basically one called activation and all continuations
    int computations;
    struct activation_struct_t* previous_activation;
    struct activation_struct_t* parent_activation;
    int number_parameters;
    dyntype_t* formal_parameters;
    dyntype_t last_pop;
    auxillary_stack_t* stack;
    int activation_type;
} activation_t;

activation_t* create_activation(int parameters);
activation_t* copy_activation(activation_t* src);
void release_activation(activation_t*);
void stack_push(activation_t*, dyntype_t);
void stack_push_literal(activation_t*, dyntype_t);
dyntype_t stack_pop(activation_t*);
auxillary_stack_t* copy_stack(auxillary_stack_t* src);
void body(int);
void body_close();

#endif