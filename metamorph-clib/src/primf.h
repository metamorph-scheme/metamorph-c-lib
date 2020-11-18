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

// Pairs

dyntype_t pair_q(dyntype_t obj);
dyntype_t cons(dyntype_t obj1, dyntype_t obj2);
dyntype_t car(dyntype_t pair);
dyntype_t cdr(dyntype_t pair);
dyntype_t set_car_ex(dyntype_t pair, dyntype_t obj);
dyntype_t set_cdr_ex(dyntype_t pair, dyntype_t obj);
// TODO caar,cadr,cdar,cddr implemented in Scheme

dyntype_t null_q(dyntype_t obj);
dyntype_t list_q(dyntype_t obj);

dyntype_t make_list(int c_k);
dyntype_t make_list_fill(int k, dyntype_t fill);

dyntype_t list(ELLIPSIS_PARAM(obj));
/* 
(define list
  (lambda xs
    xs))
*/


#endif