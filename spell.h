#ifndef SPELL_H
#define SPELL_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define MAXLINES 1024
#define DEFAULT_DICTIONARY "words"

typedef struct server_config_t {
  int connectedfd;
  char **word_list;
  size_t word_index;
} Serv_conf;

int read_dictionary(char ***string, char *dict_fname);

#endif
