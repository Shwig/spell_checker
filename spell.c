#include "spell.h"
#include "socket_utils.h"

pthread_mutex_t lock;
pthread_cond_t cv;
int client_connection_count = 0;

void *client_worker(void *config){

  Serv_conf *client_conf = (Serv_conf*) config;
  char line[MAX_LINE];
  char *message;
  size_t totRead;

  //Greet  client
  message = "  Welcome to the Spellcheck server!\nEneter a word to check: ";
  if (write(client_conf->connectedfd , message , strlen(message)) < 0) {
    fprintf(stderr, "Error writting to socket!\n");
    return (void *)EXIT_FAILURE;
  }

  while ((totRead = readLine(client_conf->connectedfd, line, MAX_LINE-1))>0) {

    if (totRead < 3) {
      message = "\nPlease enter a valid word! >> ";
      if (write(client_conf->connectedfd , message , strlen(message)) < 0) {
        fprintf(stderr, "Error writting to socket!\n");
        return (void *)EXIT_FAILURE;
      }
      continue;
    }

    line[totRead-2] = '\0';   // remove line breaks from input

    message = "\n  searching...\n";
    if (write(client_conf->connectedfd , message , strlen(message)) < 0) {
      fprintf(stderr, "Error writting to socket!\n");
      return (void *)EXIT_FAILURE;
    }

    int w_found = 0;
    printf("\nSearching Dictionary for: %s", line);
    for (size_t j = 0; j < client_conf->word_index; j++) {
      if(!strcmp(line, client_conf->word_list[j])){
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

    if (write(client_conf->connectedfd , message , strlen(message)) < 0) {
      fprintf(stderr, "Error writting to socket!\n");
      return (void *)EXIT_FAILURE;
    }
    puts("  write sucess...\n  awaiting next input...");

  }
  puts("\n  !Connection Closed!");
  close(client_conf->connectedfd);

  /* after client disconnect decrement connection count and signal*/
  pthread_mutex_lock(&lock);
  client_connection_count--;
  printf("  *conn_count:%d", client_connection_count);
  pthread_cond_signal(&cv);
  pthread_mutex_unlock(&lock);
  return (void *)EXIT_SUCCESS;
}

int main(int argc, char **argv) {

  // char **word_list = NULL;    /* an array to hold a copy of the dictionary FILE */
  char *dfile_name;           /* dictionary file name */
  // size_t word_index = 0;      /* index for number of lines in the dictionary FILE */

  int listenfd;// , connectedfd;
  struct sockaddr_in client;
  socklen_t client_addr_size;
  // char line[MAX_LINE];
  // char *message;
  char *port;
  // size_t totRead;

  Serv_conf *config;
  config = malloc(sizeof(Serv_conf));
  config->word_index = 0;
  config->word_list = NULL;

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
  if (!(config->word_index = read_dictionary(&config->word_list, dfile_name))) {
    printf("Memory allocation Error during dictionary file read\n");
    return EXIT_FAILURE;
  }

  listenfd = getlistenfd(port);

  // serve 1 user, queue the rest of the clients in the listenerfd queue
  if (client_connection_count > 1) {
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&cv, &lock);
    pthread_mutex_unlock(&lock);
  }

  //Accept and incoming connection
  puts("Waiting for incoming connections...");
  client_addr_size = sizeof(struct sockaddr_in);

  while(1) {

    if ((config->connectedfd = accept(listenfd, (struct sockaddr *)&client, &client_addr_size)) < 0) {
      fprintf(stderr, "Error accepting connection from client!\n");
    }
    puts("\n  !New Connection accepted!\n");

    // threads
    pthread_t sniffer_thread;

    // check that thread is created and then serve the connection
    if( pthread_create( &sniffer_thread , NULL , client_worker , (void*) config) < 0)	{
      perror("could not create thread");
      return EXIT_FAILURE;
    } else {
      pthread_mutex_lock(&lock);
      client_connection_count++;
      printf("  *current connection count:%d\n", client_connection_count);
      pthread_mutex_unlock(&lock);
    }

    //Now join the thread , so that we dont terminate before the thread
    // pthread_join( sniffer_thread , NULL);
    // puts("Handler assigned");
    if (config->connectedfd<0)	{
  		perror("  *accept failed!");
  		return EXIT_FAILURE;
  	}
  }

  /* free back to heap */
  for (size_t i = 0; i < config->word_index; i++) {
    free(config->word_list[i]);
  }
  free(config->word_list);

  return EXIT_SUCCESS;
}
