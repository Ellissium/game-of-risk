#include "Deck.h"
#include <algorithm>

/**
 * Deck constructor.
 * Initializes the pseudorandom number generator and human-readable
 * rank/suit lookup tables used to build image filenames.
 */
Deck::Deck()
{
	rng = std::mt19937(std::random_device{}());
	rankNames = { "", "ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king" };
	suitNames = { "", "clubs", "diamonds", "hearts", "spades" };
}

/**
 * init - populate the deck with a standard 52-card set in canonical order.
 * After init() the deck contains a deterministic ordered set; call shuffle()
 * to produce gameplay randomness.
 */
void Deck::init()
{
	cards.clear();
	cards.reserve(52);
	for (int rank = 1; rank <= 13; ++rank)
		for (int suit = 1; suit <= 4; ++suit)
			cards.push_back(Card{ rank, suit });
}

/**
 * shuffle - randomize the order of remaining cards using the stored RNG.
 */
void Deck::shuffle()
{
	std::shuffle(cards.begin(), cards.end(), rng);
}

/**
 * draw - remove and return the top card from the deck.
 * Caller must ensure the deck is non-empty.
 */
Deck::Card Deck::draw()
{
	Card c = cards.back();
	cards.pop_back();
	return c;
}

/**
 * loadCardSprites - load PNG textures for each card and initialize sprites.
 * Expects files named like images/ace_of_spades.png. Failing to find files
 * will leave textures uninitialized and may cause rendering issues.
 */
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

/**
 * getSprite - return a Sprite configured for the given rank and suit.
 * The returned Sprite references textures owned by this Deck instance.
 */
sf::Sprite Deck::getSprite(int rank, int suit) const
{
	return cardSprites[rank][suit - 1];
}

/**
 * getCardsSnapshot - return a copy of the remaining cards vector.
 * Useful for simulations that must not mutate the live deck state.
 */
std::vector<Deck::Card> Deck::getCardsSnapshot() const
{
	return cards;
}
