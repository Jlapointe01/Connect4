#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>   //Changer pour list.hpp
#include <vector> //changer pour vector.hpp


//Postion coin en bas a gauche : 67 ,940
//Difference haut et bas : 175 px 
//Difference gauche et droite : 174 px

using namespace sf;
using namespace std;

void left(CircleShape &triangle);
void right(CircleShape &triangle);

int main()
{
	RenderWindow window(sf::VideoMode(1278, 1106), "Connect4");
	CircleShape shape(50.f);
	CircleShape triangle(35,3);
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
			break;
				//triangle.setPosition(Vector2f(300, 60));
		case Keyboard::Left:
			//fonction gauche
			break;
		case Keyboard::Return:
			//fonction placer jeton		
			break;
		}

	}
	return 0;
}

void left(CircleShape &triangle) {


}


void right(CircleShape &triangle) {

}