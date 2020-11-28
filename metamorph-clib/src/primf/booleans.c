#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf_internal.h"
#include <string.h>

NONSTANDARD scheme_boolean_t obj_to_boolean(dyntype_t obj) {
    return (obj.type != SCHEME_TYPE_BOOLEAN || *obj.data.boolean_val != FALSE);
}

dyntype_t not(dyntype_t obj) {
    return scheme_new_boolean(!(obj_to_boolean(obj)));
}

dyntype_t boolean_q(dyntype_t obj) {
    return scheme_new_boolean(obj.type == SCHEME_TYPE_BOOLEAN);
}

dyntype_t boolean_eq(ELLIPSIS_PARAM(boolean)) {
    if (boolean[0].type != SCHEME_TYPE_BOOLEAN)
        return scheme_new_boolean(FALSE);

    for (int i = 1; i < len; i++) {
        if (boolean[i].type != SCHEME_TYPE_BOOLEAN
            || *boolean[0].data.boolean_val != *boolean[i].data.boolean_val)
            return scheme_new_boolean(FALSE);
    }
    return scheme_new_boolean(TRUE);
}