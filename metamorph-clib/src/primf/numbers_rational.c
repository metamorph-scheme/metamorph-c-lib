#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf_internal.h"
#include "../cache.h"
#include "../functions.h"
#include "../tommath/tommath.h"
#include <string.h>

// manipulates rational instead of copying!
void rational_min(scheme_rational_t* q) {
	mp_int gcd;
	CATCH_MP_ERROR(mp_init(&gcd));

	CATCH_MP_ERROR(mp_gcd(&q->denominator, &q->numerator, &gcd));

    CATCH_MP_ERROR(mp_div(&q->denominator, &gcd, &q->denominator, NULL));
	CATCH_MP_ERROR(mp_div(&q->numerator, &gcd, &q->numerator, NULL));

	mp_clear(&gcd);
}

// determine the sign state of a rational
scheme_rational_sign_t rational_sign(scheme_rational_t a) {
	if (a.denominator.sign == MP_ZPOS && a.numerator.sign == MP_ZPOS)
		return SCHEME_RATIONAL_B_POS;
	if (a.denominator.sign == MP_NEG && a.numerator.sign == MP_NEG)
		return SCHEME_RATIONAL_B_NEG;
	if (a.denominator.sign == MP_NEG)
		return SCHEME_RATIONAL_D_NEG;

	return SCHEME_RATIONAL_N_NEG;
}

scheme_boolean_t rational_sign_negative(scheme_rational_sign_t sign) {
	if (sign == SCHEME_RATIONAL_B_POS || sign == SCHEME_RATIONAL_B_NEG)
		return FALSE;

	return TRUE;
}

scheme_rational_t rational_add(scheme_rational_t a, scheme_rational_t b) {
	mp_int new_numerator, new_denominator, tmp;

	CATCH_MP_ERROR(mp_init_multi(&new_numerator, &new_denominator, &tmp, NULL));

	CATCH_MP_ERROR(mp_mul(&b.denominator, &a.numerator, &tmp));
	CATCH_MP_ERROR(mp_mul(&b.numerator, &a.denominator, &new_numerator));

	CATCH_MP_ERROR(mp_add(&tmp, &new_numerator, &new_numerator));

	mp_clear(&tmp);

	CATCH_MP_ERROR(mp_mul(&a.denominator, &b.denominator, &new_denominator));

	scheme_rational_t res = (scheme_rational_t) {
		.numerator = new_numerator,
		.denominator = new_denominator
	};

	rational_min(&res);
	return res;
}

scheme_rational_t rational_sub(scheme_rational_t a, scheme_rational_t b) {
	mp_int new_numerator, new_denominator, tmp;

	CATCH_MP_ERROR(mp_init_multi(&new_numerator, &new_denominator, &tmp, NULL));


	CATCH_MP_ERROR(mp_mul(&b.denominator, &a.numerator, &tmp));
	CATCH_MP_ERROR(mp_mul(&b.numerator, &a.denominator, &new_numerator));

	CATCH_MP_ERROR(mp_sub(&tmp, &new_numerator, &new_numerator));
	mp_clear(&tmp);

	CATCH_MP_ERROR(mp_mul(&a.denominator, &b.denominator, &new_denominator));

	scheme_rational_t res = (scheme_rational_t) {
		.numerator = new_numerator,
		.denominator = new_denominator
	};
	rational_min(&res);
	return res;
}


scheme_rational_t rational_mul(scheme_rational_t a, scheme_rational_t b) {
	mp_int new_numerator, new_denominator;

	CATCH_MP_ERROR(mp_init_multi(&new_numerator, &new_denominator, NULL));


	CATCH_MP_ERROR(mp_mul(&b.numerator, &a.numerator, &new_numerator));

	CATCH_MP_ERROR(mp_mul(&a.denominator, &b.denominator, &new_denominator));

	scheme_rational_t res = (scheme_rational_t){
		.numerator = new_numerator,
		.denominator = new_denominator
	};
	rational_min(&res);
	return res;
}

scheme_rational_t rational_div(scheme_rational_t a, scheme_rational_t b) {
	mp_int new_numerator, new_denominator;

	CATCH_MP_ERROR(mp_init_multi(&new_numerator, &new_denominator, NULL));

	scheme_rational_t inverse_b = (scheme_rational_t){
		.numerator = b.denominator,
		.denominator = b.numerator
	};

	return rational_mul(a, inverse_b);
}

scheme_rational_t rational_neg(scheme_rational_t a) {
	mp_int new_numerator, new_denominator;
	CATCH_MP_ERROR(mp_init(&new_numerator));

	CATCH_MP_ERROR(mp_neg(&a.numerator, &new_numerator));
	CATCH_MP_ERROR(mp_init_copy(&new_denominator, &a.denominator));

	return (scheme_rational_t){
		.numerator = new_numerator,
		.denominator = new_denominator
	};
}

// no copy and no initialization
scheme_rational_t rational_create(scheme_integer_t a, scheme_integer_t b) {
	return (scheme_rational_t) {
		.numerator = a,
		.denominator = b
	};
}

scheme_rational_t rational_create_raw(char * a_bytes, size_t alen, char * b_bytes, size_t blen) {
	return (scheme_rational_t) {
		.numerator = integer_create(a_bytes, alen),
		.denominator = integer_create(b_bytes, blen)
	};
}

scheme_integer_t rational_denominator(scheme_rational_t x) {
	return x.denominator;
}

scheme_integer_t rational_numerator(scheme_rational_t x) {
	return x.numerator;
}

scheme_ord_t rational_cmp(scheme_rational_t a, scheme_rational_t b) {
	scheme_rational_sign_t a_sign = rational_sign(a);
	scheme_rational_sign_t b_sign = rational_sign(b);
	if (rational_sign_negative(a_sign) == rational_sign_negative(b_sign)) {
		// signs are equal - compare absolute values
		mp_int lcm, factor_a, factor_b;

		CATCH_MP_ERROR(mp_init_multi(&lcm, &factor_a, &factor_b, NULL));

		CATCH_MP_ERROR(mp_lcm(&a.denominator, &b.denominator, &lcm));

		CATCH_MP_ERROR(mp_div(&lcm, &a.denominator, &factor_a, NULL));
		CATCH_MP_ERROR(mp_div(&lcm, &b.denominator, &factor_b, NULL));

		// reuse factor variables instead of clearing and reallocating new mp_ints
		CATCH_MP_ERROR(mp_mul(&a.numerator, &factor_a, &factor_a));
		CATCH_MP_ERROR(mp_mul(&b.numerator, &factor_b, &factor_b));
		
		mp_ord res = mp_cmp_mag(&factor_a, &factor_b);

		mp_clear_multi(&lcm, &factor_a, &factor_b, NULL);

		return from_mp_ord(res);
	}
	else {
		if (rational_sign_negative(a_sign)) {
			// a is negative and b must be positive
			return SCHEME_LT;
		}
		else {
			// a is positive and b must be negative
			return SCHEME_GT;
		}
		// return based on sign
	}
}

scheme_boolean_t rational_eq(scheme_rational_t a, scheme_rational_t b) {
	if (rational_cmp(a, b) == SCHEME_EQ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t rational_lt(scheme_rational_t a, scheme_rational_t b) {
	if (rational_cmp(a, b) == SCHEME_LT) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t rational_gt(scheme_rational_t a, scheme_rational_t b) {
	if (rational_cmp(a, b) == SCHEME_GT) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t rational_lte(scheme_rational_t a, scheme_rational_t b) {
	scheme_ord_t ord = rational_cmp(a, b);
	if (ord == SCHEME_LT || ord == SCHEME_EQ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t rational_gte(scheme_rational_t a, scheme_rational_t b) {
	scheme_ord_t ord = rational_cmp(a, b);
	if (ord == SCHEME_GT || ord == SCHEME_EQ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_rational_t integer_to_rational(scheme_integer_t n) {
	mp_int denom, num;
	CATCH_MP_ERROR(mp_init_copy(&num, &n));
	CATCH_MP_ERROR(mp_init_u32(&denom, 1));
	return rational_create(denom, n);
}

dyntype_t rational_to_string(scheme_rational_t a) {
    char n_str[10001];
    char d_str[10001];
    char str[20003];

    CATCH_MP_ERROR(mp_to_radix(&a.numerator, n_str, 10000, NULL, 10));
    CATCH_MP_ERROR(mp_to_radix(&a.denominator, d_str, 10000, NULL, 10));

    strcat(str, n_str);
    strcat(str, "/");
    strcat(str, d_str);

    return scheme_new_string(str);
}

void rational_release(scheme_rational_t x) {
	mp_clear(&x.denominator);
	mp_clear(&x.numerator);
}

scheme_rational_t rational_copy(scheme_rational_t obj) {
	mp_int a_copied, b_copied;
	CATCH_MP_ERROR(mp_init_copy(&a_copied, &obj.numerator));
	CATCH_MP_ERROR(mp_init_copy(&b_copied, &obj.denominator));

	return rational_create(a_copied, b_copied);
}