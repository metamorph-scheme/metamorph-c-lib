#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf_internal.h"
#include "../cache.h"
#include "../functions.h"


// Arbitrary lenght integer implementation
// based on uint64_t

scheme_integer_t from_int_array(uint64_t ints[], uint8_t length) {
	uint64_t* ptr = REQUEST_ARRAY(uint64_t, length);
	for (int i = 0; i < length; i++) {
		ptr[i] = ints[i];
	}
	return (scheme_integer_t) { .block = ptr, .length = length };
}

scheme_integer_t from_unsigned_int64(uint64_t int_) {
	// const size_t BITS = 8 * sizeof(signedInt); = 32

	uint64_t *a = REQUEST_ARRAY(uint64_t, 1);
	a[0] = int_;

	scheme_integer_t new_int = (scheme_integer_t){
		.length = 1,
		.block = a
	};

	return new_int;
}

uint64_t half_add(uint64_t a, uint64_t b, bool_t *carry_out) {
	uint64_t res = a + b;
	*carry_out = (res < a) ? TRUE : FALSE;
	return res;
}

uint64_t full_add(uint64_t a, uint64_t b, bool_t carry_in, bool_t* carry_out) {
	bool_t carry_out_first_half_add = FALSE;
	uint64_t sum = half_add(a, b, &carry_out_first_half_add);
	bool_t carry_out_second_half_add = FALSE;
	uint64_t res = half_add(sum, carry_in, &carry_out_second_half_add);
	*carry_out = (carry_out_first_half_add || carry_out_second_half_add);
	return res;
}

scheme_integer_t i_integer_plus(scheme_integer_t a, scheme_integer_t b) {
	// b is longer

	uint64_t msb_check = 1ull << (sizeof(uint64_t) * 8 - 1);

	bool_t a_msb = (a.block[0] & msb_check) == 0u ? FALSE : TRUE;
	bool_t b_msb = (b.block[0] & msb_check) == 0u ? FALSE : TRUE;
	uint64_t a_fill = a_msb ? 0xFFFFFFFFFFFFFFFF : 0ull;

	bool_t carry_out;
	bool_t carry_in = FALSE;
	uint64_t *ptr = REQUEST_ARRAY(uint64_t, b.length + 1);
	for (int i = 1; i <= a.length; i++) {
		ptr[b.length - i + 1] = full_add(a.block[a.length - i], b.block[b.length - i], carry_in, &carry_out);
		carry_in = carry_out;
	}
	for (int i = a.length + 1; i <= b.length; i++) {
		ptr[b.length - i + 1] = full_add(a_fill, b.block[b.length - i], carry_in, &carry_out);
		carry_in = carry_out;
	}

	bool_t ptr_msb = (ptr[1] & msb_check) == 0 ? FALSE : TRUE;

	if (a_msb == b_msb && ptr_msb != a_msb) {
		// overflow
		// use carry
		ptr[0] = carry_out ? 0xFFFFFFFFFFFFFFFF : 0ull;
	}
	else {
		// ignore carry
		ptr[0] = ptr_msb ? 0xFFFFFFFFFFFFFFFF : 0ull;
	}
	return (scheme_integer_t) {
		.block = ptr,
		.length = b.length + 1
	};
}

scheme_integer_t integer_plus(scheme_integer_t a, scheme_integer_t b) {
	if (a.length > b.length) {
		// a is longer
		return i_integer_plus(b, a);
	}
	else {
		// b is longer
		return i_integer_plus(a, b);
	}
}

scheme_integer_t integer_negate(scheme_integer_t n) {
	uint64_t* ptr = REQUEST_ARRAY(uint64_t, n.length);
	for (int i = 0; i < n.length; i++) {
		ptr[i] = ~n.block[i];
	}
	return integer_plus((scheme_integer_t) { .block = ptr, .length = n.length }, from_unsigned_int64(1));
}

scheme_integer_t integer_subtract(scheme_integer_t a, scheme_integer_t b) {
	return integer_plus(a, integer_negate(b));
}