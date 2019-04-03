#ifndef SPELL_H
#define SPELL_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define MAXLINES 1024

char **read_dictionary(char **string, size_t *string_index, char *dict_fname);

#endif
