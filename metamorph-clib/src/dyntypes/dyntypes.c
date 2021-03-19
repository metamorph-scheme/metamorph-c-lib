#include <stdlib.h>
#include <string.h>
#include "../dyntypes.h"
#include "../common.h"
#include "dyntypes_internal.h"
#include "../functions.h"
#include "../lambda.h"
#include "../continuation.h"
#include "../primf.h"
#include <malloc.h>
#include "../tommath/tommath.h"

OBJ_CREATION_FUNCS(boolean, SCHEME_TYPE_BOOLEAN)
OBJ_CREATION_FUNCS(pair, SCHEME_TYPE_PAIR)
OBJ_CREATION_FUNCS(procedure, SCHEME_TYPE_PROCEDURE)
OBJ_CREATION_FUNCS(continuation, SCHEME_TYPE_CONTINUATION)
OBJ_CREATION_FUNCS(number, SCHEME_TYPE_NUMBER)
OBJ_CREATION_FUNCS(char, SCHEME_TYPE_CHAR)
OBJ_CREATION_FUNCS(port, SCHEME_TYPE_PORT)

dyntype_t scheme_symbol(scheme_symbol_t obj, bool_t _mutable) {
    char** ptr = REQUEST(char*);
    (*ptr) = REQUEST_ARRAY(char, strlen(obj));
    strcpy(*ptr, obj);
    return (dyntype_t) {
        .type = SCHEME_TYPE_SYMBOL,
            ._mutable = _mutable,
            .data.symbol_val = ptr
    };
}

dyntype_t scheme_string(scheme_string_t obj, bool_t _mutable) {
  char** ptr = REQUEST(char*);
  (*ptr) = REQUEST_ARRAY(char, strlen(obj));
  strcpy(*ptr, obj);
  return (dyntype_t) {
    .type = SCHEME_TYPE_STRING,
    ._mutable = _mutable,
    .data.string_val = ptr
  };
}

//Copies from obj, obj needs to be deallocated
dyntype_t scheme_new_string(scheme_string_t obj) {
  return scheme_string(obj, TRUE);
}
//Copies from obj, obj needs to be deallocated
dyntype_t scheme_literal_string(scheme_string_t obj) {\
  return scheme_string(obj, FALSE);
}

//Copies from obj, obj needs to be deallocated
dyntype_t scheme_new_symbol(scheme_symbol_t obj) {
    return scheme_symbol(obj, TRUE);
}
//Copies from obj, obj needs to be deallocated
dyntype_t scheme_literal_symbol(scheme_symbol_t obj) {
    return scheme_symbol(obj, FALSE);
}

scheme_number_t scheme_exact_integer(scheme_integer_t obj) {
  SIMPLE_MALLOC(scheme_integer_t, obj)
  return (scheme_number_t) {
      .type = SCHEME_NUMERICAL_TYPE_EXACT_INTEGER,
      .data.exact_integer_val = ptr
  };
}

scheme_number_t scheme_exact_rational(scheme_rational_t obj) {
    SIMPLE_MALLOC(scheme_rational_t, obj)
    return (scheme_number_t) {
        .type = SCHEME_NUMERICAL_TYPE_EXACT_RATIONAL,
        .data.exact_rational_val = ptr
    };
}

scheme_number_t scheme_inexact_rational(scheme_real_t obj) {
    SIMPLE_MALLOC(scheme_real_t, obj)
        return (scheme_number_t) {
        .type = SCHEME_NUMERICAL_TYPE_INEXACT_RATIONAL,
            .data.inexact_real_val = ptr
    };
}

scheme_number_t scheme_inexact_integer(scheme_real_t obj) {
    SIMPLE_MALLOC(scheme_real_t, obj)
        return (scheme_number_t) {
        .type = SCHEME_NUMERICAL_TYPE_INEXACT_INTEGER,
        .data.inexact_real_val = ptr
    };
}

scheme_number_t scheme_inexact_real(scheme_real_t obj) {
    SIMPLE_MALLOC(scheme_real_t, obj)
        return (scheme_number_t) {
        .type = SCHEME_NUMERICAL_TYPE_INEXACT_REAL,
            .data.inexact_real_val = ptr
    };
}

scheme_ord_t from_mp_ord(mp_ord ord) {
    switch (ord) {
    case MP_GT: return SCHEME_GT;
    case MP_LT: return SCHEME_LT;
    case MP_EQ: return SCHEME_EQ;
    }
}

void release_dyntype(dyntype_t dyntype){
    switch(dyntype.type){
        case(SCHEME_TYPE_PROCEDURE):{
            scheme_procedure_t procedure;
            procedure = *dyntype.data.procedure_val;
            release_procedure(procedure);
            RELEASE(scheme_procedure_t, dyntype.data.procedure_val)
            break;
        }
        case(SCHEME_TYPE_BOOLEAN): {
            scheme_boolean_t boolean;
            boolean = *dyntype.data.boolean_val;
            //release_boolean(boolean);
            RELEASE(scheme_boolean_t, dyntype.data.boolean_val)
            break;
        }
        case(SCHEME_TYPE_STRING): {
            scheme_string_t string;
            string = *dyntype.data.string_val;
            RELEASE_ARRAY(char, strlen(string), string);
            RELEASE(scheme_string_t, dyntype.data.string_val)
            break;
        }
        //case(SCHEME_TYPE_SYMBOL): {
        //    scheme_symbol_t symbol;
        //    symbol = *dyntype.data.symbol_val;
        //    RELEASE_ARRAY(char, strlen(symbol), symbol);
        //    RELEASE(scheme_symbol_t, dyntype.data.symbol_val)
        //    break;
        //}
        //case(SCHEME_TYPE_PAIR): {
        //    scheme_pair_t pair;
        //    pair = *dyntype.data.pair_val;
        //    release_pair(pair);
        //    RELEASE(scheme_pair_t, dyntype.data.pair_val)
        //    break;
        //}
        case(SCHEME_TYPE_CONTINUATION): {
            scheme_continuation_t cont;
            cont = *dyntype.data.continuation_val;
            release_continuation(cont);
            RELEASE(scheme_continuation_t, dyntype.data.continuation_val)
            break;
        }
        //case(SCHEME_TYPE_NUMBER): {
        //    scheme_number_t number;
        //    number = *dyntype.data.number_val;
        //    release_number(number);
        //    RELEASE(scheme_number_t, dyntype.data.number_val)
        //    break;
        //}
        case(SCHEME_TYPE_CHAR): {
            scheme_char_t c;
            c = *dyntype.data.char_val;
            RELEASE(scheme_char_t, dyntype.data.char_val)
            break;
        }
        case(SCHEME_TYPE_PORT): {
            scheme_port_t port;
            port = *dyntype.data.port_val;
            RELEASE(scheme_port_t, dyntype.data.port_val)
            break;
        }
        default: break;
    }
}


// TODO copy mutable flag correctly
dyntype_t copy_dyntype(dyntype_t dyntype) {
    switch (dyntype.type) {
    case(SCHEME_TYPE_PROCEDURE): {
        scheme_procedure_t procedure;
        procedure = *dyntype.data.procedure_val;
        return copy_procedure(dyntype);
    }
    case(SCHEME_TYPE_CONTINUATION): {
        scheme_continuation_t cont;
        cont = *dyntype.data.continuation_val;
        return copy_continuation(dyntype);
    }
    case(SCHEME_TYPE_BOOLEAN): {
        scheme_boolean_t boolean;
        boolean = *dyntype.data.boolean_val;
        //return copy_boolean(boolean);
        return scheme_new_boolean(boolean);
    }
    case(SCHEME_TYPE_STRING): {
        scheme_string_t string = REQUEST_ARRAY(char, strlen(*dyntype.data.string_val));
        strcpy(string, *dyntype.data.string_val);
        return scheme_new_string(string);
    }
    case(SCHEME_TYPE_SYMBOL): {
        scheme_symbol_t symbol = REQUEST_ARRAY(char, strlen(*dyntype.data.symbol_val));
        strcpy(symbol, *dyntype.data.symbol_val);
        return scheme_new_symbol(symbol);
    }
    //case(SCHEME_TYPE_PAIR): {
    //    scheme_pair_t pair;
    //    pair = *dyntype.data.pair_val;
    //    return copy_pair(pair);
    //}
    //case(SCHEME_TYPE_NUMBER): {
    //    scheme_number_t number;
    //    number = *dyntype.data.number_val;
    //    return copy_number(number);
    //}
    case(SCHEME_TYPE_CHAR): {
        scheme_char_t c;
        c = *dyntype.data.char_val;
        return scheme_new_char(c);
    }
    case(SCHEME_TYPE_PORT): {
        scheme_port_t port;
        port = *dyntype.data.port_val;
        return scheme_new_port(port);
    }
    default: break;
    }

    //Assume not mentioned dyntypes are valuetypes
    return dyntype;
}

int count_references_dyntype(dyntype_t dyntype, struct activation_struct_t* activation) {
    switch (dyntype.type)
    {
    case(SCHEME_TYPE_PROCEDURE): {
        scheme_procedure_t procedure = *dyntype.data.procedure_val;
        return count_references_procedure(procedure,activation);
    }
    case(SCHEME_TYPE_CONTINUATION): {
        scheme_continuation_t cont = *dyntype.data.continuation_val;
        return count_references_continuation(cont,activation);
    }
    default:
        return 0;
    }
}