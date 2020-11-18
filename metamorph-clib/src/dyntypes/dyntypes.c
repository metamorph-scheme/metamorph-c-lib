#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "dyntypes_internal.h"
#include "../functions.h"
#include "../lambda.h"
#include <malloc.h>


OBJ_CREATION_FUNCS(boolean, SCHEME_TYPE_BOOLEAN)
OBJ_CREATION_FUNCS(symbol, SCHEME_TYPE_SYMBOL)
OBJ_CREATION_FUNCS(string, SCHEME_TYPE_STRING)
OBJ_CREATION_FUNCS(pair, SCHEME_TYPE_PAIR)
OBJ_CREATION_FUNCS(procedure, SCHEME_TYPE_PROCEDURE)

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
            //release_string(string);
            RELEASE(scheme_string_t, dyntype.data.string_val)
            break;
        }
        case(SCHEME_TYPE_PAIR): {
            scheme_pair_t pair;
            pair = *dyntype.data.pair_val;
            //release_pair(pair);
            RELEASE(scheme_pair_t, dyntype.data.pair_val)
            break;
        }
        case(SCHEME_TYPE_SYMBOL): {
            scheme_symbol_t symbol;
            symbol = *dyntype.data.symbol_val;
            //release_symbol(symbol);
            RELEASE(scheme_symbol_t, dyntype.data.symbol_val)
            break;
        }
        default: break;
    }
}

dyntype_t copy_dyntype(dyntype_t dyntype) {
    switch (dyntype.type) {
    case(SCHEME_TYPE_PROCEDURE): {
        scheme_procedure_t procedure;
        procedure = *dyntype.data.procedure_val;
        return copy_procedure(dyntype);
        //return scheme_literal_procedure(procedure);
    }
    case(SCHEME_TYPE_BOOLEAN): {
        scheme_boolean_t boolean;
        boolean = *dyntype.data.boolean_val;
        //return copy_boolean(boolean);
        return scheme_new_boolean(boolean);
    }
    case(SCHEME_TYPE_STRING): {
        scheme_string_t string;
        string = *dyntype.data.string_val;
        //return copy_string(string);
        return scheme_new_string(string);
    }
    case(SCHEME_TYPE_PAIR): {
        scheme_pair_t pair;
        pair = *dyntype.data.pair_val;
        //return copy_pair(pair);
        return scheme_new_pair(pair);
    }
    case(SCHEME_TYPE_SYMBOL): {
        scheme_symbol_t symbol;
        symbol = *dyntype.data.symbol_val;
        //return copy_symbol(symbol);
        return scheme_new_symbol(symbol);
    }
    default: break;
    }
}

int count_references_dyntype(dyntype_t dyntype, activation_t* activation) {
    switch (dyntype.type)
    {
    case(SCHEME_TYPE_PROCEDURE): {
        scheme_procedure_t procedure = *dyntype.data.procedure_val;
        return count_references_procedure(procedure,activation);
    }
    default:
        return 0;
    }
}