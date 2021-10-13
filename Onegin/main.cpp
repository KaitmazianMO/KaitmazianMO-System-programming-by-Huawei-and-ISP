#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>

#include "onegin_text.h"
#include "../Utilities/Log/log.h"
#include "../Utilities/Log/gcc_trace.h"

int custom_line_comparator (const void *l, const void *r);
int custom_reverse_line_comparator (const void *l, const void *r);
bool verify_line (Token tok);
bool is_chapter_title (const char* str);

LOG_WIHTOUT_TRACE
int main (int argc, char *argv[])
{
    LOG_SET_FILE_NAME ("onegin.log");

    if (argc != 3)
    {
        fprintf (stderr, "Usage: %s <input-file> <output-file>\n", argv[0]);
        exit (1);
    }

    FILE *onegin_text_f = fopen (argv[1], "rb");
    if (!onegin_text_f)
    {
        fprintf (stderr, "Can't open %s\n", argv[1]);    
        exit (1);
    }                                                                             

    int err = 0;
    OneginText onegin_text = {};
    if ((err = onegin_text_ctor (&onegin_text, onegin_text_f, verify_line)) == ONEGIN_TEXT_SUCCESS)
    {  
        err |= onegin_text_sort (&onegin_text, custom_line_comparator);

        FILE *onegin_output_f = fopen (argv[2], "wb");
        if (!onegin_output_f)
        {
            fprintf (stderr, "Can't open %s\n", argv[2]); 
            err |= onegin_text_dtor (&onegin_text); 
            return err;  
        }           

        err |= onegin_text_dump (&onegin_text, onegin_output_f, "Евгений Онегин, отсортированный по возрастанию");
        err |= onegin_text_rhymes_dump (&onegin_text, onegin_output_f, "Евгений Онегин, by Netflix", custom_reverse_line_comparator);
        err |= onegin_text_raw_dump (&onegin_text, onegin_output_f, "Оригинальный Евгений Онегин");

        fclose (onegin_output_f);
    }
    else
    {
        return err;
    }

    err |= onegin_text_dtor (&onegin_text);
    fclose (onegin_text_f);

    return err;
}

int custom_line_comparator (const void *l, const void *r)
{
    const Token *str1 = (const Token *)l;
    const Token *str2 = (const Token *)r;

    assert (str1->beg != NULL);
    assert (str2->beg != NULL);

    size_t i = 0;
    size_t j = 0;
    const size_t min_sz = MIN (str1->size, str2->size);
    for (i = 0, j = 0; i < min_sz && j < min_sz; ++i, ++j)
    {
        if (isgraph (*(str1->beg + i)))
        {
            i++;
            if (isspace (*(str1->beg + i)))
                i++;
        }

        if (isgraph (*(str2->beg + j)))
        {
            j++;
            if (isspace (*(str2->beg + j)))
                j++;
        }

        if (*(str1->beg + i) != *(str2->beg + j))
            return *(str1->beg + i) - *(str2->beg + j);
    }

    return *(str1->beg + i) - *(str2->beg + j);
}

int custom_reverse_line_comparator (const void *l, const void *r)
{
    assert (l);
    assert (r);

    const Token *str1 = (const Token *)l;
    const Token *str2 = (const Token *)r;

    assert (str1->beg != NULL);
    assert (str2->beg != NULL);

    size_t i = str1->size;
    size_t j = str2->size;
    for(; i > 0 && j > 0; --i, --j)
        {
        while (isprint (*(str1->beg + i)) || isdigit (*(str1->beg + i))) --i;
        while (isprint (*(str2->beg + j)) || isdigit (*(str2->beg + j))) --j;

        if (*(str1->beg + i) != *(str2->beg + j))
            return *(str1->beg + i) - *(str2->beg + j);
        }

    return *(str1->beg + i) - *(str2->beg + j);    
}

bool verify_line (Token tok)
{
    return !is_chapter_title (tok.beg);
}

bool is_chapter_title (const char* str)
{
    for (size_t i = 0; *(str + i) != '\n' && *(str + i) != 0; ++i)
    {
        if (isspace (*(str + i))) continue;

		if (str[i] == '\xD0' && str[i + 1] >= '\x90' && str[i + 1] <= '\xAF')
		{
			++i;
			continue;
		}

        if ( !((str[i] >= 'A' && str[i] <= 'Z') || ispunct (str[i])))
            return false;
    }

    return true;    
}