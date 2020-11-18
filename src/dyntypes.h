#ifndef DYNTYPES_GLOB
#define DYNTYPES_GLOB

#include "common.h"

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
#define INTERNAL_TYPE_TYPE_EXCEPTION 13
#define INTERNAL_TYPE_BAD_ARGUMENT_EXCEPTION 14
#define INTERNAL_TYPE_SETTING_IMMUTABLE_LOCATION 15

typedef char scheme_type_t;

// Value Types

typedef bool_t scheme_boolean_t;

typedef struct {
  char* name;
} scheme_symbol_t;

typedef struct {
  scheme_type_t wanted;
  scheme_type_t received;
  int position;
} internal_type_exception_t;

typedef struct
{
  int position;
  char* message;
} internal_bad_argument_exception_t;


typedef char* scheme_string_t;


struct scheme_pair_struct_t;

// dyntype

typedef struct dyntype_t_struct {
  scheme_type_t type;
  bool_t _mutable;
  union {
    scheme_boolean_t* boolean_val;
    scheme_symbol_t* symbol_val;
    scheme_string_t* string_val;
    struct scheme_pair_struct_t* pair_val;
    internal_type_exception_t internal_type_exception_val;
    internal_bad_argument_exception_t internal_bad_argument_exception_t;
  } data;
} dyntype_t;

typedef struct scheme_pair_struct_t {
  dyntype_t car;
  dyntype_t cdr;
  bool_t list;
} scheme_pair_t;

#define INTERNAL_TYPE_EXCEPTION(WANTED, RECEIVED, POS) (dyntype_t) {\
  .data.internal_type_exception_val = (internal_type_exception_t) {\
    .wanted = WANTED,\
    .received = RECEIVED,\
    .position = POS\
  },\
  .type = INTERNAL_TYPE_TYPE_EXCEPTION,\
  ._mutable = FALSE\
}

#define INTERNAL_SETTING_IMMUTABLE_LOCATION (dyntype_t) {\
  .type = INTERNAL_TYPE_SETTING_IMMUTABLE_LOCATION,\
  ._mutable = TRUE,\
  .data = NULL\
}

#define INTERNAL_BAD_ARGUMENT_EXCEPTION(POS, MSG) (dyntype_t) {\
  .type = INTERNAL_TYPE_BAD_ARGUMENT_EXCEPTION,\
  ._mutable = TRUE,\
  .data.internal_bad_argument_exception_t = (internal_bad_argument_exception_t) {\
    .position = POS,\
    .message = MSG\
  }\
}

#define REQUIRE_SCHEME_SYMBOL(PARAM, POS) scheme_symbol_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_SYMBOL) { \
    c_##PARAM = *PARAM.data.symbol_val;\
  } else {\
    return INTERNAL_TYPE_EXCEPTION(SCHEME_TYPE_SYMBOL, PARAM.type, POS);\
  }

#define REQUIRE_SCHEME_PAIR(PARAM, POS) scheme_pair_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_PAIR) { \
    c_##PARAM = *PARAM.data.pair_val;\
  } else {\
    return INTERNAL_TYPE_EXCEPTION(SCHEME_TYPE_PAIR, PARAM.type, POS);\
  }

#define REQUIRE_SCHEME_BOOLEAN(PARAM, POS) scheme_boolean_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_BOOLEAN) { \
    c_##PARAM = *PARAM.data.boolean_val;\
  } else {\
    return INTERNAL_TYPE_EXCEPTION(SCHEME_TYPE_BOOLEAN, PARAM.type, POS);\
  }

#define REQUIRE_SCHEME_STRING(PARAM, POS) scheme_string_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_STRING) { \
    c_##PARAM = *PARAM.data.string_val;\
  } else {\
    return INTERNAL_TYPE_EXCEPTION(SCHEME_TYPE_STRING, PARAM.type, POS);\
  }

dyntype_t scheme_new_boolean(scheme_boolean_t obj);
dyntype_t scheme_literal_boolean(scheme_boolean_t obj);
dyntype_t scheme_new_string(scheme_string_t obj);
dyntype_t scheme_literal_string(scheme_string_t obj);
dyntype_t scheme_new_pair(scheme_pair_t obj);
dyntype_t scheme_literal_pair(scheme_pair_t obj);
dyntype_t scheme_new_symbol(scheme_symbol_t obj);
dyntype_t scheme_literal_symbol(scheme_symbol_t obj);


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

//dyntype_t eqv_q(dyntype_t a, dyntype_t b);

#endif