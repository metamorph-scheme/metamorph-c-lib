#ifndef DYNTYPES_GLOB
#define DYNTYPES_GLOB

#include "common.h"
#include <stdint.h>
#include "tommath/tommath.h"

// Types of dyntype

#define SCHEME_TYPE_BOOLEAN 0
#define SCHEME_TYPE_CHAR 1
#define SCHEME_TYPE_NULL 2 // empty list
#define SCHEME_TYPE_PAIR 3
#define SCHEME_TYPE_PROCEDURE 4 // lambda
#define SCHEME_TYPE_SYMBOL 5
#define SCHEME_TYPE_VECTOR 6
#define SCHEME_TYPE_BYTEVECTOR 7
#define SCHEME_TYPE_STRING 8
#define SCHEME_TYPE_NUMBER 9
#define SCHEME_TYPE_EOF_OBJECT 10
#define SCHEME_TYPE_PORT 11
#define SCHEME_TYPE_UNSPECIFIED 12

#define SCHEME_NUMERICAL_TYPE_EXACT_INTEGER 13
#define SCHEME_NUMERICAL_TYPE_INEXACT_INTEGER 14
#define SCHEME_NUMERICAL_TYPE_EXACT_RATIONAL 15
#define SCHEME_NUMERICAL_TYPE_INEXACT_RATIONAL 16
#define SCHEME_NUMERICAL_TYPE_INEXACT_REAL 17

#define SCHEME_RATIONAL_B_POS 1
#define SCHEME_RATIONAL_B_NEG 2
#define SCHEME_RATIONAL_D_NEG 3
#define SCHEME_RATIONAL_N_NEG 4

#define INTERNAL_TYPE_TYPE_EXCEPTION 0
#define INTERNAL_TYPE_BAD_ARGUMENT_EXCEPTION 1
#define INTERNAL_TYPE_SETTING_IMMUTABLE_LOCATION 2
#define INTERNAL_TYPE_TOMMATH_NUMBER_EXCEPTION 3

#define INTERNAL_GLOBAL_EXCEPTION 1

typedef char scheme_type_t;
typedef char internal_exception_type_t;
typedef char scheme_numerical_type_t;
typedef char scheme_rational_sign_t;

// Value Structs

struct scheme_pair_struct_t;
struct activation_struct_t;

typedef bool_t scheme_boolean_t;

typedef char* scheme_string_t;

typedef struct {
  int function_id;
  struct activation_struct_t* activation;
} scheme_procedure_t;

typedef struct {
  char* name;
} scheme_symbol_t;

typedef mp_int scheme_integer_t;

typedef double scheme_real_t;

typedef struct scheme_rational_struct_t {
    mp_int numerator;
    mp_int denominator;
} scheme_rational_t;

typedef struct scheme_number_struct_t {
    scheme_numerical_type_t type;
    union {
        scheme_integer_t* exact_integer_val;
        scheme_rational_t* exact_rational_val;
        scheme_real_t* inexact_real_val;
    } data;
} scheme_number_t;

// Exception Structs

typedef struct internal_type_exception_struct_t {
  scheme_type_t wanted;
  scheme_type_t received;
  int position;
} internal_type_exception_t;

typedef struct internal_bad_argument_exception_struct_t
{
  int position;
  char* message;
} internal_bad_argument_exception_t;

typedef struct exception_struct_t {
    union {
        internal_bad_argument_exception_t bad_argument_exception;
        internal_type_exception_t type_exception;
    } data;
    internal_exception_type_t internal_error_type;
} exception_t;

extern exception_t global_exception;

// Dyntype

typedef struct dyntype_t_struct {
  scheme_type_t type;
  bool_t _mutable;
  union {
    scheme_boolean_t* boolean_val;
    scheme_symbol_t* symbol_val;
    scheme_string_t* string_val;
    scheme_procedure_t* procedure_val;
    struct scheme_pair_struct_t* pair_val;
    scheme_number_t* number_val;
  } data;
} dyntype_t;

typedef struct scheme_pair_struct_t {
    dyntype_t car;
    dyntype_t cdr;
    bool_t list;
} scheme_pair_t;

// Throw Exception Macros

#define SET_TYPE_EXCEPTION(WANTED, RECEIVED, POS) {\
    internal_type_exception_t ex = { \
      .wanted = WANTED,\
      .received = RECEIVED,\
      .position = POS\
    };\
    global_exception.data.type_exception = ex;\
    global_exception.internal_error_type = INTERNAL_TYPE_TYPE_EXCEPTION;\
    CRASH(INTERNAL_GLOBAL_EXCEPTION)\
}

#define SET_SETTING_IMMUTABLE_LOCATION {\
    global_exception.internal_error_type=INTERNAL_TYPE_SETTING_IMMUTABLE_LOCATION;\
    CRASH(INTERNAL_GLOBAL_EXCEPTION)\
}

#define SET_TOMMATH_NUMBER_EXCEPTION {\
    global_exception.internal_error_type=INTERNAL_TYPE_TOMMATH_NUMBER_EXCEPTION;\
    CRASH(INTERNAL_GLOBAL_EXCEPTION)\
}

#define SET_BAD_ARGUMENT_EXCEPTION(POS, MSG)  {\
   global_exception.internal_error_type=INTERNAL_TYPE_BAD_ARGUMENT_EXCEPTION;\
   global_exception.data.bad_argument_exception = (internal_bad_argument_exception_t) {\
    .position = POS,\
    .message = MSG\
   };\
  CRASH(INTERNAL_GLOBAL_EXCEPTION)\
}

// Require Macros

#define REQUIRE_SCHEME_SYMBOL(PARAM, POS) scheme_symbol_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_SYMBOL) { \
    c_##PARAM = *PARAM.data.symbol_val;\
  } else {\
    SET_TYPE_EXCEPTION(SCHEME_TYPE_SYMBOL, PARAM.type, POS)\
  }

#define REQUIRE_SCHEME_PAIR(PARAM, POS) scheme_pair_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_PAIR) { \
    c_##PARAM = *PARAM.data.pair_val;\
  } else {\
    SET_TYPE_EXCEPTION(SCHEME_TYPE_PAIR, PARAM.type, POS);\
  }

#define REQUIRE_SCHEME_BOOLEAN(PARAM, POS) scheme_boolean_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_BOOLEAN) { \
    c_##PARAM = *PARAM.data.boolean_val;\
  } else {\
    SET_TYPE_EXCEPTION(SCHEME_TYPE_BOOLEAN, PARAM.type, POS);\
  }

#define REQUIRE_SCHEME_STRING(PARAM, POS) scheme_string_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_STRING) { \
    c_##PARAM = *PARAM.data.string_val;\
  } else {\
    SET_TYPE_EXCEPTION(SCHEME_TYPE_STRING, PARAM.type, POS);\
  }

#define REQUIRE_SCHEME_NUMBER(PARAM, POS) scheme_number_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_NUMBER) { \
    c_##PARAM = *PARAM.data.number_val;\
  } else {\
    SET_TYPE_EXCEPTION(SCHEME_TYPE_NUMBER, PARAM.type, POS);\
  }

#define REQUIRE_SCHEME_EXACT_INTEGER(PARAM,POS) scheme_integer_t cn_##PARAM;\
  REQUIRE_SCHEME_NUMBER(PARAM, POS)\
  if (c_##PARAM.type == SCHEME_NUMERICAL_TYPE_EXACT_INTEGER) { \
    cn_##PARAM = *c_##PARAM.data.exact_integer_val;\
  } else {\
    SET_TYPE_EXCEPTION(SCHEME_NUMERICAL_TYPE_EXACT_INTEGER, PARAM.type, POS);\
  }

#define REQUIRE_SCHEME_PROCEDURE(PARAM, POS) scheme_procedure_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_PROCEDURE) { \
    c_##PARAM = *PARAM.data.procedure_val;\
  } else {\
    SET_TYPE_EXCEPTION(SCHEME_TYPE_STRING, PARAM.type, POS);\
  }

// Value constructors

dyntype_t scheme_new_boolean(scheme_boolean_t obj);
dyntype_t scheme_literal_boolean(scheme_boolean_t obj);
dyntype_t scheme_new_string(scheme_string_t obj);
dyntype_t scheme_literal_string(scheme_string_t obj);
dyntype_t scheme_new_pair(scheme_pair_t obj);
dyntype_t scheme_literal_pair(scheme_pair_t obj);
dyntype_t scheme_new_symbol(scheme_symbol_t obj);
dyntype_t scheme_literal_symbol(scheme_symbol_t obj);
dyntype_t scheme_literal_procedure(scheme_procedure_t obj);
dyntype_t scheme_new_number(scheme_number_t obj);
dyntype_t scheme_literal_number(scheme_number_t obj);

scheme_number_t scheme_exact_integer(scheme_integer_t obj);
scheme_number_t scheme_exact_rational(scheme_rational_t obj);
scheme_number_t scheme_inexact_rational(scheme_real_t obj);
scheme_number_t scheme_inexact_integer(scheme_real_t obj);
scheme_number_t scheme_inexact_real(scheme_real_t obj);


#define SCHEME_UNSPECIFIED (dyntype_t) {\
  .type = SCHEME_TYPE_UNSPECIFIED,\
  ._mutable = TRUE,\
  .data = NULL\
}

#define SCHEME_NULL (dyntype_t) {\
  .type = SCHEME_TYPE_NULL,\
  ._mutable = TRUE,\
  .data = NULL\
}

void release_dyntype(dyntype_t);
dyntype_t copy_dyntype(dyntype_t);

int count_references_dyntype(dyntype_t dyntype, struct activation_struct_t *activation);

//dyntype_t eqv_q(dyntype_t a, dyntype_t b);

#endif