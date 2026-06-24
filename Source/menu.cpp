#include "menu.h"
#include <SFML/Graphics.hpp>

/**
 * menu - displays a blocking menu screen with three interactive options (Play, About, Exit).
 * The menu runs in a loop until the user selects Play (closing the menu window)
 * or chooses Exit (terminating the application).
 * 
 * Parameters:
 *  - menuwindow: SFML RenderWindow to render the menu into.
 */
void menu(sf::RenderWindow& menuwindow) {
	// Load textures for menu items from disk
	sf::Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("../Assets/images/Play.png");
	menuTexture2.loadFromFile("../Assets/images/About.png");
	menuTexture3.loadFromFile("../Assets/images/Exit1.png");
	aboutTexture.loadFromFile("../Assets/images/Aboutgame.jpg");
	menuBackground.loadFromFile("../Assets/images/kartyfon.jpg");

	// Create sprites from loaded textures
	sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);

	// Menu state tracking
	bool isMenu = true;
	int menuNum = 0; // 0 = no selection, 1 = Play, 2 = About, 3 = Exit

	// Set fixed positions for menu items (hardcoded screen coordinates)
	menu1.setPosition(932, 750);
	menu2.setPosition(908, 825);
	menu3.setPosition(932, 900);
	menuBg.setPosition(0, 0);

	// Main menu event loop
	while (isMenu)
	{
		sf::Event event;
		while (menuwindow.pollEvent(event))
		{
			// Close window if user clicks the X button
			if (event.type == sf::Event::Closed) menuwindow.close();

			// Reset all menu items to white color (default state)
			menu1.setColor(sf::Color::White);
			menu2.setColor(sf::Color::White);
			menu3.setColor(sf::Color::White);
			menuNum = 0;

			// Clear background with light blue
			menuwindow.clear(sf::Color(129, 181, 221));

			// Check which menu item is hovered and highlight it in black
			if (sf::IntRect(932, 750, 300, 50).contains(sf::Mouse::getPosition(menuwindow))) { menu1.setColor(sf::Color::Black); menuNum = 1; }
			if (sf::IntRect(908, 825, 300, 50).contains(sf::Mouse::getPosition(menuwindow))) { menu2.setColor(sf::Color::Black); menuNum = 2; }
			if (sf::IntRect(932, 900, 300, 50).contains(sf::Mouse::getPosition(menuwindow))) { menu3.setColor(sf::Color::Black); menuNum = 3; }

			// Handle left mouse button clicks for menu item selection
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (menuNum == 1) 
				{ 
					// Play button: close menu and return to game
					isMenu = false; 
					menuwindow.close(); 
				}
				if (menuNum == 2) 
				{ 
					// About button: display about screen until Escape is pressed
					menuwindow.draw(about); 
					menuwindow.display(); 
					while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)); 
				}
				if (menuNum == 3) 
				{ 
					// Exit button: terminate application
					exit(0); 
				}
			}

			// Render all menu sprites to window
			menuwindow.draw(menuBg);
			menuwindow.draw(menu1);
			menuwindow.draw(menu2);
			menuwindow.draw(menu3);

			// Display rendered frame
			menuwindow.display();
		}
	}
}
