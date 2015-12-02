#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "SortVisits.h"
#include "FromXMLToGoogleMapHTTPRequest.h"
#include "FromGoogleMapXMLToDistanceTable.h"

int main(int argc, char *argv[]) {
    //Securite pour verif les arguments
    if (argc < 3) {
        std::cout << "Veuillez entrer le nom du fichier a analyser ainsi que l'identifiant de l'infirmiare en paramètre de la commande : testParsers filename.xml id" << std::endl;
        std::cout << "Exemple: " << std::endl;
        std::cout << "> ./testParsers ../data/cabinetInfirmier.xml 1" << std::endl << std::endl;
        return 1;
    }

    //Recup des args
    char * filename = argv[1];
    char * id = argv[2];
    int idInt = atoi(id);


    //Création de variables
    LwSaxParser simpleParser;   //Verif doc xml valide
    FromXMLToGoogleMapHTTPRequest dataBaseParser;   //Recup la requête GoogleMap
    FromGoogleMapXMLToDistanceTable googleMapParser;    //Parser le résultat de la requête GoogleMap
    SortVisits sorter;

    std::vector<std::string> adresses;
    std::vector< std::vector<double> > distances;
    std::string inputStd(filename);
    std::string googleMapFilename;
    std::string outputFilename;
    std::string xsltFilename;
    
    int option = -1;
    while (option != 0) 
    {
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "Que voulez-vous faire avec ce fichier ?" << std::endl;
        std::cout << "0 quitter l'application" << std::endl;
        std::cout << "1 tester le parseur LwSaxParser" << std::endl;
        std::cout << "2 obtenir la requête HTTP à envoyer à GoogleMap pour récupérer les matrices de distances entre les adresses " << std::endl;
        std::cout << "3 créer un tableau c++ à partir du fichier XML renvoyé par GoogleMap " << std::endl;
        std::cout << "4 Modifier la base de données XML donnée avec une liste d'adresses ordonnées pour une infirmière" << std::endl;
        std::cout << "5 Créer un document HTML à partir du doc XML de base de données et d'une feuille de transfomration XSLT (en passant un id en paramètre) " << std::endl;
        std::cout << "Veuillez taper 0, 1, 2, 3, 4 ou 5" << std::endl;
        std::cout << "-------------------------------------------" << std::endl;

        std::cin >> option;

        if (std::cin.fail()) {
            std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), std::cin.widen ( '\n' ));
            option = -1;
        }

        switch (option) 
        {
            case 0: // On ne fait rien et l'on s'en va...
                break;

            case 1: //Tester le parseur
                std::cout << std::endl;
                simpleParser.parseDocument(filename);
                std::cout << std::endl;
                break;

            case 2: // obtenir la requête HTTP à envoyer à GoogleMap pour récupérer les matrices de distances entre les adresses
                std::cout << std::endl;
                std::cout << std::endl;
                std::cout << "La requête HTTP est: " << dataBaseParser.getGoogleHttpRequest(filename, idInt);
                break;

            case 3: // créer un tableau c++ à partir du fichier XML renvoyé par GoogleMap

				// Pour le GoogleMapParser, ce n'est pas le fichier "cabinetInfirmier.xml" qu'il faut lire, mais le fichier renvoyé par Google.
				// On demande donc ici son entrée manuelle.
				std::cout << "Veuillez entrer le nom (i.e. le chemin relatif + nomDuFichier + extension) du fichier xml qui contient le résultat de la requête GoogleMap " << std::endl;
				std::cin >> googleMapFilename;
				googleMapParser.parseDocument(googleMapFilename.c_str());
				std::cout << std::endl;
				std::cout << std::endl;
				adresses = googleMapParser.getAdresses();
				distances = googleMapParser.getDistances();
				
				// écrire la liste des adresses:
				for (unsigned int i = 0; i < adresses.size(); i++)
				{
					std::cout << "| " << i << " | " << adresses.at(i) << " | " << std::endl;
				}
				std::cout << std::endl;
				std::cout << std::endl;
				
				// écrire le tableau des distances avec les index
				std::cout << "     | ";
				for(unsigned int c = 0; c < adresses.size(); c++) {
					std::string spacesBefore = "";
					if (c < 10)
						spacesBefore = "   ";
					else if (c < 100)
						spacesBefore = "  ";
					else if (c < 1000)
						spacesBefore = " ";
					else
						spacesBefore = "";
				
					std::cout << spacesBefore << c <<  " | ";
				}
				std::cout << std::endl << "  --------------------------------" << std::endl;
				
				for (unsigned int ligne = 0; ligne < distances.size(); ligne++)
				{
					std::cout << "   " << ligne << " | ";
					for (unsigned int colonne = 0; colonne < distances.at(ligne).size(); colonne++)
					{
						double value = (distances.at(ligne)).at(colonne);
						std::string spacesBefore = "";
						if (value < 10.0)
							spacesBefore = "   ";
						else if (value < 100.0)
							spacesBefore = "  ";
						else if (value < 1000.0)
							spacesBefore = " ";
						else
							spacesBefore = "";
				
						std::cout << spacesBefore << value << " | ";
					}
					std::cout << std::endl << "  --------------------------------" << std::endl;
				}

                break;

                case 4: // Modifier la base de données XML donnée avec une liste d'adresses ordonnées pour une infirmière
					
					// On demande donc ici son entrée manuelle.
					std::cout << "Veuillez entrer le nom (i.e. le chemin relatif + nomDuFichier + extension) du fichier xml qui contient le résultat de la requête GoogleMap " << std::endl;
					std::cin >> googleMapFilename;
					googleMapParser.parseDocument(googleMapFilename.c_str());
					std::cout << std::endl;
					std::cout << std::endl;
					//adresses = googleMapParser.getAdresses();
					//distances = googleMapParser.getDistances();


					// Ici, on triche, on récupère la matrice des adresses du fichier d'origine et l'on échange 2 lignes...
					// En réalité, il faudra demander à la partie RO de ré-organiser cette matrice !
					std::cout << "Test TSP"<<std::endl;
					sorter.processDistanceMatrix(filename, googleMapFilename.c_str(), idInt, xsltFilename.c_str(), outputFilename.c_str());
					std::cout << "Test FIN TSP"<<std::endl;

					// On échange 2 adresses pour le test...
					if (adresses.size() > 3) {
						std::string tmp = adresses.at(3);
						adresses.at(3) = adresses.at(2);
						adresses.at(2) = tmp;
					}
					// écrire la liste des adresses:
					for (unsigned int i = 0; i < adresses.size(); i++)
					{
						std::cout << "| " << i << " | " << adresses.at(i) << " | " << std::endl;
					}
					std::cout << std::endl;
					std::cout << std::endl;
					
					// La méthode écrit un nouveau fichier xml.
					// On demande le nom de ce fichier.
					std::cout << "Veuillez entrer le nom du fichier de sortie: " << std::endl;
					std::cin >> outputFilename;
					sorter.modifyFile(filename, adresses, outputFilename.c_str());

                    break;

                case 5: // Display the HTML file
                	
					// On demande à l'utilisateur le nom du fichier xslt
					std::cout << "Veuillez entrer le nom du fichier de la feuille XSLT... " << std::endl;
					std::cin >> xsltFilename;
					// On demande à l'utilisateur le nom du fichier html qu'il veut savuegarder
					std::cout << "Veuillez entrer le nom du fichier html de sortie ..." << std::endl;
					std::cin >> outputFilename;
					
					sorter.saveXHTMLFile(filename, xsltFilename.c_str(), outputFilename.c_str(), idInt);
					
                    break;

            default:
                option = -1;
                break;
        }
        std::cout << std::endl << "-------------------------------------------" << std::endl << std::endl;
    }

	return 0;
}
