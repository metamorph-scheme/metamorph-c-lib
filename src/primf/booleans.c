#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf.h"
#include <string.h>

// non-standard
scheme_boolean_t obj_to_boolean(dyntype_t obj) {
  return (obj.type != SCHEME_TYPE_BOOLEAN || obj.data.boolean_val != FALSE);
}

// not
dyntype_t not(dyntype_t obj) {
  return SCHEME_NEW_BOOLEAN(!(obj_to_boolean(obj)));
}

// boolean?
dyntype_t boolean_q(dyntype_t obj) {
  return SCHEME_NEW_BOOLEAN(obj.type == SCHEME_TYPE_BOOLEAN);
}

// boolean=?
dyntype_t boolean_eq(ELLIPSIS_PARAM(boolean)) {
  if(boolean[0].type != SCHEME_TYPE_BOOLEAN)
    return SCHEME_NEW_BOOLEAN(FALSE);

  for(int i = 1; i < len; i++) {
    if(boolean[i].type != SCHEME_TYPE_BOOLEAN
      || boolean[0].data.boolean_val != boolean[i].data.boolean_val)
      return SCHEME_NEW_BOOLEAN(FALSE);
  }
  return SCHEME_NEW_BOOLEAN(TRUE);
}