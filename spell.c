#include "spell.h"

int main(int argc, char **argv) {

  /* Check for command line args */
  if (argc == 1) {
    argv[1] = DEFAULT_DICTIONARY; /* use the default dictionary file */
  }
  if (argc > 2) {
    fprintf(stderr, "Spell_Checker only takes 0 or 1"
    " command line args! Use: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }

  char **word_list = NULL;    /* an array to hold a copy of the dictionary FILE */
  size_t line_index = 0;      /* index for number of lines in the dictionary FILE */

  /* read_dictionary */
  // word_list = read_dictionary(word_list, &line_index, argv[1]);
  line_index = read_dictionary(&word_list, argv[1]);

  if (!word_list) {
    printf("Word list is empty \n");
    return EXIT_FAILURE;
  }

  /* display all words in the word list */
  // for (size_t j = 0; j < line_index; j++) {
  //   printf("%s\n", word_list[j] );
  // }

  char test[15];
  strcpy(test, "zoologist's");
  for (size_t j = 0; j < line_index; j++) {

    if(!strcmp(test, word_list[j])){
      printf("string found %s at index [%zu] \n", word_list[j], j );
      break;
    }
  }

  /* free back to heap */
  for (size_t i = 0; i < line_index; i++) {
    free(word_list[i]);
  }
  free(word_list);

  return EXIT_SUCCESS;
}
