#include <SFML/Graphics.hpp>
#include <iostream>


//Postion coin en bas a gauche : 67 ,940
//Difference haut et bas : 175 px 
//Difference gauche et droite : 174 px

using namespace sf;
using namespace std;
int main()
{
	RenderWindow window(sf::VideoMode(1278, 1106), "Connect4");
	CircleShape shape(50.f);
	CircleShape triangle(35,3);

	Texture map;
	

	if (!map.loadFromFile("Connect4_map.png"))
		cout << "Erreur";

	Sprite _map(map);
	shape.setFillColor(sf::Color::Red);

	shape.setPosition(Vector2f(241, 765));

	triangle.setFillColor(Color::Red);
	triangle.setPosition(Vector2f(150, 60));
	triangle.rotate(180);
	// 1278 pixels x 1106 pixels

	while (window.isOpen()){
		Event event;
		Event keyPressed;

		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(_map);
		window.draw(shape);
		window.draw(triangle);
		window.display();

		switch (event.key.code){
		case Keyboard::Right:
			//fonction droite
				//triangle.setPosition(Vector2f(300, 60));
		case Keyboard::Left:
			//fonction gauche

		}

	}
	return 0;
}