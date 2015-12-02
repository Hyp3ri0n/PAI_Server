#include "tsp.h"
using namespace std;

/**
 * Permet de générer le parcours en profondeur du graphe passé en paramètre
 *
 * @param T Le graphe contenant l'arbre couvrant de poids min de G
 * @param sommetCourant Le sommet courant (sur lequel on travail)
 * @param chemin Le vector à remplir (contiendra le chemin à parcourir)
 * @param visite Le vector qui contient à l'indice du sommet : "1" -> déjà visité | "0" -> non visité
 */
void parcours(vector<vector<double> > &T, int sommetCourant, vector<int> &chemin, vector<int> &visite)
{
    chemin.push_back(sommetCourant);
    visite[sommetCourant] = 1;

    //Pour chaque sommets du graphe
    for(unsigned int i = 0; i < T.size(); i++)
    {
        //Si le sommet est accessible et n'a pas déjà été visité
        //if((T[sommetCourant][i] != -1 && T[i][sommetCourant] != -1) && visite[i] == 0)
        if(T[sommetCourant][i] != -1 && visite[i] == 0)
        {
            //Visite les voisins du sommet
            parcours(T,i,chemin,visite);
        }
    }
}


/**
 * Permet de récupérer les sommets dans l'ordre de visite d'un tour de longueur au plus 2 fois l'optimal
 *
 * @param G Le graphe qu'il faut traiter
 * @param depart Le sommet de départ du parcours
 * @return Un vector de "int" regroupant les sommets dans l'ordre du parcours
 */
vector<int> travelingSalesmanPerson(vector<vector<double> > &G, int depart)
{
	vector<vector<double> > T = G; // Copie de G
	vector<int> visite(G.size(), 0); // Tableau permettant de noter les sommets visité par le parcours en profondeur
	vector<int> travel; // Liste des sommets dans l'ordre de visite

    //Si T arbre couvrant de poids min. -> recherche du plus court chemin
	if (kruskal(G, T))
        parcours(T, depart, travel, visite);
    else
    {
        cout<<"Le graphe ne possède pas d'arbre couvrant minimum."<<endl;
        travel.clear();
    }

	return travel;
}
