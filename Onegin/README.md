# Onegin
This is a second task in compiler course. 
The task is simple you need to sort big russian text, for example Evgeniy Onegin by Alexander Pushkin. Then you have to write the text in an output file. After that you have to generate new rhymes wich bysed on Evgeniy Onegin text and also dump in a file. At the end you write basic text in a file.
To solve the problem was developed a struct OneginText wich based on struct Text:
```C++
struct OneginText
{
    Text text;
};
```
Onegin Text supports ctor/dtor functions and it also supports 4 function to work with text.
```C++
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
```
The text struct is simple and doesn't support many functions because it basic text working class and if you want to expand functionality of the struct you can follow the OneginText example.
```C++
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
```
