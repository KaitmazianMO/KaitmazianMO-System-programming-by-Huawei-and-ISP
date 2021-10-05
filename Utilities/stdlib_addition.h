#ifndef STDLIB_ADDITION_H_INCLUDED
#define STDLIB_ADDITION_H_INCLUDED

#include <stdio.h>

char *read_buff (FILE *file, size_t *size);
/*! Dumping null terminated buffer to file, in errer case returns 0. */
int dump_buff (char *buff, size_t buff_sz, FILE *file);
long  file_size (FILE *file);
bool  allocation_error (int _errno);

#endif