#ifndef TOK_H_INCLUDED
#define TOK_H_INCLUDED

#include <stddef.h>
#include "../../Utilities/Strings/StringView.h"
#include "../Object/val.h"

enum TokenType {
    INT_NUMBER,
    DEC_NUMBER,
    IDENTIFIER,
    COLON, // ':'
    FINAL, // '\0'
    INCORRECT
};

struct Token {
    TokenType type;
    StringView name;
    size_t nline;
    union {
        decimal_n decimal;
        integer_n integer;    
    } num;
};

inline Token make_token (TokenType ttype, StringView tname, size_t tnline) {
    return Token { .type = ttype, .name = tname, .nline = tnline, .num = {} };
}


#endif // TOK_H_INCLUDED