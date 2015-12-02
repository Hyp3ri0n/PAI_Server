/********************************************************************/
/*																	*/
/*							Client PAI								*/
/*																	*/
/*																	*/
/*	Date : 01/12/2015												*/
/*	Description : Serveur métier du projet PAI L3 MIAGE 2015.		*/
/*	Spécification : Utilisation de sockets TCP (Emission).			*/
/*	Developpeurs :	- MARTINIER Alexis								*/
/*					- VAILLANT Hugo									*/
/*					- ORTIZ Luc										*/
/*					- FERNANDES Aurélien							*/
/*																	*/
/********************************************************************/

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

	/********************************************************************/
	/*																	*/
	/*						GESTION DE L'INTERFACE						*/
	/*																	*/
	/********************************************************************/

	/** int port : Entier qui defini le port entré par l'utilisateur **/
	int port;
	/** char* server : Chaine de caractères conteant le nom de la machine **/
	char server[1024];

	if (argc == 5)
	{
		port = atoi(argv[2]);
		strcpy(server, argv[4]);
	}
	else if (argc == 3)
	{
		port = atoi(argv[2]);

		//Si pas d'adresse ip renseigner alors faire en local
		server[1023] = '\0';
		gethostname(server, 1023);
	}
	else
	{
		perror("INFO : Vous n'avez pas saisi toutes les informations nécessaire.\n");
		perror("INFO : Arrêt de l'application.\n");
		exit(0);
	}


	/********************************************************************/
	/*																	*/
	/*						GESTION DES SOCKETS							*/
	/*																	*/
	/********************************************************************/

	/** struct hostent* : Structure qui contient la configuration de la machine **/
	struct hostent* hostinfos = gethostbyname(server);

	/** struct sockaddr_in* p : Structure qui contient la configuration de la socket **/
	struct sockaddr_in p;

	//Création de la socket
	int id_socket_client_emmet = socket(AF_INET, SOCK_STREAM, 0);

	if (id_socket_client_emmet != -1)
	{
		printf("SUCCESS : Création socket d'émission.\n");

		//Configuration de la socket
		p.sin_family = AF_INET;
		p.sin_port = htons(port);

		//Copie de l'adresse de la machine dans la structure de config de la socket
		memcpy(&(p.sin_addr.s_addr), hostinfos->h_addr, hostinfos->h_length);

		if (connect(id_socket_client_emmet, (struct sockaddr *)&p, sizeof(struct sockaddr_in)) == 0)
		{
			printf("SUCCESS : Connect socket d'émission.\n");

			//TODO : Buffer à remplir avec la requête
			char* bufferEnvoi = "POST /INFIRMIERE HTTP/1.1\nHost: 127.0.0.1:4444\nContent-Type: app...\nContent-Length: 12345\nVia: test...\nX-Forwarded-For: 192.168.0.1\nCache-Control: test...\nConnection: test...\n\nid=001&xml=%3C";

			//Gestion de l'envoi
			int len = strlen(bufferEnvoi);
			int len_sent = 0;
			while (len_sent < len)
			{
				len_sent = len_sent + write(id_socket_client_emmet, bufferEnvoi + len_sent, 255);
				printf("INFO : Write octets -> %i.\n", len_sent);
			}
		}
		else
			perror("ERROR : Connect socket d'émission.\n");
	}
	else
		perror("ERROR : Création socket d'émission.\n");


	return 0;
}
