#include<SFML\Graphics.hpp>
#include <string>
using namespace std;
void main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "My window");
	sf::Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			// fermeture de la fenêtre lorsque l'utilisateur le souhaite
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// effacement de la fenêtre en noir
		window.clear(sf::Color::Black);

		// c'est ici qu'on dessine tout
		// window.draw(...);

		// fin de la frame courante, affichage de tout ce qu'on a dessiné
		window.display();
	}

}