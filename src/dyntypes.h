#ifndef DYNTYPES_GLOB
#define DYNTYPES_GLOB

#include "common.h"

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
 
typedef char scheme_type_t;

typedef bool_t scheme_boolean_t;
typedef struct {
  char* name;
} scheme_symbol_t;
typedef struct {
  scheme_type_t wanted;
  scheme_type_t received;
  int position;
} internal_type_exception_t;
typedef char* scheme_string_t;

typedef struct {
  scheme_type_t type;
  bool_t _mutable;
  union {
    scheme_boolean_t boolean_val;
    scheme_symbol_t symbol_val;
    scheme_string_t string_val;
    internal_type_exception_t internal_type_exception_val;
  } data;
} dyntype_t;

#define INTERNAL_TYPE_EXCEPTION(WANTED, RECEIVED, POS) (dyntype_t) {\
  .data.internal_type_exception_val = (internal_type_exception_t) {\
    .wanted = WANTED,\
    .received = RECEIVED,\
    .position = POS\
  },\
  .type = INTERNAL_TYPE_TYPE_EXCEPTION,\
  ._mutable = FALSE\
}

#define REQUIRE_SCHEME_SYMBOL(PARAM, POS) scheme_symbol_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_SYMBOL) { \
    c_##PARAM = PARAM.data.symbol_val;\
  } else {\
    return INTERNAL_TYPE_EXCEPTION(SCHEME_TYPE_SYMBOL, PARAM.type, POS);\
  }

#define REQUIRE_SCHEME_BOOLEAN(PARAM, POS) scheme_boolean_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_BOOLEAN) { \
    c_##PARAM = PARAM.data.boolean_val;\
  } else {\
    return INTERNAL_TYPE_EXCEPTION(SCHEME_TYPE_BOOLEAN, PARAM.type, POS);\
  }

#define REQUIRE_SCHEME_STRING(PARAM, POS) scheme_string_t c_##PARAM;\
  if (PARAM.type == SCHEME_TYPE_STRING) { \
    c_##PARAM = PARAM.data.string_val;\
  } else {\
    return INTERNAL_TYPE_EXCEPTION(SCHEME_TYPE_STRING, PARAM.type, POS);\
  }

#define SCHEME_LITERAL_SYMBOL(OBJ) (dyntype_t) {\
  .type = SCHEME_TYPE_SYMBOL,\
  ._mutable = FALSE,\
  .data.symbol_val = OBJ\
}
#define SCHEME_NEW_SYMBOL(OBJ) (dyntype_t) {\
  .type = SCHEME_TYPE_SYMBOL,\
  ._mutable = TRUE,\
  .data.symbol_val = OBJ\
}

#define SCHEME_LITERAL_BOOLEAN(OBJ) (dyntype_t) {\
  .type = SCHEME_TYPE_BOOLEAN,\
  ._mutable = FALSE,\
  .data.boolean_val = OBJ\
}
#define SCHEME_NEW_BOOLEAN(OBJ) (dyntype_t) {\
  .type = SCHEME_TYPE_BOOLEAN,\
  ._mutable = TRUE,\
  .data.boolean_val = OBJ\
}

#define SCHEME_LITERAL_STRING(OBJ) (dyntype_t) {\
  .type = SCHEME_TYPE_STRING,\
  ._mutable = FALSE,\
  .data.string_val = OBJ\
}
#define SCHEME_NEW_STRING(OBJ) (dyntype_t) {\
  .type = SCHEME_TYPE_STRING,\
  ._mutable = TRUE,\
  .data.string_val = OBJ\
}

//dyntype_t eqv_q(dyntype_t a, dyntype_t b);

#endif