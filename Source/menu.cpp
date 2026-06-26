#include "menu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

void menu(sf::RenderWindow& menuwindow)
{
	// ========================================================================
	// Resource loading
	// ========================================================================
	sf::Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("../Assets/images/Play.png");
	menuTexture2.loadFromFile("../Assets/images/About.png");
	menuTexture3.loadFromFile("../Assets/images/Exit1.png");
	aboutTexture.loadFromFile("../Assets/images/Aboutgame.jpg");
	menuBackground.loadFromFile("../Assets/images/kartyfon.jpg");

	// ========================================================================
	// Audio setup
	// ========================================================================
	sf::SoundBuffer hoverBuffer, clickBuffer;
	sf::Sound hoverSound, clickSound;

	if (!hoverBuffer.loadFromFile("../Assets/audio/button-hover.wav")) {
		std::cout << "Warning: button-hover.wav not found" << std::endl;
	}
	if (!clickBuffer.loadFromFile("../Assets/audio/button-click.wav")) {
		std::cout << "Warning: button-click.wav not found" << std::endl;
	}
	hoverSound.setBuffer(hoverBuffer);
	clickSound.setBuffer(clickBuffer);

	sf::Music menuMusic;
	if (!menuMusic.openFromFile("../Assets/audio/menu-ambient.ogg")) {
		std::cout << "Warning: menu-ambient.ogg not found" << std::endl;
	}
	menuMusic.setLoop(true);
	menuMusic.setVolume(20.f);
	menuMusic.play();

	// ========================================================================
	// Sprite creation
	// ========================================================================
	sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);

	// ========================================================================
	// Menu state
	// ========================================================================
	bool isMenu = true;
	int menuNum = 0;
	int prevMenuNum = 0;

	menu1.setPosition(932, 750);
	menu2.setPosition(908, 825);
	menu3.setPosition(932, 900);
	menuBg.setPosition(0, 0);

	// ========================================================================
	// Main menu loop
	// ========================================================================
	while (isMenu)
	{
		sf::Event event;
		while (menuwindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) menuwindow.close();

			menu1.setColor(sf::Color::White);
			menu2.setColor(sf::Color::White);
			menu3.setColor(sf::Color::White);
			menuNum = 0;

			menuwindow.clear(sf::Color(129, 181, 221));

			if (sf::IntRect(932, 750, 300, 50).contains(sf::Mouse::getPosition(menuwindow))) { menu1.setColor(sf::Color::Black); menuNum = 1; }
			if (sf::IntRect(908, 825, 300, 50).contains(sf::Mouse::getPosition(menuwindow))) { menu2.setColor(sf::Color::Black); menuNum = 2; }
			if (sf::IntRect(932, 900, 300, 50).contains(sf::Mouse::getPosition(menuwindow))) { menu3.setColor(sf::Color::Black); menuNum = 3; }

			if (menuNum != 0 && prevMenuNum != menuNum) {
				hoverSound.play();
			}
			prevMenuNum = menuNum;

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (menuNum == 1)
				{
					clickSound.play();
					sf::sleep(sf::milliseconds(150));
					isMenu = false;
				}
				else if (menuNum == 2)
				{
					clickSound.play();
					menuwindow.draw(about);
					menuwindow.display();
					while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));
				}
				else if (menuNum == 3)
				{
					clickSound.play();
					sf::sleep(sf::milliseconds(150));
					menuMusic.stop();
					exit(0);
				}
			}

			menuwindow.draw(menuBg);
			menuwindow.draw(menu1);
			menuwindow.draw(menu2);
			menuwindow.draw(menu3);
			menuwindow.display();
		}
	}
	menuMusic.stop();
}
