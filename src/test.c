#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dyntypes.h"
#include "common.h"
#include "primf.h"

#define TEST(NAME) { char name[100] = NAME; printf("Testing %s \n", name);
#define ASSERTION_SUCCESS printf("\t Assertion was successful\n");
#define FAIL(MSG) printf("\tAssertion for %s failed: %s\n\n", name, MSG); return 1;
#define ASSERT_EQ(EXPECTED,ACTUAL) if(EXPECTED == ACTUAL) { ASSERTION_SUCCESS } else { FAIL("actual value is not equal to the expected one") }
#define ENDTEST printf("Test %s successful\n\n", name);}
#define CHECK_TYPE(OBJ, TYPE) if (OBJ.type != TYPE) { FAIL("type of " #OBJ " is not " #TYPE) }
#define ASSERT_TYPE_ERROR(OBJ,WANTED,RECEIVED,POS) \
      CHECK_TYPE(OBJ, INTERNAL_TYPE_TYPE_EXCEPTION)\
      ASSERT_EQ(WANTED, OBJ.data.internal_type_exception_val.wanted)\
      ASSERT_EQ(RECEIVED, OBJ.data.internal_type_exception_val.received)\
      ASSERT_EQ(POS, OBJ.data.internal_type_exception_val.position)

int main() {

   TEST("string->symbol")
      scheme_string_t str = "nicesymbol";
      dyntype_t dyn = SCHEME_LITERAL_STRING(str);
      
      dyntype_t sym_dyn = string_to_symbol(dyn);

      CHECK_TYPE(sym_dyn, SCHEME_TYPE_SYMBOL)
      ASSERT_EQ(0, strcmp(sym_dyn.data.symbol_val.name, str))
      ASSERT_EQ(TRUE, sym_dyn._mutable)
   ENDTEST

   TEST("string->symbol wrong type")
      dyntype_t dyn = SCHEME_LITERAL_BOOLEAN(TRUE);
      
      dyntype_t sym_dyn = string_to_symbol(dyn);

      ASSERT_TYPE_ERROR(sym_dyn, SCHEME_TYPE_STRING, SCHEME_TYPE_BOOLEAN, 0)
   ENDTEST

   TEST("symbol->string")
      dyntype_t dyn = SCHEME_LITERAL_SYMBOL("nicesymbol");
      
      dyntype_t str_dyn = symbol_to_string(dyn);

      CHECK_TYPE(str_dyn, SCHEME_TYPE_STRING)
      ASSERT_EQ(0, strcmp(dyn.data.string_val, "nicesymbol"))
      ASSERT_EQ(FALSE, str_dyn._mutable)
   ENDTEST

   TEST("symbol->string wrong datatype")
      dyntype_t dyn = SCHEME_LITERAL_STRING("nicesymbol");
      
      dyntype_t str_dyn = symbol_to_string(dyn);

      ASSERT_TYPE_ERROR(str_dyn, SCHEME_TYPE_SYMBOL, SCHEME_TYPE_STRING, 0)
   ENDTEST

   TEST("symbol?")
      dyntype_t dyn = SCHEME_LITERAL_SYMBOL("nicesymbol");
      
      dyntype_t boolean_dyn = symbol_q(dyn);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(TRUE, boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol? negative")
      dyntype_t dyn = SCHEME_LITERAL_STRING("mystring");
      
      dyntype_t boolean_dyn = symbol_q(dyn);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=?")
      dyntype_t dyn1 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyn2 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyn3 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(TRUE, boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=? negative")
      dyntype_t dyn1 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyn2 = SCHEME_LITERAL_SYMBOL("nicesymbol2");
      dyntype_t dyn3 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=? negative")
      dyntype_t dyn1 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyn2 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyn3 = SCHEME_LITERAL_SYMBOL("nicesymbol2");
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=? negative")
      dyntype_t dyn1 = SCHEME_LITERAL_SYMBOL("nicesymbol1");
      dyntype_t dyn2 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyn3 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=? wrong type")
      dyntype_t dyn1 = SCHEME_LITERAL_STRING("nicesymbol");
      dyntype_t dyn2 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyn3 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=? wrong type")
      dyntype_t dyn1 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyn2 = SCHEME_LITERAL_SYMBOL("nicesymbol");
      dyntype_t dyn3 = SCHEME_LITERAL_STRING("nicesymbol");
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   printf("All tests were successful!\n");
}