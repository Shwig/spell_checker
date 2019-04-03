#ifndef SPELL_H
#define SPELL_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define MAXLINES 1024

char **read_dictionary(size_t *line_index, char **word_list, char **argv);

#endif
