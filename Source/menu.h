#include <SFML/Graphics.hpp>

using namespace sf;
void menu(RenderWindow& menuwindow) {
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("../Assets/images/Play.png");
	menuTexture2.loadFromFile("../Assets/images/About.png");
	menuTexture3.loadFromFile("../Assets/images/Exit1.png");
	aboutTexture.loadFromFile("../Assets/images/Aboutgame.jpg");
	menuBackground.loadFromFile("../Assets/images/kartyfon.jpg");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(932, 750);
	menu2.setPosition(908, 825);
	menu3.setPosition(932, 900);
	menuBg.setPosition(0, 0);
	while (isMenu)
	{
		Event event;
		while (menuwindow.pollEvent(event))
		{
			if (event.type == Event::Closed) menuwindow.close();
			menu1.setColor(Color::White);
			menu2.setColor(Color::White);
			menu3.setColor(Color::White);
			menuNum = 0;
			menuwindow.clear(Color(129, 181, 221));

			if (IntRect(932, 750, 300, 50).contains(Mouse::getPosition(menuwindow))) { menu1.setColor(Color::Black); menuNum = 1; }
			if (IntRect(908, 825, 300, 50).contains(Mouse::getPosition(menuwindow))) { menu2.setColor(Color::Black); menuNum = 2; }
			if (IntRect(932, 900, 300, 50).contains(Mouse::getPosition(menuwindow))) { menu3.setColor(Color::Black); menuNum = 3; }

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (menuNum == 1) isMenu = false, menuwindow.close();
				if (menuNum == 2) { menuwindow.draw(about); menuwindow.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
				if (menuNum == 3) { exit(0); }

			}

			menuwindow.draw(menuBg);
			menuwindow.draw(menu1);
			menuwindow.draw(menu2);
			menuwindow.draw(menu3);

			menuwindow.display();
		}
	}
}