#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf_internal.h"
#include "../functions.h"

/*

Metamorph Number Implementation:

Limiting the real range limits both integer and rational range, as every integer must be a real but not every real must be an integer

exact: 
	integer: practically unbounded
	rational: practically unbounded
	real: not supported
		when encountering an exact real, a compile-time is produced (see a) )
	complex: not supported
		when encountering a complex number, a compile-time error must be produced

#i3 => 3.0

inexact:
	integer: use inexact real internally and only annotate as integer
	rational: use inexact real internally and only annotate as rational
	real: IEEE 754 double floating point
	complex: not supported
		when encountering a complex number, a compile-time error must be produced

Quotes from R7RS Standard:
	a)
	If an implementation encounters an exact numerical constant that it cannot
	represent as an exact number, then it may either report a violation of an
	implementation restriction or it may silently represent the constant by an inexact number.
*/

/*
Backend:

inexact integer => create exact integer and then use rational_to_real

*/

bool_t i_number_q(dyntype_t obj) {
	return (obj.type == SCHEME_TYPE_NUMBER);
}

dyntype_t number_q(dyntype_t obj) {
	return scheme_new_boolean(i_number_q(obj));
}

dyntype_t complex_q(dyntype_t obj) {
	return number_q(obj);
}

bool_t i_real_q(scheme_number_t obj) {

	if (i_integer_q(obj)) {
		return TRUE;
	}
	if (i_rational_q(obj)) {
		return TRUE;
	}

	scheme_numerical_type_t number_type = obj.type;
	return (number_type == SCHEME_NUMERICAL_TYPE_INEXACT_REAL);
}

dyntype_t real_q(dyntype_t obj) {
	if (!i_number_q(obj)) {
		return scheme_new_boolean(FALSE);
	}

	return scheme_new_boolean(i_real_q(*obj.data.number_val));
}

bool_t i_rational_q(scheme_number_t obj) {

	if (i_integer_q(obj)) {
		return TRUE;
	}

	scheme_numerical_type_t number_type = obj.type;
	return (number_type == SCHEME_NUMERICAL_TYPE_EXACT_RATIONAL
		|| number_type == SCHEME_NUMERICAL_TYPE_INEXACT_RATIONAL);

    // TODO true for inexact reals that can be represented by rationals (rational? 3.5) => #t
}

dyntype_t rational_q(dyntype_t obj) {
	if (!i_number_q(obj)) {
		return scheme_new_boolean(FALSE);
	}

	return scheme_new_boolean(i_rational_q(*obj.data.number_val));
}

bool_t i_integer_q(scheme_number_t obj) {

	scheme_numerical_type_t number_type = obj.type;
	return (number_type == SCHEME_NUMERICAL_TYPE_EXACT_INTEGER
		|| number_type == SCHEME_NUMERICAL_TYPE_INEXACT_INTEGER);

	// TODO true for every inexact real x where (round x) is equal to x
}

dyntype_t integer_q(dyntype_t obj) {
	if (!i_number_q(obj)) {
		return scheme_new_boolean(FALSE);
	}

	return scheme_new_boolean(i_integer_q(*obj.data.number_val));
}

bool_t i_inexact_q(scheme_number_t z) {	
	scheme_numerical_type_t number_type = z.type;
	return (number_type == SCHEME_NUMERICAL_TYPE_INEXACT_INTEGER
		|| number_type == SCHEME_NUMERICAL_TYPE_INEXACT_RATIONAL
		|| number_type == SCHEME_NUMERICAL_TYPE_INEXACT_REAL);
}

dyntype_t inexact_q(dyntype_t z) {
	REQUIRE_SCHEME_NUMBER(z, 0);
	return scheme_new_boolean(i_inexact_q(c_z));
}

bool_t i_exact_q(scheme_number_t z) {
	scheme_numerical_type_t number_type = z.type;
	return (number_type == SCHEME_NUMERICAL_TYPE_EXACT_INTEGER
		|| number_type == SCHEME_NUMERICAL_TYPE_EXACT_RATIONAL);
}

dyntype_t exact_q(dyntype_t z) {
	REQUIRE_SCHEME_NUMBER(z, 0);

	return scheme_new_boolean(i_exact_q(c_z));
}

scheme_ord_t i_num_cmp(scheme_number_t a, scheme_number_t b) {

	if (i_inexact_q(a) && i_inexact_q(b)) {
		// only inexact => inexact real comparison

		return real_cmp(*a.data.inexact_real_val, *b.data.inexact_real_val);
	} else if (i_exact_q(a) && i_exact_q(b)) {
		// both exact
		if (i_integer_q(a) && i_inexact_q(b)) {
			// both exact integer
			return integer_cmp(*a.data.exact_integer_val, *b.data.exact_integer_val);
		}
		else if (i_integer_q(a)) {
			// b is the rational
			return rational_cmp(integer_to_rational(*a.data.exact_integer_val), *b.data.exact_rational_val);
		}
		else if (i_integer_q(b)) {
			// a is the rational
			return rational_cmp(integer_to_rational(*b.data.exact_integer_val), *a.data.exact_rational_val);
		}
		else {
			// both are rational
			return rational_cmp(*a.data.exact_rational_val, *b.data.exact_rational_val);
		}
	}
	else {
		// one is inexact and has to be converted to exact
		scheme_rational_t cnv_exact;
		if (i_inexact_q(a)) {
			// a is inexact
			cnv_exact = real_to_rational(*a.data.inexact_real_val);
			if (i_integer_q(b)) {
				// b exact integer
				return rational_cmp(cnv_exact, integer_to_rational(*b.data.exact_integer_val));
			}
			else
			{
				// b exact rational
				return rational_cmp(cnv_exact, *b.data.exact_rational_val);
			}
		}
		else {
			// b is inexact
			cnv_exact = real_to_rational(*b.data.inexact_real_val);
			if (i_integer_q(a)) {
				// a exact integer
				return rational_cmp(cnv_exact, integer_to_rational(*a.data.exact_integer_val));
			}
			else
			{
				// a exact rational
				return rational_cmp(cnv_exact, *a.data.exact_rational_val);
			}
		}
	}
}

dyntype_t i_generic_num_cmp(scheme_ord_t ord, ELLIPSIS_PARAM(x)) {
	if (len == 1) {
		return scheme_new_boolean(TRUE);
	}

	for (int i = 1; i < len; i++) {
		dyntype_t prev = x[i-1];
		dyntype_t next = x[i];
		REQUIRE_SCHEME_NUMBER(prev, i - 1);
		REQUIRE_SCHEME_NUMBER(next, i);

		scheme_ord_t cmp_res = i_num_cmp(c_prev, c_next);

		if (cmp_res != ord) {
			return scheme_new_boolean(FALSE);
		}
	}
	return scheme_new_boolean(TRUE);
}

dyntype_t i_generic_num_cmp_or_equal(scheme_ord_t ord, ELLIPSIS_PARAM(x)) {
	if (len == 1) {
		return scheme_new_boolean(TRUE);
	}

	for (int i = 1; i < len; i++) {
		dyntype_t prev = x[i - 1];
		dyntype_t next = x[i];
		REQUIRE_SCHEME_NUMBER(prev, i - 1);
		REQUIRE_SCHEME_NUMBER(next, i);

		scheme_ord_t cmp_res = i_num_cmp(c_prev, c_next);

		if (cmp_res != SCHEME_EQ && cmp_res != ord) {
			return scheme_new_boolean(FALSE);
		}
	}
	return scheme_new_boolean(TRUE);
}

dyntype_t num_eq(ELLIPSIS_PARAM(x)) {
	return i_generic_num_cmp(SCHEME_EQ, x, len);
}

dyntype_t num_lt(ELLIPSIS_PARAM(x)) {
	return i_generic_num_cmp(SCHEME_LT, x, len);
}

dyntype_t num_gt(ELLIPSIS_PARAM(x)) {
	return i_generic_num_cmp(SCHEME_GT, x, len);
}

dyntype_t num_gte(ELLIPSIS_PARAM(x)) {
	return i_generic_num_cmp_or_equal(SCHEME_GT, x, len);
}

dyntype_t num_lte(ELLIPSIS_PARAM(x)) {
	return i_generic_num_cmp_or_equal(SCHEME_LT, x, len);
}

scheme_number_t i_add(scheme_number_t a, scheme_number_t b) {
	if (i_inexact_q(a) && i_inexact_q(b)) {
		// only inexact => inexact real comparison

		return scheme_inexact_real(real_add(*a.data.inexact_real_val, *b.data.inexact_real_val));
	}
	else if (i_exact_q(a) && i_exact_q(b)) {
		// both exact
		if (i_integer_q(a) && i_inexact_q(b)) {
			// both exact integer
			return scheme_exact_integer(integer_add(*a.data.exact_integer_val, *b.data.exact_integer_val));
		}
		else if (i_integer_q(a)) {
			// b is the rational
			return scheme_exact_rational(rational_add(integer_to_rational(*a.data.exact_integer_val), *b.data.exact_rational_val));
		}
		else if (i_integer_q(b)) {
			// a is the rational
			return scheme_exact_rational(rational_add(integer_to_rational(*b.data.exact_integer_val), *a.data.exact_rational_val));
		}
		else {
			// both are rational
			return scheme_exact_rational(rational_add(*a.data.exact_rational_val, *b.data.exact_rational_val));
		}
	}
	else {
		// one is inexact and has to be converted to exact
		scheme_rational_t cnv_exact;
		if (i_inexact_q(a)) {
			// a is inexact
			cnv_exact = real_to_rational(*a.data.inexact_real_val);
			if (i_integer_q(b)) {
				// b exact integer
				return scheme_inexact_real(rational_to_real(rational_add(cnv_exact, integer_to_rational(*b.data.exact_integer_val))));
			}
			else
			{
				// b exact rational
				return scheme_inexact_real(rational_to_real(rational_add(cnv_exact, *b.data.exact_rational_val)));
			}
		}
		else {
			// b is inexact
			cnv_exact = real_to_rational(*b.data.inexact_real_val);
			if (i_integer_q(a)) {
				// a exact integer
				return scheme_inexact_real(rational_to_real(rational_add(cnv_exact, integer_to_rational(*a.data.exact_integer_val))));
			}
			else
			{
				// a exact rational
				return scheme_inexact_real(rational_to_real(rational_add(cnv_exact, *a.data.exact_rational_val)));
			}
		}
	}
}

dyntype_t add(ELLIPSIS_PARAM(x)) {
	if (len == 1) {
		return x[0];
	}

	dyntype_t f = x[0];
	dyntype_t s = x[1];

	REQUIRE_SCHEME_NUMBER(f, 0);
	REQUIRE_SCHEME_NUMBER(s, 1);
	scheme_number_t acc = i_add(c_f, c_s);

	for (int i = 2; i < len; i++) {
		dyntype_t elem = x[i];
		REQUIRE_SCHEME_NUMBER(elem, i);

		scheme_number_t res = i_add(acc, c_elem);
		release_number(acc);
		acc = res;
	}
	return scheme_new_number(acc);
}


scheme_number_t i_mul(scheme_number_t a, scheme_number_t b) {
	if (i_inexact_q(a) && i_inexact_q(b)) {
		// only inexact => inexact real comparison

		return scheme_inexact_real(real_mul(*a.data.inexact_real_val, *b.data.inexact_real_val));
	}
	else if (i_exact_q(a) && i_exact_q(b)) {
		// both exact
		if (i_integer_q(a) && i_inexact_q(b)) {
			// both exact integer
			return scheme_exact_integer(integer_mul(*a.data.exact_integer_val, *b.data.exact_integer_val));
		}
		else if (i_integer_q(a)) {
			// b is the rational
			return scheme_exact_rational(rational_mul(integer_to_rational(*a.data.exact_integer_val), *b.data.exact_rational_val));
		}
		else if (i_integer_q(b)) {
			// a is the rational
			return scheme_exact_rational(rational_mul(integer_to_rational(*b.data.exact_integer_val), *a.data.exact_rational_val));
		}
		else {
			// both are rational
			return scheme_exact_rational(rational_mul(*a.data.exact_rational_val, *b.data.exact_rational_val));
		}
	}
	else {
		// one is inexact and has to be converted to exact
		scheme_rational_t cnv_exact;
		if (i_inexact_q(a)) {
			// a is inexact
			cnv_exact = real_to_rational(*a.data.inexact_real_val);
			if (i_integer_q(b)) {
				// b exact integer
				return scheme_inexact_real(rational_to_real(rational_mul(cnv_exact, integer_to_rational(*b.data.exact_integer_val))));
			}
			else
			{
				// b exact rational
				return scheme_inexact_real(rational_to_real(rational_mul(cnv_exact, *b.data.exact_rational_val)));
			}
		}
		else {
			// b is inexact
			cnv_exact = real_to_rational(*b.data.inexact_real_val);
			if (i_integer_q(a)) {
				// a exact integer
				return scheme_inexact_real(rational_to_real(rational_mul(cnv_exact, integer_to_rational(*a.data.exact_integer_val))));
			}
			else
			{
				// a exact rational
				return scheme_inexact_real(rational_to_real(rational_mul(cnv_exact, *a.data.exact_rational_val)));
			}
		}
	}
}

dyntype_t mul(ELLIPSIS_PARAM(x)) {
	if (len == 1) {
		return x[0];
	}

	dyntype_t f = x[0];
	dyntype_t s = x[1];

	REQUIRE_SCHEME_NUMBER(f, 0);
	REQUIRE_SCHEME_NUMBER(s, 1);
	scheme_number_t acc = i_mul(c_f, c_s);

	for (int i = 2; i < len; i++) {
		dyntype_t elem = x[i];
		REQUIRE_SCHEME_NUMBER(elem, i);

		scheme_number_t res = i_mul(acc, c_elem);
		release_number(acc);
		acc = res;
	}
	return scheme_new_number(acc);
}

scheme_number_t i_sub(scheme_number_t a, scheme_number_t b) {
	if (i_inexact_q(a) && i_inexact_q(b)) {
		// only inexact => inexact real comparison

		return scheme_inexact_real(real_sub(*a.data.inexact_real_val, *b.data.inexact_real_val));
	}
	else if (i_exact_q(a) && i_exact_q(b)) {
		// both exact
		if (i_integer_q(a) && i_inexact_q(b)) {
			// both exact integer
			return scheme_exact_integer(integer_sub(*a.data.exact_integer_val, *b.data.exact_integer_val));
		}
		else if (i_integer_q(a)) {
			// b is the rational
			return scheme_exact_rational(rational_sub(integer_to_rational(*a.data.exact_integer_val), *b.data.exact_rational_val));
		}
		else if (i_integer_q(b)) {
			// a is the rational
			return scheme_exact_rational(rational_sub(integer_to_rational(*b.data.exact_integer_val), *a.data.exact_rational_val));
		}
		else {
			// both are rational
			return scheme_exact_rational(rational_sub(*a.data.exact_rational_val, *b.data.exact_rational_val));
		}
	}
	else {
		// one is inexact and has to be converted to exact
		scheme_rational_t cnv_exact;
		if (i_inexact_q(a)) {
			// a is inexact
			cnv_exact = real_to_rational(*a.data.inexact_real_val);
			if (i_integer_q(b)) {
				// b exact integer
				return scheme_inexact_real(rational_to_real(rational_sub(cnv_exact, integer_to_rational(*b.data.exact_integer_val))));
			}
			else
			{
				// b exact rational
				return scheme_inexact_real(rational_to_real(rational_sub(cnv_exact, *b.data.exact_rational_val)));
			}
		}
		else {
			// b is inexact
			cnv_exact = real_to_rational(*b.data.inexact_real_val);
			if (i_integer_q(a)) {
				// a exact integer
				return scheme_inexact_real(rational_to_real(rational_sub(cnv_exact, integer_to_rational(*a.data.exact_integer_val))));
			}
			else
			{
				// a exact rational
				return scheme_inexact_real(rational_to_real(rational_sub(cnv_exact, *a.data.exact_rational_val)));
			}
		}
	}
}

scheme_number_t i_neg(scheme_number_t x) {
	if (i_inexact_q(x)) {
		return scheme_inexact_real(real_neg(*x.data.inexact_real_val));
	}
	else {
		if (i_integer_q(x)) {
			return scheme_exact_integer(integer_neg(*x.data.exact_integer_val));
		}
		else
		{
			return scheme_exact_rational(rational_neg(*x.data.exact_rational_val));
		}
	}
}

dyntype_t sub(ELLIPSIS_PARAM(x)) {
	if (len == 1) {
		dyntype_t f = x[0];
		REQUIRE_SCHEME_NUMBER(f, 0);
		return scheme_new_number(i_neg(c_f));
	}

	dyntype_t f = x[0];
	dyntype_t s = x[1];

	REQUIRE_SCHEME_NUMBER(f, 0);
	REQUIRE_SCHEME_NUMBER(s, 1);
	scheme_number_t acc = i_mul(c_f, c_s);

	for (int i = 2; i < len; i++) {
		dyntype_t elem = x[i];
		REQUIRE_SCHEME_NUMBER(elem, i);

		scheme_number_t res = i_mul(acc, c_elem);
		release_number(acc);
		acc = res;
	}
	return scheme_new_number(acc);
}

bool_t i_zero_q(scheme_number_t x) {

	char constant_zero[] = { 0, 0b00000000 };
	scheme_number_t zero = scheme_exact_integer(integer_create(constant_zero, 2));

	bool_t res = (i_num_cmp(zero, x) == SCHEME_EQ);
	release_number(zero);
	return res;
}

dyntype_t zero_q(dyntype_t x) {
	REQUIRE_SCHEME_NUMBER(x, 0);

	return scheme_new_boolean(i_zero_q(c_x));
}

scheme_number_t i_div(scheme_number_t a, scheme_number_t b) {
	if (i_zero_q(b)) {
		SET_BAD_ARGUMENT_EXCEPTION(2, "Divisor must not be zero");
	}

	//TODO return exact zero when a is zero

	if (i_inexact_q(a) && i_inexact_q(b)) {
		// only inexact => inexact real comparison

		return scheme_inexact_real(real_div(*a.data.inexact_real_val, *b.data.inexact_real_val));
	}
	else if (i_exact_q(a) && i_exact_q(b)) {
		// both exact
		if (i_integer_q(a) && i_inexact_q(b)) {
			// both exact integer
			return scheme_exact_rational(rational_create(*a.data.exact_integer_val, *b.data.exact_integer_val));
		}
		else if (i_integer_q(a)) {
			// b is the rational
			return scheme_exact_rational(rational_div(integer_to_rational(*a.data.exact_integer_val), *b.data.exact_rational_val));
		}
		else if (i_integer_q(b)) {
			// a is the rational
			return scheme_exact_rational(rational_div(integer_to_rational(*b.data.exact_integer_val), *a.data.exact_rational_val));
		}
		else {
			// both are rational
			return scheme_exact_rational(rational_div(*a.data.exact_rational_val, *b.data.exact_rational_val));
		}
	}
	else {
		// one is inexact and has to be converted to exact
		scheme_rational_t cnv_exact;
		if (i_inexact_q(a)) {
			// a is inexact
			cnv_exact = real_to_rational(*a.data.inexact_real_val);
			if (i_integer_q(b)) {
				// b exact integer
				return scheme_inexact_real(rational_to_real(rational_div(cnv_exact, integer_to_rational(*b.data.exact_integer_val))));
			}
			else
			{
				// b exact rational
				return scheme_inexact_real(rational_to_real(rational_div(cnv_exact, *b.data.exact_rational_val)));
			}
		}
		else {
			// b is inexact
			cnv_exact = real_to_rational(*b.data.inexact_real_val);
			if (i_integer_q(a)) {
				// a exact integer
				return scheme_inexact_real(rational_to_real(rational_div(cnv_exact, integer_to_rational(*a.data.exact_integer_val))));
			}
			else
			{
				// a exact rational
				return scheme_inexact_real(rational_to_real(rational_div(cnv_exact, *a.data.exact_rational_val)));
			}
		}
	}
}

dyntype_t scheme_div(ELLIPSIS_PARAM(x)) {
	if (len == 1) {
		dyntype_t f = x[0];
		REQUIRE_SCHEME_NUMBER(f, 0);

		char constant_one[] = { 0, 0b00000001 };
		scheme_number_t one = scheme_exact_integer(integer_create(constant_one, 2));
		return scheme_new_number(i_div(one, c_f));
	}

	dyntype_t f = x[0];
	dyntype_t s = x[1];

	REQUIRE_SCHEME_NUMBER(f, 0);
	REQUIRE_SCHEME_NUMBER(s, 1);
	scheme_number_t acc = i_div(c_f, c_s);

	for (int i = 2; i < len; i++) {
		dyntype_t elem = x[i];
		REQUIRE_SCHEME_NUMBER(elem, i);

		scheme_number_t res = i_div(acc, c_elem);
		release_number(acc);
		acc = res;
	}
	return scheme_new_number(acc);
}

void release_number(scheme_number_t number) {
	char type = number.type;
	switch (type) {
		case SCHEME_NUMERICAL_TYPE_EXACT_INTEGER:
			integer_release(*number.data.exact_integer_val);
			RELEASE(scheme_integer_t, number.data.exact_integer_val)
			break;
		case SCHEME_NUMERICAL_TYPE_EXACT_RATIONAL:
			rational_release(*number.data.exact_rational_val);
			RELEASE(scheme_rational_t, number.data.exact_rational_val)
			break;
		default:
			RELEASE(scheme_real_t, number.data.inexact_real_val)
			break;
	}
}

dyntype_t copy_number(scheme_number_t number) {
	char type = number.type;
	switch (type) {
	case SCHEME_NUMERICAL_TYPE_EXACT_INTEGER:
		return scheme_new_number(scheme_exact_integer(integer_copy(*number.data.exact_integer_val)));
	case SCHEME_NUMERICAL_TYPE_EXACT_RATIONAL:
		return scheme_new_number(scheme_exact_rational(rational_copy(*number.data.exact_rational_val)));
	case SCHEME_NUMERICAL_TYPE_INEXACT_INTEGER:
		return scheme_new_number(scheme_inexact_integer(*number.data.inexact_real_val));
	case SCHEME_NUMERICAL_TYPE_INEXACT_RATIONAL:
		return scheme_new_number(scheme_inexact_rational(*number.data.inexact_real_val));
	case SCHEME_NUMERICAL_TYPE_INEXACT_REAL:
		return scheme_new_number(scheme_inexact_real(*number.data.inexact_real_val));
	}
}