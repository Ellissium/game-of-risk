#pragma once
// Deck.h - part of Game of Risk
// Manages a standard 52-card deck, textures and sprites.
// Public API: init(), shuffle(), draw(), loadCardSprites(), getSprite(rank,suit)
// Adds utilities to access card textures and sprites for rendering.

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <random>
#include <string>

/**
 * Deck - manages a standard 52-card deck, associated textures and sprites.
 * Responsibilities:
 *  - initialize and shuffle the in-memory card list
 *  - provide draw() to consume cards
 *  - load textures for each card image and supply sprites for rendering
 *
 * The class keeps texture and sprite arrays indexed by rank/suit to avoid
 * reloading images frequently.
 */
// Public API: init(), shuffle(), draw(), loadCardSprites(), getSprite(rank,suit)
class Deck
{
public:
	/**
	 * Construct a Deck object and initialize RNG and lookup names.
	 */
	Deck();

	/**
	 * Fill the internal card vector with a canonical 52-card sequence.
	 * After calling init(), shuffle() should be used to randomize order.
	 */
	void init();

	/**
	 * Randomize order of remaining cards using the stored RNG.
	 */
	void shuffle();

	/**
	 * Simple POD representing a single card by rank (1..13) and suit (1..4).
	 */
	struct Card { int rank; int suit; };

	/**
	 * Remove and return the top card from the deck. Caller must ensure
	 * the deck is not empty before calling.
	 */
	Card draw();

	/**
	 * Load textures from disk and initialize sprites for each rank/suit.
	 * Expects image files at images/<rank>_of_<suit>.png matching rankNames/suitNames.
	 */
	void loadCardSprites();

	/**
	 * Return a prepared Sprite for rendering the specified card.
	 * The returned Sprite references an internally stored Texture.
	 */
	sf::Sprite getSprite(int rank, int suit) const;

	/**
	 * Return a copy of the remaining (not-yet-drawn) cards. This is a
	 * snapshot useful for simulation or AI; it does not modify internal state.
	 */
	std::vector<Card> getCardsSnapshot() const;

private:
	std::vector<Card> cards;
	std::mt19937 rng;
	std::array<std::array<sf::Texture, 4>, 14> cardTextures;
	std::array<std::array<sf::Sprite, 4>, 14> cardSprites;
	std::array<std::string, 14> rankNames;
	std::array<std::string, 5> suitNames;
};
