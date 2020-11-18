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
      dyntype_t dyn = scheme_literal_string(str);
      
      dyntype_t sym_dyn = string_to_symbol(dyn);

      CHECK_TYPE(sym_dyn, SCHEME_TYPE_SYMBOL)
      ASSERT_EQ(0, strcmp(sym_dyn.data.symbol_val->name, str))
      ASSERT_EQ(TRUE, sym_dyn._mutable)
   ENDTEST

   TEST("string->symbol wrong type")
      dyntype_t dyn = scheme_literal_boolean(TRUE);
      
      dyntype_t sym_dyn = string_to_symbol(dyn);

      ASSERT_TYPE_ERROR(sym_dyn, SCHEME_TYPE_STRING, SCHEME_TYPE_BOOLEAN, 0)
   ENDTEST

   TEST("symbol->string")
      dyntype_t dyn = scheme_literal_symbol((scheme_symbol_t) { "nicesymbol" });
      
      dyntype_t str_dyn = symbol_to_string(dyn);

      CHECK_TYPE(str_dyn, SCHEME_TYPE_STRING)
      ASSERT_EQ(0, strcmp(*dyn.data.string_val, "nicesymbol"))
      ASSERT_EQ(FALSE, str_dyn._mutable)
   ENDTEST

   TEST("symbol->string wrong datatype")
      dyntype_t dyn = scheme_literal_string("nicestring");
      
      dyntype_t str_dyn = symbol_to_string(dyn);

      ASSERT_TYPE_ERROR(str_dyn, SCHEME_TYPE_SYMBOL, SCHEME_TYPE_STRING, 0)
   ENDTEST

   TEST("symbol?")
      dyntype_t dyn = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      
      dyntype_t boolean_dyn = symbol_q(dyn);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(TRUE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol? negative")
      dyntype_t dyn = scheme_literal_string("mystring");
      
      dyntype_t boolean_dyn = symbol_q(dyn);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=?")
      dyntype_t dyn1 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyn3 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(TRUE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=? negative")
      dyntype_t dyn1 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol2"});
      dyntype_t dyn3 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=? negative")
      dyntype_t dyn1 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyn3 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol2"});
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=? negative")
      dyntype_t dyn1 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol1"});
      dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyn3 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=? wrong type")
      dyntype_t dyn1 = scheme_literal_string("nicesymbol");
      dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyn3 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("symbol=? wrong type")
      dyntype_t dyn1 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      dyntype_t dyn3 = scheme_literal_string("nicesymbol");
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = symbol_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("evaluate empty string to true")
      
      scheme_boolean_t result = obj_to_boolean(scheme_literal_string(""));
      ASSERT_EQ(TRUE, result);
   ENDTEST

   TEST("evaluate #f to false")
      
      scheme_boolean_t result = obj_to_boolean(scheme_literal_boolean(FALSE));
      ASSERT_EQ(FALSE, result);
   ENDTEST

   TEST("boolean?")
      dyntype_t dyn = scheme_literal_boolean(TRUE);
      
      dyntype_t boolean_dyn = boolean_q(dyn);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(TRUE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("boolean? negative")
      dyntype_t dyn = scheme_literal_string("mystring");
      
      dyntype_t boolean_dyn = boolean_q(dyn);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("not")
      dyntype_t dyn = scheme_literal_string("");
      dyntype_t res = not(dyn);

      CHECK_TYPE(res, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *res.data.boolean_val)
      ASSERT_EQ(TRUE, res._mutable)
   ENDTEST

   TEST("not [2]")
      dyntype_t dyn = scheme_literal_boolean(TRUE);
      dyntype_t res = not(dyn);

      CHECK_TYPE(res, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *res.data.boolean_val)
      ASSERT_EQ(TRUE, res._mutable)
   ENDTEST

   TEST("not [3]")
      dyntype_t dyn = scheme_literal_boolean(FALSE);
      dyntype_t res = not(dyn);

      CHECK_TYPE(res, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(TRUE, *res.data.boolean_val)
      ASSERT_EQ(TRUE, res._mutable)
   ENDTEST


   TEST("boolean=?")
      dyntype_t dyn1 = scheme_literal_boolean(TRUE);
      dyntype_t dyn2 = scheme_literal_boolean(TRUE);
      dyntype_t dyn3 = scheme_literal_boolean(TRUE);
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = boolean_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(TRUE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("boolean=? one boolean")
      dyntype_t dyn1 = scheme_literal_boolean(TRUE);
      dyntype_t dyns[] = {dyn1};

      dyntype_t boolean_dyn = boolean_eq(dyns, 1);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(TRUE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("boolean=? wrong datatype")
      dyntype_t dyn1 = scheme_literal_boolean(TRUE);
      dyntype_t dyn2 = scheme_literal_string("abc");
      dyntype_t dyn3 = scheme_literal_boolean(TRUE);
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = boolean_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("boolean=? negative")
      dyntype_t dyn1 = scheme_literal_boolean(TRUE);
      dyntype_t dyn2 = scheme_literal_boolean(FALSE);
      dyntype_t dyn3 = scheme_literal_boolean(TRUE);
      dyntype_t dyns[] = {dyn1, dyn2, dyn3};

      dyntype_t boolean_dyn = boolean_eq(dyns, 3);

      CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN)
      ASSERT_EQ(FALSE, *boolean_dyn.data.boolean_val)
      ASSERT_EQ(TRUE, boolean_dyn._mutable)
   ENDTEST

   TEST("make-list")
      dyntype_t list = make_list(3);

      CHECK_TYPE(list, SCHEME_TYPE_PAIR)
      scheme_type_t last_car_type = car(cdr(cdr(list))).type;
      scheme_type_t last_cdr_type = cdr(cdr(cdr(list))).type;
      ASSERT_EQ(SCHEME_TYPE_UNSPECIFIED, last_car_type)
      ASSERT_EQ(SCHEME_TYPE_NULL, last_cdr_type)
   ENDTEST

   TEST("make-list-fill")
      dyntype_t str = scheme_literal_string("abc");
      dyntype_t list = make_list_fill(3, str);

      CHECK_TYPE(list, SCHEME_TYPE_PAIR)
      CHECK_TYPE(car(cdr(cdr(list))), SCHEME_TYPE_STRING)
      CHECK_TYPE(cdr(cdr(cdr(list))), SCHEME_TYPE_NULL)
      ASSERT_EQ("abc", *car(list).data.string_val)
      ASSERT_EQ("abc", *car(cdr(list)).data.string_val)
      ASSERT_EQ("abc", *car(cdr(cdr(list))).data.string_val)
   ENDTEST

   TEST("length")
      dyntype_t list = make_list(3);
      int len = length(list);

      printf("len: %d", len);

      ASSERT_EQ(3, len)
   ENDTEST

   TEST("append")
      dyntype_t str = scheme_literal_string("abc");
      dyntype_t list = make_list_fill(2, str);
      dyntype_t sym = scheme_literal_symbol((scheme_symbol_t) { .name = "a"});
      dyntype_t args[] = {list, sym};

      dyntype_t appended_list = append(args, 2);

      CHECK_TYPE(appended_list, SCHEME_TYPE_PAIR)
      CHECK_TYPE(car(cdr(appended_list)), SCHEME_TYPE_STRING)
      CHECK_TYPE(cdr(cdr(appended_list)), SCHEME_TYPE_SYMBOL)
      ASSERT_EQ("abc", *car(appended_list).data.string_val)
      ASSERT_EQ("abc", *car(cdr(appended_list)).data.string_val)
      ASSERT_EQ("a", cdr(cdr(appended_list)).data.symbol_val->name)
   ENDTEST

   TEST("append [2]")
      dyntype_t str = scheme_literal_string("abc");
      dyntype_t str2 = scheme_literal_string("cba");
      dyntype_t list = make_list_fill(2, str);
      dyntype_t list2 = make_list_fill(1, str2);
      dyntype_t sym = scheme_literal_symbol((scheme_symbol_t) { .name = "a"});
      dyntype_t args[] = {list, list2, sym};

      dyntype_t appended_list = append(args, 3);

      CHECK_TYPE(appended_list, SCHEME_TYPE_PAIR)
      CHECK_TYPE(car(cdr(appended_list)), SCHEME_TYPE_STRING)
      CHECK_TYPE(cdr(cdr(cdr(appended_list))), SCHEME_TYPE_SYMBOL)
      ASSERT_EQ("abc", *car(appended_list).data.string_val)
      ASSERT_EQ("abc", *car(cdr(appended_list)).data.string_val)
      ASSERT_EQ("cba", *car(cdr(cdr(appended_list))).data.string_val)
      ASSERT_EQ("a", cdr(cdr(cdr(appended_list))).data.symbol_val->name)
   ENDTEST

   TEST("append [3]")
      dyntype_t str = scheme_literal_string("abc");
      dyntype_t str2 = scheme_literal_string("cba");
      dyntype_t list = make_list_fill(2, str);
      dyntype_t list2 = make_list_fill(1, str2);
      dyntype_t args[] = {list, list2};

      dyntype_t appended_list = append(args, 2);

      CHECK_TYPE(appended_list, SCHEME_TYPE_PAIR)
      CHECK_TYPE(car(cdr(appended_list)), SCHEME_TYPE_STRING)
      CHECK_TYPE(cdr(cdr(cdr(appended_list))), SCHEME_TYPE_NULL)
      ASSERT_EQ("abc", *car(appended_list).data.string_val)
      ASSERT_EQ("abc", *car(cdr(appended_list)).data.string_val)
      ASSERT_EQ("cba", *car(cdr(cdr(appended_list))).data.string_val)
      ASSERT_EQ(TRUE, *list_q(appended_list).data.boolean_val)
   ENDTEST

   TEST("append [4]")
      dyntype_t str = scheme_literal_string("abc");
      dyntype_t args[] = {str};

      dyntype_t appended_list = append(args, 1);

      CHECK_TYPE(appended_list, SCHEME_TYPE_STRING)
      ASSERT_EQ("abc", *appended_list.data.string_val)
   ENDTEST

   TEST("append [5]")
      dyntype_t args[] = {};

      dyntype_t appended_list = append(args, 0);

      CHECK_TYPE(appended_list, SCHEME_TYPE_NULL)
   ENDTEST

   TEST("append [6]")
      dyntype_t str = scheme_literal_string("abc");
      dyntype_t str2 = scheme_literal_string("cba");
      dyntype_t list = make_list_fill(2, str);
      dyntype_t list2 = make_list_fill(1, str2);
      dyntype_t sym = scheme_literal_symbol((scheme_symbol_t) { .name = "a"});
      dyntype_t args[] = {list, sym, list2};

      dyntype_t appended_list = append(args, 3);

      CHECK_TYPE(appended_list, INTERNAL_TYPE_BAD_ARGUMENT_EXCEPTION)
   ENDTEST

   TEST("list-set!")
      dyntype_t replacement = scheme_literal_string("cde");
      dyntype_t str = scheme_literal_string("abc");
      dyntype_t list = make_list_fill(3, str);

      dyntype_t unspec = list_set(list, 1, replacement);

      CHECK_TYPE(unspec, SCHEME_TYPE_UNSPECIFIED)
      ASSERT_EQ("cde", *car(cdr(list)).data.string_val)
   ENDTEST

   TEST("list-set! [2]")
      dyntype_t replacement = scheme_literal_string("cde");
      dyntype_t str = scheme_literal_string("abc");
      dyntype_t list = make_list_fill(3, str);

      dyntype_t err = list_set(list, 3, replacement);

      CHECK_TYPE(err, INTERNAL_TYPE_BAD_ARGUMENT_EXCEPTION)
   ENDTEST

   TEST("list-set! [3]")
      dyntype_t replacement = scheme_literal_string("cde");
      dyntype_t str = scheme_literal_string("abc");
      dyntype_t list = make_list_fill(3, str);
      list._mutable = FALSE;

      dyntype_t err = list_set(list, 3, replacement);

      CHECK_TYPE(err, INTERNAL_TYPE_SETTING_IMMUTABLE_LOCATION)
   ENDTEST

   TEST("list-copy")
      dyntype_t str = scheme_literal_string("abc");
      dyntype_t list = make_list_fill(3, str);

      dyntype_t copied_list = list_copy(list);

      CHECK_TYPE(copied_list, SCHEME_TYPE_PAIR)
      ASSERT_EQ(TRUE, *list_q(copied_list).data.boolean_val)
      ASSERT_EQ("abc", *car(cdr(copied_list)).data.string_val)
   ENDTEST

   printf("All tests were successful!\n");
}