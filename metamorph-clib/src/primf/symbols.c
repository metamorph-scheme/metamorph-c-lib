#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "../functions.h"
#include "primf_internal.h"
#include <string.h>

dyntype_t symbol_q(dyntype_t obj) {
    return scheme_new_boolean(obj.type == SCHEME_TYPE_SYMBOL);
}

dyntype_t symbol_eq(ELLIPSIS_PARAM(obj)) {
    if (obj[0].type != SCHEME_TYPE_SYMBOL)
        return scheme_new_boolean(FALSE);

    for (int i = 1; i < len; i++) {
        if (obj[i].type != SCHEME_TYPE_SYMBOL
            || (strcmp(obj[0].data.symbol_val->name, obj[i].data.symbol_val->name) != 0))
            return scheme_new_boolean(FALSE);
    }
    return scheme_new_boolean(TRUE);
}

// return immutable string on purpose - see r7rs page 44
dyntype_t symbol_to_string(dyntype_t sym) {
    REQUIRE_SCHEME_SYMBOL(sym, 0);
    return scheme_literal_string(c_sym.name);
}

dyntype_t string_to_symbol(dyntype_t str) {
    REQUIRE_SCHEME_STRING(str, 0);
    scheme_symbol_t sym = {
      .name = c_str
    };
    return scheme_new_symbol(sym);
}