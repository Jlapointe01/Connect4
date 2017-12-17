#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>   //Changer pour list.hpp
#include <vector> //changer pour vector.hpp

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
void left(CircleShape &triangle, int &x, int y);
void right(CircleShape &triangle, int &x, int y);
void positionMouse(CircleShape &triangle, int &x, int y, int &colonne);
void changementTour(CircleShape &triangle, int &tour);
void initialiserGrille(vector<list<int>> &grille);
bool insererJeton(vector<list<int>> &grille, int couleurJeton, int colonne);
bool rechercheGagnant(vector<list<int>> grille, point jeton, int couleurJeton);
bool rechercheVertical(vector<list<int>> grille, point jeton, int couleurJeton);
bool rechercheHorizontal(vector<list<int>> grille, point jeton, int couleurJeton);
bool rechercheDiagonal_NE_SW(vector<list<int>> grille, point jeton, int couleurJeton);
bool rechercheDiagonal_NW_SE(vector<list<int>> grille, point jeton, int couleurJeton);

int main()
{
	
	int posTriangleX = 150;
	int posTriangleY = 60;
	int tour = 1;
	int colonne = 0;
	RenderWindow window(sf::VideoMode(1278, 1106), "Connect 4");
	CircleShape shape(50.f);
	CircleShape triangle(35, 3);
	Texture map;

	list<int>::iterator it;
	vector<list<int>> _grille(nbX, list<int>());
	initialiserGrille(_grille);
	it = _grille[0].begin();


	if (!map.loadFromFile("Connect4_map.png"))
		cout << "Erreur";

	Sprite _map(map);
	shape.setFillColor(sf::Color::Red);

	shape.setPosition(Vector2f(241, 765));

	triangle.setFillColor(Color::Red);
	triangle.setPosition(Vector2f(posTriangleX, posTriangleY));
	triangle.rotate(180);
	
	// 1278 pixels x 1106 pixels
	while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case Keyboard::Right:
					if (posTriangleX == 1200)
					{
						right(triangle, posTriangleX, posTriangleY);
						posTriangleX = 150;
					}
					else
					{
						right(triangle, posTriangleX, posTriangleY);
						posTriangleX += 175;
					}
					break;
				case Keyboard::Left:
					if (posTriangleX == 150)
					{
						left(triangle, posTriangleX, posTriangleY);
						posTriangleX = 1200;
					}
					else
					{
						left(triangle, posTriangleX, posTriangleY);
						posTriangleX -= 175;
					}
					break;
				case Keyboard::Return:
					//fonction placer jeton		
					break;
				}
			}
			if (event.type == sf::Event::MouseMoved)
			{
				positionMouse(triangle, event.mouseMove.x, posTriangleY, colonne);//puisqu'on veut que le triangle reste en haut
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				//mettre jeton
				changementTour(triangle, tour);
			}
		}
		window.clear();
		window.draw(_map);
		window.draw(shape);
		window.draw(triangle);
		window.display();

	

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
void initialiserGrille(vector<list<int>> &grille)
{
	for (int i = 0; i < nbX; i++)
	{
		for (int j = 0; j < nbY; j++)
		{
			grille[i].push_back(0);
		}
	}
}

void left(CircleShape &triangle, int &x, int y)
{
	if (x == 150)
	{
		x = 1200;
		triangle.setPosition(Vector2f(x, y));
	}
	else
	{
		triangle.setPosition(Vector2f(x - 175, y));
	}
}


void right(CircleShape &triangle, int &x, int y) {
	if (x == 1200)
	{
		x = 150;
		triangle.setPosition(Vector2f(x, y));
	}
	else
	{
		triangle.setPosition(Vector2f(x + 175, y));
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
void changementTour(CircleShape &triangle, int &tour)
{
	if (triangle.getFillColor() == sf::Color::Red)
	{
		triangle.setFillColor(sf::Color::Yellow);
		tour = 2;
	}
	else
	{
		triangle.setFillColor(sf::Color::Red);
		tour = 1;
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

	point posJeton(colonne, 0);


	do
	{
		if (*it == 0) {
			*it = couleurJeton;
			rechercheGagnant(grille, posJeton, couleurJeton);
		}
		else {
			it++;
			posJeton.y += 1;
			if (it == grille[colonne].end())
				return false;
		}
	} while (*it != 0);
	return true;
}

bool rechercheGagnant(vector<list<int>> grille, point jeton, int couleurJeton) {
	if (rechercheHorizontal(grille, jeton, couleurJeton) == true || rechercheVertical(grille, jeton, couleurJeton) == true
		|| rechercheDiagonal_NE_SW(grille, jeton, couleurJeton) == true || rechercheDiagonal_NW_SE(grille, jeton, couleurJeton) == true)
		return true;
	return false;
}

bool rechercheVertical(vector<list<int>> grille, point jeton, int couleurJeton) {

	list<int>::iterator it = grille[jeton.x].begin();

	for (int i = 0; i < jeton.y; i++)
		it++;

	for (int i = 0; i < 3; i++) {
		it--;
		if (*it != couleurJeton)
			return false;
	}
	return true;
}

bool rechercheHorizontal(vector<list<int>> grille, point jeton, int couleurJeton) {

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

bool rechercheDiagonal_NE_SW(vector<list<int>> grille, point jeton, int couleurJeton) {

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

	while (x1 != 6 && y1 != 6) // regarde NE
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

bool rechercheDiagonal_NW_SE(vector<list<int>> grille, point jeton, int couleurJeton) {

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

	while (x1 != 0 && y1 != 6) //regarde NW
	{
		x1--;
		y1++;

		list<int>::iterator it = grille[x1].begin();
		for (int i = 0; i < y1; i++)
			it++;

		if (*it != couleurJeton)
			break;

		nb++;

		if (nb == 4)
			return true;
	}
	return false;
}