/* hecho - Simple HTTP Echo server
   Simple command-line based server that will echo any HTTP messages sent to it
   Intended mainly as a debugging aid to verify messages (with headers) sent to HTTP servers.

   Based loosely on nweb.c - https://gist.githubusercontent.com/sumpygump/9908417/raw/5fa991fda103d0b7a0c38512394a83ccada9ad6c/nweb23.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 64*1024
#define MAX_CONNECTIONS 1
#define EXIT_PARAMERROR 2

void exit_error(int error_num, char *message)
{
  printf("%s\nError %d: %s\n", message, error_num, strerror(error_num));
  exit(error_num);
}

// Echo function to run in a thread for a specific client
void do_echo(int client, int c_num)
{
  int size = 1;
  static char buffer[BUFSIZE+1];

  printf("Client %d Connected.\n", c_num);
  while (size > 0) {
	memset(buffer, 0, BUFSIZE+1);
	if ((size = read(client, buffer, BUFSIZE)) > 0)
	{
		printf("Received from client %d:\n%s\n", c_num, buffer);
	}
	else
	  printf("Error reading from client %d, Ending.", c_num);
  }
  close(client);
  exit(0);
}

int main(int argc, char **argv)
{
  int port, pid, listener, client, c_num;
  socklen_t len;
  static struct sockaddr_in c_addr;
  static struct sockaddr_in s_addr;

  if (argc < 2  || !strcmp(argv[1], "-?") ) {
	printf("%s [Port Number]\n"
		"\tSimple echo server, will echo any data sent to the specified port to the console\n"
		"\tExample: %s 8181\n", argv[0], argv[0]);
	exit(EXIT_PARAMERROR);
  }

  port = atoi(argv[1]);
  if (port <= 0) {
	printf("Invalid port number: \"%s\". Exiting.\n", argv[1]);
	exit(EXIT_PARAMERROR);
  }

  /* Set up initial server socket */
  if ((listener = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	exit_error(errno, "Unable to initalize server listener socket.");

  s_addr.sin_family = AF_INET;
  s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  s_addr.sin_port = htons(port);
  if (bind(listener, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0)
	exit_error(errno, "Unable to bind port to server listener socket.");
  if (listen(listener, MAX_CONNECTIONS) < 0)
	exit_error(errno, "Unable to listen at specified port.");

  c_num = 0;
  while (1) {
	len = sizeof(c_addr);
	if ((client = accept(listener, (struct sockaddr *)&c_addr, &len)) < 0)
		exit_error(errno, "Unable to accept connection on server socket.");

	c_num++;
	if ((pid = fork()) >= 0)
	{
		if (pid == 0) {
			// child - close listener and start echoing
			close(listener);
			do_echo(client, c_num);
		} else {
			// parent - continue listening
			close(client);
		}
	}
	else
		exit_error(errno, "Unable to spawn client listener. Aborting\n");
  }
  exit(0);
}
