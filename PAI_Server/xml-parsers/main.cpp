#include "tsp.h"
#include <iostream>
#include <vector>

using namespace std;

/**
 * Permet de générer le graphe par défaut
 *
 * @param G Le graphe à remplir
 */
void initialistaionDefaultGraph(vector<vector<double> > &G, int &sommetDepart)
{
    sommetDepart = 0;
    int n = 8; // taille du graphe
    G.resize(n);

    //création matrice 8*8
    for (int i = 0; i < n; i++)
        G[i].resize(n);

    /*

    _________________________________________________________________

    |	-1	|	3	|	4	|	1	|	3	|	10	|	4	|	11	|
    _________________________________________________________________

    |	3	|	-1	|	4	|	2	|	6	|	10	|	4	|	11	|
    _________________________________________________________________

    |	4	|	4	|	-1	|	5	|	7	|	14	|	8	|	11	|
    _________________________________________________________________

    |	1	|	2	|	5	|	-1	|	4	|	9	|	3	|	10	|
    _________________________________________________________________

    |	3	|	6	|	7	|	4	|	-1	|	9	|	3	|	9	|
    _________________________________________________________________

    |	10	|	10	|	14	|	9	|	9	|	-1	|	6	|	13	|
    _________________________________________________________________

    |	4	|	4	|	8	|	3	|	3	|	6	|	-1	|	7	|
    _________________________________________________________________

    |	11	|	11	|	11	|	10	|	9	|	13	|	7	|	-1	|
    _________________________________________________________________
    */


    G[0][0] = -1;
    G[0][1] = 3;
    G[0][2] = 4;
    G[0][3] = 1;
    G[0][4] = 3;
    G[0][5] = 10;
    G[0][6] = 4;
    G[0][7] = 11;

    G[1][0] = 3;
    G[1][1] = -1;
    G[1][2] = 4;
    G[1][3] = 2;
    G[1][4] = 6;
    G[1][5] = 10;
    G[1][6] = 4;
    G[1][7] = 11;

    G[2][0] = 4;
    G[2][1] = 4;
    G[2][2] = -1;
    G[2][3] = 5;
    G[2][4] = 7;
    G[2][5] = 14;
    G[2][6] = 8;
    G[2][7] = 11;

    G[3][0] = 1;
    G[3][1] = 2;
    G[3][2] = 5;
    G[3][3] = -1;
    G[3][4] = 4;
    G[3][5] = 9;
    G[3][6] = 3;
    G[3][7] = 10;

    G[4][0] = 3;
    G[4][1] = 6;
    G[4][2] = 7;
    G[4][3] = 4;
    G[4][4] = -1;
    G[4][5] = 9;
    G[4][6] = 3;
    G[4][7] = 9;

    G[5][0] = 10;
    G[5][1] = 10;
    G[5][2] = 14;
    G[5][3] = 9;
    G[5][4] = 9;
    G[5][5] = -1;
    G[5][6] = 6;
    G[5][7] = 13;

    G[6][0] = 4;
    G[6][1] = 4;
    G[6][2] = 8;
    G[6][3] = 3;
    G[6][4] = 3;
    G[6][5] = 6;
    G[6][6] = -1;
    G[6][7] = 7;

    G[7][0] = 11;
    G[7][1] = 11;
    G[7][2] = 11;
    G[7][3] = 10;
    G[7][4] = 9;
    G[7][5] = 13;
    G[7][6] = 7;
    G[7][7] = -1;


    /*

    _________________________________________

    |	-1	|	7	|	9	|	8	|	2	|
    _________________________________________

    |	7	|	-1	|	3	|	4	|	8	|
    _________________________________________

    |	9	|	3	|	-1	|	2	|	10	|
    _________________________________________

    |	8	|	4	|	2	|	-1	|	10	|
    _________________________________________

    |	2	|	8	|	10	|	10	|	-1	|
    _________________________________________


    G[0][0] = -1;
    G[0][1] = 7;
    G[0][2] = 9;
    G[0][3] = 8;
    G[0][4] = 2;

    G[1][0] = 7;
    G[1][1] = -1;
    G[1][2] = 3;
    G[1][3] = 4;
    G[1][4] = 8;

    G[2][0] = 9;
    G[2][1] = 3;
    G[2][2] = -1;
    G[2][3] = 2;
    G[2][4] = 10;

    G[3][0] = 8;
    G[3][1] = 4;
    G[3][2] = 2;
    G[3][3] = -1;
    G[3][4] = 10;

    G[4][0] = 2;
    G[4][1] = 8;
    G[4][2] = 10;
    G[4][3] = 10;
    G[4][4] = -1;*/
}


/**
 * Permet de générer un graphe à partir de l'input de l'utilisateur
 *
 * @param G Le graphe à remplir
 */
void initialistaionUtilisateurGraph(vector<vector<double> > &G, int &sommetDepart)
{

    int n = 0; // taille du graphe

    cout << endl << "Le nombre de sommet du graph : ";
    cin >> n;

    cout << endl << "Le sommet de départ : ";
    cin >> sommetDepart;

    G.resize(n);
    double poidArete;

    //création matrice n*n
    for (int i = 0; i < n; i++)
    {
        G[i].resize(n);
        cout << endl;
        for(int j = 0; j < n; j++)
        {
            if (i != j)
            {
                cout << "[POIDS] G [" << i << "] [" << j << "] : ";
                cin >> poidArete;
                G[i][j] = poidArete;
            }
            else
            {
                cout << "[POIDS] G [" << i << "] [" << j << "] : -1" << endl;
                G[i][j] = -1;
            }
        }
    }
}

/**
 * Permet d'afficher un graphe
 *
 * @param G Le graphe à afficher
 */
void printGraph(vector<vector<double> > &G)
{
    cout<<endl<<"Représentation du graphe : "<<endl;
    for(int i = 0; i < G.size(); i++)
    {
        cout<<"\t";
        for(int y = 0; y < G.size(); y++)
        {
          cout<<"| "<<G[i][y]<<"\t";
        }
        cout<<" |"<<endl;
    }
}

/**
 * Permet d'executer le programme
 */
int main()
{
    vector<vector<double> > G;
    vector<int> chemin;
    char defaultGraph = 'y';
    int sommetDepart = 0;

    cout<<"Voulez-vous utiliser un graphe par défaut (y/n) ? ";
    cin>>defaultGraph;

    if (defaultGraph == 'y' || defaultGraph == 'Y')
        initialistaionDefaultGraph(G, sommetDepart);
    else
        initialistaionUtilisateurGraph(G, sommetDepart);

    //Affichage du graphe
    printGraph(G);

    //APPEL TSP
    chemin = travelingSalesmanPerson(G, sommetDepart);
    //GARDER !!!!!!!!!

    //Affichage du chemin opti.
    cout<<endl<<"Le chemin optimisé a 2 approx. :\n\t| ";
    for(int i = 0; i < chemin.size(); i++)
    {
        cout<<chemin[i]<<" | ";
    }
    cout<<endl;

    return 0;
}
