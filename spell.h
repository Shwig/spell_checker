#ifndef SPELL_H
#define SPELL_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include<sys/socket.h>

#define MAXLINES 1024
#define DEFAULT_DICTIONARY "words"

// char **read_dictionary(char **string, size_t *string_index, char *dict_fname);
int read_dictionary(char ***string, char *dict_fname);

#endif
