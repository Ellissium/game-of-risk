#pragma once

#include <SFML/Graphics.hpp>
#include "Deck.h"

// ============================================================================
// Constants
// ============================================================================
inline constexpr int MAX_HAND = 6;
inline constexpr int WINDOW_WIDTH = 1900;
inline constexpr int WINDOW_HEIGHT = 1080;

// ============================================================================
// Type aliases
// ============================================================================
using Card = Deck::Card;

// ============================================================================
// Card value computation
// ============================================================================

// cardValue - compute Blackjack numeric value for a card rank.
// Face cards return 10; aces are handled at hand-level in computeHandTotal.
int cardValue(int rank);

// ============================================================================
// Click detection
// ============================================================================

// isClickInside - test whether integer coordinates fall inside an IntRect.
bool isClickInside(int x, int y, const sf::IntRect& rect);

// ============================================================================
// Card dealing
// ============================================================================

// dealCard - remove a card from deck and add to hand, updating count and total.
void dealCard(Deck& deckObj, Card hand[], int& count, int& total);

// dealToDealer - dealer drawing policy (hit until 17 or hand full).
void dealToDealer(Deck& deckObj, Card dealer[], int& dealerCount, int& dealerTotal);

// ============================================================================
// Hand computation
// ============================================================================

// computeHandTotal - compute Blackjack hand total with ace adjustment.
// Aces are initially 11, then reduced to 1 (subtract 10) if hand exceeds 21.
int computeHandTotal(const Card hand[], int count);

// ============================================================================
// Special checks
// ============================================================================

// hasThreeSevens - check if player has three 7s (777 jackpot).
bool hasThreeSevens(const Card hand[], int count);

// ============================================================================
// Round management
// ============================================================================

// resetRound - reset UI and gameplay state for a new round.
void resetRound(bool& betPlaced, bool& betBarEnabled, bool& firstRound, bool& gameFinish, bool& ready,
	int& playerTotal, int& dealerTotal, int& playerCount, int& dealerCount, int& balance,
	sf::Text& chanceText, sf::RectangleShape& bareq,
	sf::Sprite& lose, sf::Sprite& win, sf::Sprite& tie, sf::Sprite& zero, sf::Sprite& fifty, sf::Sprite& hundred);

// ============================================================================
// Probability estimation
// ============================================================================

// estimateStandWinProbability - estimate player's win probability using Monte-Carlo.
// Returns percentage (0..100) based on remaining deck and dealer's visible card.
double estimateStandWinProbability(const Deck& deckSnapshot,
	const Card playerHand[], int playerCount,
	const Card dealerHand[], int dealerCount,
	int trials = 1000);

// ============================================================================
// Main game loop
// ============================================================================

// runGame - main application entry point; runs the game loop.
// Returns zero on normal exit or non-zero on fatal resource load failure.
int runGame();
