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

SIGNATUR(CONT_TEST);
SIGNATUR(CONT_STORE);
SIGNATUR(TEST1);

struct X{
   int x;
   int y;
   char str[100];
};
START(4)           
    printf("start\n");
    clock_t c1;

    PUSH_LITERAL(scheme_literal_string("OUTPUT TEST 1\n"))
    PUSH_LITERAL(LAMBDA(write_string,-1))
    APPLICATE(1,23)

    //PUSH_LITERAL(LAMBDA_BASE(current_error_port));
    //APPLICATE(0, 32432)
    ////PUSH_LITERAL(scheme_literal_string("OUTPUT TEST 1\n"))
    //PUSH_LITERAL(LAMBDA_BASE(current_input_port))
    //APPLICATE(0, 884038)
    PUSH_LITERAL(LAMBDA(read_line,-1))
    APPLICATE(0,1532);
    PUSH_LITERAL(LAMBDA(write_string,-1))
    APPLICATE(1,9734)
    PUSH_LITERAL(LAMBDA(TEST1, 1))
    SET_GLOBAL_BOUND(0)
    POP;
    PUSH_LITERAL(LAMBDA(CONT_TEST, 1))
    SET_GLOBAL_BOUND(1)
    POP;
    PUSH_LITERAL(scheme_new_boolean(2))
    SET_GLOBAL_BOUND(2)
    POP;
    //PUSH_LITERAL(LAMBDA_BASE(current_output_port))
    //APPLICATE(0,34534)
    //PUSH_LITERAL(scheme_new_string("OUTPUT TEST 2\n"))
    //PUSH_LITERAL(LAMBDA_BASE(write_string))
    //APPLICATE(2,8479)
    //PUSH_LITERAL(scheme_new_string("OUTPUT TEST 2\n"))
    //PUSH_LITERAL(scheme_new_string("OUTPUT TEST 2\n"))

    PUSH_LITERAL(scheme_new_boolean(9000))
    PUSH(GLOBAL_BOUND(1))
    APPLICATE(1, 9683)
    SET_GLOBAL_BOUND(3)
    POP;
    if (*(BOUND(0, 3)).data.boolean_val == 10) {
        PUSH_LITERAL(scheme_new_boolean(54))
        PUSH(GLOBAL_BOUND(2))
        APPLICATE(1, 803423)
    }
    if (*(BOUND(0,3)).data.boolean_val == 54) {
        PUSH_LITERAL(scheme_new_boolean(22))
        PUSH(GLOBAL_BOUND(2))
        APPLICATE(1, 352345)
    }
    if (*(BOUND(0, 3)).data.boolean_val == 22) {
        PUSH_LITERAL(scheme_new_boolean(33))
        PUSH(GLOBAL_BOUND(2))
        APPLICATE(1, 345345)
    }
    //PARAMETER_LITERAL(scheme_new_boolean(1))
    //PARAMETER_LITERAL(scheme_new_boolean(2))
    //PARAMETER_LITERAL(scheme_new_boolean(3))
    //PARAMETER_LITERAL(scheme_new_boolean(4))
    //APPLICATE(GLOBAL_BOUND(6), 93452)
    PUSH_LITERAL(scheme_new_boolean(3))
    SET_GLOBAL_BOUND(2)
    POP;

    printf("Last return value: %d \n", *(BOUND(0, 2)).data.boolean_val);
EXIT

FUNCTION(TEST1)
    if (*(BOUND(0,0).data.boolean_val) == 0) {
        PUSH_LITERAL(scheme_new_boolean(1))
        RETURN
    }
    PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val) - 1))
    PUSH(GLOBAL_BOUND(1))
    APPLICATE(1, 55)
    PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val)* (*POP.data.boolean_val)))
    RETURN
END
/*
    (define (f x) (if (= (- x 1) 0) 0 (f (- x 1))))
*/
FUNCTION(CONT_TEST)
    PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val) - 1))
    PUSH_LITERAL(scheme_new_boolean(*(POP.data.boolean_val) == 5))
    
    if (*(POP.data.boolean_val)) {
        BODY(6)
        PUSH_LITERAL(scheme_new_boolean(4))
        SET_BOUND(0, 0)
        POP;
        PUSH(BOUND(0,0))
        PUSH_LITERAL(LAMBDA(CONT_STORE,1))
        PUSH_LITERAL(LAMBDA(call_with_current_continutation,-1))
        APPLICATE(1, 9508)
        printf("value of internal define: %d \n", *(BOUND(0,0).data.boolean_val));
        SET_BOUND(0,0)
        POP;
        printf("return value of call/cc: %d \n", *(BOUND(0, 0).data.boolean_val));
        printf("pop stack: %d \n", *(POP.data.boolean_val));
        PUSH_LITERAL(scheme_new_boolean(3))
        PUSH(BOUND(0, 0))
        BODY(1)
            PUSH_LITERAL(LAMBDA(CONT_STORE,1))
            SET_BOUND(0,0)
        BODY_CLOSE
        RETURN
        BODY_CLOSE
    }
    else {
        BODY(3)
        BODY(3)
        PUSH_LITERAL(scheme_new_boolean(*(BOUND(2, 0).data.boolean_val) - 1))
            PUSH(GLOBAL_BOUND(1))
            TAIL_APPLICATE(1)

        SET_BOUND(2,0)
        POP;
        BODY_CLOSE
        BODY_CLOSE

        PUSH(BOUND(0,0))
        RETURN
    }
END
FUNCTION(CONT_STORE)
    PUSH(BOUND(0, 0))
    SET_GLOBAL_BOUND(2)
    POP;
    //CONTINUATION_RESULT_LITERAL(scheme_new_boolean(99))
    //APPLICATE_CONTINUATION(BOUND(0,0))
    PUSH_LITERAL(scheme_new_boolean(10))
    RETURN
END

FUNCTION(TEST2)
    PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val) - 1))
    PUSH_LITERAL(scheme_new_boolean(*(POP.data.boolean_val) == 5))
    
    if (*(POP.data.boolean_val)) {
        PUSH_LITERAL(scheme_new_boolean(0));
        RETURN

    }
    else {  
        PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val) - 1))
        PUSH(GLOBAL_BOUND(5))
        TAIL_APPLICATE(1)
        RETURN
    }
    
END
#endif