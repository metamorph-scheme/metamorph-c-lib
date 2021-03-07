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
struct X{
   int x;
   int y;
   char str[100];
};

START(4)           
   printf("start\n");
   clock_t c0 = clock();
   clock_t c1;
   PUSH_LITERAL(current_output_port())
   PUSH_LITERAL(scheme_literal_string("DAS IST EIN TEST\n"))
   write_string_stack();

   PUSH_LITERAL(LAMBDA(89, 1))
   SET_GLOBAL_BOUND(0)
   PUSH_LITERAL(LAMBDA(456, 1))
   SET_GLOBAL_BOUND(1)
   PUSH_LITERAL(scheme_new_boolean(3))
   SET_GLOBAL_BOUND(2)

   //SET_GLOBAL_BOUND_LITERAL(5, LAMBDA(864,1))
   //SET_GLOBAL_BOUND_LITERAL(6, LAMBDA_VARIADIC(54,1))

        PUSH_LITERAL(scheme_new_boolean(9000))
        PUSH(GLOBAL_BOUND(1))
        APPLICATE(1, 9683)

    if (*return_value.data.boolean_val == 10) {
         PUSH_LITERAL(scheme_new_boolean(54))
         PUSH(GLOBAL_BOUND(2))
         APPLICATE(1, 803423)
    }
   if (*return_value.data.boolean_val == 54) {
        PUSH_LITERAL(scheme_new_boolean(22))
        PUSH(GLOBAL_BOUND(2))
        APPLICATE(1, 352345)
   }
   if (*return_value.data.boolean_val == 22) {
       PUSH_LITERAL(scheme_new_boolean(33))
       PUSH(GLOBAL_BOUND(2))
       APPLICATE(1, 345345)
   }
   //    PARAMETER_LITERAL(scheme_new_boolean(1))
   //    PARAMETER_LITERAL(scheme_new_boolean(2))
   //    PARAMETER_LITERAL(scheme_new_boolean(3))
   //    PARAMETER_LITERAL(scheme_new_boolean(4))
   //    APPLICATE(GLOBAL_BOUND(6), 93452)
    PUSH_LITERAL(scheme_new_boolean(3))
    SET_GLOBAL_BOUND(2)
    c1 = clock();
    double runtime_diff_ms = (c1 - c0) * 1000. / CLOCKS_PER_SEC;
    printf("Runtime: %f \n", runtime_diff_ms);
    printf("Last return value: %d \n", *return_value.data.boolean_val);
    EXIT

FUNCTION(54)
    PUSH(BOUND(0, 1))
    RETURN

FUNCTION(89)
if (*(ARGUMENT(0).data.boolean_val) == 0) {
    PUSH_LITERAL(scheme_new_boolean(1))
    RETURN
   }
   PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val) - 1))
   PUSH(GLOBAL_BOUND(1))
    APPLICATE(1, 55)
   PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val)* (*return_value.data.boolean_val)))
   RETURN




/*
    (define (f x) (if (= (- x 1) 0) 0 (f (- x 1))))
*/
FUNCTION(456)
    PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val) - 1))
    PUSH_LITERAL(scheme_new_boolean(*(POP.data.boolean_val) == 5))
    
    if (*(POP.data.boolean_val)) {

        BODY(6)
        PUSH_LITERAL(scheme_new_boolean(4))
        SET_BOUND(0, 0)
        PUSH(BOUND(0,0))
        PUSH_LITERAL(CONTINUATION(9508))
        PUSH_LITERAL(LAMBDA(4869,1))
        APPLICATE(1, 9508)
        printf("return value of call/cc: %d \n", *return_value.data.boolean_val);
        printf("value of internal define: %d \n", *(BOUND(0,0).data.boolean_val));
        printf("pop stack: %d \n", *(POP.data.boolean_val));
        PUSH_LITERAL(scheme_new_boolean(3))

        PUSH(return_value)
        RETURN
        BODY_CLOSE
    }
    else {
        
        BODY(3)
        BODY(3)

        PUSH_LITERAL(scheme_new_boolean(*(BOUND(2, 0).data.boolean_val) - 1))
         PUSH(GLOBAL_BOUND(1))
         TAIL_APPLICATE(1)


        BODY_CLOSE
        BODY_CLOSE

        PUSH(return_value)
        RETURN
    }


   FUNCTION(4869)
       PUSH(BOUND(0, 0))
        SET_GLOBAL_BOUND(2)
        //CONTINUATION_RESULT_LITERAL(scheme_new_boolean(99))
        //APPLICATE_CONTINUATION(BOUND(0,0))
        PUSH_LITERAL(scheme_new_boolean(10))
        RETURN

FUNCTION(8984)
    PUSH_LITERAL(scheme_new_boolean(5))
    RETURN

FUNCTION(864)
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
        PUSH(return_value)
        RETURN
    }
    
END
#endif