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
#include <curl/curl.h>
#include "client.h"



//Variables globales
char buffer[1024] = "";
char* request_body = NULL;



/**
 * Permet de prendre connaissance de la mort d'un fils
 *
 * @param sig L'identifiant du signal ayant déclenché l'action (int)
 */
void receiveKillFromChild(int sig)
{
	//int status1 = 0;
	wait();
	printf("INFO : Reception de la mort d'un fils.\n");
}


/**
 * Permet de lire ligne à ligne la requête et de remplir la structure InfRequest
 */
void readLine(int id_socket)
{
	memset(buffer, 0, sizeof(buffer));
	int i = 0;
	int lu;
	char caractere[1];
	char* lastChar;

	while(1)
	{
		lu = read(id_socket, caractere, 1);

		if (lu == 0 || caractere[0] == '\n')
		{
			buffer[i] = caractere[0];
			buffer[i+1] = '\0';
			break;
		}

		buffer[i] = caractere[0];
		i++;

	}
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
	a.sa_handler = receiveKillFromChild;
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
			//recupere l'hote sur 1023 caracteres
			gethostname(hostname, 1023);
			struct hostent* hostinfos = gethostbyname(hostname);
			printf("INFO : Hostname -> %s\n", hostinfos->h_name);

			if (listen(id_socket_server_listen, nbMaxCo) == 0)
			{
				printf("SUCCESS : Listen socket écoute.\n");

				int sizeofSockaddr_in = sizeof(struct sockaddr_in);
				int id_socket_server_service = accept(id_socket_server_listen, (struct sockaddr *)&p, (socklen_t*)&sizeofSockaddr_in);

				printf("CREATION %i\n", id_socket_server_service);

				while (id_socket_server_service != -1)
				{
					/** int pid : PID du processus fils **/
					int pid;

					//Variables SUPER UTILES
					char* xmlContent;
					char id_infirmiere[3];
					char* xml = "&xml=";
					char* startXmlParam;
					char* request_good;
					char* startXmlValue;
					int length_request = 0; // récupéré depuis la requete reçue
					int length_read = 0; // incrémenté par READ
					int i; // itération boucle

					FILE* fichierXmlSave = NULL;

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

								fichierXmlSave = fopen("../data/xmlRequest.xml","w");
								if(fichierXmlSave == NULL)
								{
									printf("ERROR : Impossible d'ouvrir le fichier\n");
								}
								else
								{
									printf("INFO : Fichier ouvert\n");
								}


								while((int)strlen(buffer) != 2)
								{
									readLine(id_socket_server_service);

									if(strstr(buffer, "Content-Length: ") != NULL)
									{
										length_request = atoi(buffer + 16);
										printf("INFO : CONTENT-LENGHT : %i\n", length_request);
									}
								}

								request_body = (char *)malloc(length_request);
								while(length_read < length_request)
								{
									length_read = length_read + read(id_socket_server_service, (request_body + length_read), length_request);
								}

								startXmlParam = strstr(request_body, xml);
								xmlContent = (char *)malloc(length_request);

								//GESTION ID + XML
								for (i = 0; i < 3; i++)
								{
									id_infirmiere[i] = request_body[3 + i];
								}

								startXmlValue = startXmlParam + 5;
								strcpy(xmlContent, startXmlValue);

								// url decode le xml
								request_good = curl_easy_unescape(curl_easy_init(), xmlContent, 0, NULL	);

								printf("INFO : ID INFIRMIERE : %s\nINFO : XML : \n%s\n", id_infirmiere, request_good);
								fputs(request_good, fichierXmlSave);
								fclose(fichierXmlSave);

								printf("INFO : Fichier fermé.\n");

								//libere la mémoire du decodage
								curl_free(request_good);
								//free
								free(request_body);
								free(xmlContent);

								// appel du client pour requete Google
								client();

								//TODO ENvoi du contenu du fichier HTML au server NODEJS avec la socket de service


								//Ferme la socket d'écoute
								close(id_socket_server_service);

								exit(0); /* fin du processus fils */
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

					id_socket_server_service = accept(id_socket_server_listen, (struct sockaddr *)&p, (socklen_t*)&sizeofSockaddr_in);
				}

				perror("ERROR : Fermeture du server EXIT.\n");
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
