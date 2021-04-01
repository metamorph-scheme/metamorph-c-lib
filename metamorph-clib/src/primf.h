#ifndef PRIMF_GLOB
#define PRIMF_GLOB

#include "dyntypes.h"
#include "functions.h"

#define ELLIPSIS_PARAM(NAME) dyntype_t* NAME, int len
#define NONSTANDARD
#define STANDARD(NAME)
#define CATCH_MP_ERROR(EXPR) if((EXPR) != MP_OKAY) { SET_TOMMATH_NUMBER_EXCEPTION } 

// Ports
BASE_FUNCTION(current_input_port);
BASE_FUNCTION(current_output_port);
BASE_FUNCTION(current_error_port);

BASE_FUNCTION(read_char);
BASE_FUNCTION(peek_char);
BASE_FUNCTION(read_line);
BASE_FUNCTION(read_string);

BASE_FUNCTION(newline);
BASE_FUNCTION(write_char);
BASE_FUNCTION(write_string);

BASE_FUNCTION(flush_output_port);

// Symbols

// symbol?
BASE_FUNCTION(symbol_q);
// symbol=?
BASE_FUNCTION(symbol_eq);
// symbol->string
BASE_FUNCTION(symbol_to_string);
// string->symbol
BASE_FUNCTION(string_to_symbol);

// Booleans

NONSTANDARD scheme_boolean_t obj_to_boolean(dyntype_t obj);
// not
BASE_FUNCTION(not);
// boolean?
BASE_FUNCTION(boolean_q);
// boolean=?
BASE_FUNCTION(boolean_eq);

// Pairs

// pair?
BASE_FUNCTION(pair_q);
// cons
BASE_FUNCTION(cons);
// car
BASE_FUNCTION(car);
// cdr
BASE_FUNCTION(cdr);
// set-car!
BASE_FUNCTION(set_car_ex);
// set-cdr!
BASE_FUNCTION(set_cdr_ex);

/*
(define caar
  (lambda (x)
    (car (car x))))
(define cadr
  (lambda (x)
    (car (cdr x))))
(define cdar
  (lambda (x)
    (cdr (car x))))
(define cddr
  (lambda (x)
    (cdr (cdr x))))
*/

// null?
BASE_FUNCTION(null_q);
// list?
BASE_FUNCTION(list_q);

// make-list
dyntype_t make_list(int c_k);
// make-list-fill
dyntype_t make_list_fill(int k, dyntype_t fill);

// list
BASE_FUNCTION(list);
dyntype_t i_list(ELLIPSIS_PARAM(obj));

// TODO number type
// length
BASE_FUNCTION(length);

// append
dyntype_t append(ELLIPSIS_PARAM(obj));

/*
(define reverse
  (let ((iterate
    (lambda (x acc)
      (if (null? x)
        acc
        (iterate (cdr x) (cons (car x) acc))))))
    (lambda (list)
      (if (list? list)
        (iterate list '())
        (error "reverse: argument is not a list"))))
*/

/*
(define list-tail
  (lambda (list k)
    (if (zero? k)
      list
      (list-tail (cdr list) (- k 1)))))
*/

/*
(define list-ref
  (lambda (list k)
    (car (list-tail list k ))))
*/

// list-set!
dyntype_t list_set(dyntype_t list, int k, dyntype_t obj);

/*
(define memq
  (lambda (obj list)
    (member obj list eq?)))
(define memv
  (lambda (obj list)
    (member obj list eqv?)))
(define member
  (lambda (obj . pair)
    (let ((len (length pair)))
      (cond
        ((= len 0)
          (error "member: no argument"))
        ((= len 1)
          (member-int obj (car pair) equal?))
        ((= len 2)
          (member-int obj (car pair) (cadr pair)))
        (else
          (error "member: too many arguments for member"))))))
(define member-int
  (lambda (obj list compare)
      (if (list? list)
        (mem-int obj list)
        (error "memq: argument is not a list"))))
(define mem-int
  (lambda (obj list compare)
    (cond
      ((null? list)
        #f)
      ((compare (car list) obj)
        list)
      (else
        (mem-int obj (cdr list))))))
*/

/*
(define assq
  (lambda (obj alist)
    (assoc obj alist eq?)))
(define assv
  (lambda (obj alist)
    (assoc obj alist eqv?)))
(define assoc
  (lambda (obj . pair)
    (let ((len (length pair)))
      (cond
        ((= len 0)
          (error "assoc: no argument"))
        ((= len 1)
          (assoc-int obj (car pair) equal?))
        ((= len 2)
          (assoc-int obj (car pair) (cadr pair)))
        (else
          (error "assoc: too many arguments for member"))))))
(define assoc-int
  (lambda (obj alist compare)
      (if (list? alist)
        (ass-int obj alist compare)
        (error "assoc: argument is not a list"))))
(define ass-int
  (lambda (obj alist compare)
    (cond
      ((null? alist)
        #f)
      ((compare (caar alist) obj)
        (car alist))
      (else
        (ass-int obj (cdr alist) compare)))))
*/

// list-copy
dyntype_t list_copy(dyntype_t list);

// Numbers

BASE_FUNCTION(number_q);

BASE_FUNCTION(complex_q);
BASE_FUNCTION(real_q);
BASE_FUNCTION(rational_q);
BASE_FUNCTION(integer_q);

BASE_FUNCTION(inexact_q);
BASE_FUNCTION(exact_q);

BASE_FUNCTION(number_to_string);

BASE_FUNCTION(num_eq);
BASE_FUNCTION(num_lt);
BASE_FUNCTION(num_gt);
BASE_FUNCTION(num_gte);
BASE_FUNCTION(num_lte);

BASE_FUNCTION(add);
BASE_FUNCTION(sub);
BASE_FUNCTION(mul);
BASE_FUNCTION(scheme_div);

BASE_FUNCTION(zero_q);

/*
(define exact-integer?
    (lambda (z) (and (integer? z) (exact? z))))
*/

// the following internal number operations only build an API for usage by the standard-library functions in numbers.c
// functions in numbers.c should only use these internal functions and no tommathlib functions or low-level struct initialization at all.

// this API should also not differentiate between inexact or exact variants

// to string functions return dyntypes for the sake of simpilicity in memory management
// it makes clear that the function that uses the function has to release the dyntype

// Metamorph Number API

scheme_integer_t integer_add(scheme_integer_t a, scheme_integer_t b);
scheme_integer_t integer_sub(scheme_integer_t a, scheme_integer_t b);
scheme_integer_t integer_mul(scheme_integer_t a, scheme_integer_t b);
scheme_integer_t integer_truncate_div(scheme_integer_t a, scheme_integer_t b);
scheme_integer_t integer_truncate_remainder(scheme_integer_t a, scheme_integer_t b);
scheme_integer_t integer_truncate_quotient(scheme_integer_t a, scheme_integer_t b);

scheme_integer_t integer_lcm(scheme_integer_t* n, size_t len);
scheme_integer_t integer_lcm(scheme_integer_t* n, size_t len);

scheme_integer_t integer_neg(scheme_integer_t a);
scheme_integer_t integer_create(char* bytes, size_t len);

scheme_ord_t integer_cmp(scheme_integer_t a, scheme_integer_t b);
scheme_boolean_t integer_eq(scheme_integer_t a, scheme_integer_t b);
scheme_boolean_t integer_lt(scheme_integer_t a, scheme_integer_t b);
scheme_boolean_t integer_gt(scheme_integer_t a, scheme_integer_t b);
scheme_boolean_t integer_gte(scheme_integer_t a, scheme_integer_t b);
scheme_boolean_t integer_lte(scheme_integer_t a, scheme_integer_t b);

dyntype_t integer_to_string(scheme_integer_t a);
int integer_to_s32int(scheme_integer_t a);
scheme_integer_t integer_create_s32(int a);

void integer_release(scheme_integer_t);
scheme_integer_t integer_copy(scheme_integer_t obj);

scheme_real_t real_add(scheme_real_t a, scheme_real_t b);
scheme_real_t real_sub(scheme_real_t a, scheme_real_t b);
scheme_real_t real_mul(scheme_real_t a, scheme_real_t b);
scheme_real_t real_div(scheme_real_t a, scheme_real_t b);
scheme_real_t real_neg(scheme_real_t a);
scheme_rational_t real_to_rational(scheme_real_t a);
scheme_real_t rational_to_real(scheme_rational_t a);
scheme_real_t real_create(double fp);

scheme_ord_t real_cmp(scheme_real_t a, scheme_real_t b);
scheme_boolean_t real_eq(scheme_real_t a, scheme_real_t b);
scheme_boolean_t real_lt(scheme_real_t a, scheme_real_t b);
scheme_boolean_t real_lte(scheme_real_t a, scheme_real_t b);
scheme_boolean_t real_gt(scheme_real_t a, scheme_real_t b);
scheme_boolean_t real_gte(scheme_real_t a, scheme_real_t b);

dyntype_t real_to_string(scheme_real_t a);

void rational_min(scheme_rational_t * q);
scheme_rational_sign_t rational_sign(scheme_rational_t a);
scheme_boolean_t rational_sign_negative(scheme_rational_sign_t sign);
scheme_rational_t rational_add(scheme_rational_t a, scheme_rational_t b);
scheme_rational_t rational_sub(scheme_rational_t a, scheme_rational_t b);
scheme_rational_t rational_mul(scheme_rational_t a, scheme_rational_t b);
scheme_rational_t rational_div(scheme_rational_t a, scheme_rational_t b);
scheme_rational_t rational_neg(scheme_rational_t a);

scheme_rational_t rational_create(scheme_integer_t a, scheme_integer_t b);
scheme_rational_t rational_create_raw(char* a_bytes, size_t alen, char* b_bytes, size_t blen);

scheme_integer_t rational_numerator(scheme_rational_t x);
scheme_integer_t rational_denominator(scheme_rational_t x);

scheme_ord_t rational_cmp(scheme_rational_t a, scheme_rational_t b);
scheme_boolean_t rational_eq(scheme_rational_t a, scheme_rational_t b);
scheme_boolean_t rational_lt(scheme_rational_t a, scheme_rational_t b);
scheme_boolean_t rational_lte(scheme_rational_t a, scheme_rational_t b);
scheme_boolean_t rational_gt(scheme_rational_t a, scheme_rational_t b);
scheme_boolean_t rational_gte(scheme_rational_t a, scheme_rational_t b);

dyntype_t rational_to_string(scheme_rational_t a);

scheme_rational_t integer_to_rational(scheme_integer_t n);

void rational_release(scheme_rational_t x);
scheme_rational_t rational_copy(scheme_rational_t obj);

// End Metamorph Number API

/*
(define quotient truncate-quotient)
(define remainder truncate-remainder)
(define modulo floor-remainder)
*/

void release_pair(scheme_pair_t pair);

dyntype_t copy_pair(dyntype_t* pair);

void release_number(scheme_number_t number);
dyntype_t copy_number(scheme_number_t number);

#endif