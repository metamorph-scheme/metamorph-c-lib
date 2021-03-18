#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf_internal.h"
#include "../cache.h"
#include "../functions.h"
#include <math.h>
#include <float.h>
#include <limits.h>
#include <string.h>

// TODO: think about this
#define BITS_SUB_ZERO 1024

mp_bool s_mp_get_bit(const mp_int* a, unsigned int b);
// produces zero bits when index is negative - adapted/copied from the deprecated mp_get_bit
mp_bool cust_mp_get_bit(const mp_int* a, unsigned int b) {
	if (b < 0) return MP_NO;

	return (s_mp_get_bit(a, (unsigned int)b) == MP_YES) ? MP_YES : MP_NO;
}

scheme_real_t real_add(scheme_real_t a, scheme_real_t b) {
	double res = a + b;
	return res;
}

scheme_real_t real_sub(scheme_real_t a, scheme_real_t b) {
	double res = a - b;
	return res;
}

scheme_real_t real_mul(scheme_real_t a, scheme_real_t b) {
	double res = a * b;
	return res;
}

scheme_real_t real_div(scheme_real_t a, scheme_real_t b) {
	double res = a / b;
	return res;
}

scheme_real_t real_neg(scheme_real_t a) {
	return -a;
}

// TODO examine NAN and infty
scheme_rational_t real_to_rational(scheme_real_t real) {

	//double in = 20031.4;
	//int exponent;

	//double mantissa = frexp(in, &exponent);
	//exponent--;

	//int sign;

	//if (sign = signbit(mantissa)) {
	//	mantissa = -mantissa;
	//}

	//unsigned long realm = mantissa * pow(FLT_RADIX, DBL_MANT_DIG);

	//unsigned long a = 1L;
	//unsigned long b = 1L;

	//for (int i = DBL_MANT_DIG-2; i>=0; i--) {
	//for (int i = 51; i >= 0; i--) {
	//	int bit = ((realm >> i) & 1);
	//	printf("%d", bit);
	//	a = a * 2u + ((realm >> i) & 1);
	//	b *= 2u;
	//}


	//if (exponent > 0)
	//	a *= 1u << exponent;
	//else
	//	b *= 1u << -exponent;

	////if (sign == 1)
	////	a *= -1;

	//printf("\nThe fraction is %lf, realm is %lu and the exponent is %u, a: %lu, b: %lu \n", mantissa, realm, exponent, a, b);
	//printf("\ndivided gives %lf instead of in %lf \n", (a / (double)b), in);


	// this program (converted decimal)
	// 10000011001100110011001100110011001100110011001100110

	// from converter
	// 0000011001100110011001100110011001100110011001100110

	// 0001101110011111000001000011010001111110101001111010
	// 0001101110011111000001000011010001111110101001111010

	int exponent;
	double mantissa = frexp(real, &exponent);

	// compensates DBL_MANT_DIG without -1 in line 106
	exponent--;

	int sign = mantissa < 0;

	// when negative negate mantissa
	if (sign) {
		mantissa = -mantissa;
	}

	// essentially left shift 53 bits
	// result has to be a natural number and can safely be stored in uint64
	// cannot overflow int64 because mantissa is always just 52 bit long and
	// the exponent plays no role here
	uint64_t realm = mantissa * pow(FLT_RADIX, DBL_MANT_DIG);

	mp_int a, b, bit;
	CATCH_MP_ERROR(mp_init_multi(&a, &b, &bit, NULL));

	mp_set_u32(&a, 1);
	mp_set_u32(&b, 1);

	// for (int i = 51; i >= 0; i--) {
	for (int i = DBL_MANT_DIG-2; i>=0; i--) {
		//a = a * 2u + ((realm >> i) & 1);
		mp_set_u32(&bit, ((realm >> i) & 1));

		CATCH_MP_ERROR(mp_mul_2(&a, &a));
		CATCH_MP_ERROR(mp_add(&a, &bit, &a));

		CATCH_MP_ERROR(mp_mul_2(&b, &b));
	}


	if (exponent > 0) {
		// a *= 1u << exponent;
		CATCH_MP_ERROR(mp_mul_2d(&a, exponent, &a));
	}
	else {
		// b *= 1u << -exponent;
		// b = b * 2^(-exponent)
		CATCH_MP_ERROR(mp_mul_2d(&b, -exponent, &b));
	}

	if (sign == 1) {
		CATCH_MP_ERROR(mp_neg(&a, &a));
	}

	scheme_rational_t res = {
		.numerator = a,
		.denominator = b
	};

	rational_min(&res);
	mp_clear(&bit);
	return res;
}

// TODO examine NAN and infty
scheme_real_t rational_to_real(scheme_rational_t a) {

	mp_int numerator;
	mp_int* denominator;
	mp_int denominator_val; // only used as copy location if denumerator is negative

	// copy original numerator
	CATCH_MP_ERROR(mp_init_copy(&numerator, &a.numerator));

	scheme_rational_sign_t sign = rational_sign(a);
	if (sign == SCHEME_RATIONAL_B_NEG || sign == SCHEME_RATIONAL_N_NEG) {
		CATCH_MP_ERROR(mp_neg(&numerator, &numerator));
	}

	// uses pointer that simplifies the following code
	// when denominator is positive, memory copy is not necessary 
	// if it is negative, memory copy and negating must be done
	// pointer denominator points to either (guaranteed positive) number.
	if (sign == SCHEME_RATIONAL_B_NEG || sign == SCHEME_RATIONAL_D_NEG) {
		CATCH_MP_ERROR(mp_init_copy(&denominator_val, &a.denominator));
		CATCH_MP_ERROR(mp_neg(&denominator_val, &denominator_val));
		denominator = &denominator_val;
	}
	else
	{
		denominator = &a.denominator;
	}

	// fast shift using mul_2d in tommath
	// essentially	numerator <<= DBL_MANT_DIG-1;
	CATCH_MP_ERROR(mp_mul_2d(&numerator, BITS_SUB_ZERO - 1, &numerator));

	// reuse numerator for integer division result
	// unsigned long significand = a / b;
	// the result carries 53 bits of "Nachkommastellen" in the end because of the shift above
	CATCH_MP_ERROR(mp_div(&numerator, denominator, &numerator, NULL));
	
	int msb = mp_count_bits(&numerator);
	if (msb > DBL_MAX_EXP) {
		// number is too big
		SET_TOMMATH_NUMBER_EXCEPTION
	}

	// zero-indexed position of MSB
	int msb_pos = msb - 1;

	// construct uint64_t with the MSB and the following 51 bits
	// this is truncating the rest of the number
	// TODO examine IEEE 754 rounding/not rounding and effects
	uint64_t mantissa = 0;
	for (int i = msb_pos; i >= msb_pos - (DBL_MANT_DIG-1); i--) {

		mantissa <<= 1;
		// set first (most right) bit of mantissa
		uint64_t bit = ((uint64_t)cust_mp_get_bit(&numerator, i));

		// if numerator is "exhausted" (msb < 52) mantissa is still shifted and bit is always 0
		// this is done to have a constant -52 exponent
		mantissa = mantissa | bit;
	}

	// exponent consists of comma shifting "forward/left" just before the MSB and then shifting
	// "back/right" to compensate the initial left shift of the numerator
	int exp = (msb_pos - (DBL_MANT_DIG - 1)) - (BITS_SUB_ZERO - 1);

	// converting to double - this has nothing to do with bit patterns.
	// We let the implementation do the dirty work of integer to double conversion.
	// TODO examine edge case ramifications of this
	double d_significand = mantissa;
	double result = ldexp(d_significand, exp);

	if (rational_sign_negative(sign))
		return -result;
	else
		return result;
}

scheme_real_t real_create(double fp) {
	return fp;
}

scheme_ord_t real_cmp(scheme_real_t a, scheme_real_t b) {
	if (a == b) {
		return SCHEME_EQ;
	} else if (a < b) {
		return SCHEME_LT;
	} else {
		return SCHEME_GT;
	}
}

scheme_boolean_t real_eq(scheme_real_t a, scheme_real_t b) {
	if (a == b) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t real_lt(scheme_real_t a, scheme_real_t b) {
	if (a < b) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t real_lte(scheme_real_t a, scheme_real_t b) {
	if (a <= b) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t real_gt(scheme_real_t a, scheme_real_t b) {
	if (a > b) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

scheme_boolean_t real_gte(scheme_real_t a, scheme_real_t b) {
	if (a > b) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

dyntype_t real_to_string(scheme_real_t a) {
    char out[1000];
    sprintf(out, "%.5lf", a);
    return scheme_new_string(out);
}
