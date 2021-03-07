#include "../continuation.h"
#include "../functions.h"
#include "../activations.h"
#include <math.h>

dyntype_t create_continuation(int id)
{
	scheme_continuation_t cont;
	//Only current activation is copied, tail of computation (previous activations) is shared among compuations 
	cont.activation = create_computation(current_activation);
	cont.continuation_id = id;
	
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
	cont.continuation_id = c_cont_src.continuation_id;


	return scheme_literal_continuation(cont);
}

void applicate_continuation(dyntype_t cont, activation_t* new_activation)
{
	REQUIRE_SCHEME_CONTINUATION(cont, 0);

	//Check number of arguments
	if (new_activation->number_parameters!=1) CRASH(INVALID_NUMBER_ARGUMENTS)
	
	//Set continuation result
	release_dyntype(return_value);
	return_value = new_activation->formal_parameters[0];

	//Destroy partially constructed activation
	new_activation->number_parameters = 0;
	release_activation(new_activation);

	return_address = c_cont.continuation_id;

	//current computation is generated from continuation, old one can be discarded
	discard_computation(current_activation);

	//activation is now part of the current computation and the computation represented by the continuation
    //previous activation will be made part of the current computation at RETURN, it is already part of the continuation computation
	current_activation = create_computation(c_cont.activation);
}

int count_references_continuation(scheme_continuation_t cont, activation_t* activation)
{
	if (cont.activation == activation)
		return 1;
	return count_references_activation(cont.activation, activation);
}
