#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>   //Changer pour list.hpp
#include <vector> //changer pour vector.hpp


//Postion coin en bas a gauche : 67 ,940
//Difference haut et bas : 175 px 
//Difference gauche et droite : 174 px
//Début 1ère colonne : 63 px
//Fin 1ère colonne : 237 px
using namespace sf;
using namespace std;

void left(CircleShape &triangle, int &x, int y);
void right(CircleShape &triangle, int &x, int y);
void positionMouse(CircleShape &triangle, int &x, int y);
int main()
{
	int posTriangleX = 150;
	int posTriangleY = 60;

	RenderWindow window(sf::VideoMode(1278, 1106), "Connect 4");
	CircleShape shape(50.f);
	CircleShape triangle(35, 3);
	Texture map;

	list<int>::iterator it;
	vector<list<int>> _grille(7, list<int>());

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
		Event keyPressed;

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
				positionMouse(triangle, event.mouseMove.x, posTriangleY);//puisqu'on veut que le triangle reste en haut
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
void positionMouse(CircleShape &triangle, int &x, int y)
{
	if (x >= 0 && x <= 237)
	{
		x = 150;
	}
	else if (x >= 238 && x <= 412)
	{
		x = 325;
	}
	else if (x >= 413 && x <= 587)
	{
		x = 500;
	}
	else if (x >= 588 && x <= 762)
	{
		x = 675;
	}
	else if (x>=763 && x <= 937)
	{
		x = 850;
	}
	else if ( x>=938 && x<=1112)
	{
		x = 1025;
	}
	else if (x >= 1113 && x <= 1278)
	{
		x = 1200;
	}
	triangle.setPosition(Vector2f(x, y));
}