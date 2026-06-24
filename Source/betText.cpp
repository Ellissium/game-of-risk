#include "betText.h"

/**
 * input - process text input events to update the typed bet value.
 * Accepts numeric characters (0-9) and allows backspace to delete digits.
 * Respects the availableBalance as a maximum cap on the typed value.
 * 
 * Parameters:
 *  - input: SFML text input event from the event loop
 *  - Enable: whether text input is currently allowed (enables/disables this method)
 *  - availableBalance: maximum value that can be typed (caps input when exceeded)
 */
void betText::input(sf::Event input, bool Enable, int availableBalance)
{
	if (Enable == true)
	{
		int charTyped = input.text.unicode;
		// Check if character is a digit (ASCII 48-57 = '0'-'9')
		if (charTyped <= 57 && charTyped >= 48)
		{
			// Map each digit character to its numeric value
			if (charTyped == 48)
			{
				add(0, availableBalance);
			}
			if (charTyped == 49) { add(1, availableBalance); }
			if (charTyped == 50) { add(2, availableBalance); }
			if (charTyped == 51) { add(3, availableBalance); }
			if (charTyped == 52) { add(4, availableBalance); }
			if (charTyped == 53) { add(5, availableBalance); }
			if (charTyped == 54) { add(6, availableBalance); }
			if (charTyped == 55) { add(7, availableBalance); }
			if (charTyped == 56) { add(8, availableBalance); }
			if (charTyped == 57) { add(9, availableBalance); }
		}
		// Check for backspace (DELETE_KEY = 8) to remove the last digit
		else if (betValue > 0 && charTyped == DELETE_KEY)
		{
			deleteLastChar();
		}
	}
}

/**
 * drawBetBar - render the current bet amount as centered text above the chip buttons.
 * Receives a preloaded font to avoid repeated disk access on each frame.
 * Centers the text both horizontally and vertically within the bet display area.
 * 
 * Parameters:
 *  - window: SFML RenderWindow to draw the text into
 *  - font: preloaded sf::Font for text rendering
 */
void betText::drawBetBar(sf::RenderWindow& window, const sf::Font& font)
{
	// Configure text appearance
	balanceBet.setFont(font);
	balanceBet.setOutlineColor(sf::Color::Black);
	balanceBet.setOutlineThickness(2);
	balanceBet.setCharacterSize(35);

	// Convert bet value to string and set as text
	h = std::to_string(betValue);
	balanceBet.setString(h);

	// Calculate local bounds to center text properly
	sf::FloatRect tb = balanceBet.getLocalBounds();
	balanceBet.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);

	// Position text centered above the chips area (fixed screen coordinates)
	balanceBet.setPosition(955.f, 745.f);
	window.draw(balanceBet);
}

/**
 * addButton - increment bet value when a chip button is clicked.
 * Adds to both betValue (total) and buttonsValue (amount from buttons only).
 * 
 * Parameters:
 *  - num: amount to add (typically chip denominations: 1, 10, 100, 1000)
 */
void betText::addButton(int num)
{
	betValue += num;
	buttonsValue += num;
}

/**
 * add - append a digit to the typed value, enforcing the available balance cap.
 * Updates betValue as the sum of buttonsValue (from chips) and typedValue (from keyboard).
 * 
 * Parameters:
 *  - value: digit to append (0-9)
 *  - availableBalance: maximum allowed typed value; caps if exceeded
 */
void betText::add(int value, int availableBalance)
{
	// Shift existing typed value left (multiply by 10) and add new digit
	int newTyped = typedValue * 10 + value;

	// Enforce balance cap
	if (newTyped > availableBalance)
		newTyped = availableBalance;

	// Update internal state
	typedValue = newTyped;
	betValue = buttonsValue + typedValue;
}

/**
 * deleteLastChar - remove the last digit from the typed value.
 * Performs integer division by 10 to shift digits right, effectively removing the last digit.
 * Updates betValue as the sum of buttonsValue and the reduced typedValue.
 */
void betText::deleteLastChar()
{
	// Integer division by 10 removes the rightmost digit
	typedValue /= 10;
	betValue = buttonsValue + typedValue;
}
