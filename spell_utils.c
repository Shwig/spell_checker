#include "spell.h"

char **read_dictionary(size_t *line_index, char **word_list, char **argv) {

  FILE *fp = NULL;
  char *read_line = NULL;     /* line read from the dictionary file */
  size_t buff_size = 0;       /* default buffer size for get_line */
  ssize_t chrs_read = 0;      /* total number of chars read */
  size_t maxlines = MAXLINES; /* inital number of line to allocate for file of unknown size */


  /* if fopen returns null  */
  if ((fp = fopen (argv[1], "r")) == NULL) {
    fprintf(stderr, "File I/O err: check that '%s' is in project directory", argv[1]);
    exit(EXIT_FAILURE);
  }

  /* allocate enough pointers for lines in default dictionary file */
  if ((word_list = calloc(MAXLINES, sizeof *word_list)) == NULL) {
    fprintf(stderr, "err: Calloc failed");
    exit(EXIT_FAILURE);
  }

  while ((chrs_read = getline(&read_line, &buff_size, fp)) != -1) {
    while (chrs_read > 0 && (read_line[chrs_read - 1] == '\n' || read_line[chrs_read -1] == '\r')) {
      read_line[--chrs_read] = 0; //  remove line breaks
    }

    /* copy the word read from the current line into the word list */
    word_list[(*line_index)++] = strdup(read_line);

    /* if the file has more lines than anticipated doble the initial size
      realloc */
    if ((*line_index) == maxlines) {
      char **temp = realloc (word_list, maxlines * 2 * sizeof *word_list);
      if (temp == NULL) {
        fprintf(stderr, "err: Reallocation failed");
        exit(EXIT_FAILURE);
      }
      word_list = temp;
      maxlines *= 2;
    }
  }

  if (fp)fclose(fp);
  if (read_line)free(read_line);

return word_list;
}
