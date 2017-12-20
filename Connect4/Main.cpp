//Auteur : Jean-Christophe Boisvert et Joel Lapointe
//But : Cr�er un jeu de Connect4
#include <SFML/Graphics.hpp>
#include <iostream>
#include "liste.hpp"
#include "vecteur.hpp"
#include "BaseDonnees.h"

struct point {
	int x;
	int y;
	point(int x1, int y1) {
		x = x1;
		y = y1;
	}
};


using namespace sf;
using namespace std;

void initialiser(vector<list<int>> &grille);//initialise la grille 
bool siVide(list<int>::iterator& it);	//v�rifie si la grille est vide

void left(CircleShape &triangle, point &posTriangle); //se d�place a gauche
void right(CircleShape &triangle, point &posTriangle);	//se d�place a droite
void positionMouse(CircleShape &triangle, int &x, int y, int &colonne); //se d�place vis a vis la colonne ou la souris est plac�
bool insererJeton(vector<list<int>> &grille, int couleurJeton, int colonne);	
bool rechercheGagnant(vector<list<int>> &grille, point jeton, int couleurJeton);
bool rechercheVertical(vector<list<int>> &grille, point jeton, int couleurJeton);
bool rechercheHorizontal(vector<list<int>> &grille, point jeton, int couleurJeton);
bool rechercheDiagonal_NE_SW(vector<list<int>> &grille, point jeton, int couleurJeton);
bool rechercheDiagonal_NW_SE(vector<list<int>> &grille, point jeton, int couleurJeton);
void dessiner(vector<list<int>> &grille, RenderWindow &window, Sprite map, CircleShape triangle);
void changementTour(CircleShape &triangle, int &joueurCourant);
bool verifieSiNul(vector<list<int>> &grille);

int main()
{
	BaseDonnees bd;
	string joueur1;
	string joueur2;
	point posTriangle(150, 60);
	const int rouge = 1,
		jaune = 2;
	int joueurCourant = rouge;
	int nbwinJoueur1 = 0;
	int nbloseJoueur1 = 0;
	int nbwinJoueur2 = 0;
	int nbloseJoueur2 = 0;
	int nbdrawJoueur1 = 0;
	int nbdrawJoueur2 = 0;

	RenderWindow window(sf::VideoMode(1278, 1106), "Connect 4");
	CircleShape triangle(35, 3);
	Texture map;
	bool terminer = false;
	int col = 0;
	int tour = 0;
	bool inscription = true;		//attente du jeu pour entrer nom des joueurs
	bool gagner = false;
	vector<list<int>> grille(7);

	initialiser(grille);

	if (!map.loadFromFile("Connect4_map.png"))
		cout << "Erreur";

	Sprite _map(map);
	triangle.setPosition(Vector2f(posTriangle.x, posTriangle.y));
	triangle.rotate(180);

	while (window.isOpen())
	{
		Event event;
		Event keyPressed;
		sf::Font font;
		sf::Text label;
		sf::Text text;
		font.loadFromFile("arial.ttf");
		text.setFont(font);
		label.setFont(font);
		text.setString("");
		label.setString("Joueur 1 : ");
		text.setFillColor(sf::Color::Black);
		label.setFillColor(sf::Color::Black);
		text.setPosition(150, 0);
		while (inscription == true)
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
					terminer = true;
					inscription = false;
				}
				if (event.type == sf::Event::TextEntered)		//va faire entrer les nom des 2 joueurs pour les mettres dans les variables joueurs 1 et joueurs 2
																//pour notre cas les deux seuls joueurs qu'on peut faire marcher sont jacklar et bobbis puisque 
																//la fonction cr�er utilisateur n'a pu �tre cr�er � temps
				{
					if (event.text.unicode == 13)//touche enter confirme le nom
					{
						if (label.getString() == "Joueur 1 : ") // apr�s joueur 1 
						{
							joueur1 = text.getString();
							label.setString("Joueur 2 : ");
							text.setString("");
						}
						else if (label.getString() == "Joueur 2 : ")//apr�s joueur 2 ferme et commence le jeu
						{
							joueur2 = text.getString();
							label.setString("");
							text.setString("");
							inscription = false;
							break;
						}
					}
					else if(event.text.unicode == 8)//touche backspace efface la derniere touche appuyer
					{
						string temp = text.getString();
						temp = temp.substr(0, temp.length() - 1);
						text.setString(temp);
					}
					else
					{
						text.setString(text.getString() + static_cast<char>(event.text.unicode));
					}
				}
				window.clear(sf::Color::White);
				window.draw(label);
				window.draw(text);
				window.display();
			}
		}
		while (terminer == false)	//tant que la partie n'est pas termin�
		{
			if (joueurCourant == rouge)
				triangle.setFillColor(Color::Red);
			else
				triangle.setFillColor(Color::Yellow);

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (gagner == true) {	//si on a gagn�
					sf::Text textWinner;
					textWinner.setFont(font);
					textWinner.setFillColor(sf::Color::Black);
					textWinner.setString("");
					bd.obtenirPointage(joueur1, nbwinJoueur1, nbloseJoueur1, nbdrawJoueur1);
					bd.obtenirPointage(joueur2, nbwinJoueur2, nbloseJoueur2, nbdrawJoueur2);
					if (tour == 1)	//si le joueur 2 a gagn�
					{
						textWinner.setString(joueur2 + " gagne" " Peser sur le X pour fermer");
						nbwinJoueur2++;
						nbloseJoueur1++;
					}
					else //si le joueur 1 a gagn�
					{
						textWinner.setString(joueur1 + " gagne" " Peser sur le X pour fermer");
						nbwinJoueur1++;
						nbloseJoueur2++;

					}
					bd.modifierPointage(joueur1, nbwinJoueur1, nbloseJoueur1, nbdrawJoueur1);
					bd.modifierPointage(joueur2, nbwinJoueur2, nbloseJoueur2, nbdrawJoueur2);
					window.clear(sf::Color::White);
					window.draw(textWinner);
					window.display();
				}
				else 
					if (verifieSiNul(grille))	//si la partie est nulle
				{
					sf::Text textDraw;
					textDraw.setFont(font);
					textDraw.setFillColor(sf::Color::Black);

					textDraw.setString("La partie est nulle" " Appuyez sur le X pour fermer");
					nbdrawJoueur2++;
					nbdrawJoueur1++;

					window.clear(sf::Color::White);
					window.draw(textDraw);
					window.display();
				}
					else 
					{
						if (event.type == sf::Event::KeyPressed)
						{
							switch (event.key.code)
							{
							case Keyboard::Right:
								if (posTriangle.x == 1200)
								{
									right(triangle, posTriangle);
									posTriangle.x = 150;
									col = 0;
								}
								else
								{
									right(triangle, posTriangle);
									posTriangle.x += 175;
									col++;
								}
							break;
							case Keyboard::Left:
								if (posTriangle.x == 150)
								{
									left(triangle, posTriangle);
									posTriangle.x = 1200;
									col = 6;
								}
								else
								{
									left(triangle, posTriangle);
									posTriangle.x -= 175;
									col--;
								}
								break;
						}
					}
					if (event.type == sf::Event::MouseMoved)
					{
						positionMouse(triangle, event.mouseMove.x, posTriangle.y, col);//puisqu'on veut que le triangle reste en haut
					}
					if (event.type == sf::Event::MouseButtonReleased)
					{
						gagner = insererJeton(grille, joueurCourant, col);

						changementTour(triangle, joueurCourant);
					}
				}
			}
			window.clear();
			if (gagner == false)
				if(!verifieSiNul(grille))
				dessiner(grille, window, _map, triangle);
		}

	

	}
	return 0;
}
bool siVide(list<int>::iterator& it)
{
	if (*it != 0)
		return false;
	return true;
}

void initialiser(vector<list<int>> &grille) {
	//Initialise la grille
	list<int>::iterator it;
	for (int i = 0; i < 7; i++)
		grille[i].clear();
	
	for (int i = 0; i < 7; i++) {
		it = grille[i].begin();
		for (int k = 0; k < 7; k++) {
			it = grille[i].insert(it,0);
			it++;
		}
	}
}

void left(CircleShape &triangle, point &posTriangle)
{
	if (posTriangle.x == 150)//si il est a la premi�re colonne met le a la fin
	{
		posTriangle.x = 1200;
		triangle.setPosition(Vector2f(posTriangle.x, posTriangle.y));
	}
	else
	{
		triangle.setPosition(Vector2f(posTriangle.x - 175, posTriangle.y));
	}
}


void right(CircleShape &triangle, point &posTriangle) {
	if (posTriangle.x == 1200)//si il est a la derni�re colonne met le au d�but
	{
		posTriangle.x = 150;
		triangle.setPosition(Vector2f(posTriangle.x, posTriangle.y));
	}
	else
	{
		triangle.setPosition(Vector2f(posTriangle.x + 175, posTriangle.y));
	}
}

void positionMouse(CircleShape &triangle, int &x, int y, int &colonne)
{
	if (x >= 0 && x <= 237)	//si il est vers la premi�re colonne
	{
		x = 150;
		colonne = 0;
	}
	else if (x >= 238 && x <= 412)//si il est vers la deuxi�me colonne
	{
		x = 325;
		colonne = 1;
	}
	else if (x >= 413 && x <= 587)//si il est vers la troisi�me colonne
	{
		x = 500;
		colonne = 2;
	}
	else if (x >= 588 && x <= 762)//si il est vers la quatri�me colonne
	{
		x = 675;
		colonne = 3;
	}
	else if (x >= 763 && x <= 937)//si il est vers la cinqui�me colonne
	{
		x = 850;
		colonne = 4;
	}
	else if (x >= 938 && x <= 1112)//si il est vers la sixi�me colonne
	{
		x = 1025;
		colonne = 5;
	}
	else if (x >= 1113 && x <= 1278)//si il est vers la septi�me colonne
	{
		x = 1200;
		colonne = 6;
	}
	triangle.setPosition(Vector2f(x, y));
}
bool verifieSiNul(vector<list<int>> &grille)
{
	for (int i = 0; i < 7; i++)
	{
		list<int>::iterator it = grille[i].begin();
		for (int k = 0; k < 6; k++)
		{
			if (*it == 0)
				return false;
			it++;
		}
	}
	return true;
}
bool insererJeton(vector<list<int>> &grille, int couleurJeton, int colonne)
{
	list<int>::iterator it = grille[colonne].begin();
	bool jetonPlacer = false;
	bool gagner = false;
	point posJeton(colonne, 0);


	do
	{
		if (*it == 0) {
			*it = couleurJeton;
			jetonPlacer = true;
			gagner = rechercheGagnant(grille, posJeton, couleurJeton);
		}
		else {
			it++;
			posJeton.y += 1;
			if (it == grille[colonne].end())
				return false;
		}
	} while (jetonPlacer == false);

	return gagner;
}

bool rechercheGagnant(vector<list<int>> &grille, point jeton, int couleurJeton) {
	if (rechercheHorizontal(grille, jeton, couleurJeton) == true || rechercheVertical(grille, jeton, couleurJeton) == true
		|| rechercheDiagonal_NE_SW(grille, jeton, couleurJeton) == true || rechercheDiagonal_NW_SE(grille, jeton, couleurJeton) == true)
		return true;
	return false;
}

bool rechercheVertical(vector<list<int>> &grille, point jeton, int couleurJeton) {

	list<int>::iterator it = grille[jeton.x].begin();

	for (int i = 0; i < jeton.y; i++)
		it++;

	if (jeton.y >= 3) {
		for (int i = 0; i < 3; i++) {
			it--;
			if (*it != couleurJeton)
				return false;
		}
		return true;
	}
	return false;
}

bool rechercheHorizontal(vector<list<int>> &grille, point jeton, int couleurJeton) {

	int x1 = jeton.x;
	int x2 = jeton.x;
	int nb = 1; //condition pour gagner

	while (x1 != 0) //regarde a gauche
	{
		x1--;

		//positionne l'iterateur
		list<int>::iterator it = grille[x1].begin();
		for (int i = 0; i < jeton.y; i++)
			it++;

		if (*it != couleurJeton)
			break;
		nb++;
		if (nb == 4)
			return true;
	}

	while (x2 != 6)//regarde a droite
	{
		x2++;

		//positionne l'iterateur
		list<int>::iterator it = grille[x2].begin();
		for (int i = 0; i < jeton.y; i++)
			it++;

		//verifie s'il a la bonne couleur
		if (*it != couleurJeton)
			break;
		nb++;

		if (nb == 4)
			return true;
	}

	return false;
}

bool rechercheDiagonal_NE_SW(vector<list<int>> &grille, point jeton, int couleurJeton) {

	int x1 = jeton.x;
	int y1 = jeton.y;
	int x2 = jeton.x;

	int y2 = jeton.y;
	int nb = 1; //condition pour gagner

	while (x1 != 0 && y1 != 0) //regarde SW
	{
		x1--;
		y1--;

		list<int>::iterator it = grille[x1].begin();
		for (int i = 0; i < y1; i++)
			it++;

		if (*it != couleurJeton)
			break;

		nb++;

		if (nb == 4)
			return true;
	}

	while (x2 != 6 && y2 != 5) // regarde NE
	{
		x2++;
		y2++;

		list<int>::iterator it = grille[x2].begin();
		for (int i = 0; i < y2; i++)
			it++;

		if (*it != couleurJeton)
			break;

		nb++;

		if (nb == 4)
			return true;
	}
	return false;

}

bool rechercheDiagonal_NW_SE(vector<list<int>> &grille, point jeton, int couleurJeton) {

	int x1 = jeton.x;
	int y1 = jeton.y;
	int x2 = jeton.x;
	int y2 = jeton.y;
	int nb = 1; //condition pour gagner


	while (x1 != 6 && y1 != 0) //regarde SE
	{
		x1++;
		y1--;

		list<int>::iterator it = grille[x1].begin();
		for (int i = 0; i < y1; i++)
			it++;

		if (*it != couleurJeton)
			break;

		nb++;

		if (nb == 4)
			return true;
	}

	while (x2 != 0 && y2 != 5) //regarde NW
	{
		x2--;
		y2++;

		list<int>::iterator it = grille[x2].begin();
		for (int i = 0; i < y2; i++)
			it++;

		if (*it != couleurJeton)
			break;

		nb++;

		if (nb == 4)
			return true;
	}
	return false;
}

void dessiner(vector<list<int>> &grille, RenderWindow &window, Sprite map, CircleShape triangle) {


	CircleShape jetons[7][7];
	int posX = 67;
	int posY = 940;

	window.draw(map);
	window.draw(triangle);

	for (int i = 0; i < 7; i++) {
		sf::Color couleur;
		list<int>::iterator it = grille[i].begin();
		for (int k = 0; k < 6; k++)
		{
			if (*it == 0)
				couleur = Color::Transparent;
			else if (*it == 1)
				couleur = Color::Red;
			else
				couleur = Color::Yellow;
			it++;

			jetons[i][k].setRadius(50.f);
			jetons[i][k].setFillColor(couleur);
			jetons[i][k].setPosition(Vector2f(posX, posY));

			window.draw(jetons[i][k]);
			posY -= 175;

		}
		posY = 940;
		posX += 175;
	}
	window.display();
}

void changementTour(CircleShape &triangle, int &joueurCourant)
{
	if (joueurCourant == 1)
	{
		triangle.setFillColor(sf::Color::Yellow);
		joueurCourant = 2;
	}
	else
	{
		triangle.setFillColor(sf::Color::Red);
		joueurCourant = 1;
	}
}
