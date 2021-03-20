#ifndef CONTHEADER
#define CONTHEADER

#include "dyntypes.h"
#include "functions.h"

dyntype_t create_continuation(target_id(*)(int),int, activation_t* activation);
void release_continuation(scheme_continuation_t);
dyntype_t copy_continuation(dyntype_t);
target_id applicate_continuation(dyntype_t cont, int n_params);
int count_references_continuation(scheme_continuation_t cont, activation_t* activation);


SIGNATUR(call_with_current_continutation);

#endif