#pragma once

#include <SFML/Graphics.hpp>

/**
 * menu - displays the main menu with Play, About, and Exit options.
 * The function runs a blocking menu loop until the user selects Play (to start the game)
 * or Exit (to close the application). About displays a static screen until Escape is pressed.
 *
 * Plays ambient music and sound feedback for hover/click events.
 *
 * Parameters:
 *  - menuwindow: SFML RenderWindow to render the menu into.
 */
void menu(sf::RenderWindow& menuwindow);
