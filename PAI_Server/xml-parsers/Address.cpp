#include "Address.h"

// Implémentation du constructeur
// Rappel, en C++, comme ne Java, le role du constructeur est d'initialiser les attributs.
Address::Address() {
    hasNumero = false;
    numero = "";
    rue = "";
    ville = "";
    codePostal = "";
}

void Address::setNumero(std::string numero) {
    this->numero.append(numero);
    this->hasNumero = true;
}
void Address::setRue(std::string rue) {
    this->rue.append(rue);
}

void Address::addCharacterToRue(std::string chars) {
    this->rue.append(chars);
}
void Address::setVille(std::string ville) {
    this->ville.append(ville);
}

void Address::addCharacterToVille(std::string chars) {
    this->ville.append(chars);
}

void Address::setCodePostal(std::string codePostal) {
    this->codePostal.append(codePostal);
}

std::string Address::getGoogleAdresse() {
    std::string resultat = ville;
    resultat += "+" + codePostal;
    if (hasNumero) {
        resultat += "+" + numero;
    }
    resultat += "+" + rue;
    
    return resultat;
}

std::string Address::getReadableAdresse()
{
  std::string resultat = numero;
  if (hasNumero)
    resultat += " ";
  resultat += rue + ", " + codePostal + " " + ville + ", France";
  
  return resultat;
}


