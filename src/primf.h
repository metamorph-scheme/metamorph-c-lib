#ifndef PRIMF_GLOB
#define PRIMF_GLOB

#define ELLIPSIS_PARAM(NAME) dyntype_t* NAME, int len
#define NONSTANDARD

// Symbols

dyntype_t symbol_q(dyntype_t obj);
dyntype_t symbol_eq(ELLIPSIS_PARAM(obj));
dyntype_t symbol_to_string(dyntype_t symbol);
dyntype_t string_to_symbol(dyntype_t string);

// Booleans

NONSTANDARD scheme_boolean_t obj_to_boolean(dyntype_t obj);
dyntype_t not(dyntype_t obj);
dyntype_t boolean_q(dyntype_t obj);
dyntype_t boolean_eq(ELLIPSIS_PARAM(boolean));

// String

#endif