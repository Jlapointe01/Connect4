#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>   //Changer pour list.hpp
#include <vector> //changer pour vector.hpp
#include "BaseDonnees.h"

//Pour liste 0 = vide, 1 = rouge, 2 = jaune
//Postion coin en bas a gauche : 67 ,940
//Difference haut et bas : 175 px 
//Difference gauche et droite : 174 px
//Début 1ère colonne : 63 px
//Fin 1ère colonne : 237 px
using namespace sf;
using namespace std;
struct point {
	int x;
	int y;
	point(int x1, int y1) {
		x = x1;
		y = y1;
	}
};
const int nbX = 7;
const int nbY = 6;
void nomJoueurs(string &Rouge, string &Jaune);//Problème pas de console c'est une fenêtre
void left(CircleShape &triangle, point &posTriangle);
void right(CircleShape &triangle, point &posTriangle);
void positionMouse(CircleShape &triangle, int &x, int y, int &colonne);
void changementTour(CircleShape &triangle, int &joueurCourant);
void initialiser(vector<list<int>> &grille);
bool insererJeton(vector<list<int>> &grille, int couleurJeton, int colonne);
bool rechercheGagnant(vector<list<int>> &grille, point jeton, int couleurJeton);
bool rechercheVertical(vector<list<int>> &grille, point jeton, int couleurJeton);
bool rechercheHorizontal(vector<list<int>> &grille, point jeton, int couleurJeton);
bool rechercheDiagonal_NE_SW(vector<list<int>> &grille, point jeton, int couleurJeton);
bool rechercheDiagonal_NW_SE(vector<list<int>> &grille, point jeton, int couleurJeton);
void dessiner(vector<list<int>> &grille, RenderWindow &window, Sprite map, CircleShape triangle);

int main()
{
	point posTriangle(150, 60);
	point posShape1();
	point posShape2();
	const int rouge = 1,
		jaune = 2;
	int joueurCourant = rouge;
	int colonne = 1;
	int tour = 1;
	bool gagne = true;
	
	RenderWindow window(sf::VideoMode(1278, 1106), "Connect 4");
	CircleShape shape1(50.f);
	CircleShape shape2(50.f);
	CircleShape triangle(35, 3);
	Texture map;
	bool terminer = false;
	int col = 0;

	vector<list<int>> grille(7);

	initialiser(grille);

	if (!map.loadFromFile("Connect4_map.png"))
		cout << "Erreur";

	Sprite _map(map);
	shape1.setFillColor(sf::Color::Red);
	shape2.setFillColor(sf::Color::Yellow);

	shape2.setPosition(Vector2f(1112, 940));
	shape1.setPosition(Vector2f(241, 765));


	triangle.setPosition(Vector2f(posTriangle.x, posTriangle.y));
	triangle.rotate(180);

	while (window.isOpen())
	{
		Event event;
		Event keyPressed;

		while (terminer == false)
		{
			if (joueurCourant == rouge)
				triangle.setFillColor(Color::Red);
			else
				triangle.setFillColor(Color::Yellow);



			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
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
					case Keyboard::Return:
						
						if (joueurCourant == rouge)
							joueurCourant = jaune;
						else
							joueurCourant = rouge;




						break;
					}
				}
				if (event.type == sf::Event::MouseMoved)
				{
					positionMouse(triangle, event.mouseMove.x, posTriangle.y, colonne);//puisqu'on veut que le triangle reste en haut
				}
				if (event.type == sf::Event::MouseButtonReleased)
				{
					gagne = insererJeton(grille, joueurCourant, colonne);
					
					changementTour(triangle, joueurCourant);
				}
			}
			window.clear();
			window.draw(_map);
			window.draw(shape2);
			window.draw(triangle);
			window.display();
		}



	}
	return 0;
}
void nomJoueurs(string &Rouge, string &Jaune)
{
	cout << "Indiquer le nom du joueur Rouge : " << endl;
	cin >> Rouge;
	cout << "Indiquer le nom du joueur Jaune : " << endl;
	cin >> Jaune;
}
void initialiser(vector<list<int>> &grille)
{
	for (int i = 0; i < nbX; i++)
	{
		for (int j = 0; j < nbY; j++)
		{
			grille[i].push_back(0);
		}
	}
}

void left(CircleShape &triangle, point &posTriangle)
{
	if (posTriangle.x == 150)
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
	if (posTriangle.x == 1200)
	{
		posTriangle.x = 150;
		triangle.setPosition(Vector2f(posTriangle.x, posTriangle.y));
	}
	else
	{
		triangle.setPosition(Vector2f(posTriangle.x + 175, posTriangle.y));
	}
}
//Début 1ère colonne : 63 px
//Fin 1ère colonne : 237 px
void positionMouse(CircleShape &triangle, int &x, int y, int &colonne)
{
	if (x >= 0 && x <= 237)
	{
		x = 150;
		colonne = 0;
	}
	else if (x >= 238 && x <= 412)
	{
		x = 325;
		colonne = 1;
	}
	else if (x >= 413 && x <= 587)
	{
		x = 500;
		colonne = 2;
	}
	else if (x >= 588 && x <= 762)
	{
		x = 675;
		colonne = 3;
	}
	else if (x>=763 && x <= 937)
	{
		x = 850;
		colonne = 4;
	}
	else if ( x>=938 && x<=1112)
	{
		x = 1025;
		colonne = 5;
	}
	else if (x >= 1113 && x <= 1278)
	{
		x = 1200;
		colonne = 6;
	}
	triangle.setPosition(Vector2f(x, y));
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
bool siVide(list<int>::iterator& it)
{
	if (*it != 0)
		return false;
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

	while (x2 != 6 && y2 != 6) // regarde NE
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

	while (x2 != 0 && y2 != 6) //regarde NW
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
		for (int k = 0; k < 7; k++)
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