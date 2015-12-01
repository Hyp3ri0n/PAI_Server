#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

// ./client -p XXXX -s SERVER
int main(int argc, char* argv[])
{
	if(argc < 5)
	{
		sprintf("Vous n'avez pas saisie toutes les informations nécessaire ! ");
	}
	else
	{
		int port = argv[2];
		char* server = argv[4];
		struct hostent* hostinfos = gethostbyname(server);

		int id_socket_client = socket(AF_INET, SOCK_STREAM, 0);
		//int id_connect_client = connect(id_socket_client, X, sizeof(X));
	}

	return 0;
}
