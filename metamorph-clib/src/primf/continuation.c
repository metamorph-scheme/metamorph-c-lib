#include "../continuation.h"
#include "../functions.h"
#include "../activations.h"
#include "../lambda.h"
#include <math.h>

dyntype_t create_continuation(target_id(*target_function)(int), int marker, activation_t* activation)
{
	scheme_continuation_t cont;
	//Only current activation is copied, tail of computation (previous activations) is shared among compuations 
	cont.activation = create_computation(activation);
	cont.marker = marker;
	cont.target_function = target_function;
	return scheme_literal_continuation(cont);
}

void release_continuation(scheme_continuation_t cont)
{
	//Releasing activation would not suffice, as it is part of a computation
	discard_computation(cont.activation);
}

dyntype_t copy_continuation(dyntype_t cont_src)
{
	REQUIRE_SCHEME_CONTINUATION(cont_src,0)
	scheme_continuation_t cont;
	//Only current activation is copied, tail of computation (previous activations) is shared among compuations 
	cont.activation = create_computation(c_cont_src.activation);
	cont.marker = c_cont_src.marker;
	cont.target_function = c_cont_src.target_function;


	return scheme_literal_continuation(cont);
}

target_id applicate_continuation(dyntype_t cont, int n_params)
{
	REQUIRE_SCHEME_CONTINUATION(cont, 0);

	//Check number of arguments
	if (n_params != 1) CRASH(INVALID_NUMBER_ARGUMENTS)

	dyntype_t return_value = POP_LITERAL;


	//current computation is generated from continuation, old one can be discarded
	discard_computation(current_activation);

	//activation is now part of the current computation and the computation represented by the continuation
    //previous activation will be made part of the current computation at RETURN, it is already part of the continuation computation
	current_activation = create_computation(c_cont.activation);

	//set return value
	PUSH_LITERAL(return_value);

	target_id next_jump;
	next_jump.marker = c_cont.marker;
	next_jump.target_function = c_cont.target_function;
	return next_jump;
}

int count_references_continuation(scheme_continuation_t cont, activation_t* activation)
{
	if (cont.activation == activation)
		return 1;
	return 0;
}

EXPORT(call_with_current_continutation)
	PUSH_LITERAL(create_continuation(current_activation->return_function,current_activation->return_marker, current_activation->previous_activation))
	PUSH(BOUND(0,0))
	TAIL_APPLICATE(1)
	RETURN
END