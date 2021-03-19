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
    release_dyntype(*target);
    *target = src;
}

dyntype_t create_lambda(activation_t* activation, target_id (*function)(int), int formal_parameters, int variadic){
    return (scheme_literal_procedure((scheme_procedure_t) {
                                function,
                                formal_parameters,
                                variadic,
                                capture_activation(activation)}));
}

dyntype_t copy_procedure(dyntype_t lambda){
    REQUIRE_SCHEME_PROCEDURE(lambda, 0);
    capture_activation(c_lambda.activation);
    return (scheme_literal_procedure(c_lambda));
}

target_id applicate_lambda(dyntype_t lambda, int id, activation_t* new_activation){
    REQUIRE_SCHEME_PROCEDURE(lambda, id);

    //Lambda activation is now referenced by temporary activation
    new_activation->parent_activation = capture_activation(c_lambda.activation);

    //Check number of arguments
    if (c_lambda.variadic) {
        //if (c_lambda.formal_parameters > new_activation->number_parameters) CRASH(INVALID_NUMBER_ARGUMENTS)
        //else {
        //    //Wrap up variadic arguments
        //    dyntype_t* ptr = REQUEST_ARRAY(dyntype_t, c_lambda.formal_parameters + 1);
        //    int i = 0;
        //    for (i = 0; i < c_lambda.formal_parameters; i++) {
        //        ptr[i] = new_activation->formal_parameters[i];
        //    }
        //    ptr[i] = i_list(new_activation->formal_parameters + i, new_activation->number_parameters - c_lambda.formal_parameters);
        //    RELEASE_ARRAY(dyntype_t, new_activation->number_parameters, new_activation->formal_parameters)
        //        new_activation->number_parameters = i;
        //    new_activation->formal_parameters = ptr;
        //}
    }
    else if (c_lambda.formal_parameters != -1){
        if (c_lambda.formal_parameters == new_activation->number_parameters);
        else CRASH(INVALID_NUMBER_ARGUMENTS)
    }

    //Check For Tailcall
    if (id != -1){
        new_activation->return_marker = id;
        new_activation->return_function = current_function;
        //Current activation will be previous activation of new activation
        new_activation->previous_activation = current_activation;
    }
    else {
        finalize_call();

        //new activation will replace base activation
        new_activation->return_marker =current_activation->return_marker;
        new_activation->return_function = current_activation->return_function;
        new_activation->previous_activation = current_activation->previous_activation;

        //Release current activation for constant memory
        //Current activation is no longer part of current computation
        remove_from_current_computation(current_activation);
    }
    current_activation = add_to_current_computation(new_activation);

    target_id next_jump;
    next_jump.marker = 0;
    next_jump.target_function = c_lambda.target_function;
    return next_jump;
}

int count_references_procedure(scheme_procedure_t procedure, 
                                activation_t* activation) {
    if (procedure.activation == activation)
        return 1;
    return 0;
}

void release_procedure(scheme_procedure_t procedure){
 
    free_activation(procedure.activation);
}