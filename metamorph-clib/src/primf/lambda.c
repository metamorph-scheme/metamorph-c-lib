#include "../functions.h"
#include "../dyntypes.h"
#include "../lambda.h"
#include "../primf.h"

#include <stdlib.h>

dyntype_t* bound(int parent, int number){
    int i;
    activation_t* activation = current_activation;
    for(i=parent;i!=0;i--)
        activation = activation->parent_activation;
    return (activation->formal_parameters+number);
}

void set_bound(dyntype_t* target, dyntype_t src) {
    set_bound_literal(target, copy_dyntype(src));
}

void set_bound_literal(dyntype_t* target, dyntype_t src) {
    release_dyntype(*target);
    *target = src;
}

dyntype_t create_lambda(int function, int formal_parameters, int variadic){
    current_activation->references++;
    return (scheme_literal_procedure((scheme_procedure_t) {
                                function,
                                formal_parameters,
                                variadic,
                                current_activation}));
}

dyntype_t copy_procedure(dyntype_t lambda){
    REQUIRE_SCHEME_PROCEDURE(lambda, 0);
    c_lambda.activation->references++;
    return (scheme_literal_procedure(c_lambda));
}

void applicate_lambda(dyntype_t lambda, int id, activation_t* new_activation){
    REQUIRE_SCHEME_PROCEDURE(lambda, id);

    //Lambda activation is now referenced by temporary activation
    new_activation->parent_activation=c_lambda.activation;
    new_activation->parent_activation->references++;

    //Check number of arguments
    if (!c_lambda.variadic) {
        if (c_lambda.formal_parameters == new_activation->number_parameters);
        else CRASH(INVALID_NUMBER_ARGUMENTS)
    }
    else {
       if (c_lambda.formal_parameters > new_activation->number_parameters) CRASH(INVALID_NUMBER_ARGUMENTS)
        else {
            //Wrap up variadic arguments
            dyntype_t* ptr = REQUEST_ARRAY(dyntype_t, c_lambda.formal_parameters + 1);
            int i = 0;
            for (i = 0; i < c_lambda.formal_parameters; i++) {
                ptr[i] = new_activation->formal_parameters[i];
            }
            ptr[i] = list(new_activation->formal_parameters + i, new_activation->number_parameters - c_lambda.formal_parameters);
            RELEASE_ARRAY(dyntype_t, new_activation->number_parameters, new_activation->formal_parameters)
            new_activation->number_parameters = i;
            new_activation->formal_parameters = ptr;
        }
    }

    //Check For Tailcall
    if (id != -1){
        new_activation->return_address = id;
        //Current activation will be previous activation of new activation
        //Current activation is still part of the computation, therefore no reference needed
        new_activation->previous_activation = current_activation;
        
    }
    else {
        //Discard extension activations get to base activation
        while (current_activation->activation_type == ACTIVATION_EXTENSION) {
            activation_t* next = current_activation->previous_activation;
            current_activation->computations--;
            release_activation(current_activation);
            current_activation = next;
        }

        new_activation->return_address = current_activation->return_address;
        //Previous activation of current activation will be previous activation of new activation
        //Current activation is still part of the compuatation, therefore no reference needed
        new_activation->previous_activation = current_activation->previous_activation;

        //Release current activation for constant memory
        //Current activation is no longer part of current computation
        current_activation->computations--;
        //Discard computation would throw away whole computation, instead of excluding one activation from it
        release_activation(current_activation);
    }

    current_activation = new_activation;

    //Set next jump to function
    return_address=c_lambda.function_id;
}

int count_references_procedure(scheme_procedure_t procedure, 
                                activation_t* activation) {
    if (procedure.activation == activation)
        return 1;
    return count_references_activation(procedure.activation, activation);
}

void release_procedure(scheme_procedure_t procedure){
    
    procedure.activation->references--;
    release_activation(procedure.activation);
}