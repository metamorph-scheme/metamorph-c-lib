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

START(7)           
   printf("start\n");
   clock_t c0 = clock();
   clock_t c1;
   PUSH_LITERAL(current_output_port())
   PUSH_LITERAL(scheme_literal_string("DAS IST EIN TEST\n"))
   write_string(PEEK(0), PEEK(1));
   POP; POP;

   SET_GLOBAL_BOUND_LITERAL(0, LAMBDA(90, 2))
   SET_GLOBAL_BOUND_LITERAL(1, LAMBDA(89,1))
   SET_GLOBAL_BOUND_LITERAL(2, LAMBDA(70,1))
   SET_GLOBAL_BOUND_LITERAL(3, LAMBDA(456,1))
   SET_GLOBAL_BOUND_LITERAL(4, scheme_new_boolean(3))
   SET_GLOBAL_BOUND_LITERAL(5, LAMBDA(864,1))
   SET_GLOBAL_BOUND_LITERAL(6, LAMBDA_VARIADIC(54,1))

    CALL(1)
        PARAMETER_LITERAL(scheme_new_boolean(9000))
        APPLICATE(GLOBAL_BOUND(3), 9683)

    if (*return_value.data.boolean_val == 10) {
        CALL(1)
            PARAMETER_LITERAL(scheme_new_boolean(54))
            APPLICATE(GLOBAL_BOUND(4),803423)
    }
   if (*return_value.data.boolean_val == 54) {
       CALL(1)
           PARAMETER_LITERAL(scheme_new_boolean(22))
           APPLICATE(GLOBAL_BOUND(4), 352345)
   }
   //CALL(4)
   //    PARAMETER_LITERAL(scheme_new_boolean(1))
   //    PARAMETER_LITERAL(scheme_new_boolean(2))
   //    PARAMETER_LITERAL(scheme_new_boolean(3))
   //    PARAMETER_LITERAL(scheme_new_boolean(4))
   //    APPLICATE(GLOBAL_BOUND(6), 93452)

    SET_GLOBAL_BOUND_LITERAL(4, scheme_new_boolean(3))
    c1 = clock();
    double runtime_diff_ms = (c1 - c0) * 1000. / CLOCKS_PER_SEC;
    printf("Runtime: %f \n", runtime_diff_ms);
    printf("Last return value: %d \n", *return_value.data.boolean_val);
    EXIT

FUNCTION(54)
    RETURN(BOUND(0,1))

FUNCTION(70)
    SET_BOUND(0,1, BOUND(0, 0));
    CALL_LITERAL(1)
       PARAMETER_LITERAL(scheme_new_boolean(5))
       APPLICATE(LAMBDA(89,1),683)
    RETURN_LITERAL(scheme_new_boolean(3))

FUNCTION(89)
   if(*(ARGUMENT(0).data.boolean_val) == 0)
      RETURN_LITERAL(scheme_new_boolean(1))
   CALL(1)
      PARAMETER_LITERAL(scheme_new_boolean(*(BOUND(0,0).data.boolean_val)-1))
      APPLICATE(GLOBAL_BOUND(1), 55)
   RETURN_LITERAL(scheme_new_boolean(*(BOUND(0,0).data.boolean_val) * (*return_value.data.boolean_val) ))


FUNCTION(90)
   int x,y,z;
   struct X* ptr[1000];
   for(x=0; x<*(ARGUMENT(0).data.boolean_val); x++){
      for(y=0; y<*(ARGUMENT(1).data.boolean_val); y++){
         for(z=0; z<1000; z++){
            ptr[z] = REQUEST(struct X);
            //ptr[z] = malloc(sizeof(struct X));
         }
         for(z=0; z<1000; z++){
            RELEASE(struct X, ptr[z]);
            CALL(1)
                PARAMETER_LITERAL(scheme_new_boolean(10))
                APPLICATE(GLOBAL_BOUND(1),9789)
            //free( ptr[z] );
         }
      }
   }
   RETURN_LITERAL(scheme_new_boolean(9));


/*
    (define (f x) (if (= (- x 1) 0) 0 (f (- x 1))))
*/
FUNCTION(456)
    PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val) - 1))
    PUSH_LITERAL(scheme_new_boolean(*(POP.data.boolean_val) == 5))
    
    if (*(POP.data.boolean_val)) {
        
        BODY(6)
        SET_BOUND_LITERAL(0, 0, scheme_new_boolean( 4 ))
        PUSH(BOUND(0,0))
        CALL_LITERAL(1)
            PARAMETER_LITERAL(CONTINUATION(9508))
            APPLICATE(LAMBDA(4869,1), 9508)
        printf("return value of call/cc: %d \n", *return_value.data.boolean_val);
        printf("value of internal define: %d \n", *(BOUND(0,0).data.boolean_val));
        printf("pop stack: %d \n", *(POP.data.boolean_val));
        PUSH_LITERAL(scheme_new_boolean(3))
        RETURN(return_value);
        BODY_CLOSE
    }
    else {
        
        BODY(3)
        BODY(3)

        PUSH_LITERAL(scheme_new_boolean(*(BOUND(2, 0).data.boolean_val) - 1))

        CALL(1)
            PARAMETER(POP)
            TAIL_APPLICATE(GLOBAL_BOUND(3))


        BODY_CLOSE
        BODY_CLOSE

        RETURN(return_value)
    }


FUNCTION(4869)
    SET_GLOBAL_BOUND(4, BOUND(0, 0))
    //CONTINUATION_RESULT_LITERAL(scheme_new_boolean(99))
    //APPLICATE_CONTINUATION(BOUND(0,0))
    RETURN_LITERAL(scheme_new_boolean(10))

FUNCTION(8984)
    RETURN_LITERAL(scheme_new_boolean(5))

FUNCTION(864)
    PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val) - 1))
    PUSH_LITERAL(scheme_new_boolean(*(POP.data.boolean_val) == 5))
    
    if (*(POP.data.boolean_val)) {
        
        RETURN_LITERAL(scheme_new_boolean(0));

    }
    else {
        
        PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val) - 1))
        CALL(1)
            PARAMETER(POP)
            TAIL_APPLICATE(GLOBAL_BOUND(5))
        RETURN(return_value)
    }
    
END
#endif