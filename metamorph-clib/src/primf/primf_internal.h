#ifndef PRIMF
#define PRIMF

#include "../primf.h"

bool_t i_list_q(dyntype_t obj);
scheme_pair_t i_cons(dyntype_t obj1, dyntype_t obj2);

bool_t i_number_q(dyntype_t obj);
bool_t i_real_q(scheme_number_t obj);
bool_t i_rational_q(scheme_number_t obj);
bool_t i_integer_q(scheme_number_t obj);

bool_t i_inexact_q(scheme_number_t obj);
bool_t i_exact_q(scheme_number_t obj);

dyntype_t i_generic_num_cmp(scheme_ord_t ord, ELLIPSIS_PARAM(x));
dyntype_t i_generic_num_cmp_or_equal(scheme_ord_t ord, ELLIPSIS_PARAM(x));

scheme_number_t i_add(scheme_number_t a, scheme_number_t b);
scheme_number_t i_mul(scheme_number_t a, scheme_number_t b);
scheme_number_t i_sub(scheme_number_t a, scheme_number_t b);
scheme_number_t i_neg(scheme_number_t x);
#endif