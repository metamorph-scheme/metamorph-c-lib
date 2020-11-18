#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf_internal.h"
#include "../functions.h"
#include <string.h>
#include <malloc.h>

dyntype_t pair_q(dyntype_t obj) {
  return scheme_new_boolean(obj.type == SCHEME_TYPE_PAIR);
}

scheme_pair_t i_cons(dyntype_t obj1, dyntype_t obj2) {
  bool_t list = i_list_q(obj2);

  scheme_pair_t pair = {
    .car = obj1,
    .cdr = obj2,
    .list = list
  };
  
  return pair;
}

dyntype_t cons(dyntype_t obj1, dyntype_t obj2) {
  return scheme_new_pair(i_cons(obj1, obj2));
}

dyntype_t car(dyntype_t pair) {
  REQUIRE_SCHEME_PAIR(pair, 0)
  return c_pair.car;
}

dyntype_t cdr(dyntype_t pair) {
  REQUIRE_SCHEME_PAIR(pair, 0)
  return c_pair.cdr;
}

dyntype_t set_car_ex(dyntype_t pair, dyntype_t obj) {
  // setting the car of a pair cannot alter the list status of a pair
  // as per definition in r7rs chapter 6.4 page 40
  REQUIRE_SCHEME_PAIR(pair, 0)

  if(!pair._mutable)
    SET_SETTING_IMMUTABLE_LOCATION;

  c_pair.car = obj;

  return SCHEME_UNSPECIFIED;
}

dyntype_t set_cdr_ex(dyntype_t pair, dyntype_t obj) {
  REQUIRE_SCHEME_PAIR(pair, 0)

  if(!pair._mutable)
     SET_SETTING_IMMUTABLE_LOCATION;

  if(i_list_q(obj))
    c_pair.list = TRUE;
  else
    c_pair.list = FALSE;
  
  c_pair.cdr = obj;

  return SCHEME_UNSPECIFIED;
}

dyntype_t null_q(dyntype_t obj) {
  return scheme_new_boolean(obj.type == SCHEME_TYPE_NULL);
}

bool_t i_list_q(dyntype_t obj) {
  return
    obj.type == SCHEME_TYPE_NULL ||
      (obj.type == SCHEME_TYPE_PAIR && obj.data.pair_val->list);
}

dyntype_t list_q(dyntype_t obj) {
  return scheme_new_boolean(i_list_q(obj));
}

dyntype_t make_list(int c_k) {
  // TODO
  // int c_k;

  dyntype_t init = SCHEME_NULL;
  dyntype_t *cur = &init;
  for(int i = 0; i < c_k; i++) {
    dyntype_t new_pair;

    dyntype_t new_car = SCHEME_UNSPECIFIED;
    dyntype_t new_empty_cdr = SCHEME_NULL;

    new_pair = cons(new_car, new_empty_cdr);

    *cur = new_pair;
    cur = &(new_pair.data.pair_val->cdr);
  }

  return init;
}

dyntype_t make_list_fill(int c_k, dyntype_t fill) {
  // TODO 
  // int c_k;

  dyntype_t init = SCHEME_NULL;
  dyntype_t *cur = &init;
  for(int i = 0; i < c_k; i++) {
    dyntype_t new_pair;

    // TODO is copying necessary? shallow/deep copy?
    dyntype_t new_car = fill;
    //memcpy(new_car, &fill, sizeof(dyntype_t));

    dyntype_t new_empty_cdr = SCHEME_NULL;

    new_pair = cons(new_car, new_empty_cdr);

    *cur = new_pair;
    cur = &(new_pair.data.pair_val->cdr);;
  }

  return init;
}

dyntype_t list(ELLIPSIS_PARAM(obj)) {

  dyntype_t init = SCHEME_NULL;
  dyntype_t *cur = &init;
  for(int i = 0; i < len; i++) {
    dyntype_t new_pair;

    dyntype_t new_empty_cdr = SCHEME_NULL;

    new_pair = cons(obj[i], new_empty_cdr);

    *cur = new_pair;
    cur = &(new_pair.data.pair_val->cdr);;
  }
  return init;
}
