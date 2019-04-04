#include "spell.h"

#define FILE_ERR -1
#define CALLOC_ERR -2
#define REALLOC_ERR -3

/* read_dictionary takes in the reference to a NULL string that will be used to
  store the contents of the dictionary file, and the name of the dictionary text
  file to be coppied. string pointers are dynamically allocated based on the
  initialtial value of MAXLINES. The dictionary file dict_fname is read line by line
  and coppied into the input string word by word by word removes new lines anticipated
  null terminates each string.
  read_dictionary returns the word count of the file on sucess, and returns a
  negetive int on failure
  Don't forget to free string from caller before exit.*/
int read_dictionary(char ***string, char *dict_fname) {

  FILE *fp = NULL;
  char *read_line = NULL;     /* line read from the dictionary file */
  size_t buff_size = 0;       /* default buffer size for get_line */
  ssize_t char_count = 0;      /* total number of characters read */
  size_t maxlines = MAXLINES; /* allocate initial number of pointer for file of unknown size */
  size_t word_count = 0;      /* dictionary file word count */

  /* if fopen returns null file not found*/
  if ((fp = fopen (dict_fname, "r")) == NULL) {
    fprintf(stderr, "File not found err: check that '%s' exist at directory", dict_fname);
    return FILE_ERR;
  }

  /* allocate enough pointers for lines in default dictionary file */
  if ((*string = calloc(MAXLINES, sizeof **string)) == NULL) {
    fprintf(stderr, "err: Calloc failed");
    return CALLOC_ERR;
  }

  while ((char_count = getline(&read_line, &buff_size, fp)) != -1) {
    while (char_count > 0 && (read_line[char_count - 1] == '\n' || read_line[char_count -1] == '\r')) {
      read_line[--char_count] = 0; //  remove line breaks
    }

    /* copy the word read from the current line into the word list */
    (*string)[word_count++] = strdup(read_line);

    /* if the file has more lines than anticipated doble the initial size
      realloc */
    if (word_count == maxlines) {
      char **temp = realloc ((*string), maxlines * 2 * sizeof **string);
      // ral_count++;
      if (temp == NULL) {
        fprintf(stderr, "err: Reallocation failed");
        return REALLOC_ERR;
      }
      *string = temp;
      maxlines *= 2;
    }
  }

  if (fp)fclose(fp);
  if (read_line)free(read_line);

 return word_count;
}
