#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../dyntypes.h"
#include "../common.h"
#include "../primf.h"
#include "../cache.h"
#include "../functions.h"

#define MU_ASSERT(message, test) do { if (!(test)) return message; } while (0)
#define MU_RUN_TEST(TEST) do { char *message = test_##TEST(); tests_run++; \
                                if (message) return message; } while (0)

int tests_run = 0;


#define TEST(NAME) static char * test_##NAME ()
#define CHECK_TYPE(OBJ, TYPE) MU_ASSERT("type of " #OBJ " is not " #TYPE, OBJ.type == TYPE)
#define CHECK_ERROR_TYPE(OBJ, TYPE) MU_ASSERT("type of " #OBJ " is not " #TYPE, OBJ.internal_error_type == TYPE)
#define ASSERT_EQ(MESSAGE, EXP, ACT) MU_ASSERT(MESSAGE, (EXP) == (ACT))

#define ASSERT_TYPE_ERROR(OBJ,WANTED,RECEIVED,POS) \
      CHECK_ERROR_TYPE(OBJ, INTERNAL_TYPE_TYPE_EXCEPTION);\
      MU_ASSERT("WANTED is not" #WANTED, WANTED == OBJ.data.type_exception.wanted);\
      MU_ASSERT("RECEIVED is not" #RECEIVED, RECEIVED == OBJ.data.type_exception.received);\
      MU_ASSERT("POS is not " #POS, POS == OBJ.data.type_exception.position);
#define TRY int code; if (!(code = setjmp(error_buffer)))
#define CATCH else

TEST(string_to_symbol) {
    scheme_string_t str = "nicesymbol";
    dyntype_t dyn = scheme_literal_string(str);
      
    dyntype_t sym_dyn = string_to_symbol(dyn);

    CHECK_TYPE(sym_dyn, SCHEME_TYPE_SYMBOL);
    ASSERT_EQ("Symbol name != nicesymbol",0,strcmp(sym_dyn.data.symbol_val->name, str));
    ASSERT_EQ("Symbol is not mutable", TRUE, sym_dyn._mutable);
    return 0;
}

TEST(string_to_symbol_wrong_type) {
    dyntype_t dyn = scheme_literal_boolean(TRUE);
    
    TRY {
        dyntype_t sym_dyn = string_to_symbol(dyn);
    }
    CATCH {
        ASSERT_TYPE_ERROR(global_exception, SCHEME_TYPE_STRING, SCHEME_TYPE_BOOLEAN, 0)
    }

    
    return 0;

}

TEST(symbol_to_string) {
    dyntype_t dyn = scheme_literal_symbol((scheme_symbol_t) { "nicesymbol" });
      
    dyntype_t str_dyn = symbol_to_string(dyn);

    CHECK_TYPE(str_dyn, SCHEME_TYPE_STRING);
    ASSERT_EQ("String != nicesymbol", 0, strcmp(*dyn.data.string_val, "nicesymbol"));
    ASSERT_EQ("String is mutable", FALSE, str_dyn._mutable);

    return 0;
}

TEST(symbol_to_string_wrong_datatype) {
    dyntype_t dyn = scheme_literal_string("nicestring");
      
    TRY {
        dyntype_t str_dyn = symbol_to_string(dyn);
    }
    CATCH{
        ASSERT_TYPE_ERROR(global_exception, SCHEME_TYPE_SYMBOL, SCHEME_TYPE_STRING, 0)
    }

    return 0;
}

TEST(symbol_q) {
    dyntype_t dyn = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
      
    dyntype_t boolean_dyn = symbol_q(dyn);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean != true", TRUE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_q_negative) {
    dyntype_t dyn = scheme_literal_string("mystring");
      
    dyntype_t boolean_dyn = symbol_q(dyn);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean != false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq) {
    dyntype_t dyn1 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyn3 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean != true", TRUE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq_negative) {
    dyntype_t dyn1 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol2"});
    dyntype_t dyn3 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean != false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq_negative_2) {
    dyntype_t dyn1 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyn3 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol2"});
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean != false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq_negative_3) {
    dyntype_t dyn1 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol1"});
    dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyn3 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean != false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq_wrong_type) {
    dyntype_t dyn1 = scheme_literal_string("nicesymbol");
    dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyn3 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq_wrong_type_2) {
    dyntype_t dyn1 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyn2 = scheme_literal_symbol((scheme_symbol_t) {"nicesymbol"});
    dyntype_t dyn3 = scheme_literal_string("nicesymbol");
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(evaluate_empty_string_to_true) {
    scheme_boolean_t result = obj_to_boolean(scheme_literal_string(""));
    ASSERT_EQ("Empty string does not yield true", TRUE, result);

    return 0;
}

TEST(evaluate_hf_to_false) {
      
    scheme_boolean_t result = obj_to_boolean(scheme_literal_boolean(FALSE));
    ASSERT_EQ("Boolean false does not yield false", FALSE, result);

    return 0;
}

TEST(boolean_q) {
    dyntype_t dyn = scheme_literal_boolean(TRUE);
      
    dyntype_t boolean_dyn = boolean_q(dyn);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not true", TRUE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(boolean_q_negative) {
    dyntype_t dyn = scheme_literal_string("mystring");
      
    dyntype_t boolean_dyn = boolean_q(dyn);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(not) {
    dyntype_t dyn = scheme_literal_string("");
    dyntype_t res = not(dyn);

    CHECK_TYPE(res, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not false", FALSE, *res.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, res._mutable);

    return 0;
}

TEST(not_2) {
    dyntype_t dyn = scheme_literal_boolean(TRUE);
    dyntype_t res = not(dyn);

    CHECK_TYPE(res, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not false", FALSE, *res.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, res._mutable);

    return 0;
}

TEST(not_3) {
    dyntype_t dyn = scheme_literal_boolean(FALSE);
    dyntype_t res = not(dyn);

    CHECK_TYPE(res, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not true", TRUE, *res.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, res._mutable);

    return 0;
}


TEST(boolean_eq) {
    dyntype_t dyn1 = scheme_literal_boolean(TRUE);
    dyntype_t dyn2 = scheme_literal_boolean(TRUE);
    dyntype_t dyn3 = scheme_literal_boolean(TRUE);
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = boolean_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not true", TRUE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(boolean_eq_one_boolean) {
    dyntype_t dyn1 = scheme_literal_boolean(TRUE);
    dyntype_t dyns[] = {dyn1};

    dyntype_t boolean_dyn = boolean_eq(dyns, 1);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not true", TRUE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(boolean_eq_wrong_datatype) {
    dyntype_t dyn1 = scheme_literal_boolean(TRUE);
    dyntype_t dyn2 = scheme_literal_string("abc");
    dyntype_t dyn3 = scheme_literal_boolean(TRUE);
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = boolean_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(boolean_eq_negative) {
    dyntype_t dyn1 = scheme_literal_boolean(TRUE);
    dyntype_t dyn2 = scheme_literal_boolean(FALSE);
    dyntype_t dyn3 = scheme_literal_boolean(TRUE);
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = boolean_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(make_list) {
    dyntype_t list = make_list(3);

    CHECK_TYPE(list, SCHEME_TYPE_PAIR);
    scheme_type_t last_car_type = car(cdr(cdr(list))).type;
    scheme_type_t last_cdr_type = cdr(cdr(cdr(list))).type;
    ASSERT_EQ("last car is not unspecified", SCHEME_TYPE_UNSPECIFIED, last_car_type);
    ASSERT_EQ("last cdr is not empty list",SCHEME_TYPE_NULL, last_cdr_type);

    return 0;
}

TEST(make_list_fill) {
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t list = make_list_fill(3, str);

    CHECK_TYPE(list, SCHEME_TYPE_PAIR);
    CHECK_TYPE(car(cdr(cdr(list))), SCHEME_TYPE_STRING);
    CHECK_TYPE(cdr(cdr(cdr(list))), SCHEME_TYPE_NULL);
    ASSERT_EQ("car is not abc", 0, strcmp("abc", *car(list).data.string_val));
    ASSERT_EQ("cadr is not abc", 0, strcmp("abc", *car(cdr(list)).data.string_val));
    ASSERT_EQ("caddr is not abc", 0, strcmp("abc", *car(cdr(cdr(list))).data.string_val));

    return 0;
}

TEST(length) {
    dyntype_t list = make_list(3);
    int len = length(list);

    ASSERT_EQ("lenght is not 3", 3, len);

    return 0;
}

TEST(append) {
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t list = make_list_fill(2, str);
    dyntype_t sym = scheme_literal_symbol((scheme_symbol_t) { .name = "a" });
    dyntype_t args[] = { list, sym };

    dyntype_t appended_list = append(args, 2);

    CHECK_TYPE(appended_list, SCHEME_TYPE_PAIR);
    CHECK_TYPE(car(cdr(appended_list)), SCHEME_TYPE_STRING);
    CHECK_TYPE(cdr(cdr(appended_list)), SCHEME_TYPE_SYMBOL);
    ASSERT_EQ("car is not string abc", 0,strcmp("abc", *car(appended_list).data.string_val));
    ASSERT_EQ("cadr is not string abc", 0, strcmp("abc", *car(cdr(appended_list)).data.string_val));
    ASSERT_EQ("caddr is not symbol a", 0, strcmp("a", cdr(cdr(appended_list)).data.symbol_val->name));

    return 0;
}

TEST(append_2) {
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t str2 = scheme_literal_string("cba");
    dyntype_t list = make_list_fill(2, str);
    dyntype_t list2 = make_list_fill(1, str2);
    dyntype_t sym = scheme_literal_symbol((scheme_symbol_t) { .name = "a" });
    dyntype_t args[] = { list, list2, sym };

    dyntype_t appended_list = append(args, 3);

    CHECK_TYPE(appended_list, SCHEME_TYPE_PAIR);
    CHECK_TYPE(car(cdr(appended_list)), SCHEME_TYPE_STRING);
    CHECK_TYPE(cdr(cdr(cdr(appended_list))), SCHEME_TYPE_SYMBOL);
    ASSERT_EQ("car is not string abc",0,strcmp("abc", *car(appended_list).data.string_val));
    ASSERT_EQ("cadr is not string abc", 0, strcmp("abc", *car(cdr(appended_list)).data.string_val));
    ASSERT_EQ("caddr is not cba", 0, strcmp("cba", *car(cdr(cdr(appended_list))).data.string_val));
    ASSERT_EQ("cdddr is not symbol a", 0, strcmp("a", cdr(cdr(cdr(appended_list))).data.symbol_val->name));

    return 0;
}

TEST(append_3) {
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t str2 = scheme_literal_string("cba");
    dyntype_t list = make_list_fill(2, str);
    dyntype_t list2 = make_list_fill(1, str2);
    dyntype_t args[] = { list, list2 };

    dyntype_t appended_list = append(args, 2);

    CHECK_TYPE(appended_list, SCHEME_TYPE_PAIR);
    CHECK_TYPE(car(cdr(appended_list)), SCHEME_TYPE_STRING);
    CHECK_TYPE(cdr(cdr(cdr(appended_list))), SCHEME_TYPE_NULL);
    ASSERT_EQ("car is not abc", 0, strcmp("abc", *car(appended_list).data.string_val));
    ASSERT_EQ("cadr is not abc", 0, strcmp("abc", *car(cdr(appended_list)).data.string_val));
    ASSERT_EQ("caddr is not cba", 0, strcmp("cba", *car(cdr(cdr(appended_list))).data.string_val));
    ASSERT_EQ("result is not a list", TRUE, *list_q(appended_list).data.boolean_val);

    return 0;
}

TEST(append_4) {
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t args[] = { str };

    dyntype_t appended_list = append(args, 1);

    CHECK_TYPE(appended_list, SCHEME_TYPE_STRING);
    ASSERT_EQ("does not return string unchanged", 0, strcmp("abc", *appended_list.data.string_val));

    return 0;
}

TEST(append_5) {
    dyntype_t args[] = {SCHEME_UNSPECIFIED};

    dyntype_t appended_list = append(args, 0);

    CHECK_TYPE(appended_list, SCHEME_TYPE_NULL);

    return 0;
}

TEST(append_6) {
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t str2 = scheme_literal_string("cba");
    dyntype_t list = make_list_fill(2, str);
    dyntype_t list2 = make_list_fill(1, str2);
    dyntype_t sym = scheme_literal_symbol((scheme_symbol_t) { .name = "a" });
    dyntype_t args[] = { list, sym, list2 };
    
    TRY {
        dyntype_t appended_list = append(args, 3);
    }
    CATCH{
        CHECK_ERROR_TYPE(global_exception, INTERNAL_TYPE_BAD_ARGUMENT_EXCEPTION);
    }

    return 0;
}

TEST(list_set) {
    dyntype_t replacement = scheme_literal_string("cde");
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t list = make_list_fill(3, str);

    dyntype_t unspec = list_set(list, 1, replacement);

    CHECK_TYPE(unspec, SCHEME_TYPE_UNSPECIFIED);
    ASSERT_EQ("cadr is string cde", 0, strcmp("cde", *car(cdr(list)).data.string_val));

    return 0;
}

TEST(list_set_2) {
    dyntype_t replacement = scheme_literal_string("cde");
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t list = make_list_fill(3, str);

    TRY {
        dyntype_t err = list_set(list, 3, replacement);
    }
    CATCH{
        CHECK_ERROR_TYPE(global_exception, INTERNAL_TYPE_BAD_ARGUMENT_EXCEPTION);
    }

    return 0;
}

TEST(list_set_3) {
    dyntype_t replacement = scheme_literal_string("cde");
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t list = make_list_fill(3, str);
    list._mutable = FALSE;

    TRY {
        dyntype_t err = list_set(list, 3, replacement);
    }
    CATCH {
        CHECK_ERROR_TYPE(global_exception, INTERNAL_TYPE_SETTING_IMMUTABLE_LOCATION);
    }

    return 0;
}

TEST(list_copy) {
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t list = make_list_fill(3, str);

    dyntype_t copied_list = list_copy(list);

    CHECK_TYPE(copied_list, SCHEME_TYPE_PAIR);
    ASSERT_EQ("copied list is not a list",TRUE, *list_q(copied_list).data.boolean_val);
    ASSERT_EQ("cadr is not string abc", 0, strcmp("abc", *car(cdr(copied_list)).data.string_val));

    return 0;

}


static char* all_tests() {
    MU_RUN_TEST(string_to_symbol);
    MU_RUN_TEST(string_to_symbol_wrong_type);
    MU_RUN_TEST(symbol_to_string);
    MU_RUN_TEST(symbol_to_string_wrong_datatype);
    MU_RUN_TEST(symbol_q);
    MU_RUN_TEST(symbol_q_negative);
    MU_RUN_TEST(symbol_eq);
    MU_RUN_TEST(symbol_eq_negative);
    MU_RUN_TEST(symbol_eq_negative_2);
    MU_RUN_TEST(symbol_eq_negative_3);
    MU_RUN_TEST(symbol_eq_wrong_type);
    MU_RUN_TEST(symbol_eq_wrong_type_2);
    MU_RUN_TEST(evaluate_empty_string_to_true);
    MU_RUN_TEST(evaluate_hf_to_false);
    MU_RUN_TEST(boolean_q);
    MU_RUN_TEST(boolean_q_negative);
    MU_RUN_TEST(not);
    MU_RUN_TEST(not_2);
    MU_RUN_TEST(not_3);
    MU_RUN_TEST(boolean_eq);
    MU_RUN_TEST(boolean_eq_one_boolean);
    MU_RUN_TEST(boolean_eq_wrong_datatype);
    MU_RUN_TEST(boolean_eq_negative);
    MU_RUN_TEST(make_list);
    MU_RUN_TEST(make_list_fill);
    MU_RUN_TEST(length);
    MU_RUN_TEST(append);
    MU_RUN_TEST(append_2);
    MU_RUN_TEST(append_3);
    MU_RUN_TEST(append_4);
    MU_RUN_TEST(append_5);
    MU_RUN_TEST(append_6);
    MU_RUN_TEST(list_set);
    MU_RUN_TEST(list_set_2);
    MU_RUN_TEST(list_set_3);
    MU_RUN_TEST(list_copy);
    return 0;
}


#ifdef _TEST_PRIMF
int main(int argc, char** argv) {
    initalize_allocator();

    char* result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
#endif