#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "../functions.h"
#include "primf_internal.h"
#include <string.h>

BASE_FUNCTION(symbol_q) {
    PARAMETER(obj)
    PUSH_LITERAL(scheme_new_boolean(obj.type == SCHEME_TYPE_SYMBOL))

    DESTROY_PARAM(obj)
}

BASE_FUNCTION(symbol_eq) {
    ELLIPSIS


    if (ellipsis[0].type != SCHEME_TYPE_SYMBOL) {
        PUSH_LITERAL(scheme_new_boolean(FALSE))
        return;
    }

    for (int i = 1; i < n_ellipsis; i++) {
        if (ellipsis[i].type != SCHEME_TYPE_SYMBOL
            || (strcmp(*ellipsis[0].data.symbol_val, *ellipsis[i].data.symbol_val) != 0)){
            PUSH_LITERAL(scheme_new_boolean(FALSE))
            return;
        }
    }
    PUSH_LITERAL(scheme_new_boolean(TRUE));

    DESTROY_ELLIPSIS
}

// return immutable string on purpose - see r7rs page 44
BASE_FUNCTION(symbol_to_string) {
    PARAMETER(sym)
    REQUIRE_SCHEME_SYMBOL(sym, 0);
    PUSH_LITERAL(scheme_literal_string(c_sym));

    // not needed because the char pointer of the symbol is reused
    //DESTROY_PARAM(sym)
}

BASE_FUNCTION (string_to_symbol) {
    PARAMETER(str)

    REQUIRE_SCHEME_STRING(str, 0);

    PUSH_LITERAL(scheme_new_symbol(c_str))

    // not needed because the char pointer of the string is reused
    //DESTROY_PARAM(str)
}