#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "lex.h"
#include "../Error/err.h"

int lex_init (Lexer *lex, const char *src) {
    assert (lex);
    
    lex->source_ = src;
    lex->curr_sym_ = src;
    lex->tok_ = {};
    lex->nline_ = 1;
    return 1;
}

inline int curr_sym (Lexer *lex) {
    return *lex->curr_sym_;
}

inline void advance (Lexer *lex, size_t n = 1) {
    lex->curr_sym_ += n;
}

inline size_t nline (Lexer *lex) {
    return lex->nline_;
}

inline void inc_line (Lexer *lex) {
    ++lex->nline_;
}

inline Token tok (Lexer *lex) {
    return lex->tok_;
}

inline Token make_number (Lexer *lex) {
    const char * const nbeg = lex->curr_sym_;
    TokenType type = INCORRECT;

    char *nend = NULL;
    errno = 0;
    decimal_n n = (decimal_n)strtod (nbeg, &nend);
    const size_t nsize = nend - nbeg;
    if (nend && (*nend == '\0' || isspace (*nend) || *nend == ',')) {
        if (errno == ERANGE) {
            type = INCORRECT;
            error ("Too big number in %zu line", nline (lex));
        } else if (memchr (nbeg, '.', nsize)) {
            type = DEC_NUMBER;
        } else {
            type = INT_NUMBER;
        }
    } else { /* extra chars at the end of the number */
        type = INCORRECT;
        error ("Extra chars at the end of the number in %zu line", nline (lex));
    }

    advance (lex, nsize);
    StringView nview = make_string_view (nbeg, nsize); 
    lex->tok_ = make_token (type, nview, nline (lex));
    if (type == INT_NUMBER) {
        lex->tok_.num.integer = dec_to_int (n);
    } else if (type == DEC_NUMBER) {
        lex->tok_.num.decimal = n;
    }
    return tok (lex);
}

inline Token make_identifier (Lexer *lex) {
    const char * const nbeg = lex->curr_sym_;
    size_t nsize = 0;

    if (isalpha (curr_sym (lex)) || curr_sym (lex) == '_') {
        ++nsize; 
        advance (lex);
    }

    while (isalpha (curr_sym (lex)) || isdigit (curr_sym (lex)) || curr_sym (lex) == '_') {
        ++nsize; 
        advance (lex);
    }

    StringView nview = make_string_view (nbeg, nsize); 
    lex->tok_ = make_token (IDENTIFIER, nview, nline (lex));
    return tok (lex);
}

inline Token make_colon (Lexer *lex) {
    lex->tok_ = make_token (
        COLON, make_string_view (lex->curr_sym_, 1),
        nline (lex));
   
    advance (lex);
    return tok (lex);
}

inline void skip_comment (Lexer *lex) {
    lex_skip_line (lex);
}

inline Token make_final (Lexer *lex) {
    lex->tok_ = make_token (
        FINAL, make_string_view (lex->curr_sym_, 1),
        nline (lex));
   
    return tok (lex);
}

inline Token make_incorrect (Lexer *lex) {
    lex->tok_ = make_token (
        INCORRECT, make_string_view (lex->curr_sym_, 1),
        nline (lex));
   
    return tok (lex);
}

Token lex_get_tok (Lexer *lex) {
    assert (lex);
    while (true) {
        switch (curr_sym (lex)) {
            case '\n': {
                inc_line (lex);
                advance (lex);
            } break;

            case '\t': case ' ': {
                advance (lex);
            } break;

            case ':': {
                return make_colon (lex);
            } break;

            case '#': {
                skip_comment (lex);
            } break;
            
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case '+': case '-': {
                return make_number (lex);
            } 

            case '\0': {
                return make_final (lex);
            } 

            default: {
                if (isalpha (curr_sym (lex)) || curr_sym (lex) == '_') { 
                    return make_identifier (lex);
                } else {
                    error ("Grammatically incorrect token at %zu line", nline (lex));
                    return make_incorrect (lex);
                }
            }
        }
    }
}

void lex_skip_line (Lexer *lex) {
    assert (lex);

    auto new_line = strchr (lex->curr_sym_, '\n');
    if (new_line) {
        advance (lex, new_line - lex->curr_sym_);
    } else {
        advance (lex, strlen (lex->curr_sym_));
    }
}