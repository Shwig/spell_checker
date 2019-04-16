#include "spell.h"
#include "socket_utils.h"

int main(int argc, char **argv) {

  char **word_list = NULL;    /* an array to hold a copy of the dictionary FILE */
  char *dfile_name;           /* dictionary file name */
  size_t line_index = 0;      /* index for number of lines in the dictionary FILE */

  int listenfd , connectedfd;// , *new_sock;
  struct sockaddr_in client;
  socklen_t client_addr_size;
  char line[MAX_LINE];
  char *message;
  char *port;

  // char *test = "hello";
  int nums = 0;
  // char string[MAX_LINE];


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
  if (!(line_index = read_dictionary(&word_list, dfile_name))) {
    printf("Allocation Error during dictionary file read\n");
    return EXIT_FAILURE;
  }

  listenfd = getlistenfd(port);

  //Accept and incoming connection
  puts("Waiting for incoming connections...");
  client_addr_size = sizeof(struct sockaddr_in);
  while( (connectedfd = accept(listenfd, (struct sockaddr *)&client, &client_addr_size)) ) {
    puts("Connection accepted");

    //Reply to the client
    message = "Pleas eneter word you would like to spell check: ";
    write(connectedfd , message , strlen(message));


    while ((readLine(connectedfd, line, MAX_LINE-1))>0) {

      printf("Searching Dictionary for: %s", line);
      message = "searching...\n\n";
      write(connectedfd, message, strlen(message));

      nums = strcspn(line, "\n");
      char string[nums];

      strncpy(string,line, nums-1);
      string[nums-1] = '\0';

      for (size_t j = 0; j < line_index; j++) {
        // printf("%s\n", word_list[j] );
        if(!strcmp(string, word_list[j])){
          puts("Word spelled correctly");
          // write(connectedfd , message , strlen(message));
          message = "word spelled correct\nEnter another word: ";
          write(connectedfd , message , strlen(message));
          break;
        }
      }
    }

    printf("%s", line);
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
  for (size_t i = 0; i < line_index; i++) {
    free(word_list[i]);
  }
  free(word_list);

  return EXIT_SUCCESS;
}
