#include "triBulle.h"
using namespace std;

/**
 * Permet de trier par ordre décroissant les aretes (tri du poids)
 *
 * @param aretes Le vector d'"Arete" à trier
 */
void triBulle(vector<Arete> &aretes)
{
    bool echange = true;

    //Tant que le tableau n'est pas trié
    while(echange)
    {
        echange = false;
        //Pour chaque arete
        for(unsigned int i = 0; i < aretes.size()-1; i++)
        {
            //On vérifie que l'ordre est respecté
            if(aretes[i].poids < aretes[i+1].poids)
            {
                //Si non respecté, on inverse -> le tableau n'est pas trié
                Arete tmp = aretes[i];
                aretes[i] = aretes[i+1];
                aretes[i+1] = tmp;
                echange = true;
            }
        }
    }
}
