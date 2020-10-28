#ifndef FUNCTIONS_GLOB
#define FUNCTIONS_GLOB

#include "dyntypes.h"
#include "cache.h"

typedef struct dyntype{
    int payload;
} Dyntype;

typedef struct activation_struct_t{
    int return_address;
    struct activation_struct_t* previous_activation;
    Dyntype formal_parameters[20];
} activation_t;

extern activation_t* current_activation;
extern activation_t* temporary_activation;
extern Dyntype return_value;
extern int return_address;


#define START  int main(){\
                table: \
                    switch(return_address){\
                    case(0):{
#define END     }}}

#define FUNCTION(NAME)  }NAME:{
#define ARGUMENT(number)    (current_activation->formal_parameters[number])
#define RETURN(VALUE)   {\
                            return_value=VALUE;\
                            return_address = current_activation->return_address;\
                            goto table;\
                        }   

#define CALL        {\
                        int i=0;\
                        temporary_activation = REQUEST(activation_t);\
                        temporary_activation->previous_activation=current_activation;
#define PARAMETER(VALUE)    temporary_activation->formal_parameters[i]=VALUE;\
                                i++;
#define JUMP(NAME,ID)   temporary_activation->return_address=ID;\
                    current_activation=temporary_activation;\
                    goto NAME;\ 
                    case(ID):\
                    temporary_activation = current_activation;\
                    current_activation = temporary_activation->previous_activation;\
                    PUTBACK(activation_t, temporary_activation);\
                    }

#endif