#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>   //Changer pour list.hpp
#include <vector> //changer pour vector.hpp

// grosseur map : 1278 pixels x 1106 pixels
//Postion coin en bas a gauche : 67 , 940
//Difference haut et bas : 175 px 
//Difference gauche et droite : 175 px

//position triangle 1 : 150 325 500 675 850 1025 1200

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

void initialiser(vector<list<int>> &grille);
bool siVide(list<int>::iterator& it);

void left(CircleShape &triangle, point &posTriangle);
void right(CircleShape &triangle, point &posTriangle);
void positionMouse(CircleShape &triangle, int &x, int y);
void insererJeton(list<int> &grille, int jeton);
bool verifierSiGagner(list<int> &grille, int jeton);


int main()
{
	point posTriangle(150, 60);
	point posShape1();
	point posShape2();
	const int rouge = 1,
			  jaune = 2;

	RenderWindow window(sf::VideoMode(1278, 1106), "Connect 4");
	CircleShape shape1(50.f);
	CircleShape shape2(50.f);
	CircleShape triangle(35, 3);
	Texture map;

	int tour = 0;

	vector<list<int>> grille(7);

	initialiser(grille);

	if (!map.loadFromFile("Connect4_map.png"))
		cout << "Erreur";

	Sprite _map(map);
	shape1.setFillColor(sf::Color::Red);
	shape2.setFillColor(sf::Color::Yellow);

	shape2.setPosition(Vector2f(1112, 940));
	//shape1.setPosition(Vector2f(241, 765));

	triangle.setFillColor(Color::Red);
	triangle.setPosition(Vector2f(posTriangle.x, posTriangle.y));
	triangle.rotate(180);



	while (window.isOpen()) {
		Event event;
		Event keyPressed;

		while (window.pollEvent(event)) {
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
					}
					else
					{
						right(triangle, posTriangle);
						posTriangle.x += 175;
					}
					break;
				case Keyboard::Left:
					if (posTriangle.x == 150)
					{
						left(triangle, posTriangle);
						posTriangle.x = 1200;
					}
					else
					{
						left(triangle, posTriangle);
						posTriangle.x -= 175;
					}
					break;
				case Keyboard::Return:				
					insererJeton(grille[0], jaune);
					break;
				}
			}
			//if (event.type == sf::Event::MouseMoved)
			//{
			//	positionMouse(triangle, event.mouseMove.x, posTriangleY);//puisqu'on veut que le triangle reste en haut
			//}
		}
		window.clear();
		window.draw(_map);
		window.draw(shape2);
		window.draw(triangle);
		window.display();
	}
	return 0;
}

void initialiser(vector<list<int>> &grille) {
	//Initialise la grille
	for (int i = 0; i < 6; i++) {
		for (int k = 0; k < 7; k++) {
			grille[k].push_back(0);
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

void positionMouse(CircleShape &triangle, int &x, int y)
{
	triangle.setPosition(Vector2f(x, y));
}

bool siVide(list<int>::iterator& it) 
{

	if (*it != 0)
		return false;
	return true;
}
void insererJeton(list<int> &grille, int jeton)
{
	list<int>::iterator it = grille.begin(); ;
	do
	{

		if (*it == 0)
			*it = jeton;
		else
			it++;
	} while (*it != 0);
}