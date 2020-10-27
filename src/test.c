#include <stdio.h>
#include "testinc.h"

int main() {
   printf("Hello World!");
   int x = 7;
   x = inc(x);
   x += 1;
   return 0;
}