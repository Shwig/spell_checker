#include "spell.h"
#include "socket_utils.h"

int main(int argc, char **argv) {

  char **word_list = NULL;    /* an array to hold a copy of the dictionary FILE */
  char *dfile_name;           /* dictionary file name */
  size_t word_index = 0;      /* index for number of lines in the dictionary FILE */

  int listenfd , connectedfd;// , *new_sock;
  struct sockaddr_in client;
  socklen_t client_addr_size;
  char line[MAX_LINE];
  char *message;
  char *port;

  size_t totRead;


  /* cmd line arg error checking */
  if (argc > 3) {
    fprintf(stderr, "Input Error!\nSpell_Checker only takes 0 or 2"
    " command line args! Use: %s <file> <port>\n", argv[0]);
    return EXIT_FAILURE;
  }

  /* Check for command line args */
  if (argc < 3) {
    dfile_name = DEFAULT_DICTIONARY;
    port = DEFAULT_PORT;
  } else {
    dfile_name = argv[1];
    port = argv[2];
  }

  /* read_dictionary */
  if (!(word_index = read_dictionary(&word_list, dfile_name))) {
    printf("Memory allocation Error during dictionary file read\n");
    return EXIT_FAILURE;
  }

  listenfd = getlistenfd(port);


  //Accept and incoming connection
  puts("Waiting for incoming connections...");
  client_addr_size = sizeof(struct sockaddr_in);
  while( (connectedfd = accept(listenfd, (struct sockaddr *)&client, &client_addr_size)) ) {
    puts("Connection accepted");

    //Greet  client
    message = "  Welcome to the Spellcheck server!\nEneter a word to check: ";
    write(connectedfd , message , strlen(message));

    while ((totRead = readLine(connectedfd, line, MAX_LINE-1))>0) {

      if (totRead < 3) {
        message = "\nPlease enter a valid word! >> ";
        if (write(connectedfd , message , strlen(message)) < 0) {
          puts("\n   ***Socket Write Failure!!!");
          return EXIT_FAILURE;
        }
        continue;
      }

      line[totRead-2] = '\0';   // remove line breaks from input

      message = "\n  searching...\n";
      if (write(connectedfd , message , strlen(message)) < 0) {
        puts("\n   ***Socket Write Failure!!!");
        return EXIT_FAILURE;
      }

      int w_found = 0;
      printf("\nSearching Dictionary for: %s", line);
      for (size_t j = 0; j < word_index; j++) {
        if(!strcmp(line, word_list[j])){
          w_found = 1;
          printf("\n  Word found at dict_index: %zd\n  responding to socket...\n",j);
          message = "  Word spelled correct!\n\nEnter another word: ";
          break;
        }
      }

      if (!w_found) {
        puts("\n  *Word not found*\n  responding to socket...");
        message = "  *Sorry word not found in Spell_serv Dictionary file*\n\nEnter another word: ";
      }

      if (write(connectedfd , message , strlen(message)) < 0) {
        puts("\n   ***Socket Write Failure!!!");
        return EXIT_FAILURE;
      }
      puts("  write sucess...\n  awaiting next input...");

    }
    printf("connection closed\n");
    close(connectedfd);

    // pthread_t sniffer_thread;
    // new_sock = malloc(1);
    // *new_sock = connectedfd;
    //
    // if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)	{
    //   perror("could not create thread");
    //   return 1;
    // }

    //Now join the thread , so that we dont terminate before the thread
    //pthread_join( sniffer_thread , NULL);
    // puts("Handler assigned");
    if (connectedfd<0)	{
  		perror("accept failed");
  		return EXIT_FAILURE;
  	}
  }

  /* free back to heap */
  for (size_t i = 0; i < word_index; i++) {
    free(word_list[i]);
  }
  free(word_list);

  return EXIT_SUCCESS;
}
