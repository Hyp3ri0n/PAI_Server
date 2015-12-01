#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void finfils(int sig)
{
	//?//
	while(wait()) {}
}


int main(int argc, char *argv[])
{
	/** int defaultPort : Entier qui defini le port par défaut pour la socket d'écoute **/
	int defaultPort = 6842;
	/** int port : Entier qui defini le port pour la socket d'écoute **/
	int port;
	/** char** ip : Chaine de caractères définissant l'adresse ip du serveur **/
	char** ip = "127.0.0.1";

	//GESTION DE L'INTERFACE

	if (argc == 3 && strcmp(argv[1], "-p") == 0)
		port = atoi(argv[2]);
	else
	{
		int rep = -1;
		while (rep == -1)
		{
			printf("Vous n'avez pas saisi de port, voulez-vous utiliser celui par défaut ? (oui : 1 | non : 0)\n>");
			scanf("%i", &rep);

			if (rep == 1)
				port = defaultPort;
			else if (rep == 0)
			{
				printf("Arrêt de l'application.\n");
				exit(0);
			}
			else
			{
				printf("Reponse non valide.\n");
				rep = -1;
			}
		}
	}


	//GESTION DES SOCKETS

	/** struct sockaddr_in* p : Pointeur sur la structure qui contient la configuration de la socket **/
	struct sockaddr_in* p;
	/** int nbMaxCo : Entier qui défini le nombre maximal de connections clients simultanées sur la même socket **/
	int nbMaxCo = 10;
	/** int id_socket_server_listen : Entier qui défini l'identifiant de la socket d'écoute **/
	int id_socket_server_listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	if (id_socket_server_listen != -1)
	{
		printf("SUCCESS : Création socket écoute.\n");

		p->sin_family = AF_INET;
		p->sin_port = port;
		p->sin_addr->s_addr = htonl(INADDR_ANY);
		p->sin_zero[8] = 0;//?//

		if (bind(id_socket_server_listen, p, sizeof(p)) == 0)
		{
			printf("SUCCESS : Bind socket écoute.\n");

			while (listen(id_socket_server_listen, nbMaxCo) == 0)
			{
				printf("SUCCESS : Listen socket écoute.\n");

				int id_socket_server_service = accept(id_socket_server_listen, p, sizeof(p));

				if(id_socket_server_service != -1)
				{
					/** int pid : PID du processus fils **/
					int pid;

					/** struct sigaction : La structure qui permet de ne pas bloquer le père à la fin de vie de ses fils **/
					struct sigaction a;
					a.sa_handler = finfils;
					a.sa_flags = SA_RESTART;

					//Application de notre structure
					sigaction(SIGCHLD, &a, NULL);

					//Création du fils
					pid = fork();

					if (pid != -1)
					{
						switch (pid)
						{
							case 0:
								/* PROCESSUS FILS */
								printf("INFO : Processus fils -> lire les donées qui transitent (pid = %d)\n", getpid() );

								//TODO : Read

								exit(0); /* fin du processus fils */ //?//
								break;
							default:
								/* PROCESSUS PERE */
								printf("INFO : Processus père -> retourne dans \"accept\" (pid=%d)\n", pid );
								break;
						}
					}
					else
						perror("ERROR : Fork sur père.\n");
				}
				else
					perror("ERROR : Accept socket écoute.\n");
			}
			/*else
				perror("ERROR : Listen socket écoute.\n");*/
		}
		else
			perror("ERROR : Bind socket écoute.\n");
	}
	else
		perror("ERROR : Création socket écoute.\n");

	return 0;
}
