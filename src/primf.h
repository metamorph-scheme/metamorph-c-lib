#ifndef PRIMF_GLOB
#define PRIMF_GLOB

#define ELLIPSIS_PARAM(NAME) dyntype_t* NAME, int len
#define NONSTANDARD
#define STANDARD(NAME)

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
  (lambda (obj list)
    (cond
      ((null? list)
        #f)
      ((eq? (car list) obj)
        list)
      (else
        (mem-int obj (cdr list))))))
*/

// TODO assocq ...

// list-copy
dyntype_t list_copy(dyntype_t list);

#endif