#include "SortVisits.h"
#include <iostream>
#include <algorithm> 
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include "Address.h"
#include "FromGoogleMapXMLToDistanceTable.h"
#include "tsp.h"

// Constructeur
SortVisits::SortVisits() {
    
}

SortVisits::~SortVisits() {
    
}

void SortVisits::processDistanceMatrix(char * inputFileName, const char * googleAnswer, int id, const char * xslFilename, const char * outputFileName) {
    FromGoogleMapXMLToDistanceTable googleMapParser;
    std::vector<std::string> adresses;
    std::vector< std::vector<double> > distances;

    googleMapParser.parseDocument(googleAnswer);
    adresses = googleMapParser.getAdresses();
    distances = googleMapParser.getDistances();
    
    // Ici, il faut appeler la fonction développée en RO
    std::vector<int> cheminOptimal = travelingSalesmanPerson(distances,0);
    
    //Affichage du chemin opti.
    std::cout<<std::endl<<"Le chemin optimisé a 2 approx. :\n\t| ";
	for(int i = 0; i < cheminOptimal.size(); i++)
	{
		std::cout<<cheminOptimal[i]<<" | ";
	}
	std::cout<<std::endl;

    std::string inputStd(inputFileName);
    std::string tmpFileName = inputStd.substr(0, inputStd.find_last_of("."));
    tmpFileName += "-sorted.xml";
    
    modifyFile(inputFileName, adresses, tmpFileName.c_str());
    saveXHTMLFile(inputFileName, xslFilename,outputFileName, id);
    
}

std::string SortVisits::getPatientNodeAdresse(xmlpp::Node * adresseNode) {
    Address adresse;
    xmlpp::Node * node;
    xmlpp::Element * element;
 
    // On déclare le namespace uilisé dans le fichier source.
    // Attention, ce namespace doit être le même que celui de votre fichier cabinetInfirmier.xml !!    
    xmlpp::Node::PrefixNsMap nsMap;
    nsMap["cab"] = "http://www.ujf-grenoble.fr/l3miage/medical";
     
    // On récupère le numéro s'il existe
    if (adresseNode->find("cab:numéro", nsMap).size() > 0) {
        node = adresseNode->find("cab:numéro", nsMap).at(0);
        element = dynamic_cast <xmlpp::Element *> (node);
        if (element != NULL) {
            std::string numero = element->get_child_text()->get_content();
            adresse.setNumero(numero);
        }
    }
     
    // On récupère la rue
    xmlpp::NodeSet possiblesRues = adresseNode->find("cab:rue", nsMap);
    // D'après le schéma, il y a forcément une rue dans l'adresse.
    // On teste tout de même si l'élément rue du namespace http://www.ujf-grenoble.fr/l3miage/medical a été trouvé...
    if (possiblesRues.size() == 0)
        return "";
    node = possiblesRues.at(0);
    element = dynamic_cast <xmlpp::Element *> (node);
    if (element != NULL) {
        std::string rue = element->get_child_text()->get_content();
        adresse.setRue(rue);
    }
     
    // On récupère la ville
    xmlpp::NodeSet possiblesVille = adresseNode->find("cab:ville", nsMap);
    // D'après le schéma, il y a forcément une ville dans l'adresse.
    // On teste tout de même si l'élément ville du namespace http://www.ujf-grenoble.fr/l3miage/medical a été trouvé...
    if (possiblesVille.size() == 0)
        return "";
    node = possiblesVille.at(0);
    element = dynamic_cast <xmlpp::Element *> (node);
    if (element != NULL) {
        std::string rue = element->get_child_text()->get_content();
        adresse.setVille(rue);
    }
     
    // On récupère le code postal
    xmlpp::NodeSet possiblesCodePostal = adresseNode->find("cab:codePostal", nsMap);
    // D'après le schéma, il y a forcément un code postal dans l'adresse.
    // On teste tout de même si l'élément codePostal du namespace http://www.ujf-grenoble.fr/l3miage/medical a été trouvé...
    if (possiblesCodePostal.size() == 0)
        return "";
    node = possiblesCodePostal.at(0);
    element = dynamic_cast <xmlpp::Element *> (node);
    if (element != NULL) {
        std::string code = element->get_child_text()->get_content();
        adresse.setCodePostal(code);
    }
     
 
    // Note: dans cette méthode, on a des instructions return qui ne sont
    //  pas la dernière instruction de la méthode. Il s'agit en fait de 
    //  terminaison de la méthode en cas d'erreur.
    return adresse.getReadableAdresse();
}


/// Faire le job...
void SortVisits::modifyFile(const char * inputFilename, std::vector<std::string> adresses, const char * outputFilename) {
    std::cout << " Modifying xml file..." << std::endl;
    xmlpp::DomParser parser;
    parser.parse_file(inputFilename);
    if(parser)
    {
        //Walk the tree:
        xmlpp::Document * doc = parser.get_document();
        const xmlpp::Node* node = doc->get_root_node(); //deleted by DomParser.
 
        // Lorsque l'on a un namespace par défaut, il faut impérativement dans libxml (et libxml++)
        //   déclarer un préfixe pour ce namespace.
        // On déclare donc le namespace uilisé dans le fichier source.
        // Attention, ce namespace doit être le même que celui de votre fichier cabinetInfirmier.xml !!    
        xmlpp::Node::PrefixNsMap nsMap;
        nsMap["cab"] = "http://www.ujf-grenoble.fr/l3miage/medical";
 
        // --------------- On ordonne les patients selon leur adresse
        // On recherche et stocke tous les patients dans une map avec pour
        // clé leur adresse
        std::map<std::string, xmlpp::Element *> patientsAdresses;
        // On doit construire les adresse de la même manière que lors du parsing
        xmlpp::NodeSet patients = node->find("//cab:patients", nsMap);
     
        if (patients.size() == 0) {
            std::cout << "Je n'ai pas trouvé le noeud patients, désolé..." << std::endl;
            return;
        }   
        xmlpp::Node * patientsNode = node->find("//cab:patients", nsMap).at(0);
        xmlpp::NodeSet allPatients = patientsNode->find("cab:patient", nsMap);
       
        if (allPatients.size() == 0) {
            std::cout << "Je n'ai pas trouvé d'éléments patient à l'intérieur de l'élément patients... " << std::endl;
            return;
        }
        for(xmlpp::NodeSet::iterator iter = allPatients.begin(); iter != allPatients.end(); ++iter)
        {
            xmlpp::Element * patient = dynamic_cast<xmlpp::Element *>(*iter);
            // Récupérer l'adresse du patient en question
            xmlpp::NodeSet adressesInPatient = patient->find("cab:adresse", nsMap);
            if (adressesInPatient.size() ==1) {
                xmlpp::Node * adresseNode = adressesInPatient.at(0);
                std::string adresse = getPatientNodeAdresse(adresseNode);
                std::cout << adresse  << std::endl;
               
                xmlpp::Document * newDoc = new xmlpp::Document();
 
                patientsAdresses[adresse] = newDoc->create_root_node_by_import(patient);
                patientsNode->remove_child(patient);              
                   
            }
        }
           
        // Maintenant que tous les patients ont été supprimés du document, il faut les
        //  remettre dans le bon ordre
           
        // On parcourt les adresses d'entrées, on les cherche dans la map, si on les trouve, on les remets
        std::cout << "On rajoute les patients..." << std::endl;
        std::vector<std::string>::iterator it;
        for (it = adresses.begin(); it != adresses.end(); it++) {
            std::string sortedAdresse = (*it);
            xmlpp::Element * element = findAdresseInMap(sortedAdresse, patientsAdresses);
            if (element != NULL) {
                std::cout << "Child found !!, element name: " << std::string(element->get_name()) << std::endl;
                patientsNode->import_node(element);
            }
        }
        // Serialize the output file
        doc->write_to_file(outputFilename, "UTF-8");
    }
         
}


xmlpp::Element * SortVisits::findAdresseInMap(std::string sortedAdresse, std::map<std::string, xmlpp::Element *> map) {
    std::string sortedLower = sortedAdresse;
    std::transform(sortedLower.begin(), sortedLower.end(), sortedLower.begin(), ::tolower);
    std::cout << std::endl << "Searching for adresse: " << sortedLower << std::endl;

    xmlpp::Element * element = NULL;
    std::map<std::string, xmlpp::Element *>::iterator it = map.begin();
    while ((it != map.end()) && (element == NULL)) {
        std::string mapLower = (it->first);
        std::transform(mapLower.begin(), mapLower.end(), mapLower.begin(), ::tolower);
    
        std::cout << "it adresse:     " << mapLower << std::endl;
        
        if (sortedLower.find(mapLower) != std::string::npos) {
            element = it->second;
            std::cout << "Found !!" << std::endl;
        }
        it++;
    }    

    return element;
}

// Ouvre le fichier inputXMLFile, lui applique la transformation XSLT contenue dans le
// fichier xslTransformationFile avec le paramètre nurseId et sauvegarde le résultat dans le
// fichier outputXHTMLFile.
//See http://libxmlplusplus.sourceforge.net/docs/manual/html/ar01s02.html
void SortVisits::saveXHTMLFile(const char * inputXMLFile, const char * xslTransformationFile, const char * outputXHTMLFile, int nurseId)
{
    // File descriptor pour le fichier de sortie
    FILE *outFile = NULL;
    
    // libXML ne prend en paramètre que des xmlChar* et non des char*. 
    // Une conversion explicite doit donc être faite
    // Fichier xml d'entrée
    const xmlChar * xmlFile  = (const xmlChar *) (inputXMLFile);
    // Fichier de transformation
    xsltStylesheetPtr xslST = xsltParseStylesheetFile((const xmlChar *) (xslTransformationFile));

    const char *params[16 + 1];    	
    int nbparams = 0;        
    
    // On transforme l'identifiant de l'infirmière en chaîne de caractères     
    // Convertir l'entier nurseNumber en std::string pour pouvoir le stocker dans l'attribut id
    // Possible en C -> lire l'API doc de la méthode itoa
    char * idStr = new char[4];
    if (nurseId < 100) {
        if (nurseId < 10) {
            sprintf(idStr, "00%d", nurseId);
        } else {
            sprintf(idStr, "0%d", nurseId);
        }
    }
    else {
        sprintf(idStr, "%d", nurseId);
    }
        
    std::cout <<"Save XHTML File: input " << inputXMLFile << ", output: " << outputXHTMLFile << " with nurseId: " << idStr << std::endl;

	params[0] = "destinedId";
    params[1] = idStr;
    params[2] = NULL;
    
	xmlSubstituteEntitiesDefault(1);
	xmlLoadExtDtdDefaultValue = 1;

	xmlDocPtr doc = xmlParseFile(inputXMLFile);    
	xmlDocPtr res = xsltApplyStylesheet(xslST, doc, params);

    outFile=(fopen(outputXHTMLFile,"w"));
       if(outFile==NULL){
           printf("Error!");
           exit(1);
       }

    xsltSaveResultToFile(outFile, res, xslST);

    fclose(outFile);
    
	xsltFreeStylesheet(xslST);
	xmlFreeDoc(res);
	xmlFreeDoc(doc);

    xsltCleanupGlobals();
    xmlCleanupParser();
    
}
