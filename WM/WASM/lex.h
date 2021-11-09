#ifndef LEX_H_INCLUDED
#define LEX_H_INCLUDED

#include "tok.h"
#include <stddef.h>

struct Lexer {
    const char *source_;
    const char *curr_sym_;
    Token tok_;
    size_t nline_;
};

int lex_init (Lexer *lex, const char *src);
Token lex_get_tok (Lexer *lex);
/* in case of error you can skip error line to avoid tied errors */
void lex_skip_line (Lexer *lex);

#endif // LEX_H_INCLUDED