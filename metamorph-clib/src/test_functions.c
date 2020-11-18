#include <stdio.h>
#include "cache.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "functions.h"
#include "dyntypes.h"
#include "lambda.h"

struct X{
   int x;
   int y;
   char str[100];
};

START(3)           
   clock_t c0 = clock();
   clock_t c1;

   GLOBAL_BOUND(0) = LAMBDA(90);
   GLOBAL_BOUND(1) = LAMBDA(89);
   GLOBAL_BOUND(2) = LAMBDA(70);
   

   CALL(2)
       PARAMETER_LITERAL(scheme_new_boolean(5))
       PARAMETER_LITERAL(scheme_new_boolean(5))
       APPLICATE(GLOBAL_BOUND(2), 345)

   CALL(1)
       PARAMETER_LITERAL(scheme_new_boolean(5));
       APPLICATE(GLOBAL_BOUND(1), 3);

    CALL(2)
        PARAMETER_LITERAL(scheme_new_boolean(100))
        PARAMETER_LITERAL(scheme_new_boolean(10))
        APPLICATE(GLOBAL_BOUND(0), 2)

   CALL(1)
       PARAMETER_LITERAL(scheme_new_boolean(5))
       APPLICATE(GLOBAL_BOUND(1), 98)



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
END



