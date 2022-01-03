#include <gtest/gtest.h>
#include <string>
#include <cstdlib>

#include "lex.h"

#define LEX_TEST( name )  TEST (LexerTest, name)
#define ARR_SIZE( arr )   (sizeof(arr)/sizeof(arr[0]))

size_t rand (size_t from, size_t to) {
    return (rand() % (to)) + from;
}   

std::string rand_string (const char *separators, size_t n_chars, size_t lenght) {
    std::string rand_str;
    for (size_t i = 0; i < lenght; ++i) {
        rand_str += std::string (1, separators[rand(0, n_chars - 1)]);
    }
}

LEX_TEST (Tokenizing) {
    const std::string tokens[] = {
        "push", "R1", 
        "pop",  "R2",
        "add",  "R3", "R1", "5"
        "call", "solve_problems"
    };

    const char separators[] = {
        ' ' , '\t', '\n', '\r'
    };

    const int weird_number = 1000;
    for (int i = 0; i < weird_number; ++i) {
        std::string code = ""; 
        for (size_t i = 0; i < ARR_SIZE (tokens); ++i) {
            code += rand_string (separators, ARR_SIZE (separators), rand (1, weird_number));
            code += tokens[i];
        }

        Lexer lex = {};
        ASSERT_NE (lex_init (&lex, code.c_str()), 0) << "Lexer intialization failed";
    
        for (int i = 0; i < ARR_SIZE (tokens); ++i) {
            Token tok = lex_get_tok (&lex);
            ASSERT_EQ (strcmp (tok.name.beg_, tokens[i].c_str()), 0) << "Wrong token";
        }        
    }
}

LEX_TEST (LineCounting) {
    const std::string code = 
/* 1 */    "push R1\n"
/* 2 */    "pop R2\n"
/* 3 */    "add R3 R1 5\n"
/* 4 */    "\n" 
/* 5 */    "call solve_problems\n"
/* 6,7 */  "\n \n "
/* 8 */    ".";
    

    const int nlines[] = {
        1, 1,
        2, 2,
        3, 3, 3, 3,
        5, 5,
        8
    };

    Lexer lex = {};
    ASSERT_NE (lex_init (&lex, code.c_str()), 0) << "Lexer intialization failed";

    int i = 0;
    for (Token tok = lex_get_tok (&lex); tok.type != FINAL; tok = lex_get_tok (&lex)) {
        ASSERT_EQ (tok.nline, nlines[i++]) << "Line number was counted incorrectly";
    }
    ASSERT_EQ (i-1, ARR_SIZE (nlines)) << "Not all tokens have been processed";
}

LEX_TEST (TypeRecognizing) {
    const std::string code = 
/* 1 */    "push 0\n"
/* 2 */    "pop R2\n"
/* 3 */    "add R3 1 5.0\n"
/* 4 */    "\n" 
/* 5 */    "call solve_problems\n"
/* 6,7 */  "\n \n "
/* 8 */    ".";

    TokenType types[] = {
        IDENTIFIER, INT_NUMBER,
        IDENTIFIER, IDENTIFIER,
        IDENTIFIER, IDENTIFIER, INT_NUMBER, DEC_NUMBER,
        IDENTIFIER, IDENTIFIER,
        IDENTIFIER
    };

    Lexer lex = {};
    ASSERT_NE (lex_init (&lex, code.c_str()), 0) << "Lexer intialization failed";

    int i = 0;
    for (Token tok = lex_get_tok (&lex); tok.type != FINAL; tok = lex_get_tok (&lex)) {
        ASSERT_EQ (tok.type, types[i++]) << "Type was recognized incorrectly";
    }
    ASSERT_EQ (i-1, ARR_SIZE (types)) << "Not all tokens have been processed";
}

LEX_TEST (NumberRecognizing) {
    std::string int_code = 
        "1    2    3    4    5 "
        "-1   -2   -3   -4   -5 ";

    int int_values[] = {
        1,  2,  3,  4,  5,
       -1, -2, -3, -4, -5,
    };

    std::string double_code =
        "1.0  2.0  3.0  4.0  5.0 "
        "1.5  2.5  3.5  4.5  5.5 ";

    double double_values[] = {
        1.,  2.,  3.,  4.,  5.,
        1.5, 2.5, 3.5, 4.5, 5.5,
    };

    Lexer int_lex = {};
    Lexer double_lex = {};
    ASSERT_NE (lex_init (&int_lex, int_code.c_str()), 0) << "Lexer intialization failed";
    ASSERT_NE (lex_init (&double_lex, double_code.c_str()), 0) << "Lexer intialization failed";

    {
        int i = 0;
        for (Token tok = lex_get_tok (&int_lex); tok.type != FINAL; tok = lex_get_tok (&int_lex)) {
            ASSERT_EQ (tok.num.integer, int_values[i++]) << "Integer number was recognized incorrectly";
        }
        ASSERT_EQ (i-1, ARR_SIZE (int_values)) << "Not all tokens have been processed";
    }

    {
        int i = 0;
        for (Token tok = lex_get_tok (&double_lex); tok.type != FINAL; tok = lex_get_tok (&double_lex)) {
            ASSERT_EQ (tok.num.decimal, double_values[i++]) << "Decimal number was recognized incorrectly";
        }
        ASSERT_EQ (i-1, ARR_SIZE (int_values)) << "Not all tokens have been processed";
    }
}

#undef LEX_TES

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}