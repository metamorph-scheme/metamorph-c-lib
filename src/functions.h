#ifndef FUNCTIONS_GLOB
#define FUNCTIONS_GLOB

#include "dyntypes.h"
#include "cache.h"

typedef struct dyntype{
    int payload;
} Dyntype;

typedef struct activation{
    int return_address;
    struct activation* previous_activation;
    Dyntype formal_parameters[20];
} Activation;

extern Activation* current_activation;
extern Activation* temporary_activation;
extern Dyntype return_value;
extern int return_address;


#define START  int main(){\
                table: \
                    switch(return_address){\
                    case(0):{
#define END     }}}

#define FUNCTION(name)  }name:{
#define ARGUMENT(number)    (current_activation->formal_parameters[number])
#define RETURN(value)   {\
                            return_value=value;\
                            return_address = current_activation->return_address;\
                            goto table;\
                        }   

#define CALL        {\
                        int i;\
                        i=0;\
                        temporary_activation = REQUEST(Activation);\
                        temporary_activation->previous_activation=current_activation;
#define PARAMETER(value)    temporary_activation->formal_parameters[i]=value;\
                                i++;
#define JUMP(name,id)   temporary_activation->return_address=id;\
                    current_activation=temporary_activation;\
                    goto name;\ 
                    case(id):\
                    temporary_activation = current_activation;\
                    current_activation = temporary_activation->previous_activation;\
                    PUTBACK(Activation, temporary_activation);\
                    }

#endif