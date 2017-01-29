#include <iostream>
#include <ctype.h>
#include <vector>
#include <stdlib.h>
#include "ia.cpp"

#include <cstdio>
#include <ctime>

using namespace std;

const int NDIM = 8;
typedef int Grille[NDIM][NDIM];

struct Case
{
  int x;
  int y;
};

/**
	Initialiser le plateau
	@param[in] gr - Une grille
*/
void initialiserPlateau(Grille& gr)
{
	for (int j = 0; j < NDIM; ++j)
	{
		for (int k = 0; k < NDIM; ++k)
		{
			gr[j][k] = 0;
		}
	}
  gr[(NDIM/2)-1][(NDIM/2)-1] = 1;
  gr[NDIM/2][NDIM/2] = 1;
  gr[(NDIM/2)-1][NDIM/2] = 2;
  gr[NDIM/2][(NDIM/2)-1] = 2;
}

/**
	Symbole d'un joueur
	@param[in] n - numero du joueur
	@return le symbole du joueur
*/
char symboleDe(int j)
{
	const char symboles[] = {'.','x','o', '@'};
	return symboles[j];
}

/**
	Affiche un plateau
	@param[in] gr - Une grille
*/
void afficherPlateau(const Grille& gr)
{


  cout << ".|";
  for(int i=1; i<=NDIM; i++)
    cout << i << "|";
  cout << endl;
	for(int j = 0; j < NDIM; ++j)
	{
    cout << j+1 << "|";
		for (int k = 0; k < NDIM; ++k)
		{
			if(gr[j][k]==1)
				cout << "\033[1;32m" << symboleDe(gr[j][k]) << "\033[0m" << "|";
			else if(gr[j][k]==2)
				cout << "\033[1;31m" << symboleDe(gr[j][k]) << "\033[0m" << "|";
			else
				cout << symboleDe(gr[j][k]) << "|";
		}
		cout << endl;
	}
  cout << endl;
}

/**
	Vérifie si les coordonnées jouées sont valides
	@param[in] x - La coordonnée x
	@param[in] y - La coordonnée y
	@return Le coup est / n'est pas dans le plateau
*/
bool coordsValides(const Grille& gr, Case &pion)
{
	if( gr[pion.x][pion.y] != 3 )
	{
		cout << "Coordonées invalides, recommencez : " << endl;
		return false;
	}
	else
  {
		return true;
  }
}

/**
  Détecte les cases jouables par le joueur après l'initialisation du plateau, le traitement d'effectue sur tout le tableau
	@param[in,out] gr - Une grille
*/
void initCasesJouables(Grille &gr)
{
  int i, j, k, l;

  for(i=0; i<NDIM; i++)
    for(j=0; j<NDIM; j++)
      if( gr[i][j]==1 || gr[i][j]==2 )
        for(k=-1; k<2; k++)
          for(l=-1; l<2; l++)
            if(gr[i+k][j+l]==0)
              gr[i+k][j+l] = 3;

}

/**
  Détecte les cases jouables, Le traitement s'effectue uniquement sur le dernier pion placé
	@param[in,out] gr - Une grille
	@param[in] pion - Un pion
*/
void casesJouables(Grille &gr, Case &pion)
{
  int k, l;

  for(k=-1; k<2; k++)
    for(l=-1; l<2; l++)
      if(gr[pion.x+k][pion.y+l]==0)
        if(pion.y+l!=-1 && pion.y+l!=8) //permet de ne pas remplacer la case opposé du tableau dans le cas ou on pose le pion sur une extrémité
          gr[pion.x+k][pion.y+l] = 3;
}

/**
  Appel de la fonction ia qui détemrine le meilleur coup à jouer
	@param[in,out] gr - Une grille
	@param[in] pion - Un pion
	@param[in] quiJoue - Un joueur
*/
void ordinateur(Grille &gr, Case &pion, const int &quiJoue)
{
	int x = 0;
	int y = 0;

	ia(x, y, gr, quiJoue);

	pion.x = x;
	pion.y = y;
	gr[pion.x][pion.y] = quiJoue;
}

/**
  Le joueur choisi les coordonnées à jouer
	@param[in] quiJoue -  Un joueur
	@param[in,out] gr - Une grille
	@param[in] pion - Un pion
*/
void joueur(const int &quiJoue, Grille &gr, Case &pion)
{
	int x, y;

	do
	{
		cout << endl << "Joueur " << quiJoue << " Quelle position ? " << endl << "x : ";
		cin >> x;
		pion.x = x-1;
		cout << "y : ";
		cin >> y;
		pion.y = y-1;
		cout << endl;
	} while(coordsValides(gr, pion) == false);

	gr[pion.x][pion.y] = quiJoue;
}


/**
	Appelle le prochain joueur à jouer (Ordinateur ou joueur réel)
	@param[in] quiJoue - Un joueur
	@param[in] gr - Une grille
	@param[in] pion - Un pion
	@param[in] typePartie - Un type de partie
*/
void jouer(int& quiJoue, Grille& gr, Case &pion, int &typePartie)
{
    int x, y;

    casesJouables(gr, pion);
    afficherPlateau(gr);

    switch(typePartie)
    {
      case 1 :
        ordinateur(gr, pion, quiJoue);
        break;
      case 2 :
        if(quiJoue==2)
        {
          ordinateur(gr, pion, quiJoue);
        }

        if(quiJoue==1)
        {
          joueur(quiJoue, gr, pion);
        }
				break;
			case 3 :
				joueur(quiJoue, gr, pion);
				break;
			default :
				break;
    }


}



/**
		Donne la main au joueur suivant
		@param[in] quiJoue - Un joueur
		@return Numéro du joueur
*/
int joueurSuivant(const int& quiJoue)
{
	switch(quiJoue)
	{
			case 1 :
					return 2;
					break;
			case 2 :
					return 1;
					break;
			default :
					break;
	}
}

/**
  Teste si un carré est présent sur la grille
	@param[in] gr - Une grille
	@param[out] gagnant - Un gagnant
*/
bool carre(const Grille &gr, int &gagnant)
{
  int i, j;

  for(i=0; i<NDIM; i++)
    for(j=0; j<NDIM; j++)
      if(gr[i][j]==1 || gr[i][j]==2)
        if(gr[i][j]==gr[i][j-1] && gr[i][j]==gr[i+1][j] && gr[i][j]==gr[i+1][j-1])
        {
          gagnant = gr[i][j];
          return true;
        }
  return false;
}

/**
  Teste si une ligne entourée est présente
	@param[in] gr - Une grille
	@param[out] gagnant - Un gagnant
*/
bool ligne(const Grille &gr, int &gagnant)
{
  int x, y, aux, naux;

	//test de victoire sur les colones
	  for(x=0; x<4; x++)
	    for(y=0; y<NDIMia; y++)
	    {
	      if(gr[x][y]==1 || gr[x][y]==2)
	      {
	        aux = gr[x][y];

	        switch(aux)
	        {
	          case 1 :
	            naux = 2;
	            break;
	          case 2 :
	            naux = 1;
	            break;
	          default :
	            break;
	        }

	        if(gr[x+1][y]==naux && gr[x+2][y]==naux && gr[x+3][y]==naux && gr[x+4][y]==aux)
					{
					  gagnant = aux;
						return true;
					}
	      }
	    }

	//test de victoire sur les lignes
	    for(x=0; x<NDIMia; x++)
	      for(y=0; y<4; y++)
	      {
	        if(gr[x][y]==1 || gr[x][y]==2)
	        {
	          aux = gr[x][y];

	          switch(aux)
	          {
	            case 1 :
	              naux = 2;
	              break;
	            case 2 :
	              naux = 1;
	              break;
	            default :
	              break;
	          }

	          if(gr[x][y+1]==naux && gr[x][y+2]==naux && gr[x][y+3]==naux && gr[x][y+4]==aux)
						{
						  gagnant = aux;
							return true;
						}
	        }
	      }

	//test victoire diagonale droite
	    for(x=0; x<4; x++)
	      for(y=0; y<4; y++)
	      {
	        if(gr[x][y]==1 || gr[x][y]==2)
	        {
	          aux = gr[x][y];

	          switch(aux)
	          {
	            case 1 :
	              naux = 2;
	              break;
	            case 2 :
	              naux = 1;
	              break;
	            default :
	              break;
	          }

	          if(gr[x+1][y+1]==naux && gr[x+2][y+2]==naux && gr[x+3][y+3]==naux && gr[x+4][y+4]==aux)
						{
							gagnant = aux;
							return true;
						}
	        }
	      }

	//test victoire diagonale gauche
	    for(x=0; x<4; x++)
	      for(y=4; y<NDIMia; y++)
	      {
	        if(gr[x][y]==1 || gr[x][y]==2)
	        {
	          aux = gr[x][y];

	          switch(aux)
	          {
	            case 1 :
	              naux = 2;
	              break;
	            case 2 :
	              naux = 1;
	              break;
	            default :
	              break;
	          }

	          if(gr[x+1][y-1]==naux && gr[x+2][y-2]==naux && gr[x+3][y-3]==naux && gr[x+4][y-4]==aux)
						{
							gagnant = aux;
							return true;
						}
	        }
	      }


    return false;
}

/**
	Vérifie si un joueur a gagné
	@param[in] gr - Une grille
	@return Indique si il y a victoire ou non
*/
bool victoireDe(const Grille& gr, int &gagnant)
{
  if(carre(gr, gagnant) || ligne(gr, gagnant))
	{
		//afficherPlateau(gr);
    return true;
	}
  return false;
}


/**
	Vérifie si le plateau est bloqué
	@param[in] gr - Une grille
	@return Indique si le plateau est rempli
*/
bool plateauBloque(const Grille& gr)
{

	int casesLibres = 0;

	for(int x = 0; x < NDIM; x++)
	{
		for(int y = 0; y < NDIM; y++)
		{
			if( (gr[x][y] == 0) || (gr[x][y] == 3) )
				casesLibres++;
		}
	}

	if(casesLibres == 0)
		return true;
	else
		return false;

}

/**
	Le jeu
*/
void neomix()
{
	Grille gr;
  int quiJoue = 1;
  int gagnant = 0;
  bool finJeu = false;
	int typePartie;

  Case pion;
  pion.x = -2;
  pion.y = -2;

  initialiserPlateau(gr);
  afficherPlateau(gr);
  initCasesJouables(gr);

	cout << "Choisissez le type de partie :" << endl;
	cout << "		1) Ordinateur contre Ordinateur" << endl;
	cout << "		2) Ordianteur contre joueur" << endl;
	cout << "		3) Joueur contre joueur" << endl;
	cin >> typePartie;

  while(finJeu == false)
  {

    jouer(quiJoue, gr, pion, typePartie);
    if(victoireDe(gr, gagnant))
    {
			afficherPlateau(gr);
      cout << gagnant << " à gagné !" << endl;
      finJeu = true;
    }
    else if(plateauBloque(gr))
    {
      cout << "Jeu bloqué" << endl;
      finJeu = true;
    }
    else
    {
      quiJoue = joueurSuivant(quiJoue);
    }

  }
}

/**
	Lance le jeu
*/
int main(int argc, char** argv)
{
	neomix();
	return 0;
}
