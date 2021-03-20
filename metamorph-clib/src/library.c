#include <stdio.h>
#include "cache.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "functions.h"
#include "dyntypes.h"
#include "lambda.h"
#include "continuation.h"
#include "common.h"
#include "primf.h"
#include "activations.h"


#ifdef _TEST_FUNCTIONS

SIGNATUR(NOT);
LIBRARY(test_lib,2)
	PUSH_LITERAL(scheme_new_boolean(123));
	SET_GLOBAL_BOUND(test_lib, 0);
	PUSH_LITERAL(LAMBDA(NOT, 1))
	SET_GLOBAL_BOUND(test_lib, 1);
	PUSH_UNSPECIFIED
	RETURN
END

FUNCTION(NOT)
PUSH(BOUND(0, 0))
if (popif()) {
	PUSH_LITERAL(scheme_new_boolean(0))
}else{
PUSH_LITERAL(scheme_new_boolean(0)) 
}
RETURN
END


#endif