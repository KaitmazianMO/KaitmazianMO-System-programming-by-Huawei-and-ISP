#include "onegin_text.h"
#include "../Utilities/Log/gcc_trace.h"
#include "../Utilities/stdlib_addition.h"

#include <string.h>
#include <time.h>

static int  default_line_comparator         (const void *l, const void *r);
static int  default_reverse_line_comparator (const void *l, const void *r);
static void write_nlines_to_file            (vector_Token *tokens, size_t nLines, FILE *file);
static void print_title                     (FILE* file, const char *str);
size_t      print_4_lines                   (FILE *file, Token tok1, Token tok2, Token tok3, Token tok4);
static bool print_rhymes                    (vector_Token *tokens, FILE *file);

ONEGIN_TEXT_ERROR onegin_text_ctor (OneginText *_this, FILE *finput, token_verifier_t tok_verify)
{
    assert (_this);
    assert (finput);

    if (text_ctor_by_file (&_this->text, finput))
        return ONEGIN_TEXT_CONSTRUCTING_FAILED;

    if (text_tokenize (&_this->text, "\n", NULL_TERMINATED, tok_verify))
    {
        return ONEGIN_TEXT_TOKENIZING_FAILED;
    }

    return ONEGIN_TEXT_SUCCESS;
}

ONEGIN_TEXT_ERROR onegin_text_sort (OneginText *_this, token_comparator_t cmp)
{
    assert (_this);

    qsort (_this->text.tokens.data.data, 
    _this->text.tokens.size, sizeof (Token), 
    cmp ? cmp : default_line_comparator);

    return ONEGIN_TEXT_SUCCESS;
}

ONEGIN_TEXT_ERROR onegin_text_rhymes_dump (OneginText *_this, FILE *fout, const char *title, token_comparator_t cmp)
{
    assert (_this);

    qsort (_this->text.tokens.data.data, 
    _this->text.tokens.size, sizeof (Token), 
    cmp ? cmp : default_reverse_line_comparator);

    print_title (fout, title);
    return !print_rhymes (&_this->text.tokens, fout) ?
     ONEGIN_TEXT_SUCCESS : ONEGIN_TEXT_RHYME_PRINTING_ERROR;
}

ONEGIN_TEXT_ERROR onegin_text_dump (OneginText *_this, FILE *fout, const char *title)
{
    assert (_this);
    assert (fout);
    assert (title);

    print_title (fout, title);
    write_nlines_to_file (&_this->text.tokens, _this->text.tokens.size, fout);

    return ONEGIN_TEXT_SUCCESS;
}

ONEGIN_TEXT_ERROR onegin_text_raw_dump (OneginText *_this, FILE *fout, const char *title)
{
    assert (_this);
    assert (fout);
    assert (title);

    print_title (fout, title);
    for (size_t i = 0; i < _this->text.buff.capacity; ++i)
    {
        if (_this->text.buff.data[i] != '\0')
            fputc (_this->text.buff.data[i], fout);
        else
            fputc ('\n', fout);
    }
    //dump_buff (_this->text.buff.data, _this->text.buff.capacity, fout)
    return ONEGIN_TEXT_SUCCESS;
}

ONEGIN_TEXT_ERROR onegin_text_dtor (OneginText *_this)
{
    if (text_dtor (&_this->text))
        return ONEGIN_TEXT_DESTRUCTING_FAILED;

    return ONEGIN_TEXT_SUCCESS;
}

int default_line_comparator (const void *l, const void *r)
{
    assert (l);
    assert (r);

    const Token *line1 = ((const Token *)l);
    const Token *line2 = ((const Token *)r);

    assert (line1->beg != NULL);
    assert (line2->beg != NULL);

    return strcmp (line1->beg, line2->beg);
}

static int default_reverse_line_comparator (const void *l, const void *r)
{
    assert (l);
    assert (r);

    const Token *line1 = ((const Token *)l);
    const Token *line2 = ((const Token *)r);

    assert (line1->beg != NULL);
    assert (line2->beg != NULL);  

    assert (false && "Does not support default_reverse_line_comparator");

    return 1;
}

static void write_nlines_to_file (vector_Token *tokens, size_t n_lines, FILE *file)
{
   assert (file != NULL);
   assert (tokens  != NULL);

   const size_t writable = MIN (n_lines, tokens->size); 
   for (size_t i = 0; i < writable; ++i)
   {
       Token tok = {};
       vec_get_elem_Token (tokens, i, &tok);
       fprintf (file, "%.*s\n", (int)tok.size, tok.beg);
   }
}

static void print_title (FILE* file, const char *str)
{
    #define PrintLine(sym) for (size_t i = 0; i < 100; ++i)     \
                               {                                \
                               fprintf (file, "%c", (sym));     \
                                   if (i == 99)                 \
                                       fprintf (file, "\n");    \
                               }

    fprintf (file, "\n\n\n");
    PrintLine('-')
    PrintLine('|')
    PrintLine('|')
    PrintLine('-')

    fprintf (file, "\n\n\n\n\n\n%60s\n\n\n\n\n\n", str);

    PrintLine('-')
    PrintLine('|')
    PrintLine('|')
    PrintLine('-')
    fprintf (file, "\n\n\n");

    #undef PrintLine
}

static bool print_rhymes (vector_Token *tokens, FILE *file)
{
    assert(tokens);
    assert(file);

    size_t str_num = 0; //< сколько строк было записано

    srand ((unsigned int)time (0));

    size_t i = rand() % (tokens->size - 2);  //< индекс для первой строки
    size_t j = rand() % (tokens->size - 2);  //< индекс для второй строки

    bool err = false;
    size_t step = 14;
    Token tok1 = {}, tok2 = {}, tok3 = {}, tok4 = {};
    for (; str_num < tokens->size; i = (i*j + tokens->size/step) % (tokens->size - 2),
                                   j = (i*j + tokens->size/step) % (tokens->size - 2))
    {
        if (str_num % step == 0 )
        {
            if (!(vec_get_elem_Token (tokens, i,     &tok1) ||
                  vec_get_elem_Token (tokens, j,     &tok2) ||
                  vec_get_elem_Token (tokens, i + 2, &tok3) ||
                  vec_get_elem_Token (tokens, j + 2, &tok4)))
                str_num += print_4_lines (file, tok1, tok2, tok3, tok4);
            else
            {
                err = true;
                break;
            }
        }

        else if (str_num % step == 4 )
        {
            if (!(vec_get_elem_Token (tokens, i,     &tok1) ||
                  vec_get_elem_Token (tokens, i + 2, &tok2) ||
                  vec_get_elem_Token (tokens, j,     &tok3) ||
                  vec_get_elem_Token (tokens, j + 2, &tok4)))
                str_num += print_4_lines (file, tok1, tok2, tok3, tok4);   
            else
            {
                err = true;
                break;
            }                        
        }

        else if (str_num % step == 8 )
        {
            if (!(vec_get_elem_Token (tokens, i,     &tok1) ||
                  vec_get_elem_Token (tokens, j,     &tok2) ||
                  vec_get_elem_Token (tokens, j + 2, &tok3) ||
                  vec_get_elem_Token (tokens, i + 2, &tok4)))
                str_num += print_4_lines (file, tok1, tok2, tok3, tok4);        
            else
            {
                err = true;
                break;
            }                 
        }

        else if (str_num % step == 12)
        {
            if (!(vec_get_elem_Token (tokens, i,     &tok1) || 
                  vec_get_elem_Token (tokens, i + 2, &tok2)))
                str_num += print_4_lines (file, tok1, tok2, {.beg = NULL, .size = 0}, {.beg = NULL, .size = 0});         
            else
            {
                err = true;
                break;
            }                  
        }
    }

    return err;
}

size_t print_4_lines (FILE *file, Token tok1, Token tok2, Token tok3, Token tok4)
{
    #define PRINT_LINE( tok )  if(tok.beg == NULL)                                    \
                                    fprintf(file, "\n");                              \
                                else                                                  \
                                {                                                     \
                                    fprintf (file, "%.*s\n", (int)tok.size, tok.beg); \
                                    ++wrote;                                          \
                                }

    size_t wrote = 0;

    PRINT_LINE (tok1)
    PRINT_LINE (tok2)
    PRINT_LINE (tok3)
    PRINT_LINE (tok4)

    #undef PRINT_LINE

    return wrote;
}