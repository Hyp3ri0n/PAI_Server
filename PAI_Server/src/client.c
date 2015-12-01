#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>


void main()
{
	int id_socket_client = socket(AF_INET, SOCK_STREAM, 0);
	//int id_connect_client = connect(id_socket_client, X, sizeof(X));
}
