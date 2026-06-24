#pragma once

#include <SFML/Graphics.hpp>
#include "Deck.h"

// Game-level constants and helpers
inline constexpr int MAX_HAND = 6;
inline constexpr int WINDOW_WIDTH = 1900;
inline constexpr int WINDOW_HEIGHT = 1080;

// Reuse Card from Deck header for consistency
using Card = Deck::Card;

// Simple helpers
int cardValue(int rank);
bool isClickInside(int x, int y, const sf::IntRect& rect);

// Deal helpers
void dealCard(Deck& deckObj, Card hand[], int& count, int& total);
void dealToDealer(Deck& deckObj, Card dealer[], int& dealerCount, int& dealerTotal);
int computeHandTotal(const Card hand[], int count);

// Estimate probability that player will win if he stands now.
// This uses Monte-Carlo simulation and is intentionally not inlined here.
double estimateStandWinProbability(const Deck& deckSnapshot,
	const Card playerHand[], int playerCount,
	const Card dealerHand[], int dealerCount,
	int trials = 1000);

// Entry point moved from original main: runs the game loop
int runGame();

// Reset helper: resets UI and gameplay state for a new round
void resetRound(bool& betPlaced, bool& betBarEnabled, bool& firstRound, bool& gameFinish, bool& ready,
	int& playerTotal, int& dealerTotal, int& playerCount, int& dealerCount, int& balance,
	sf::Text& chanceText, sf::RectangleShape& bareq,
	sf::Sprite& lose, sf::Sprite& win, sf::Sprite& tie, sf::Sprite& zero, sf::Sprite& fifty, sf::Sprite& hundred);
