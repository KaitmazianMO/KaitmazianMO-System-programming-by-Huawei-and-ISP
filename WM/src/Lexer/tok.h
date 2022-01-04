#ifndef TOK_H_INCLUDED
#define TOK_H_INCLUDED

#include <stddef.h>
#include "str_view.h"
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
    StrView   name;
    size_t    nline;
    Value     num;
};

inline Token make_token (TokenType ttype, StrView tname, size_t tnline) {
    return Token { .type = ttype, .name = tname, .nline = tnline, .num = {} };
}


#endif // TOK_H_INCLUDED