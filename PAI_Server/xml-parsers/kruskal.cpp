#include "kruskal.h"
using namespace std;


/**
 * Permet d'additionner les valeur d'un vector de "int"
 *
 * @param visite Le vector de "int" contenant les sommets déjà visités
 * @return La somme des valeurs
 */
int sommeVector(vector<int> &visite)
{
    int somme = 0;

    for(unsigned int i = 0; i < visite.size(); i++)
        somme += visite[i];

    return somme;
}


/**
 * Permet de savoir si le graphe passé en paramètre est connexe ou non
 *
 * @param T Le graphe à modifier
 * @param visite Le vector qui contient les sommets déjà visités (1)
 * @param sommetCourant Le sommet courant (sur lequel on travail)
 * @return La valeur "true" si le graph est connexe sinon "false"
 */
bool connexe(vector<vector<double> > &T, vector<int> &visite,int sommetCourant = 0)
{
    bool estConnexe = false;
    visite[sommetCourant] = 1;
    for(unsigned int i = 0; i < T.size(); i++)
    {
		//if(visite[i] == 0 && (T[sommetCourant][i] != -1 && T[i][sommetCourant] != -1))
        if(visite[i] == 0 && T[sommetCourant][i] != -1)
            connexe(T, visite, i);
    }

    if(sommeVector(visite) == T.size())
        estConnexe = true;

    return estConnexe;
}


/**
 * Permet de stocker dans un graphe un arbre couvrant de poids min
 *
 * @param G Le graphe rempli
 * @param T Le graphe à remplir avec l'arbre de poids couvrant
 * @return La valeur "true" si l'algo c'est bien passé sinon "false"
 */
bool kruskal(vector<vector<double> > &G, vector<vector<double> > &T)
{
    /*  Recoit de tsp deux graphes (G & T)
        Tri par ordre décroissant les aretes de G
        Supprime, en vérifiant la connexité, les aretes de T
        Retourne a kruskal l'arbre de poids min. T
    */
    vector<int> visite(G.size(), 0);
    vector<Arete> aretes;
    Arete A;

    bool done = true;

    //Rempli le tableau (vector) d'aretes
    for(unsigned int i = 0; i < G.size(); i++)
    {
        for(unsigned int y = 0; y < G.size(); y++)
        {
            A.u = i;
            A.v = y;
            A.poids = G[i][y];

            aretes.push_back(A);
        }
    }

    //Tri le tabmleau d'aretes par poids décroissant
    triBulle(aretes);


    //Supprime les aretes sur T en vérifiant la connexité
    for(unsigned int i = 0; i < aretes.size(); i++)
    {
        int u = aretes[i].u;
        int v = aretes[i].v;
        double poids = aretes[i].poids;

        //suppression de l'arete sur le graphe
        T[u][v] = -1;

        //Vérifie si le graphe reste connexe ou non
        //si non connexe, on remet l'arete
        if(!connexe(T, visite))
            T[u][v] = poids;

        //Remet à zéro le tableau des sommets visités
        visite.clear();
        visite.resize(G.size(), 0);

    }

    //Si, une fois toutes les aretes traités, le graphe est connexe, on modifie le boolean (true)
    if(connexe(T, visite))
        done = true;

    return done;
}
