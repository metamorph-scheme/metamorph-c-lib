#ifndef CONTHEADER
#define CONTHEADER

#include "dyntypes.h"

dyntype_t create_continuation(int);

#define CONTINUATION(ID) create_continuation(ID);

#define APPLICATE_CONTINUATION(CONT, VALUE) 

#endif