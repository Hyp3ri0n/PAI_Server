#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

// ./client -p XXXX -s SERVER
int main(int argc, char* argv[])
{
	if (argc == 5)
	{
		/** int port : ... **/
		int port = atoi(argv[2]);
		/** char* server : Chaine de caractères conteant le nom de la machine **/
		char* server = argv[4];
		/** struct hostent* : ... **/


		char hostname[1024];
		hostname[1023] = '\0';
		gethostname(hostname, 1023);
		struct hostent* hostinfos = gethostbyname(hostname);
		printf("Hostname FULL: %s\n", hostinfos->h_name);

		/** struct sockaddr_in* p : Pointeur sur la structure qui contient la configuration de la socket **/
		struct sockaddr_in p;

		int id_socket_client_emmet = socket(AF_INET, SOCK_STREAM, 0);

		if (id_socket_client_emmet != -1)
		{
			printf("SUCCESS : Création socket d'émission.\n");

			p.sin_family = AF_INET;
			p.sin_port = htons(port);

			memcpy(&(p.sin_addr.s_addr), hostinfos->h_addr, hostinfos->h_length);

			if (connect(id_socket_client_emmet, (struct sockaddr *)&p, sizeof(struct sockaddr_in)) == 0)
			{
				printf("SUCCESS : Connect socket d'émission.\n");

				//TODO : Write
				char* bufferEnvoi = "Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test ! Salut, ceci est un test !FIN DE MESSAGE";

				int len = strlen(bufferEnvoi);
				int len_sent = 0;
				while (len_sent < len)
				{
					len_sent = len_sent + write(id_socket_client_emmet, bufferEnvoi + len_sent, 255);
					printf("INFO : Write -> %i\n", len_sent);
				}
			}
			else
				perror("ERROR : Connect socket d'émission.\n");
		}
		else
			perror("ERROR : Création socket d'émission.\n");

	}
	else
	{
		perror("Vous n'avez pas saisie toutes les informations nécessaire.\n");
	}



	return 0;
}
