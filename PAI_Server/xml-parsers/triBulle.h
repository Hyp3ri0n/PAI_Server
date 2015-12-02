#ifndef TRIBULLE_H
#define TRIBULLE_H
#include <vector>

/**
 * Structure représentant une arête
 */
struct Arete
{
    int u;
    int v;
    double poids;
};

/**
 * Permet de trier par ordre décroissant les aretes (tri du poids)
 *
 * @param aretes Le vector d'"Arete" à trier
 */
void triBulle(std::vector<Arete> &aretes);
#endif