#pragma once

#include <SFML/Graphics.hpp>

/**
 * menu - simple blocking menu function used at application start.
 * This function displays three menu entries (Play, About, Exit) and
 * handles basic mouse hover/click interactions. It is kept simple
 * and synchronous for the demo; a production version would separate
 * rendering and input and avoid blocking loops.
 *
 * Parameters:
 *  - menuwindow: SFML RenderWindow to draw the menu into.
 */
void menu(sf::RenderWindow& menuwindow);
