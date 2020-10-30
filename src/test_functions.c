#include <stdio.h>
#include "cache.h"
#include <string.h>
#include <time.h>
#include "functions.h"

struct X{
   int x;
   int y;
   char str[100];
};


START
   initalize_allocator();

   clock_t c0 = clock();
   clock_t c1;
   int i,j;

   struct X* data = REQUEST(struct X);
   data->x = 3;
   data->y = 3;

   strcpy(data->str, "Test");
   
   CALL
      PARAMETER((dyntype_t){10})
      PARAMETER((dyntype_t){100})
      JUMP(func, 1)

   CALL
      PARAMETER((dyntype_t){6})
      JUMP(fak, 3)

   c1 = clock();
   double runtime_diff_ms = (c1 - c0) * 1000. / CLOCKS_PER_SEC;
   printf("%f \n", runtime_diff_ms);
   printf("%d", return_value.payload);

FUNCTION(fak)
   if(ARGUMENT(0).payload == 0)
      RETURN((dyntype_t) {1})
   CALL
      PARAMETER(((dyntype_t) {ARGUMENT(0).payload - 1}))
      JUMP(fak, 2)
   RETURN((dyntype_t) {return_value.payload * ARGUMENT(0).payload})


FUNCTION(func)
   int x,y,z;
   struct X* ptr[1000];
   for(x=0; x<ARGUMENT(0).payload; x++){
      for(y=0; y<ARGUMENT(1).payload; y++){
         for(z=0; z<1000; z++){
            ptr[z] = REQUEST(struct X);
            //ptr[z] = malloc(sizeof(struct X));
         }
         for(z=0; z<1000; z++){
            PUTBACK(struct X, ptr[z]);
            //free( ptr[z]);
         }
      }
   }
   RETURN((dyntype_t) {0})
END



void func2(int a,int b);

int main2() {
   initalize_allocator();

   clock_t c0 = clock();
   clock_t c1;
   int i,j;

   struct X* data = REQUEST(struct X);
   data->x = 3;
   data->y = 3;

   strcpy(data->str, "Test");

   func2(1000,1000);

   c1 = clock();
   double runtime_diff_ms = (c1 - c0) * 1000. / CLOCKS_PER_SEC;
   printf("%f", runtime_diff_ms);
   return 0;
}

void func2(int a,int b){
   int x,y,z;
   struct X* ptr[1000];
   for(x=0; x<a; x++){
      for(y=0; y<b; y++){
         for(z=0; z<1000; z++){
            //ptr[z] = REQUEST(struct X);
            ptr[z] = malloc(sizeof(struct X));
         }
         for(z=0; z<1000; z++){
            //PUTBACK(struct X, ptr[z]);
            free( ptr[z]);
         }
      }
   }
}