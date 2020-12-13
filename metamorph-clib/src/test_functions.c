#include <stdio.h>
#include "cache.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "functions.h"
#include "dyntypes.h"
#include "lambda.h"
#include "common.h"

#ifdef _TEST_FUNCTIONS
struct X{
   int x;
   int y;
   char str[100];
};

START(4)           
   clock_t c0 = clock();
   clock_t c1;

   GLOBAL_BOUND(0) = LAMBDA(90);
   GLOBAL_BOUND(1) = LAMBDA(89);
   GLOBAL_BOUND(2) = LAMBDA(70);
   GLOBAL_BOUND(3) = LAMBDA(456);
   

   CALL(1)
       PARAMETER_LITERAL(scheme_new_boolean(9000000))
       APPLICATE(GLOBAL_BOUND(3), 342)

   c1 = clock();
   double runtime_diff_ms = (c1 - c0) * 1000. / CLOCKS_PER_SEC;
   printf("%f \n", runtime_diff_ms);
   printf("%d \n", *return_value.data.boolean_val);
   EXIT

 FUNCTION(70)
    SET_BOUND(0,1, BOUND(0, 0));
    CALL(1)
       PARAMETER_LITERAL(scheme_new_boolean(5))
       APPLICATE_LITERAL(LAMBDA(89),683)
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
    PUSH_LITERAL(scheme_new_boolean(*(POP.data.boolean_val) == 0))
    POP_FORCE_GC
    if (*(POP.data.boolean_val)) {
        POP_FORCE_GC
        RETURN_LITERAL(scheme_new_boolean(0))
    }
    else {
        POP_FORCE_GC
        PUSH_LITERAL(scheme_new_boolean(*(BOUND(0, 0).data.boolean_val) - 1))
        CALL(1)
            PARAMETER_LITERAL(POP)
            APPLICATE(GLOBAL_BOUND(3), -1)
        RETURN(return_value)
    }
END

#endif