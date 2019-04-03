#include "spell.h"


/* read_dictionary takes in a NULL string that will be used to store pthreads
   contents of the dictionary file, size_t pointer initalized to zero to keep
   track of the string index, and the name of the dictionary text file to
   be coppied. string pointers are dynamically allocated based on the initialtial
   value of MAXLINES. dictionary file dict_fname is read line by line  and
   coppied into the input string before returning reference to string pointer.
   Don't forget to free string before exit.*/
char **read_dictionary(char **string, size_t *string_index, char *dict_fname) {

  FILE *fp = NULL;
  char *read_line = NULL;     /* line read from the dictionary file */
  size_t buff_size = 0;       /* default buffer size for get_line */
  ssize_t chrs_read = 0;      /* total number of characters read */
  size_t maxlines = MAXLINES; /* allocate initial number of pointer for file of unknown size */
  // size_t ral_count = 0;


  /* if fopen returns null file not found*/
  if ((fp = fopen (dict_fname, "r")) == NULL) {
    fprintf(stderr, "File not found err: check that '%s' exist at directory", dict_fname);
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
      // ral_count++;
      if (temp == NULL) {
        fprintf(stderr, "err: Reallocation failed");
        exit(EXIT_FAILURE);
      }
      string = temp;
      maxlines *= 2;
    }
  }
  // printf("number of reallocations:%zu\n",ral_count );

  if (fp)fclose(fp);
  if (read_line)free(read_line);

return string;
}
