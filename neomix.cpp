#include <iostream>
#include <ctype.h>

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
  cout << ".";
  for(int i=1; i<=NDIM; i++)
    cout << i;
  cout << endl;
	for(int j = 0; j < NDIM; ++j)
	{
    cout << j+1;
		for (int k = 0; k < NDIM; ++k)
		{
			cout << symboleDe(gr[j][k]);
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
  Détecte les cases jouables par le joueur après l'initialisation du plateau
  Le traitement d'effectue sur tout le tableau
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
  Détecte les cases jouables lorsque la partie à commencé
  Le traitement s'effectue uniquement sur le dernier pion placé
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
	Résoud un tour
	@param[in] quiJoue - Un joueur
	@param[in] gr - Une grille
*/
void jouer(int& quiJoue, Grille& gr, Case &pion)
{
    int x, y;

    casesJouables(gr, pion);
    afficherPlateau(gr);

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
*/
bool ligne(const Grille &gr, int &gagnant)
{
  int x, y, aux, naux;

  for(x=0; x<NDIM; x++)
    for(y=0; y<NDIM; y++)
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

        if(gr[x][y+1]==aux && gr[x][y+2]==aux)
          if(gr[x][y-1]==naux && gr[x][y+3]==naux)
          {
            gagnant = gr[x][y-1];
            return true;
          }

        if(gr[x-1][y]==aux && gr[x-2][y]==aux)
          if(gr[x+1][y]==naux && gr[x-3][y]==naux)
          {
            gagnant = gr[x+1][y];
            return true;
          }

        if(gr[x-1][y+1]==aux && gr[x-2][y+2]==aux)
          if(gr[x+1][y-1]==naux && gr[x-3][y+3]==naux)
          {
            gagnant = gr[x+1][y-1];
            return true;
          }

        if(gr[x-1][y-1]==aux && gr[x-2][y-2]==aux)
          if(gr[x+1][y+1]==naux && gr[x-3][y-3]==naux)
          {
            gagnant = gr[x+1][y+1];
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
    return true;
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
	Lance le jeu
*/
int main(int argc, char** argv)
{
  Grille gr;
  int quiJoue = 1;
  int gagnant = 0;
  bool finJeu = false;

  Case pion;
  pion.x = -2;
  pion.y = -2;

  initialiserPlateau(gr);
  afficherPlateau(gr);
  initCasesJouables(gr);

  while(finJeu == false)
  {

    jouer(quiJoue, gr, pion);
    if(victoireDe(gr, gagnant))
    {
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
	return 0;
}
