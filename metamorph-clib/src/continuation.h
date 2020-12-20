#ifndef CONTHEADER
#define CONTHEADER

#include "dyntypes.h"
#include "functions.h"

dyntype_t create_continuation(int);
void release_continuation(scheme_continuation_t);
dyntype_t copy_continuation(dyntype_t);
void applicate_continuation(dyntype_t cont);
void applicate_continuation_literal(dyntype_t cont);
void continuation_result(dyntype_t value);
void continuation_result_literal(dyntype_t value);
int count_references_continuation(scheme_continuation_t cont, activation_t* activation);



#define CONTINUATION(ID)	create_continuation(ID)

#define CONTINUATION_RESULT(VALUE){\
									continuation_result(VALUE);

#define CONTINUATION_RESULT_LITERAL(VALUE){\
									continuation_result_literal(VALUE);

#define APPLICATE_CONTINUATION_LITERAL(CONT)\
											applicate_continuation_literal(CONT);\
											goto table;}

#define APPLICATE_CONTINUATION(CONT) \
									applicate_continuation(CONT);\
									goto table;}


#endif