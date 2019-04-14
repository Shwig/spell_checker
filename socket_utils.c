#include "socket_utils.h"


/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc) {
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];

	//Send some messages to the client
	message = "Greetings! I am your connection handler\n";
	write(sock , message , strlen(message));

	message = "Now type something and i shall repeat what you type \n";
	write(sock , message , strlen(message));

	//Receive a message from client
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ) {
		//Send the message back to client
		write(sock , client_message , strlen(client_message));
	}

	if(read_size == 0) {
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1) {
		perror("recv failed");
	}

	//Free the socket pointer
	free(socket_desc);

	return 0;
}

/* FROM KERRISK

   Read characters from 'fd' until a newline is encountered. If a newline
   character is not encountered in the first (n - 1) bytes, then the excess
   characters are discarded. The returned string placed in 'buf' is
   null-terminated and includes the newline character if it was read in the
   first (n - 1) bytes. The function return value is the number of bytes
   placed in buffer (which includes the newline character if encountered,
   but excludes the terminating null byte). */
ssize_t readLine(int fd, void *buffer, size_t n) {
  ssize_t numRead;                    /* # of bytes fetched by last read() */
  size_t totRead;                     /* Total bytes read so far */
  char *buf;
  char ch;

  if (n <= 0 || buffer == NULL) {
    errno = EINVAL;
    return -1;
  }

  buf = buffer;                       /* No pointer arithmetic on "void *" */

  totRead = 0;
  for (;;) {
    numRead = read(fd, &ch, 1);

    if (numRead == -1) {
      if (errno == EINTR)         /* Interrupted --> restart read() */
        continue;
      else
        return -1;              /* Some other error */

    } else if (numRead == 0) {      /* EOF */
      if (totRead == 0)           /* No bytes read; return 0 */
        return 0;
      else                        /* Some bytes read; add '\0' */
        break;

    } else {                        /* 'numRead' must be 1 if we get here */
      if (totRead < n - 1) {      /* Discard > (n - 1) bytes */
        totRead++;
        *buf++ = ch;
      }

      if (ch == '\n')
        break;
    }
  }

  *buf = '\0';
  return totRead;
}


/* given a port number or service as string, returns a
   descriptor that we can pass to accept() */
int getlistenfd(char *port) {
  int listenfd, status;
  struct addrinfo hints, *res, *p;

  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM; /* TCP */
  hints.ai_family = AF_INET;       /* IPv4 */

  if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error %s\n", gai_strerror(status));
    exit(EXIT_GETADDRINFO_ERROR);
  }

  /* try to bind to the first available address/port in the list.
     if we fail, try the next one. */
  for(p = res;p != NULL; p = p->ai_next) {
    if ((listenfd=socket(p->ai_family, p->ai_socktype, p->ai_protocol))<0) {
      continue;
    }

    if (bind(listenfd, p->ai_addr, p->ai_addrlen)==0) {
      break;
    }
  }
  freeaddrinfo(res);
  if (p==NULL) {
    exit(EXIT_BIND_FAILURE);
  }

  if (listen(listenfd, BACKLOG)<0) {
    close(listenfd);
    exit(EXIT_LISTEN_FAILURE);
  }
  return listenfd;
}
