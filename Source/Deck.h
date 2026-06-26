#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <random>
#include <string>

/**
 * Deck - manages a standard 52-card deck with associated textures and sprites.
 * 
 * Responsibilities:
 *  - Initialize and shuffle card list
 *  - Provide draw() to consume cards during gameplay
 *  - Load and cache card textures/sprites for efficient rendering
 *
 * Public API:
 *  - init(): fill deck with 52 cards
 *  - shuffle(): randomize card order
 *  - draw(): remove and return top card
 *  - loadCardSprites(): load textures from disk
 *  - getSprite(rank, suit): return sprite for rendering
 *  - getCardsSnapshot(): return remaining cards for simulation
 */
class Deck
{
public:
	// ====================================================================
	// Card representation
	// ====================================================================

	// Simple POD representing a single card by rank (1..13) and suit (1..4).
	struct Card { int rank; int suit; };

	// ====================================================================
	// Initialization and lifecycle
	// ====================================================================

	// Deck() - construct and initialize RNG and card name lookups.
	Deck();

	// init() - fill the deck with a canonical 52-card sequence.
	void init();

	// shuffle() - randomize remaining card order using RNG.
	void shuffle();

	// ====================================================================
	// Card operations
	// ====================================================================

	// draw() - remove and return the top card from the deck.
	Card draw();

	// ====================================================================
	// Sprite and texture management
	// ====================================================================

	// loadCardSprites() - load card image textures from disk and initialize sprites.
	// Expects files at images/<rank>_of_<suit>.png
	void loadCardSprites();

	// getSprite(rank, suit) - return a prepared sprite for rendering a card.
	sf::Sprite getSprite(int rank, int suit) const;

	// ====================================================================
	// Snapshot for simulations
	// ====================================================================

	// getCardsSnapshot() - return copy of remaining cards (does not modify deck).
	std::vector<Card> getCardsSnapshot() const;

private:
	// ====================================================================
	// Internal state
	// ====================================================================
	std::vector<Card> cards;
	std::mt19937 rng;
	std::array<std::array<sf::Texture, 4>, 14> cardTextures;
	std::array<std::array<sf::Sprite, 4>, 14> cardSprites;
	std::array<std::string, 14> rankNames;
	std::array<std::string, 5> suitNames;
};
