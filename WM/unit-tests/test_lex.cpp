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
        rand_str += separators[rand(0, n_chars - 1)];
    }
}

LEX_TEST (TokenizingTest) {
    const std::string tokens[] = {
        "push", "R1", 
        "pop", "R2",
        "add", "R3", "R1", "5"
        "jmp", "solve_problems"
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

#undef LEX_TES

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}