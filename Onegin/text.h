#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

#include "Token.h"
#include "buffer.h"
#define T Token
#include "vector.h"

typedef bool (*token_verifier_t) (Token tok);

enum TEXT_ERRORS
{
    TEXT_SUCCESS = 0,
    TEXT_BAD_FILE = 256, TEXT_ALLOCATION_FAILED,
    TEXT_TOKENIZING_FAILED, TEXT_VECTOR_OF_TOKENS_CONSTRUCTING_FAILED,
    TEXT_DESTRUCTING_FAILED,
};

enum TokenFormat
{
    NOT_NULL_TERMINATED = 0, NULL_TERMINATED  = 1,
};

struct Text
{
    Buffer        buff;
    vector_Token  tokens;
    bool          null_terminated;
};

/*! \brief Construct Text object. Reading all symbols from pfile. */
TEXT_ERRORS text_ctor_by_file (Text *_this, FILE *pfile);

/*! 
 * \brief Tokenizing the text of previously constructed Text object. 
 * \param delim: Text deim symbols.
 * \param null_retm: cahnging last token symbols to '\0' if it's true
 * \param tok_verify: Token verificator. Must return true for valid tokens, invalid tokens will be ignored. 
 */

TEXT_ERRORS text_tokenize (Text *_this, const char *delim, 
                           bool null_term, token_verifier_t tok_verify);
                           
/*! \brief Distructs previously constructed Text object. */
TEXT_ERRORS text_dtor (Text *_this);

#endif