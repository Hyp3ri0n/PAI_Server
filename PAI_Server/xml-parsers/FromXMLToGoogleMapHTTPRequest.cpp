#include <string.h>
#include <iostream>
#include <curl.h>
#include <stdlib.h>
#include <stdio.h>
#include "Address.h"
#include "FromXMLToGoogleMapHTTPRequest.h"

// Constructeur
FromXMLToGoogleMapHTTPRequest::FromXMLToGoogleMapHTTPRequest() : LwSaxParser()
{
    // On initialise les attributs...
    isCabinet = false;
    nurseId = "";
    currentState = OTHER;
    addressList = "";
    request = "";
    currentAddress = NULL;
}

FromXMLToGoogleMapHTTPRequest::~FromXMLToGoogleMapHTTPRequest() {
    // On libère l'adresse courante
    if (currentAddress != NULL) {
        delete currentAddress;
    }    
}

char * FromXMLToGoogleMapHTTPRequest::getGoogleHttpRequest(char * dataBaseFileName, int nurseNumber) {
    char * result;
    // Convertir l'entier nurseNumber en std::string pour pouvoir le stocker dans l'attribut id
    // Possible en C -> lire l'API doc de la méthode itoa ou sprintf
    // Plus simple en C++ en utilisant la STL
    std::ostringstream nbStr;
    if (nurseNumber < 100) {
        nbStr << "0";
    }
    if (nurseNumber < 10) {
        nbStr << "0";
    }
    nbStr << nurseNumber;
    nurseId = nbStr.str();
    std::cout << "Nurse Id: " << nurseId.c_str() << std::endl;
    
    // Parser le docuemnt
    // Le parsing a pour but de compléter addressList, l'instruction suivante la met en forme...
    parseDocument(dataBaseFileName);
    
    
    //TODO : SUPPR DERNIER PIPE EN TROP ADDRESS LIST
    //std::cout << "Adresse liste : " << addressList.c_str() << std::endl;
    
    // Une fois que l'on a terminé le parsing du document, on converti la liste d'adresse en requête Google
    // et on la stocke dans l'attribut request de type std::string (plus facile à manipuler que char *).
    request = "origins=" + std::string(curl_easy_escape(NULL,addressList.c_str(),0)) + "&destinations=" + std::string(curl_easy_escape(NULL,addressList.c_str(),0));

    // Convertir la requête au format std::string en char *
    //    result = request.c_str(); -> ne fonctionne pas car request.c_str() renvoie quelque chose de const
    result = new char[request.length() + 1];
    strcpy(result, request.c_str());
        
    // Renvoyer le résultat    
    return result;
    
}

/*
 * Méthode d'aide qui permet de trouver un attribut qui a un certain nom dans une liste d'attribut.
 * Cette méthode existe de base dans l'API Sax de Java, mais pas ici, c'est pourquoi elle est donnée.
 */
std::string FromXMLToGoogleMapHTTPRequest::findAttribute(const AttributeList& attributeList, std::string attributeName) {
    std::string resultat = "";
    xmlpp::SaxParser::AttributeList::const_iterator iter = attributeList.begin();
    while ((iter != attributeList.end()) && (iter->name != attributeName)) {
        iter++;
    }
    if (iter != attributeList.end()) {
        resultat = iter->value;
    }
    return resultat;
}


/**
 * Gestion SAXParse
 */
void FromXMLToGoogleMapHTTPRequest::on_start_element(const Glib::ustring& name, const AttributeList& attributes)
{
	bool verif = false;
	
	//std::cout << "Nom du noeud : " << name.c_str() << std::endl;
  
  if (strcmp(name.c_str(), "visite") == 0)
  {
	  currentState = VISITE;
	  for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
	  {
		if (strcmp(iter->name.c_str(), "intervenant") == 0)
			if (iter->value.c_str() == nurseId)
				verif = true;
	  }
	  
	  if (!verif)
			  currentAddress = NULL;
  }
  else if (strcmp(name.c_str(), "adresse") == 0)
  {
	  currentState = ADRESSE;
	  currentAddress = new Address();
  }
  else if (strcmp(name.c_str(), "etage") == 0)
  {
	  currentState = ETAGE;
  }
  else if (strcmp(name.c_str(), "numero") == 0)
  {
	  currentState = NUMERO;
  }
  else if (strcmp(name.c_str(), "rue") == 0)
  {
	  currentState = RUE;
  }
  else if (strcmp(name.c_str(), "codePostal") == 0)
  {
	  currentState = CODEPOSTAL;
  }
  else if (strcmp(name.c_str(), "ville") == 0)
  {
	  currentState = VILLE;
  }
  else if (strcmp(name.c_str(), "infirmiers") == 0)
  {
	  currentState = INFIRMIERS;
	
	  if (currentAddress != NULL)
	  {
		  //On rempli l'adresselist avec l'adresse courante + l'adresselist
		  addressList += currentAddress->getGoogleAdresse() + "|";
		  currentAddress = NULL;
	  }
  }
  else
  {
	  currentState = OTHER;
	  //std::cout << "[XML-CABINET]Noeud non géré : " << name.c_str() << std::endl;
  }
}

void FromXMLToGoogleMapHTTPRequest::on_end_element(const Glib::ustring& name)
{	
	//Pour gerer les espaces en trop
	//A chaque fin d'élément state à OTHER car aucun traitement
	//Puis retourne sur un noeud donc startElement déclenchée 
	//donc state redéfini correctement
	currentState = OTHER;
	 
	if (strcmp(name.c_str(), "patient") == 0)
	{
		if (currentAddress != NULL)
		{
		  //On rempli l'adresselist avec l'adresse courante + l'adresselist
		  addressList += currentAddress->getGoogleAdresse() + "|";
		  currentAddress = NULL;
		}
	}
}

void FromXMLToGoogleMapHTTPRequest::on_characters(const Glib::ustring& text)
{
    /*std::cout << "Appel de la méthode: on_characters(const Glib::ustring& text) " << std::endl;
    std::cout << "    avec les arguments suivants: " << std::endl;
    std::cout << "      - text: " << text.c_str() << std::endl;*/
    
    std::string textRue;
    std::string textVille;
    
    
	switch (currentState)
	{
		case ETAGE:
		  
		  //PAS GERE PAR GOOGLE
		  
		  break;
		  
		case NUMERO:
		  
		  //std::cout << "[FromXMLToGoogleMapHTTPRequest] Test numéro: Avant Set avec " << text.c_str() << std::endl;
		  //WIP set des num, etc...
		  currentAddress->setNumero(text.c_str());
		  break;
		  
		case RUE:
		  
		  //WIP set des num, etc...
		  textRue = text.c_str();
		  replaceAll(textRue, "\'", "\\\'");
		  replaceAll(textRue, " ", "+");
		  //std::cout << "[FromXMLToGoogleMapHTTPRequest] Test rue: Avant Set avec " << textRue << std::endl;
		  currentAddress->setRue(textRue);
		  
		  break;
		  
		case CODEPOSTAL:
		  
		  //WIP set des num, etc...
		  //std::cout << "[FromXMLToGoogleMapHTTPRequest] Test cp : Avant Set avec " << text.c_str() << std::endl;
		  currentAddress->setCodePostal(text.c_str());
		  
		  break;
		  
		case VILLE:
		  
		  //WIP set des num, etc...
		  textVille = text.c_str();
		  replaceAll(textVille, "\'", "\\\'");
		  replaceAll(textVille, " ", "+");
		  //std::cout << "[FromXMLToGoogleMapHTTPRequest] Test ville: Avant Set avec " << textVille << std::endl;
		  currentAddress->setVille(textVille);
		  
		  break;
		  
		default :
		
		  break;
	}
}


void FromXMLToGoogleMapHTTPRequest::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

