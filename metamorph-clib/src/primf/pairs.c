#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf_internal.h"
#include "../functions.h"
#include <string.h>
#include <malloc.h>

BASE_FUNCTION(pair_q) {
    PARAMETER(obj)
    PUSH_LITERAL(scheme_new_boolean(obj.type == SCHEME_TYPE_PAIR));
    //DESTROY_PARAM(obj)
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

BASE_FUNCTION(cons) {
    PARAMETER(obj1)
    PARAMETER(obj2)
    PUSH_LITERAL(scheme_new_pair(i_cons(obj1, obj2)))
    //DESTROY_PARAM(obj1)
    //DESTROY_PARAM(obj2)
}

BASE_FUNCTION(car) {
    PARAMETER(pair)
    REQUIRE_SCHEME_PAIR(pair, 0)
    PUSH_LITERAL(c_pair.car);
    //DESTROY_PARAM(pair)
}

BASE_FUNCTION(cdr) {
    PARAMETER(pair)
    REQUIRE_SCHEME_PAIR(pair, 0)
    PUSH_LITERAL(c_pair.cdr)
    //DESTROY_PARAM(pair)
}

BASE_FUNCTION(set_car_ex) {
    PARAMETER(pair)
    PARAMETER(obj)

    // setting the car of a pair cannot alter the list status of a pair
    // as per definition in r7rs chapter 6.4 page 40
    REQUIRE_SCHEME_PAIR(pair, 0)

    if (!pair._mutable) {
        SET_SETTING_IMMUTABLE_LOCATION
    }

    c_pair.car = obj;

    PUSH_UNSPECIFIED
    DESTROY_PARAM(pair)
    DESTROY_PARAM(obj)
}

BASE_FUNCTION(set_cdr_ex) {
    PARAMETER(pair)
    PARAMETER(obj)

    REQUIRE_SCHEME_PAIR(pair, 0)

    if (!pair._mutable) {
        SET_SETTING_IMMUTABLE_LOCATION;
    }

    if (i_list_q(obj))
        c_pair.list = TRUE;
    else
        c_pair.list = FALSE;

    c_pair.cdr = obj;

    PUSH_UNSPECIFIED
    DESTROY_PARAM(pair)
    DESTROY_PARAM(obj)
}

BASE_FUNCTION(null_q) {
    PARAMETER(obj)
    PUSH_LITERAL(scheme_new_boolean(obj.type == SCHEME_TYPE_NULL));
    DESTROY_PARAM(obj)
}

bool_t i_list_q(dyntype_t obj) {
    return
        obj.type == SCHEME_TYPE_NULL ||
        (obj.type == SCHEME_TYPE_PAIR && obj.data.pair_val->list);
}

BASE_FUNCTION(list_q) {
    PARAMETER(obj)
    PUSH_LITERAL(scheme_new_boolean(i_list_q(obj)))
    DESTROY_PARAM(obj)
}

dyntype_t make_list(int c_k) {
    // TODO
    // int c_k;

    dyntype_t init = SCHEME_NULL;
    dyntype_t* cur = &init;
    for (int i = 0; i < c_k; i++) {
        dyntype_t new_pair;

        dyntype_t new_car = SCHEME_UNSPECIFIED;
        dyntype_t new_empty_cdr = SCHEME_NULL;

        new_pair = scheme_new_pair(i_cons(new_car, new_empty_cdr));

        *cur = new_pair;
        cur = &(new_pair.data.pair_val->cdr);
    }

    return init;
}

dyntype_t make_list_fill(int c_k, dyntype_t fill) {
    // TODO 
    // int c_k;

    dyntype_t init = SCHEME_NULL;
    dyntype_t* cur = &init;
    for (int i = 0; i < c_k; i++) {
        dyntype_t new_pair;

        // TODO is copying necessary? shallow/deep copy?
        dyntype_t new_car = fill;
        //memcpy(new_car, &fill, sizeof(dyntype_t));

        dyntype_t new_empty_cdr = SCHEME_NULL;

        new_pair = scheme_new_pair(i_cons(new_car, new_empty_cdr));

        *cur = new_pair;
        cur = &(new_pair.data.pair_val->cdr);
    }

    return init;
}

dyntype_t i_list(ELLIPSIS_PARAM(x)) {

    dyntype_t init = SCHEME_NULL;
    dyntype_t* cur = &init;
    for (int i = 0; i < len; i++) {
        dyntype_t new_pair;

        dyntype_t new_empty_cdr = SCHEME_NULL;

        new_pair = scheme_new_pair(i_cons(copy_dyntype(x[i]), new_empty_cdr));

        *cur = new_pair;
        cur = &(new_pair.data.pair_val->cdr);
    }
    return init;
}

BASE_FUNCTION(list) {
    ELLIPSIS

    PUSH_LITERAL((i_list(ellipsis, n_ellipsis)))

    DESTROY_ELLIPSIS
}

BASE_FUNCTION(length) {
    PARAMETER(list)

    if (list.type == SCHEME_TYPE_NULL) {
        PUSH_LITERAL(scheme_new_number(scheme_exact_integer(integer_create_s32(0))))
        DESTROY_PARAM(list)
        return;
    }

    if(!i_list_q(list)) {
        SET_BAD_ARGUMENT_EXCEPTION(0, "argument to length must be a list")
    }

    int init = 1;
    dyntype_t* cur = &list;

    while (cur->data.pair_val->cdr.type != SCHEME_TYPE_NULL) {
        init++;
        cur = &(cur->data.pair_val->cdr);
    }

    PUSH_LITERAL(scheme_new_number(scheme_exact_integer(integer_create_s32(init))))
    DESTROY_PARAM(list)
}

// TODO uncomment and convert
//
//dyntype_t append(ELLIPSIS_PARAM(lists)) {
//    if (len == 0)
//        return SCHEME_NULL;
//
//    if (len == 1)
//        return lists[0];
//
//    dyntype_t init = SCHEME_NULL;
//    dyntype_t* cur = &init;
//    for (int i = 0; i < len - 1; i++) {
//        if (!i_list_q(lists[i]))
//            SET_BAD_ARGUMENT_EXCEPTION(i, "append: argument is not a list")
//
//        dyntype_t* list_progress = lists + i;
//        while (list_progress->type != SCHEME_TYPE_NULL) {
//            dyntype_t new_pair;
//            dyntype_t new_empty_cdr = SCHEME_NULL;
//
//            new_pair = cons(car(*list_progress), new_empty_cdr);
//
//            *cur = new_pair;
//            cur = &(new_pair.data.pair_val->cdr);
//            list_progress = &list_progress->data.pair_val->cdr;
//        }
//    }
//    *cur = lists[len - 1];
//    return init;
//}
//
//dyntype_t list_set(dyntype_t list, int k, dyntype_t obj) {
//    // TODO number
//    int c_k = k;
//
//    if (!i_list_q(list))
//        SET_BAD_ARGUMENT_EXCEPTION(0, "list-set!: argument ist not a list");
//
//    if (!list._mutable)
//        SET_SETTING_IMMUTABLE_LOCATION;
//
//    dyntype_t* cur = &list;
//    for (int i = 0; i < c_k; i++) {
//        if (cdr(*cur).type == SCHEME_TYPE_NULL)
//            SET_BAD_ARGUMENT_EXCEPTION(1, "list-set!: index is out of bounds");
//
//        cur = &(cur->data.pair_val->cdr);
//    }
//
//    cur->data.pair_val->car = obj;
//
//    return SCHEME_UNSPECIFIED;
//}
//
//dyntype_t list_copy(dyntype_t list) {
//    if (!i_list_q(list))
//        SET_BAD_ARGUMENT_EXCEPTION(0, "list-copy: argument ist not a list");
//
//    dyntype_t init = SCHEME_NULL;
//    dyntype_t* cur_new_list = &init;
//    dyntype_t* cur_orig_list = &list;
//    while (cur_orig_list->type != SCHEME_TYPE_NULL) {
//        dyntype_t new_pair;
//        dyntype_t new_empty_cdr = SCHEME_NULL;
//
//        new_pair = cons(car(*cur_orig_list), new_empty_cdr);
//
//        *cur_new_list = new_pair;
//        cur_new_list = &(new_pair.data.pair_val->cdr);
//        cur_orig_list = &(cur_orig_list->data.pair_val->cdr);
//    }
//    return init;
//}

void release_pair(scheme_pair_t pair) {
    release_dyntype(pair.car);
    release_dyntype(pair.cdr);
}

dyntype_t copy_pair(scheme_pair_t pair) {
    return scheme_new_pair(i_cons(copy_dyntype(pair.car), copy_dyntype(pair.cdr)));
}