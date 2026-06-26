#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <string>

inline constexpr int DELETE_KEY = 8;

/**
 * betText - manages bet display and text input for chip/keyboard bet entry.
 * Tracks bet value from both chip buttons (buttonsValue) and keyboard input (typedValue).
 * Provides input processing and centered text rendering above the chips area.
 */
class betText
{
public:
	// ====================================================================
	// Public state
	// ====================================================================
	sf::Text balanceBet;
	int betValue = 0;
	int typedValue = 0;
	int buttonsValue = 0;
	std::string h;

	// ====================================================================
	// Public interface
	// ====================================================================

	// input - process text input events (keyboard digits and backspace).
	// Enable: whether input is currently allowed.
	// availableBalance: maximum bet cap.
	void input(sf::Event input, bool Enable, int availableBalance);

	// drawBetBar - render the current bet amount as centered text.
	void drawBetBar(sf::RenderWindow& window, const sf::Font& font);

	// addButton - increment bet when a chip button is pressed.
	void addButton(int num);

private:
	// ====================================================================
	// Private helpers
	// ====================================================================

	// add - append a digit to the typed value, respecting the balance cap.
	void add(int value, int availableBalance);

	// deleteLastChar - remove the last digit from typed value.
	void deleteLastChar();
};
