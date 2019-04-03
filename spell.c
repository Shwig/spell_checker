#include "spell.h"

int main(int argc, char **argv) {

  if (argc == 1) {
    argv[1] = "words"; /* words is the default dictionary file */
  }
  if (argc > 2) {
    fprintf(stderr, "Spell_Checker only takes 0 or 1"
    " command line args! Use: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }

  char **word_list = NULL;    /* an array to hold a copy of the dictionary FILE */
  size_t line_index = 0;      /* index for number of lines in the dictionary FILE */

  /* read_dictionary */
  word_list = read_dictionary(&line_index, word_list, argv);

  /* display all words in the word list */
  for (size_t j = 0; j < line_index; j++) {
    printf("%s\n", word_list[j] );
  }

  /* free back to heap */
  for (size_t i = 0; i < line_index; i++) {
    free(word_list[i]);
  }
  free(word_list);

  return EXIT_SUCCESS;
}
