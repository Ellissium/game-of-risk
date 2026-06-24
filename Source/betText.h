#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <string>

inline constexpr int DELETE_KEY = 8;

/**
 * textBox - small UI helper for managing bet text and typed/buttons values.
 * Provides methods to accept numeric keyboard input (optional),
 * draw the centered bet amount above chip buttons, and update via chip presses.
 */
class betText
{
public:
	sf::Text balanceBet; // SFML text used to display the current bet amount
	int betValue = 0;
	int typedValue = 0; // value entered from keyboard (not from chip buttons)
	int buttonsValue = 0; // value added via chip buttons
	std::string h;

	/**
	 * Process text input events to update typedValue when the text entry is enabled.
	 * Only numeric characters 0-9 are accepted. Backspace (DELETE_KEY) removes last digit.
	 * Parameters:
	 *  - input: SFML Event containing text input
	 *  - Enable: whether text input is currently allowed
	 *  - availableBalance: maximum allowed typed value (caps input)
	 */
	void input(sf::Event input, bool Enable, int availableBalance);

	/**
	 * Draw the bet amount centered above the chips area.
	 * This function receives a preloaded font and centers the text using local bounds.
	 */
	void drawBetBar(sf::RenderWindow& window, const sf::Font& font);

	void addButton(int num);
private:
	/**
	 * Append a digit to the typed value, respecting the available balance cap.
	 * Updates betValue = buttonsValue + typedValue.
	 */
	void add(int value, int availableBalance);

	/**
	 * Remove the last digit from typedValue and update betValue accordingly.
	 */
	void deleteLastChar();
};
