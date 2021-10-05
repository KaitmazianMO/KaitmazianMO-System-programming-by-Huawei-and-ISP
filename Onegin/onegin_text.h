#ifndef ONEGIN_TEXT_H_INCLUDED
#define ONEGIN_TEXT_H_INCLUDED

#include <stdio.h>

#include "text.h"

enum ONEGIN_TEXT_ERROR
{
    ONEGIN_TEXT_SUCCESS = 0,
    ONEGIN_TEXT_CONSTRUCTING_FAILED = 256, ONEGIN_TEXT_DESTRUCTING_FAILED,
    ONEGIN_TEXT_TOKENIZING_FAILED, ONEGIN_TEXT_RHYME_PRINTING_ERROR,
    ONEGIN_TEXT_SOMETHING_FAILED_I_DO_NOT_KNOW_WHAT_BUT_AM_SURE_YOU_ARE_A_CLEVER_BOY_SO_YOU_MUST_KNOW_WHAT_HAPPEND,
};

typedef int (*token_comparator_t) (const void *l, const void *r);

/*!
 * \brief: Specific struct for sorting text and generate new rhymes by using text with rhymes. 
 *         Based on struct text.
 */
struct OneginText
{
    Text text;
};

/*!
 * \brief OneginText constructor, must be used when creating an OneginText object 
 *        tokenizing text with ' ' and '\n' as delims and ignore tokens 
 *        that are not verified by tok_verify function.
 */
ONEGIN_TEXT_ERROR onegin_text_ctor (OneginText *_this, FILE *finput, token_verifier_t tok_verify);

/*!
 *  \brief Sorts OneginText tokens.
 *  \param cmp:  may be NULL if you want to use default comparator based on strcmp or 
 *               you can write your own custom comparator.
 */
ONEGIN_TEXT_ERROR onegin_text_sort (OneginText *_this, token_comparator_t cmp);

/*!
 *  \brief Prints rhymes using OneginText in fout.
 *  \param cmp:  may be NULL if you want to use default reverse Token comparator or 
 *               you can write your own custom reverse comparator.
 *  \param title: framed title befor rhyme dump.
 *  \note  After call OneginText will be sorted by reverse comporator, because the rhyme printing alorithm need it. 
 */
ONEGIN_TEXT_ERROR onegin_text_rhymes_dump (OneginText *_this, FILE *fout, const char *title, token_comparator_t cmp);

/*! \brief Dumping OneginText tekens under the framed title. */
ONEGIN_TEXT_ERROR onegin_text_dump (OneginText *_this, FILE *fout, const char *title);

ONEGIN_TEXT_ERROR onegin_text_raw_dump (OneginText *_this, FILE *fout, const char *title);

/*! \brief OneginText destructor, must be used when destructing an OneginText object */
ONEGIN_TEXT_ERROR onegin_text_dtor (OneginText *_this);

#endif