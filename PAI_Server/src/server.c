/********************************************************************/
/*																	*/
/*							Serveur PAI								*/
/*																	*/
/*																	*/
/*	Date : 01/12/2015												*/
/*	Description : Serveur métier du projet PAI L3 MIAGE 2015.		*/
/*	Spécification : Utilisation de sockets TCP (Reception).			*/
/*	Developpeurs :	- MARTINIER Alexis								*/
/*					- VAILLANT Hugo									*/
/*					- ORTIZ Luc										*/
/*					- FERNANDES Aurélien							*/
/*																	*/
/********************************************************************/

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
#include <string.h>


#define NOTHING 0
#define POST 1
#define HOST 2
#define LENGTH 3
#define CONTENT 4



/**
 * Structure
 */
struct S_InfRequest {
	char* hostIp;
	int hostPort;
	int contentLength;
	int idInf;
	char* xmlContent;
};
typedef struct S_InfRequest InfRequest;




/**
 * Permet de prendre connaissance de la mort d'un fils
 *
 * @param sig L'identifiant du signal ayant déclenché l'action (int)
 */
void finfils(int sig)
{
	printf("INFO : Reception de la mort d'un fils.\n");
	wait();
}

/**
 * Permet de lire ligne à ligne la requête et de remplir la structure InfRequest
 */
void readLine(char* request, InfRequest* r)
{
	char* recup;
	int lengthRequest = strlen(request);
	int lengthRead = 0;
	int finLigne = 1;
	int requestOK = 0;
	int i;
	int indiceLigne;
	char* pointeurTemp;
	char* portHost[4];
	char* lengthNumber, ip;
	int state;

	while(lengthRequest > lengthRead)
	{
		if(request[lengthRead] != '\n')
		{
			if(finLigne == 1)
			{
				indiceLigne = 0;
				finLigne = 0;
				recup = malloc(sizeof(char)*request[lengthRead] );
				state = NOTHING;
			}
			else
			{
				pointeurTemp = realloc(recup, sizeof(recup) + sizeof(char*));

				if (pointeurTemp != NULL)
					recup = pointeurTemp;
				else
					free(pointeurTemp);
			}

			recup[indiceLigne] = request[lengthRead];
			if(state == NOTHING)
			{
				if(strcmp(recup,"POST") == 0)
				{ //POST /INFIRMIERE HTTP/1.1
					state = POST;
				}
				else if(strcmp(recup,"Host") == 0)
				{
					state = HOST;
				}
				else if(strcmp(recup,"Content-Length") == 0)
				{
					state = LENGTH;
				}
				else if(strcmp(recup,"id") == 0)
				{
					state = CONTENT;
				}
			}
		}
		else
		{
			printf("INFO : Request -> EOL.\n");
			//fflush(stdout);
			finLigne = 1;
			switch(state)
			{
				case POST:
					/*if(strcmp(recup,"POST /INFIRMIERE HTTP/1.1"))
						requestOK = 1;*/
					printf("INFO : Request -> POST : %s.\n", recup);
					break;

				case HOST:
					/*//Récup port
					for (i = 0; i < 4; i++)
					{
						portHost[i] = recup[(sizeof(recup)+(i-4))];
						//recup[(sizeof(recup)-1)] = '\0';
					}
					//On enlève le ':'
					//recup[(sizeof(recup)-5)] = '\0';
					infos.hostPort = atoi(portHost);

					//Recup IP
					ip = malloc((sizeof(recup) - 11));
					for (i = 7; i < sizeof(recup); i++)
					{
						ip[i-7] = recup[i];
					}
					infos.hostIp = recup;*/
					printf("INFO : Request -> HOST : %s.\n", recup);
					break;

				case LENGTH:
					/*lengthNumber = malloc((sizeof(recup)-16));
					for (i = 17; i < sizeof(recup); i++)
					{
						lengthNumber[i-17] = recup[i];
					}
					info.contentLength = atoi(lengthNumber);*/
					printf("INFO : Request -> LENGHT : %s.\n", recup);
					break;

				case CONTENT:
					//infos.xmlContent = recup;
					//infos.idInf = recup;
					printf("INFO : Request -> CONTENT : %s.\n", recup);
					break;

				default:
					break;
			}

			recup = "";
			free(recup);
			state = NOTHING;
		}
		lengthRead++;
		indiceLigne++;
	}



	if (requestOK == 0)
		r = NULL;
}


int main(int argc, char *argv[])
{
	/********************************************************************/
	/*																	*/
	/*						GESTION DES SIGNAUX							*/
	/*																	*/
	/********************************************************************/

	/** struct sigaction : La structure qui permet de ne pas bloquer le père à la fin de vie de ses fils **/
	struct sigaction a;
	a.sa_handler = finfils;
	a.sa_flags = SA_RESTART;

	//Application de la structure sigaction
	sigaction(SIGCHLD, &a, NULL);


	/********************************************************************/
	/*																	*/
	/*						GESTION DE L'INTERFACE						*/
	/*																	*/
	/********************************************************************/

	/** int defaultPort : Entier qui defini le port par défaut pour la socket d'écoute **/
	int defaultPort = 6842;
	/** int port : Entier qui defini le port pour la socket d'écoute **/
	int port;
	/** char** ip : Chaine de caractères définissant l'adresse ip du serveur **/
	char* ip = "127.0.0.1";

	if (argc == 3 && strcmp(argv[1], "-p") == 0)
		port = atoi(argv[2]);
	else
	{
		int rep = -1;
		while (rep == -1)
		{
			printf("INFO : Vous n'avez pas saisi de port, voulez-vous utiliser celui par défaut ? (oui : 1 | non : 0)\n>");
			scanf("%i", &rep);

			if (rep == 1)
				port = defaultPort;
			else if (rep == 0)
			{
				printf("INFO : Arrêt de l'application.\n");
				exit(0);
			}
			else
			{
				printf("INFO : Reponse non valide.\n");
				rep = -1;
			}
		}
	}


	/********************************************************************/
	/*																	*/
	/*						GESTION DES SOCKETS							*/
	/*																	*/
	/********************************************************************/


	/** struct sockaddr_in* p : Pointeur sur la structure qui contient la configuration de la socket **/
	struct sockaddr_in p;
	/** int nbMaxCo : Entier qui défini le nombre maximal de connections clients simultanées sur la même socket **/
	int nbMaxCo = 10;
	/** int id_socket_server_listen : Entier qui défini l'identifiant de la socket d'écoute **/
	int id_socket_server_listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	if (id_socket_server_listen != -1)
	{
		printf("SUCCESS : Création socket écoute. port : %i\n", port);

		p.sin_family = AF_INET;
		p.sin_port = htons(port);
		p.sin_addr.s_addr = htonl(INADDR_ANY);

		printf("SUCCESS : Assign socket écoute3.\n");

		if (bind(id_socket_server_listen, (struct sockaddr *)&p, sizeof(struct sockaddr_in)) == 0)
		{
			printf("SUCCESS : Bind socket écoute.\n");

			//Pour savoir le nom de la machine
			char hostname[1024];
			hostname[1023] = '\0';
			gethostname(hostname, 1023);
			struct hostent* hostinfos = gethostbyname(hostname);
			printf("INFO : Hostname -> %s\n", hostinfos->h_name);

			if (listen(id_socket_server_listen, nbMaxCo) == 0)
			{
				printf("SUCCESS : Listen socket écoute.\n");

				int sizeofSockaddr_in = sizeof(struct sockaddr_in);
				int id_socket_server_service = accept(id_socket_server_listen, (struct sockaddr *)&p, &sizeofSockaddr_in);

				while (id_socket_server_service != -1)
				{
					/** int pid : PID du processus fils **/
					int pid;
					char bufferReception[255];
					char bufferFull[2048];
					int nbOctetRecus = 0;
					int nbOctetRecusFull = 0;

					//Création du fils
					pid = fork();

					if (pid != -1)
					{
						switch (pid)
						{
							case 0:
								/* PROCESSUS FILS */
								printf("INFO : Processus fils -> lire les donées qui transitent (pid = %d).\n", getpid());

								//Ferme la socket d'écoute
								close(id_socket_server_listen);

								while ((nbOctetRecus = read(id_socket_server_service, bufferReception, sizeof(bufferReception))) != 0)
								{
									nbOctetRecusFull = nbOctetRecus + nbOctetRecusFull;
									strcat(bufferFull, bufferReception);
								}

								printf("INFO : Read octets -> %i\n> %s\n\n", nbOctetRecusFull, bufferFull);

								InfRequest* request;
								readLine(bufferFull, request);

								//Ferme la socket d'écoute
								close(id_socket_server_service);

								exit(0); /* fin du processus fils */ //Quand interaction finie, dans une fonction, recup la requete, contacter gg, traitement
								break;
							default:
								/* PROCESSUS PERE */
								//Ferme la socket de service
								close(id_socket_server_service);
								printf("INFO : Processus père -> retourne dans \"accept\" (pid=%d)\n", pid);
								break;
						}
					}
					else
						perror("ERROR : Fork sur père.\n");

					//"Pourquoi pas" - Hugo
					id_socket_server_service = accept(id_socket_server_listen, (struct sockaddr *)&p, &sizeofSockaddr_in);
				}
				/*else
					perror("ERROR : Accept socket écoute.\n");*/
			}
			else
				perror("ERROR : Listen socket écoute.\n");
		}
		else
			perror("ERROR : Bind socket écoute.\n");
	}
	else
		perror("ERROR : Création socket écoute.\n");

	return 0;
}
