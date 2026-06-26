#include "betText.h"

// ============================================================================
// Text input handling
// ============================================================================

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
				add(0, availableBalance);
			else if (charTyped == 49) add(1, availableBalance);
			else if (charTyped == 50) add(2, availableBalance);
			else if (charTyped == 51) add(3, availableBalance);
			else if (charTyped == 52) add(4, availableBalance);
			else if (charTyped == 53) add(5, availableBalance);
			else if (charTyped == 54) add(6, availableBalance);
			else if (charTyped == 55) add(7, availableBalance);
			else if (charTyped == 56) add(8, availableBalance);
			else if (charTyped == 57) add(9, availableBalance);
		}
		// Check for backspace (DELETE_KEY = 8) to remove the last digit
		else if (betValue > 0 && charTyped == DELETE_KEY)
		{
			deleteLastChar();
		}
	}
}

// ============================================================================
// Rendering
// ============================================================================

void betText::drawBetBar(sf::RenderWindow& window, const sf::Font& font)
{
	balanceBet.setFont(font);
	balanceBet.setOutlineColor(sf::Color::Black);
	balanceBet.setOutlineThickness(2);
	balanceBet.setCharacterSize(35);

	h = std::to_string(betValue);
	balanceBet.setString(h);

	sf::FloatRect tb = balanceBet.getLocalBounds();
	balanceBet.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);

	balanceBet.setPosition(955.f, 745.f);
	window.draw(balanceBet);
}

// ============================================================================
// Bet management
// ============================================================================

void betText::addButton(int num)
{
	betValue += num;
	buttonsValue += num;
}

void betText::add(int value, int availableBalance)
{
	int newTyped = typedValue * 10 + value;

	if (newTyped > availableBalance)
		newTyped = availableBalance;

	typedValue = newTyped;
	betValue = buttonsValue + typedValue;
}

void betText::deleteLastChar()
{
	typedValue /= 10;
	betValue = buttonsValue + typedValue;
}
