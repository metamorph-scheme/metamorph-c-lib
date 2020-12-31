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

scheme_integer_t integer_gcd(scheme_integer_t* n, size_t len) {
	mp_int result;
	CATCH_MP_ERROR(mp_init(&result))
	
	mp_set_u32(&result, 0u);

	for (unsigned int i = 0; i < len; i++) {
		scheme_integer_t elem = n[i];	
		//REQUIRE_SCHEME_EXACT_INTEGER(elem, i)

		CATCH_MP_ERROR(mp_gcd(&result, &elem, &result))
	}

	return result;
}

scheme_integer_t integer_lcm(scheme_integer_t * n, size_t len) {
	mp_int result;
	CATCH_MP_ERROR(mp_init(&result));

	mp_set_u32(&result, 1u);

	for (unsigned int i = 0; i < len; i++) {
		scheme_integer_t elem = n[i];
		//REQUIRE_SCHEME_EXACT_INTEGER(elem, i)

		CATCH_MP_ERROR(mp_lcm(&result, &elem, &result))
	}

	return result;
}

scheme_integer_t integer_neg(scheme_integer_t a) {
	mp_int result;
	CATCH_MP_ERROR(mp_init(&result))

	CATCH_MP_ERROR(mp_neg(&a, &result))

	return result;
}

scheme_integer_t integer_create(char* bytes, size_t len) {
	mp_int x;
	CATCH_MP_ERROR(mp_init(&x));

	CATCH_MP_ERROR(mp_from_sbin(&x, bytes, len));

	return x;
}

scheme_ord_t integer_cmp(scheme_integer_t a, scheme_integer_t b) {
	return from_mp_ord(mp_cmp(&a, &b));
}

scheme_boolean_t integer_eq(scheme_integer_t a, scheme_integer_t b) {
	if (integer_cmp(a, b) == SCHEME_EQ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t integer_lt(scheme_integer_t a, scheme_integer_t b) {
	if (integer_cmp(a, b) == SCHEME_LT) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t integer_gt(scheme_integer_t a, scheme_integer_t b) {
	if (integer_cmp(a, b) == SCHEME_GT) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t integer_lte(scheme_integer_t a, scheme_integer_t b) {
	scheme_ord_t ord = integer_cmp(a, b);
	if (ord == SCHEME_LT || ord == SCHEME_EQ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t integer_gte(scheme_integer_t a, scheme_integer_t b) {
	scheme_ord_t ord = integer_cmp(a, b);
	if (ord == SCHEME_GT || ord == SCHEME_EQ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void integer_release(scheme_integer_t x) {
	mp_clear(&x);
}

scheme_integer_t integer_copy(scheme_integer_t obj) {
	mp_int copied;
	CATCH_MP_ERROR(mp_init_copy(&copied, &obj));
	return copied;
}