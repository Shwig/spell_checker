#include "spell.h"


/*  */
char **read_dictionary(size_t *string_index, char **string, char **dict_fname) {

  FILE *fp = NULL;
  char *read_line = NULL;     /* line read from the dictionary file */
  size_t buff_size = 0;       /* default buffer size for get_line */
  ssize_t chrs_read = 0;      /* total number of characters read */
  size_t maxlines = MAXLINES; /* allocate initial number of pointer for file of unknown size */


  /* if fopen returns null file not found*/
  if ((fp = fopen (dict_fname[1], "r")) == NULL) {
    fprintf(stderr, "File not found err: check that '%s' exist at directory", dict_fname[1]);
    exit(EXIT_FAILURE);
  }

  /* allocate enough pointers for lines in default dictionary file */
  if ((string = calloc(MAXLINES, sizeof *string)) == NULL) {
    fprintf(stderr, "err: Calloc failed");
    exit(EXIT_FAILURE);
  }

  while ((chrs_read = getline(&read_line, &buff_size, fp)) != -1) {
    while (chrs_read > 0 && (read_line[chrs_read - 1] == '\n' || read_line[chrs_read -1] == '\r')) {
      read_line[--chrs_read] = 0; //  remove line breaks
    }

    /* copy the word read from the current line into the word list */
    string[(*string_index)++] = strdup(read_line);

    /* if the file has more lines than anticipated doble the initial size
      realloc */
    if ((*string_index) == maxlines) {
      char **temp = realloc (string, maxlines * 2 * sizeof *string);
      if (temp == NULL) {
        fprintf(stderr, "err: Reallocation failed");
        exit(EXIT_FAILURE);
      }
      string = temp;
      maxlines *= 2;
    }
  }

  if (fp)fclose(fp);
  if (read_line)free(read_line);

return string;
}
