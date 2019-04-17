#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h> //for threading , link with lpthread

#define NUM_WORKERS 50
#define DEFAULT_PORT "50000"

#define EXIT_USAGE_ERROR 1
#define EXIT_GETADDRINFO_ERROR 2
#define EXIT_BIND_FAILURE 3
#define EXIT_LISTEN_FAILURE 4

#define MAX_LINE 64

ssize_t readLine(int fd, void *buffer, size_t n);
int getlistenfd(char *port);
void *connection_handler(void *);

#endif
