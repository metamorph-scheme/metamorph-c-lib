#include <stdlib.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "primf.h"
#include <string.h>

// symbol?
dyntype_t symbol_q(dyntype_t obj) {
  if(obj.type == SCHEME_TYPE_SYMBOL) {
    return SCHEME_NEW_BOOLEAN(TRUE);
  } else {
    return SCHEME_NEW_BOOLEAN(FALSE);
  }
}

// symbol=?
dyntype_t symbol_eq(ELLIPSIS_PARAM(obj)) {
  if(obj[0].type != SCHEME_TYPE_SYMBOL)
    return SCHEME_NEW_BOOLEAN(FALSE);

  bool_t match = TRUE;
  for(int i = 1; i < len; i++) {
    if(obj[i].type == SCHEME_TYPE_SYMBOL) {
      if(strcmp(obj[0].data.symbol_val.name, obj[i].data.symbol_val.name) != 0) {
        match = FALSE;
      }
    } else {
      return SCHEME_NEW_BOOLEAN(FALSE);
    }
  }
  return SCHEME_NEW_BOOLEAN(match);
}

// symbol->string
// return immutable string on purpose - see r7rs page 44
dyntype_t symbol_to_string(dyntype_t sym) {
  REQUIRE_SCHEME_SYMBOL(sym, 0)
  return SCHEME_LITERAL_STRING(c_sym.name);
}

// string->symbol
dyntype_t string_to_symbol(dyntype_t str) {
  REQUIRE_SCHEME_STRING(str, 0);
  scheme_symbol_t sym = {
    .name = c_str
  };
  return SCHEME_NEW_SYMBOL(sym);
}