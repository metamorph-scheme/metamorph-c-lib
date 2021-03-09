#ifndef CONTHEADER
#define CONTHEADER

#include "dyntypes.h"
#include "functions.h"

dyntype_t create_continuation(int);
void release_continuation(scheme_continuation_t);
dyntype_t copy_continuation(dyntype_t);
void applicate_continuation(dyntype_t cont, activation_t*);
int count_references_continuation(scheme_continuation_t cont, activation_t* activation);



#define CONTINUATION(ID)	create_continuation(ID)



#endif