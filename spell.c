#include "spell.h"

int main(int argc, char **argv) {

  if (argc == 1) {
    argv[1] = "words"; /* the default dictionary file */
  }
  if (argc > 2) {
    fprintf(stderr, "Spell_Checker only takes 0 or 1"
    " command line args! Use: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }

  char **word_list = NULL;    /* an array to hold a copy of the dictionary FILE */
  // char *read_line = NULL;     /* line read from the dictionary file */
  // FILE *fp = NULL;
  // size_t buff_size = 0;       /* default buffer size for get_line */
  // ssize_t chrs_read = 0;      /* total number of chars read */
  size_t line_index = 0;      /* index for number of lines in file */
  // size_t maxlines = MAXLINES; /* inital number of line to allocate for file of unknown size */

  //
  word_list = read_dictionary(&line_index, word_list, argv);

  // /* if fopen returns null  */
  // if ((fp = fopen (argv[1], "r")) == NULL) {
  //   fprintf(stderr, "File I/O err: check that '%s' is in project directory", argv[1]);
  //   return EXIT_FAILURE;
  // }
  //
  // /* allocate enough pointers for lines in default dictionary file */
  // if ((word_list = calloc(MAXLINES, sizeof *word_list)) == NULL) {
  //   fprintf(stderr, "err: Calloc failed");
  //   return EXIT_FAILURE;
  // }
  //
  // while ((chrs_read = getline(&read_line, &buff_size, fp)) != -1) {
  //   while (chrs_read > 0 && (read_line[chrs_read - 1] == '\n' || read_line[chrs_read -1] == '\r')) {
  //     read_line[--chrs_read] = 0; //  remove line breaks
  //   }
  //
  //   /* copy the word read from the current line into the word list */
  //   word_list[line_index++] = strdup(read_line);
  //
  //   /* if the file has more lines than anticipated doble the initial size
  //     realloc */
  //   if (line_index == maxlines) {
  //     char **temp = realloc (word_list, maxlines * 2 * sizeof *word_list);
  //     if (temp == NULL) {
  //       fprintf(stderr, "err: Reallocation failed");
  //       return EXIT_FAILURE;
  //     }
  //     word_list = temp;
  //     maxlines *= 2;
  //   }
  // }
  //
  // if (fp)fclose(fp);
  // if (read_line)free(read_line);

  for (size_t j = 0; j < line_index; j++) {
    printf("%s\n", word_list[j] );
  }

  for (size_t i = 0; i < line_index; i++) {
    free(word_list[i]);
  }
  free(word_list);

  return EXIT_SUCCESS;
}
