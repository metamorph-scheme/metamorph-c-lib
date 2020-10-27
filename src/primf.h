#ifndef PRIMF_GLOB
#define PRIMF_GLOB

#define ELLIPSIS_PARAM(NAME) dyntype_t* NAME, int len

// Symbols

dyntype_t symbol_q(dyntype_t obj);
dyntype_t symbol_eq(ELLIPSIS_PARAM(obj));
dyntype_t symbol_to_string(dyntype_t sym);
dyntype_t string_to_symbol(dyntype_t str);

#endif