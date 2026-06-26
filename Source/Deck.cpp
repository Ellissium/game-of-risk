#include "Deck.h"
#include <algorithm>

// ============================================================================
// Initialization
// ============================================================================

Deck::Deck()
{
	rng = std::mt19937(std::random_device{}());
	rankNames = { "", "ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king" };
	suitNames = { "", "clubs", "diamonds", "hearts", "spades" };
}

void Deck::init()
{
	cards.clear();
	cards.reserve(52);
	for (int rank = 1; rank <= 13; ++rank)
		for (int suit = 1; suit <= 4; ++suit)
			cards.push_back(Card{ rank, suit });
}

// ============================================================================
// Shuffling and drawing
// ============================================================================

void Deck::shuffle()
{
	std::shuffle(cards.begin(), cards.end(), rng);
}

Deck::Card Deck::draw()
{
	Card c = cards.back();
	cards.pop_back();
	return c;
}

// ============================================================================
// Sprite and texture management
// ============================================================================

void Deck::loadCardSprites()
{
	for (int rank = 1; rank <= 13; ++rank)
	{
		for (int suit = 1; suit <= 4; ++suit)
		{
			std::string path = "../Assets/images/" + rankNames[rank] + "_of_" + suitNames[suit] + ".png";
			cardTextures[rank][suit - 1].loadFromFile(path);
			cardSprites[rank][suit - 1].setTexture(cardTextures[rank][suit - 1]);
		}
	}
}

sf::Sprite Deck::getSprite(int rank, int suit) const
{
	return cardSprites[rank][suit - 1];
}

// ============================================================================
// State snapshot for simulations
// ============================================================================

std::vector<Deck::Card> Deck::getCardsSnapshot() const
{
	return cards;
}
