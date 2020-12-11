#ifndef PRIMF
#define PRIMF

bool_t i_list_q(dyntype_t obj);
scheme_pair_t i_cons(dyntype_t obj1, dyntype_t obj2);

bool_t i_number_q(dyntype_t obj);
bool_t i_real_q(dyntype_t obj);
bool_t i_rational_q(dyntype_t obj);
bool_t i_integer_q(dyntype_t obj);

bool_t i_inexact_q(dyntype_t obj);
bool_t i_exact_q(dyntype_t obj);


#endif