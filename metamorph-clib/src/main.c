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


START(8)
PUSH_LITERAL(scheme_new_boolean(FALSE))
SET_GLOBAL_BOUND(0)
current_output_port(0);
SET_GLOBAL_BOUND(1)
PUSH_LITERAL(LAMBDA(2, 1))
SET_GLOBAL_BOUND(2)
PUSH_LITERAL(LAMBDA(3, 2))
SET_GLOBAL_BOUND(3)
PUSH_LITERAL(LAMBDA(4, 1))
SET_GLOBAL_BOUND(4)
PUSH_LITERAL(LAMBDA(5, 1))
SET_GLOBAL_BOUND(5)
PUSH_LITERAL(LAMBDA(6, 1))
SET_GLOBAL_BOUND(6)
PUSH_LITERAL(LAMBDA(7, 1))
SET_GLOBAL_BOUND(7)
PUSH_LITERAL(scheme_new_boolean(TRUE))
PUSH(GLOBAL_BOUND(7))
APPLICATE(1, 8)
EXIT
FUNCTION(2)
PUSH(BOUND(0, 0))
{
} if (popif()) {
		PUSH_LITERAL(scheme_new_string("TRUE"))
	}
else {
		PUSH_LITERAL(scheme_new_string("FALSE"))
	}
	RETURN
		FUNCTION(9)
		PUSH(BOUND(0, 0))
		PUSH(BOUND(1, 1))
		APPLICATE(1, 10)
		PUSH(BOUND(1, 0))
		TAIL_APPLICATE(1)
		RETURN
		FUNCTION(3)
		PUSH_LITERAL(LAMBDA(9, 1))
		RETURN
		FUNCTION(4)
		PUSH(GLOBAL_BOUND(1))
		PUSH(BOUND(0, 0))
		write_string(2);
	RETURN
		FUNCTION(5)
		PUSH(BOUND(0, 0))
		SET_GLOBAL_BOUND(0)
		RETURN
		FUNCTION(6)
		PUSH(BOUND(0, 0))
if (popif()) {
			PUSH_LITERAL(scheme_new_boolean(FALSE))
		}
	else {
			PUSH_LITERAL(scheme_new_boolean(TRUE))
		}
		RETURN
			FUNCTION(7)
			PUSH_LITERAL(CONTINUATION(11))
			PUSH(GLOBAL_BOUND(5))
			APPLICATE(1, 11)
			PUSH(BOUND(0, 0))
			PUSH(GLOBAL_BOUND(6))
			APPLICATE(1, 12)
			SET_BOUND(0, 0)
			PUSH(BOUND(0, 0))
			PUSH(GLOBAL_BOUND(2))
			PUSH(GLOBAL_BOUND(4))
			PUSH(GLOBAL_BOUND(3))
			APPLICATE(2, 13)
			TAIL_APPLICATE(1)
			RETURN
			END
