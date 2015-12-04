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

#include "client.h"

char c_buffer[1024] = "";

void client_readLine(int id_socket)
{
	memset(c_buffer, 0, sizeof(c_buffer));
	int i = 0;
	int lu;
	char caractere[1];
	char* lastChar;

	while(1)
	{
		lu = read(id_socket, caractere, 1);

		if (lu == 0 || caractere[0] == '\n')
		{
			c_buffer[i] = caractere[0];
			c_buffer[i+1] = '\0';
			break;
		}

		c_buffer[i] = caractere[0];
		i++;

	}
}

/**
 * Fonction appelé par le serveur pour communiquer avec Google
 */
int client()
{
	char buffer_client[5000];
	FILE* fichierXmlGoogle = NULL;
	/********************************************************************/
	/*																	*/
	/*						GESTION DES SOCKETS							*/
	/*																	*/
	/********************************************************************/

	/** struct hostent* : Structure qui contient la configuration de la machine **/
	struct hostent* hostinfos = gethostbyname("www-cache.ujf-grenoble.fr");

	/** struct sockaddr_in* p : Structure qui contient la configuration de la socket **/
	struct sockaddr_in p;

	//Création de la socket
	int id_socket_client_emmet = socket(AF_INET, SOCK_STREAM, 0);

	if (id_socket_client_emmet != -1)
	{
		printf("\nSUCCESS : Création socket d'émission.\n");

		//Configuration de la socket
		p.sin_family = AF_INET;
		p.sin_port = htons(3128);

		//Copie de l'adresse de la machine dans la structure de config de la socket
		memcpy(&p.sin_addr.s_addr, hostinfos->h_addr, hostinfos->h_length);

		int sizeofSockaddr_in = sizeof(struct sockaddr_in);

		if (connect(id_socket_client_emmet, (struct sockaddr *)&p, (socklen_t)sizeofSockaddr_in) == 0)
		{
			printf("SUCCESS : Connect socket d'émission.\n");
			fichierXmlGoogle = fopen("../data/reponseGoogle.xml","w");
			if(fichierXmlGoogle == NULL)
			{
				printf("ERROR : Erreur ouverture du fichier (GOOGLE)\n");
			}
			else
			{
				printf("INFO : Fichier ouvert (GOOGLE)\n");
			}
			FromXMLToGoogleMapHTTPRequest r;

			/*char* debutGet = "GEThttp://maps.googleapis.com/maps/api/distancematrix/xml?sensor=false&mode=driving&unit=metric&";
			char* finGet = " HTTP/1.1\r\nAccept: text/html, application/xhtml+xml,application/xml\r\nAccept-language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3-us";*/

			char* req = r.getGoogleHttpRequest("../data/xmlRequest.xml", 001);

			//Exemple pour test
			char* bufferEnvoi = (char*)malloc(300 + strlen(req) + 1);
			sprintf(bufferEnvoi,"GET http://maps.googleapis.com/maps/api/distancematrix/xml?sensor=false&mode=driving&unit=metric&%s\r\nHTTP/1.1\r\nAccept: text/html,application/xhtml+xml,application/xml\nAccept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3-us", req);
			//bufferEnvoi = "GET http://maps.googleapis.com/maps/api/distancematrix/xml?sensor=false&mode=driving&unit=metric&origins=St%2BMartin%2Bd%5C%27H%C3%A8res%2B38400%2B60%2BRue%2Bde%2Bla%2Bchimie%7CGrenoble%2B38031%2B46%2BAvenue%2BFelix%2Bviallet%7CLa%2BTronche%2B38700%2BRond-Point%2Bde%2Bla%2BCroix%2Bde%2BVie%7C&destinations=St%2BMartin%2Bd%5C%27H%C3%A8res%2B38400%2B60%2BRue%2Bde%2Bla%2Bchimie%7CGrenoble%2B38031%2B46%2BAvenue%2BFelix%2Bviallet%7CLa%2BTronche%2B38700%2BRond-Point%2Bde%2Bla%2BCroix%2Bde%2BVie%7C\r\nHTTP/1.1\r\nAccept: text/html,application/xhtml+xml,application/xml\nAccept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3-us";
			printf("\n\nINFO : requete (obtenue par fonction getGoogleHttpRequest) ->\n\n%s\n\nINFO : requete à envoyer à Google ->\n\n%s\n\n", req, bufferEnvoi);

			// Partie non fonctionnelle
			//
			printf("ID SOCKET : %i\n", id_socket_client_emmet);
			printf("BUUFER : %s\n", bufferEnvoi);
			printf("SIZE : %i\n", strlen(bufferEnvoi));

			//Envoi vers Google via le proxy
			write(id_socket_client_emmet, bufferEnvoi, strlen(bufferEnvoi));
			printf("INFO : FIN DE L'ENVOI\n");



			//Lecture de la réponse
			//read(id_socket_client_emmet, buffer_client, 4500);
			printf("INFO : Réponse reçu ->\n%s\n", buffer_client);

			while((int)strlen(c_buffer) != 2)
			{
				client_readLine(id_socket_client_emmet);
			}

			while(read(id_socket_client_emmet, buffer_client, 255) != 0)
			{
				fputs(buffer_client, fichierXmlGoogle);
				memset(buffer_client,0,sizeof(buffer_client));
			}

			fclose(fichierXmlGoogle);

			//TODO : Verifications
			SortVisits triVisites;
			triVisites.processDistanceMatrix("../data/xmlRequest.xml","../data/reponseGoogle.xml", 1, "../data/cabinet.xslt", "../data/cabinet.html");
		}
		else
			perror("ERROR : Connect socket d'émission.\n");
	}
	else
		perror("ERROR : Création socket d'émission.\n");


	return 0;
}
