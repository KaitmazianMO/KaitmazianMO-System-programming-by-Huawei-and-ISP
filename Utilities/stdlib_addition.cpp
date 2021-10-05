#ifdef __unix__
#include <unistd.h>
#ifdef _POSIX_VERSION
#include <sys/stat.h>
#endif
#endif
#include <assert.h>

#include <stdlib.h>
#include <errno.h>

#include "stdlib_addition.h"

char *read_buff (FILE *pfile, size_t *psize)
{
    assert (pfile);
    assert (psize);

    const long fsize = file_size (pfile);
    if (fsize <= 0)
        return NULL;

    char *pbuff = (char *)calloc (fsize, sizeof (char));
    if (pbuff)
        *psize = fread (pbuff, sizeof(char), fsize, pfile);

    return pbuff;
}

int dump_buff (char *buff, size_t buff_sz, FILE *file)
{
    assert (buff);
    assert (file);

    const size_t wrote = fwrite (buff, buff_sz, sizeof (*buff), file);

    return wrote == buff_sz ? 0 : 1;
}

long file_size (FILE *file)
{
    assert (file);

    #ifdef _POSIX_VERSION
        struct stat st = {};
        if (fstat (fileno (file), &st) == -1)
        {
            return 0;
        }
        return st.st_size;
    #else
        size_t cur_pos = ftell(file);
        fseek (file, 0, SEEK_END);
        const long size = ftell (file);
        fseek (file, cur_pos, SEEK_SET);
        return size;
    #endif  
}

bool allocation_error (int _errno)
{
    return _errno == ENOMEM;
}