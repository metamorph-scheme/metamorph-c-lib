#include "../continuation.h"
#include "../functions.h"
#include <math.h>

dyntype_t create_continuation(int id)
{
	scheme_continuation_t cont;
	cont.activation = copy_activation(current_activation);
	cont.continuation_id = id;

	//Continuation references activation
	cont.activation->references++;

	//The activation is part of the computation represented by the continuation
	cont.activation->computations++;

	//The activations below are now a part of this computation
	cont.activation->previous_activation->computations++;

	return scheme_literal_continuation(cont);
}

void release_continuation(scheme_continuation_t cont)
{
	cont.activation->references--;
	discard_computation(cont.activation);
}

dyntype_t copy_continuation(dyntype_t cont_src)
{
	REQUIRE_SCHEME_CONTINUATION(cont_src,0)
	scheme_continuation_t cont;
	cont.activation = copy_activation(c_cont_src.activation);
	cont.continuation_id = c_cont_src.continuation_id;

	//Continuation references activation
	cont.activation->references++;

	//The activation is part of the computation represented by the continuation
	cont.activation->computations++;

	//The activations below are now a part of this computation
	cont.activation->previous_activation->computations++;

	return scheme_literal_continuation(cont);
}

void applicate_continuation(dyntype_t cont)
{
	REQUIRE_SCHEME_CONTINUATION(cont, 0);
	discard_computation(current_activation);
	current_activation = c_cont.activation;
	current_activation->computations++;
	return_address = c_cont.continuation_id;
}

void applicate_continuation_literal(dyntype_t cont)
{
	applicate_continuation(cont);
	release_dyntype(cont);
}

void continuation_result(dyntype_t value)
{
	continuation_result(copy_dyntype(value));
}

void continuation_result_literal(dyntype_t value)
{
	release_dyntype(return_value);
	return_value = value;
}

int count_references_continuation(scheme_continuation_t cont, activation_t* activation)
{
	if (cont.activation == activation)
		return 1;
	return count_references_activation(cont.activation, activation);
}
