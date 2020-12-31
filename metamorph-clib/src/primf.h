#ifndef PRIMF_GLOB
#define PRIMF_GLOB

#define ELLIPSIS_PARAM(NAME) dyntype_t* NAME, int len
#define NONSTANDARD
#define STANDARD(NAME)
#define CATCH_MP_ERROR(EXPR) if((EXPR) != MP_OKAY) { SET_TOMMATH_NUMBER_EXCEPTION } 

// Symbols

// symbol?
dyntype_t symbol_q(dyntype_t obj);
// symbol=?
dyntype_t symbol_eq(ELLIPSIS_PARAM(obj));
// symbol->string
dyntype_t symbol_to_string(dyntype_t symbol);
// string->symbol
dyntype_t string_to_symbol(dyntype_t string);

// Booleans

NONSTANDARD scheme_boolean_t obj_to_boolean(dyntype_t obj);
// not
dyntype_t not(dyntype_t obj);
// boolean?
dyntype_t boolean_q(dyntype_t obj);
// boolean=?
dyntype_t boolean_eq(ELLIPSIS_PARAM(boolean));

// Pairs

// pair?
dyntype_t pair_q(dyntype_t obj);
// cons
dyntype_t cons(dyntype_t obj1, dyntype_t obj2);
// car
dyntype_t car(dyntype_t pair);
// cdr
dyntype_t cdr(dyntype_t pair);
// set-car!
dyntype_t set_car_ex(dyntype_t pair, dyntype_t obj);
// set-cdr!
dyntype_t set_cdr_ex(dyntype_t pair, dyntype_t obj);

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
dyntype_t null_q(dyntype_t obj);
// list?
dyntype_t list_q(dyntype_t obj);

// make-list
dyntype_t make_list(int c_k);
// make-list-fill
dyntype_t make_list_fill(int k, dyntype_t fill);

// list
dyntype_t list(ELLIPSIS_PARAM(obj));

// TODO number type
// length
int length(dyntype_t list);

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


dyntype_t number_q(dyntype_t obj);

dyntype_t complex_q(dyntype_t obj);
dyntype_t real_q(dyntype_t obj);
dyntype_t rational_q(dyntype_t obj);
dyntype_t integer_q(dyntype_t obj);

dyntype_t inexact_q(dyntype_t z);
dyntype_t exact_q(dyntype_t z);

/*
(define exact-integer?
    (lambda (z) (and (integer? z) (exact? z))))
*/

// the following internal number operations only build an API for usage by the standard-library functions in numbers.c
// functions in numbers.c should only use these internal functions and no tommathlib functions or low-level struct initialization at all.

// this API should also not differentiate between inexact or exact variants

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

void rational_release(scheme_rational_t x);
scheme_rational_t rational_copy(scheme_rational_t obj);

/*
(define quotient truncate-quotient)
(define remainder truncate-remainder)
(define modulo floor-remainder)
*/

void release_number(scheme_number_t number);
dyntype_t copy_number(scheme_number_t number);

#endif