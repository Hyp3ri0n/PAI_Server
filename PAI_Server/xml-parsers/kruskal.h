#ifndef KRUSKAL_H
#define KRUSKAL_H
#include <vector>
#include "triBulle.h"


/**
 * Permet d'additionner les valeur d'un vector de "int"
 *
 * @param visite Le vector de "int" contenant les sommets déjà visités
 * @return La somme des valeurs
 */
int sommeVector(std::vector<int> &visite);


/**
 * Permet de savoir si le graphe passé en paramètre est connexe ou non
 *
 * @param T Le graphe à modifier
 * @param visite Le vector qui contient les sommets déjà visités (1)
 * @param sommetCourant Le sommet courant (sur lequel on travail)
 * @return La valeur "true" si le graph est connexe sinon "false"
 */
bool connexe(std::vector<std::vector<double> > &T, std::vector<int> &visite,int sommetCourant);


/**
 * Permet de stocker dans un graphe un arbre couvrant de poids min
 *
 * @param G Le graphe rempli
 * @param T Le graphe à remplir avec l'arbre de poids couvrant
 * @return La valeur "true" si l'algo c'est bien passé sinon "false"
 */
bool kruskal(std::vector<std::vector<double> > &G, std::vector<std::vector<double> > &T);
#endif