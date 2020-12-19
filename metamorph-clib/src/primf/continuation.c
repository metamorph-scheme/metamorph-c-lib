#include "../continuation.h"
#include "../functions.h"

dyntype_t create_continuation(int id)
{
	scheme_continuation_t cont;
	cont.activation = copy_activation(current_activation);
	cont.continuation_id = id;

	return scheme_literal_continuation(cont);
}