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
    ASSERT_EQ("Symbol name != nicesymbol",0,strcmp(*sym_dyn.data.symbol_val, str));
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
    dyntype_t dyn1 = scheme_literal_symbol( "nicesymbol");
    dyntype_t dyn2 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyn3 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean != true", TRUE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq_negative) {
    dyntype_t dyn1 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyn2 = scheme_literal_symbol("nicesymbol2");
    dyntype_t dyn3 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean != false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq_negative_2) {
    dyntype_t dyn1 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyn2 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyn3 = scheme_literal_symbol("nicesymbol2");
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean != false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq_negative_3) {
    dyntype_t dyn1 = scheme_literal_symbol("nicesymbol1");
    dyntype_t dyn2 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyn3 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean != false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq_wrong_type) {
    dyntype_t dyn1 = scheme_literal_string("nicesymbol");
    dyntype_t dyn2 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyn3 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyns[] = {dyn1, dyn2, dyn3};

    dyntype_t boolean_dyn = symbol_eq(dyns, 3);

    CHECK_TYPE(boolean_dyn, SCHEME_TYPE_BOOLEAN);
    ASSERT_EQ("Boolean is not false", FALSE, *boolean_dyn.data.boolean_val);
    ASSERT_EQ("Boolean is not mutable", TRUE, boolean_dyn._mutable);

    return 0;
}

TEST(symbol_eq_wrong_type_2) {
    dyntype_t dyn1 = scheme_literal_symbol("nicesymbol");
    dyntype_t dyn2 = scheme_literal_symbol("nicesymbol");
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
    dyntype_t sym = scheme_literal_symbol( "a" );
    dyntype_t args[] = { list, sym };

    dyntype_t appended_list = append(args, 2);

    CHECK_TYPE(appended_list, SCHEME_TYPE_PAIR);
    CHECK_TYPE(car(cdr(appended_list)), SCHEME_TYPE_STRING);
    CHECK_TYPE(cdr(cdr(appended_list)), SCHEME_TYPE_SYMBOL);
    ASSERT_EQ("car is not string abc", 0,strcmp("abc", *car(appended_list).data.string_val));
    ASSERT_EQ("cadr is not string abc", 0, strcmp("abc", *car(cdr(appended_list)).data.string_val));
    ASSERT_EQ("caddr is not symbol a", 0, strcmp("a", *cdr(cdr(appended_list)).data.symbol_val));

    return 0;
}

TEST(append_2) {
    dyntype_t str = scheme_literal_string("abc");
    dyntype_t str2 = scheme_literal_string("cba");
    dyntype_t list = make_list_fill(2, str);
    dyntype_t list2 = make_list_fill(1, str2);
    dyntype_t sym = scheme_literal_symbol("a" );
    dyntype_t args[] = { list, list2, sym };

    dyntype_t appended_list = append(args, 3);

    CHECK_TYPE(appended_list, SCHEME_TYPE_PAIR);
    CHECK_TYPE(car(cdr(appended_list)), SCHEME_TYPE_STRING);
    CHECK_TYPE(cdr(cdr(cdr(appended_list))), SCHEME_TYPE_SYMBOL);
    ASSERT_EQ("car is not string abc",0,strcmp("abc", *car(appended_list).data.string_val));
    ASSERT_EQ("cadr is not string abc", 0, strcmp("abc", *car(cdr(appended_list)).data.string_val));
    ASSERT_EQ("caddr is not cba", 0, strcmp("cba", *car(cdr(cdr(appended_list))).data.string_val));
    ASSERT_EQ("cdddr is not symbol a", 0, strcmp("a", *cdr(cdr(cdr(appended_list))).data.symbol_val));

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
    dyntype_t sym = scheme_literal_symbol( "a" );
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

/*
TEST(integer_plus) {
    scheme_integer_t a = from_unsigned_int64(0x4000000000000000);
    scheme_integer_t b = from_unsigned_int64(0x4000000000000000);

    scheme_integer_t c = integer_plus(a, b);
    ASSERT_EQ("first block is not zero", 0u, c.block[0]);
    ASSERT_EQ("last block is not one", 0x8000000000000000, c.block[1]);
}

TEST(integer_plus_negative) {
    scheme_integer_t a = from_unsigned_int64(0b0010110100001111001011110011101010001001110000111110011010101101); // 3246865784823342765
    scheme_integer_t b = from_unsigned_int64(0b1111111111010000111001010101000000100110111110000100001101000110); // -13258666468162746

    scheme_integer_t c = integer_plus(a, b); // should be 3233607118355180019
    ASSERT_EQ("first block is not zero", 0u, c.block[0]);
    ASSERT_EQ("last block is not one", 0b0010110011100000000101001000101010110000101111000010100111110011, c.block[1]);
}

TEST(integer_plus_2) {
    scheme_integer_t a = from_unsigned_int64(0b0111111101001111001011110011101010001001110000111110011010101101); // 9173602894442915501
    scheme_integer_t b = from_unsigned_int64(0b0000110010111011000111100101001010100111011000000110001111001100); // 917360289444291532

    scheme_integer_t c = integer_plus(a, b); // should be 10090963183887207033
    ASSERT_EQ("first block is not zero", 0u, c.block[0]);
    ASSERT_EQ("last block is not one", 0b1000110000001010010011011000110100110001001001000100101001111001, c.block[1]);
}

TEST(integer_plus_3) {
    scheme_integer_t a = from_unsigned_int64(0b0111111101001111001011110011101010001001110000111110011010101101); // 9173602894442915501
    uint64_t b_arr[] = { 0b0000000000000000000000000000000000100000100110110000110010010010, 0b0101001001110100010101011100010101100101110100010100101000110000 };
    scheme_integer_t b = from_int_array(b_arr, 2); // 10090963183887207033323211312

    scheme_integer_t c = integer_plus(a, b); // should be 10090963193060809927766126813
    ASSERT_EQ("first block is not zero", 0u, c.block[0]);                                
    ASSERT_EQ("second block is not expected binary", 0b0000000000000000000000000000000000100000100110110000110010010010, c.block[1]);
    ASSERT_EQ("last block is not expected binary", 0b1101000111000011100001001111111111101111100101010011000011011101, c.block[2]);
}

TEST(integer_subtract) {
    scheme_integer_t a = from_unsigned_int64(0b0000000000000000000000000000000000000000000000000000000000101000); // 40
    scheme_integer_t b = from_unsigned_int64(0b0000000000000000000000000000000000000000000000000000000000000111); // 7

    scheme_integer_t c = integer_subtract(a, b); // should be 33
    ASSERT_EQ("first block is not zero", 0u, c.block[0]);
    ASSERT_EQ("last block is not 33", 0b0000000000000000000000000000000000000000000000000000000000100001, c.block[1]);
}
*/

TEST(tommath_negative_division) {
    mp_int a,b,c,d;
    if (mp_init_multi(&a, &b, &c, &d, NULL) != MP_OKAY) {
        MU_ASSERT("Number initialisation failed", FALSE);
    }

    mp_set_i64(&a, 5);
    mp_set_i64(&b, -2);


    mp_div(&a, &b, &c, &d);

    char c_str[200];
    char d_str[200];

    mp_to_radix(&c, c_str, 200, NULL, 10);
    mp_to_radix(&d, d_str, 200, NULL, 10);

    printf("c: %s, d: %s\n", c_str, d_str);

    mp_clear_multi(&a, &b, &c, &d, NULL);

    return 0;
}


TEST(rational_to_real) {
    mp_int a, b;
 
    if (mp_init_multi(&a, &b, NULL) != MP_OKAY) {
        MU_ASSERT("Number initialisation failed", FALSE);
    }
    
    // 4253529586511731/21267647932558653966460912964485513216
    mp_set_i64(&a, -4253529586511731);
    mp_read_radix(&b, "21267647932558653966460912964485513216", 10);
    //mp_set_u64(&b, 3);

    scheme_rational_t rat = {
        .numerator = a,
        .denominator = b
    };

    scheme_real_t res = rational_to_real(rat);

    char str[35];
    sprintf(str, "%.30lf", res);

    mp_clear_multi(&a, &b, NULL);

    ASSERT_EQ("result string does not match", 0, strcmp(str, "-0.000000000000000000000200000000"));

    return 0;
}


TEST(real_to_rational) {

    scheme_rational_t res = real_to_rational(-0.34);
    char d[100], n[100];

    mp_to_radix(&res.denominator, d, 100, NULL, 10);
    mp_to_radix(&res.numerator, n, 100, NULL, 10);

    ASSERT_EQ("numerator is wrong", 0, strcmp(n, "-6124895493223875"));
    ASSERT_EQ("denominator is wrong", 0, strcmp(d, "18014398509481984"));

    mp_clear_multi(&res.denominator, &res.numerator, NULL);

    return 0;
}

TEST(real_to_rational_overflow_check) {
    scheme_rational_t res = real_to_rational(0.000000000000000000000002);
    char d[100], n[100];

    mp_to_radix(&res.denominator, d, 100, NULL, 10);
    mp_to_radix(&res.numerator, n, 100, NULL, 10);

    ASSERT_EQ("numerator is wrong", 0, strcmp(n, "5444517870735015"));
    ASSERT_EQ("denominator is wrong", 0, strcmp(d, "2722258935367507707706996859454145691648"));

    mp_clear_multi(&res.denominator, &res.numerator, NULL);

    return 0;
}

TEST(rational_add) {
    mp_int a, b, c, d;
    if (mp_init_multi(&a, &b, &c, &d, NULL) != MP_OKAY) {
        MU_ASSERT("Number initialisation failed", FALSE);
    }

    mp_set_i64(&a, -20);
    mp_set_i64(&b, 6);
    mp_set_i64(&c, 9);
    mp_set_i64(&d, 4);

    scheme_rational_t n1 = {
        .numerator = a,
        .denominator = b
    };

    scheme_rational_t n2 = {
        .numerator = c,
        .denominator = d
    };

    scheme_rational_t res = rational_add(n1, n2);

    char ds[100], ns[100];

    mp_to_radix(&res.denominator, ds, 100, NULL, 10);
    mp_to_radix(&res.numerator, ns, 100, NULL, 10);

    ASSERT_EQ("numerator is wrong", 0, strcmp(ns, "-13"));
    ASSERT_EQ("denominator is wrong", 0, strcmp(ds, "12"));

    mp_clear_multi(&a, &b, &c, &d, NULL);

    return 0;
}


TEST(rational_sub) {
    mp_int a, b, c, d;
    if (mp_init_multi(&a, &b, &c, &d, NULL) != MP_OKAY) {
        MU_ASSERT("Number initialisation failed", FALSE);
    }

    mp_set_i64(&a, -20);
    mp_set_i64(&b, 6);
    mp_set_i64(&c, 9);
    mp_set_i64(&d, 4);

    scheme_rational_t n1 = {
        .numerator = a,
        .denominator = b
    };

    scheme_rational_t n2 = {
        .numerator = c,
        .denominator = d
    };

    scheme_rational_t res = rational_sub(n1, n2);

    char ds[100], ns[100];

    mp_to_radix(&res.denominator, ds, 100, NULL, 10);
    mp_to_radix(&res.numerator, ns, 100, NULL, 10);


    ASSERT_EQ("numerator is wrong", 0, strcmp(ns, "-67"));
    ASSERT_EQ("denominator is wrong", 0, strcmp(ds, "12"));

    mp_clear_multi(&a, &b, &c, &d, NULL);

    return 0;
}


TEST(rational_mul) {
    mp_int a, b, c, d;
    if (mp_init_multi(&a, &b, &c, &d, NULL) != MP_OKAY) {
        MU_ASSERT("Number initialisation failed", FALSE);
    }

    mp_set_i64(&a, -20);
    mp_set_i64(&b, 6);
    mp_set_i64(&c, 9);
    mp_set_i64(&d, 4);

    scheme_rational_t n1 = {
        .numerator = a,
        .denominator = b
    };

    scheme_rational_t n2 = {
        .numerator = c,
        .denominator = d
    };

    scheme_rational_t res = rational_mul(n1, n2);

    char ds[100], ns[100];

    mp_to_radix(&res.denominator, ds, 100, NULL, 10);
    mp_to_radix(&res.numerator, ns, 100, NULL, 10);


    ASSERT_EQ("numerator is wrong", 0, strcmp(ns, "-15"));
    ASSERT_EQ("denominator is wrong", 0, strcmp(ds, "2"));

    mp_clear_multi(&a, &b, &c, &d, NULL);

    return 0;
}


TEST(rational_div) {
    mp_int a, b, c, d;
    if (mp_init_multi(&a, &b, &c, &d, NULL) != MP_OKAY) {
        MU_ASSERT("Number initialisation failed", FALSE);
    }

    mp_set_i64(&a, -20);
    mp_set_i64(&b, 6);
    mp_set_i64(&c, 9);
    mp_set_i64(&d, 4);

    scheme_rational_t n1 = {
        .numerator = a,
        .denominator = b
    };

    scheme_rational_t n2 = {
        .numerator = c,
        .denominator = d
    };

    scheme_rational_t res = rational_div(n1, n2);

    char ds[100], ns[100];

    mp_to_radix(&res.denominator, ds, 100, NULL, 10);
    mp_to_radix(&res.numerator, ns, 100, NULL, 10);


    ASSERT_EQ("numerator is wrong", 0, strcmp(ns, "-40"));
    ASSERT_EQ("denominator is wrong", 0, strcmp(ds, "27"));

    mp_clear_multi(&a, &b, &c, &d, NULL);

    return 0;
}

TEST(integer_import) {
    mp_int a;
    if (mp_init(&a) != MP_OKAY) {
        MU_ASSERT("Number initialisation failed", FALSE);
    }

    char inp[] = { 0, 0b01011001, 0b00100000 };
    mp_from_sbin(&a, inp, 3);

    char out[200];

    mp_to_radix(&a, out, 200, NULL, 10);

    ASSERT_EQ("number was not imported correctly", 0, strcmp(out, "22816"));

    return 0;
}


TEST(integer_import_negative) {
    mp_int a;
    if (mp_init(&a) != MP_OKAY) {
        MU_ASSERT("Number initialisation failed", FALSE);
    }

    char inp[] = { 1, 0b01011001, 0b00100000 };
    mp_from_sbin(&a, inp, 3);

    char out[200];

    mp_to_radix(&a, out, 200, NULL, 10);

    ASSERT_EQ("number was not imported correctly", 0, strcmp(out, "-22816"));

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
    //MU_RUN_TEST(integer_plus);
    //MU_RUN_TEST(integer_plus_2);
    //MU_RUN_TEST(integer_plus_3);
    //MU_RUN_TEST(integer_plus_negative);
    //MU_RUN_TEST(integer_subtract);
    //MU_RUN_TEST(tommath_negative_division);
    MU_RUN_TEST(rational_to_real);
    MU_RUN_TEST(real_to_rational);
    MU_RUN_TEST(real_to_rational_overflow_check);
    MU_RUN_TEST(rational_add);
    MU_RUN_TEST(rational_sub);
    MU_RUN_TEST(rational_mul);
    MU_RUN_TEST(rational_div);
    MU_RUN_TEST(integer_import);
    MU_RUN_TEST(integer_import_negative);
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