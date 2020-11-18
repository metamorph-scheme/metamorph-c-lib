#ifndef DYNTYPES
#define DYNTYPES

#include "../cache.h"

#define SIMPLE_MALLOC(TYPE, VAL)  TYPE *ptr = REQUEST(TYPE); *ptr = VAL;
#define OBJ_CREATION_FUNCS(TYPE, SCHEME_TYPE) dyntype_t scheme_##TYPE(scheme_##TYPE##_t obj, bool_t _mutable) {\
  SIMPLE_MALLOC(scheme_##TYPE##_t, obj)\
  return (dyntype_t) {\
    .type = SCHEME_TYPE,\
    ._mutable = _mutable,\
    .data.TYPE##_val = ptr\
  };\
}\
dyntype_t scheme_new_##TYPE(scheme_##TYPE##_t obj) {\
  return scheme_##TYPE(obj, TRUE);\
}\
dyntype_t scheme_literal_##TYPE(scheme_##TYPE##_t obj) {\
  return scheme_##TYPE(obj, FALSE);\
}

dyntype_t scheme_boolean(scheme_boolean_t boolean, bool_t mutable);

#endif