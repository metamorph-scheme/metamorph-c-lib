#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf_internal.h"
#include "../cache.h"
#include "../functions.h"
#include "../tommath/tommath.h"

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

// TODO: multiple return values, derive floor division from mp_div result
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