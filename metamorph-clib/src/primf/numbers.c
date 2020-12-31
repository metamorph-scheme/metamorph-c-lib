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

bool_t i_number_q(dyntype_t obj) {
	return (obj.type == SCHEME_TYPE_NUMBER);
}

dyntype_t number_q(dyntype_t obj) {
	return scheme_new_boolean(i_number_q(obj));
}

dyntype_t complex_q(dyntype_t obj) {
	return number_q(obj);
}

bool_t i_real_q(dyntype_t obj) {
	if (!i_number_q(obj)) {
		return FALSE;
	}

	if (i_integer_q(obj)) {
		return TRUE;
	}
	if (i_rational_q(obj)) {
		return TRUE;
	}

	scheme_numerical_type_t number_type = obj.data.number_val->type;
	return (number_type == SCHEME_NUMERICAL_TYPE_INEXACT_REAL);
}

dyntype_t real_q(dyntype_t obj) {
	return scheme_new_boolean(i_real_q(obj));
}

bool_t i_rational_q(dyntype_t obj) {
	if (!i_number_q(obj)) {
		return FALSE;
	}

	if (i_integer_q(obj)) {
		return TRUE;
	}

	scheme_numerical_type_t number_type = obj.data.number_val->type;
	return (number_type == SCHEME_NUMERICAL_TYPE_EXACT_RATIONAL
		|| number_type == SCHEME_NUMERICAL_TYPE_INEXACT_RATIONAL);

    // TODO true for inexact reals that can be represented by rationals (rational? 3.5) => #t
}

dyntype_t rational_q(dyntype_t obj) {
	return scheme_new_boolean(i_rational_q(obj));
}

bool_t i_integer_q(dyntype_t obj) {
	if (!i_number_q(obj)) {
		return FALSE;
	}

	scheme_numerical_type_t number_type = obj.data.number_val->type;
	return (number_type == SCHEME_NUMERICAL_TYPE_EXACT_INTEGER
		|| number_type == SCHEME_NUMERICAL_TYPE_INEXACT_INTEGER);

	// TODO true for every inexact real x where (round x) is equal to x
}

dyntype_t integer_q(dyntype_t obj) {
	return scheme_new_boolean(i_integer_q(obj));
}

bool_t i_inexact_q(dyntype_t z) {
	REQUIRE_SCHEME_NUMBER(z, 0);
	
	scheme_numerical_type_t number_type = c_z.type;
	return (number_type == SCHEME_NUMERICAL_TYPE_INEXACT_INTEGER
		|| number_type == SCHEME_NUMERICAL_TYPE_INEXACT_RATIONAL
		|| number_type == SCHEME_NUMERICAL_TYPE_INEXACT_REAL);
}

dyntype_t inexact_q(dyntype_t z) {
	return scheme_new_boolean(i_inexact_q(z));
}

bool_t i_exact_q(dyntype_t z) {
	REQUIRE_SCHEME_NUMBER(z, 0);

	scheme_numerical_type_t number_type = c_z.type;
	return (number_type == SCHEME_NUMERICAL_TYPE_EXACT_INTEGER
		|| number_type == SCHEME_NUMERICAL_TYPE_EXACT_RATIONAL);
}

dyntype_t exact_q(dyntype_t z) {
	return scheme_new_boolean(i_exact_q(z));
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