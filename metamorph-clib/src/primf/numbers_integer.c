#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf_internal.h"
#include "../cache.h"
#include "../functions.h"
#include "../tommath/tommath.h"

// libtommath wrapper

scheme_integer_t integer_add(scheme_integer_t a, scheme_integer_t b) {
	mp_int c;
	CATCH_MP_ERROR(mp_init(&c))

	CATCH_MP_ERROR(mp_add(&a, &b, &c))

	return c;
}

scheme_integer_t integer_sub(scheme_integer_t a, scheme_integer_t b) {
	mp_int c;
	CATCH_MP_ERROR(mp_init(&c))

	CATCH_MP_ERROR(mp_sub(&a, &b, &c))

	return c;
}

scheme_integer_t integer_mul(scheme_integer_t a, scheme_integer_t b) {
	mp_int c;
	CATCH_MP_ERROR(mp_init(&c))

	CATCH_MP_ERROR(mp_mul(&a, &b, &c))

	return c;
}

// TODO: multiple return values, derive floor division from mp_div result
scheme_integer_t integer_truncate_div(scheme_integer_t a, scheme_integer_t b) {
	mp_int c,d;
	CATCH_MP_ERROR(mp_init_multi(&c, &d, NULL))

	CATCH_MP_ERROR(mp_div(&a, &b, &c, &d))

	return c;
}

scheme_integer_t integer_truncate_remainder(scheme_integer_t a, scheme_integer_t b) {
	mp_int c, d;
	CATCH_MP_ERROR(mp_init_multi(&c, &d, NULL))

	CATCH_MP_ERROR(mp_div(&a, &b, &c, &d))

	return d;
}

scheme_integer_t integer_truncate_quotient(scheme_integer_t a, scheme_integer_t b) {
	mp_int c, d;
	CATCH_MP_ERROR(mp_init_multi(&c, &d, NULL) != MP_OKAY) {
		SET_TOMMATH_NUMBER_EXCEPTION
	}

	CATCH_MP_ERROR(mp_div(&a, &b, &c, &d) != MP_OKAY) {
		SET_TOMMATH_NUMBER_EXCEPTION
	}

	return c;
}

dyntype_t integer_gcd(ELLIPSIS_PARAM(n)) {
	mp_int result;
	CATCH_MP_ERROR(mp_init(&result))
	
	mp_set_u32(&result, 0u);

	for (int i = 0; i < len; i++) {
		dyntype_t elem = n[i];	
		REQUIRE_SCHEME_EXACT_INTEGER(elem, i)

		CATCH_MP_ERROR(mp_gcd(&result, &cn_elem, &result))
	}

	return scheme_new_number(scheme_exact_integer(result));
}

dyntype_t integer_lcm(ELLIPSIS_PARAM(n)) {
	mp_int result;
	CATCH_MP_ERROR(mp_init(&result))

		mp_set_u32(&result, 1u);

	for (int i = 0; i < len; i++) {
		dyntype_t elem = n[i];
		REQUIRE_SCHEME_EXACT_INTEGER(elem, i)

		CATCH_MP_ERROR(mp_lcm(&result, &cn_elem, &result))
	}

	return scheme_new_number(scheme_exact_integer(result));
}

scheme_integer_t integer_neg(scheme_integer_t a) {
	mp_int result;
	CATCH_MP_ERROR(mp_init(&result))

	CATCH_MP_ERROR(mp_neg(&a, &result))

	return result;
}