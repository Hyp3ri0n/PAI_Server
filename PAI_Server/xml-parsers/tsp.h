#ifndef TRIBULLE_H
#define TRIBULLE_H
#include <vector>
#include <stack>
#include "kruskal.h"
#include <iostream>
/**
 * Permet de générer le parcours en profondeur du graphe passé en paramètre
 *
 * @param T Le graphe contenant l'arbre couvrant de poids min de G
 * @param sommetCourant Le sommet courant (sur lequel on travail)
 * @param chemin Le vector à remplir (contiendra le chemin à parcourir)
 * @param visite Le vector qui contient à l'indice du sommet : "1" -> déjà visité | "0" -> non visité
 */
void parcours(std::vector<std::vector<double> > &T, int sommetCourant, std::vector<int> &chemin, std::vector<int> &visite);


/**
 * Permet de récupérer les sommets dans l'ordre de visite d'un tour de longueur au plus 2 fois l'optimal
 *
 * @param G Le graphe qu'il faut traiter
 * @param depart Le sommet de départ du parcours
 * @return Un vector de "int" regroupant les sommets dans l'ordre du parcours
 */
std::vector<int> travelingSalesmanPerson(std::vector<std::vector<double> > &G, int depart);
#endif