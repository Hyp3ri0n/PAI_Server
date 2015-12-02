#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>

class Address {

private:
  /// Attributs
    bool hasNumero;
    std::string numero;
    std::string rue;
    std::string ville;
    std::string codePostal;
    
public:
    // Constructeur
    Address();
    void setNumero(std::string numero);
    void setRue(std::string rue);
    void addCharacterToRue(std::string chars);

    void setVille(std::string ville);
    void addCharacterToVille(std::string chars);

    void setCodePostal(std::string codePostal);
    std::string getGoogleAdresse();
    std::string getReadableAdresse();

   
    
};

#endif
