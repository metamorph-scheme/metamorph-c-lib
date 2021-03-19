#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf_internal.h"
#include <string.h>

NONSTANDARD scheme_boolean_t obj_to_boolean(dyntype_t obj) {
    return (obj.type != SCHEME_TYPE_BOOLEAN || *obj.data.boolean_val != FALSE);
}

BASE_FUNCTION(not) {
    PARAMETER(obj)

    PUSH_LITERAL(scheme_new_boolean(!(obj_to_boolean(obj))))
    DESTROY_PARAM(obj)
}

BASE_FUNCTION(boolean_q) {
    PARAMETER(obj)
    PUSH_LITERAL(scheme_new_boolean(obj.type == SCHEME_TYPE_BOOLEAN))
    DESTROY_PARAM(obj)
}

BASE_FUNCTION(boolean_eq) {
    ELLIPSIS
    if (ellipsis[0].type != SCHEME_TYPE_BOOLEAN) {
        PUSH_LITERAL(scheme_new_boolean(FALSE))
        DESTROY_ELLIPSIS
        return;
    }

    for (int i = 1; i < n_ellipsis; i++) {
        if (ellipsis[i].type != SCHEME_TYPE_BOOLEAN
            || *ellipsis[0].data.boolean_val != *ellipsis[i].data.boolean_val) {
            PUSH_LITERAL(scheme_new_boolean(FALSE))
            DESTROY_ELLIPSIS
            return;
        }
    }
    PUSH_LITERAL(scheme_new_boolean(TRUE))
    DESTROY_ELLIPSIS
}