#include <assert.h>
#include <errno.h>
#include <string.h>

#include "text.h"
#include "../Utilities/stdlib_addition.h"    

#include "../Utilities/Log/log.h"
#include "../Utilities/Log/gcc_trace.h"

Token get_token (char *text, const char *delim);

TEXT_ERRORS text_ctor_by_file (Text *_this, FILE *pfile)
{
    assert (_this);    
    assert (pfile);

    if (buf_ctor_by_file (&_this->buff, pfile) != BUFF_SUCCESS)
    {
        if (allocation_error (errno))
            return TEXT_ALLOCATION_FAILED;
        else 
            return TEXT_BAD_FILE;
    }

    if (vec_ctor_Token (&_this->tokens, 0) != VEC_SUCCESS)
    {
        return TEXT_VECTOR_OF_TOKENS_CONSTRUCTING_FAILED;
    }

    return TEXT_SUCCESS;
}

TEXT_ERRORS text_tokenize (Text *_this, const char *delim, bool null_term, token_verifier_t tok_verify)
{
    assert (_this);
    assert (delim);

    LOG_MSG (LOG, "START TOKENISZING");

    _this->null_terminated = null_term;

    for (Token curr_tok = get_token (_this->buff.data, delim); 
        curr_tok.beg && *curr_tok.beg; 
        curr_tok = get_token (curr_tok.beg + curr_tok.size + 1, delim)) 
    {
        if (null_term)
            curr_tok.beg[curr_tok.size] = 0;

        if ((tok_verify && tok_verify (curr_tok)) || tok_verify == NULL)
        {
            if (vec_push_back_Token (&_this->tokens, curr_tok) != VEC_SUCCESS)
                return TEXT_TOKENIZING_FAILED;
        }
    };

    return TEXT_SUCCESS;
}

TEXT_ERRORS text_dtor (Text *_this)
{
    assert (_this);

    int err = 0;
    err |= buf_dtor (&_this->buff);
    err |= vec_dtor_Token (&_this->tokens);
    
    return err ? TEXT_DESTRUCTING_FAILED : TEXT_SUCCESS;
}

Token get_token (char *text, const char *delim)
{
    assert (text);
    assert (delim);

    Token tok {
        .beg = text,
        .size = 0
    };

    tok.beg += strspn (tok.beg, delim);
    tok.size = strcspn (tok.beg, delim);
    
    return tok;
}