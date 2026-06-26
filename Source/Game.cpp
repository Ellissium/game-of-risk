#include "Game.h"
#include "betText.h"
#include "menu.h"
#include <array>
#include <string>
#include <iostream>
#include <random>
#include <SFML/Audio.hpp>

using namespace sf;

// ============================================================================
// Global audio resources
// ============================================================================
Music backgroundMusic;
SoundBuffer globalDealBuffer;
Sound globalDealSound;
int cardValue(int rank)
{
	return rank > 9 ? 10 : rank;
}

// ============================================================================
// Probability estimation
// ============================================================================

// Estimate player's win probability if the player stands now.
// Uses Monte-Carlo simulation: repeatedly shuffle remaining deck and
// let the dealer draw according to house rules (hit until 17).
// Returns percentage (0..100). Ties count as half-win in the estimate.
double estimateStandWinProbability(const Deck& deckSnapshot,
	const Card playerHand[], int playerCount,
	const Card dealerHand[], int dealerCount,
	int trials)
{
	// get remaining cards from current deck state
	std::vector<Card> remaining = deckSnapshot.getCardsSnapshot();
	if (trials <= 0) trials = 1000;

	std::mt19937 rng(std::random_device{}());
	int wins = 0;
	int ties = 0;

	// compute player's fixed total once (player stands)
	int playerTotal = computeHandTotal(playerHand, playerCount);
	// if player already busts, probability is zero
	if (playerTotal > 21) return 0.0;

		for (int t = 0; t < trials; ++t)
	{
		// shuffle a local copy of remaining cards
		std::vector<Card> deckLocal = remaining;
		std::shuffle(deckLocal.begin(), deckLocal.end(), rng);

		// simulate dealer starting hand (copy known dealer cards)
			// Only the dealer's first card is visible to the player.
			// Use dealerHand[0] as the known card and draw the hidden card(s)
			// from the remaining deck for simulation.
			std::vector<Card> simDealer;
			if (dealerCount > 0)
				simDealer.push_back(dealerHand[0]);
			// draw dealer's hidden card (simulate unknown card)
			if (!deckLocal.empty()) { simDealer.push_back(deckLocal.back()); deckLocal.pop_back(); }

		// dealer draws until total >= 17 (Ace handling inside computeHandTotal)
		int dealerTotal = computeHandTotal(simDealer.data(), static_cast<int>(simDealer.size()));
		while (dealerTotal < 17 && !deckLocal.empty())
		{
			simDealer.push_back(deckLocal.back()); deckLocal.pop_back();
			dealerTotal = computeHandTotal(simDealer.data(), static_cast<int>(simDealer.size()));
		}

		// determine outcome
		if (dealerTotal > 21) { ++wins; }
		else if (playerTotal > dealerTotal) { ++wins; }
		else if (playerTotal == dealerTotal) { ++ties; }
		// else dealer wins, no increment
	}

	double score = static_cast<double>(wins) + 0.5 * static_cast<double>(ties);
	return (score / static_cast<double>(trials)) * 100.0;
}

// ============================================================================
// Utility helpers
// ============================================================================

// isClickInside - test whether integer coordinates fall inside an IntRect.
// This helper centralizes the bounds check for click handling.
bool isClickInside(int x, int y, const IntRect& rect)
{
	return rect.contains(x, y);
}

// ============================================================================
// Card dealing and computation
// ============================================================================

// dealCard - remove a card from deckObj and append it to hand[].
// Updates count and plays the deal sound if buffer is loaded.
void dealCard(Deck& deckObj, Card hand[], int& count, int& total)
{
	if (count >= MAX_HAND) return;
	Card card = deckObj.draw();
	hand[count++] = card;
	total += cardValue(card.rank);
	if (globalDealSound.getBuffer() != nullptr) {
		globalDealSound.play();
	}
}

// dealToDealer - implement the dealer drawing policy (hit until 17).
// Repeatedly deals cards to the dealer until the total reaches 17 or hand is full.
void dealToDealer(Deck& deckObj, Card dealer[], int& dealerCount, int& dealerTotal)
{
	while (dealerTotal < 17 && dealerCount < MAX_HAND)
	{
		dealCard(deckObj, dealer, dealerCount, dealerTotal);
	}
}

// computeHandTotal - compute Blackjack hand total with ace adjustment.
// Aces are initially counted as 11, then reduced to 1 (subtract 10)
// while the hand value exceeds 21. Returns the final integer total.
int computeHandTotal(const Card hand[], int count)
{
	int sum = 0;
	int aces = 0;
	for (int i = 0; i < count; ++i)
	{
		int rank = hand[i].rank;
		if (rank == 1)
		{
			sum += 11;
			++aces;
		}
		else if (rank > 9)
			sum += 10;
		else
			sum += rank;
	}
	while (sum > 21 && aces > 0)
	{
		sum -= 10;
		--aces;
	}
	return sum;
}

// ============================================================================
// Game round management
// ============================================================================

// hasThreeSevens - check if player has exactly three 7s (777 jackpot).
bool hasThreeSevens(const Card hand[], int count)
{
	int sevenCount = 0;
	for (int i = 0; i < count; ++i)
	{
		if (hand[i].rank == 7) ++sevenCount;
	}
	return sevenCount >= 3;
}

// resetRound - reset visible UI and gameplay variables to prepare a new round.
// This places temporary result sprites off-screen, clears flags and numeric accumulators.
void resetRound(bool& betPlaced, bool& betBarEnabled, bool& firstRound, bool& gameFinish, bool& ready,
	int& playerTotal, int& dealerTotal, int& playerCount, int& dealerCount, int& balance,
	Text& chanceText, RectangleShape& bareq,
	Sprite& lose, Sprite& win, Sprite& tie, Sprite& zero, Sprite& fifty, Sprite& hundred)
{
	gameFinish = false;
	betPlaced = false;
	betBarEnabled = true;
	firstRound = true;
	ready = false;
	playerTotal = 0;
	dealerTotal = 0;
	playerCount = 0;
	dealerCount = 0;
	// hide result sprites off-screen
	chanceText.setPosition(1200, 2000);
	bareq.setPosition(2000, 1200);
	lose.setPosition(2000, 1200);
	win.setPosition(2000, 1200);
	tie.setPosition(2000, 1200);
	zero.setPosition(2000, 1200);
	fifty.setPosition(2000, 1200);
	hundred.setPosition(2000, 1200);
}

/**
 * runGame - main application routine. Loads resources, initializes game state,
 * enters the render/event loop and coordinates input, gameplay updates and
 * rendering until the window closes. Returns zero on normal exit or a non-zero
 * code on fatal resource load failure.
 */
int runGame()
{
	// Create windows and load resources
	Deck cardDeck;
	cardDeck.init();
	cardDeck.loadCardSprites();
	cardDeck.shuffle();

	// Create the main game window with title and fullscreen style
	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game of Risk", Style::Titlebar | Style::Fullscreen);

	// Load and set window icon
	Image icon;
	if (icon.loadFromFile("../Assets/images/ace-of-spades.png")) {
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
	else {
		std::cout << "Error: Could not load ace-of-spades.png" << std::endl;
	}

	// Display the initial menu before starting the game loop
	menu(window);

	SoundBuffer clickBuffer, chipHoverBuffer, chipClickBuffer, winBuffer, loseBuffer, tieBuffer, dealBuffer, moneyBuffer;
	Sound clickSound, chipHoverSound, chipClickSound, winSound, loseSound, tieSound, dealSound, moneySound;

	// Track previous hover state for chip buttons only
	std::array<bool,6> prevHoverBet = {false,false,false,false,false,false};

	// Load background music and set it to loop. If loading fails, print an error message.
	if (!backgroundMusic.openFromFile("../Assets/audio/game-of-risk-ambient.ogg")) {
		std::cout << "Error: Could not load game-of-risk-ambient.ogg" << std::endl;
	}
	backgroundMusic.setLoop(true);
	backgroundMusic.setVolume(100.f);
	backgroundMusic.play();

	// Load sound effects for button clicks, chip interactions, and game results.
	if (!clickBuffer.loadFromFile("../Assets/audio/button-click.wav")) {
		std::cout << "Error loading button-click.wav" << std::endl;
	}
	if (!chipHoverBuffer.loadFromFile("../Assets/audio/poker-chips-hover.wav")) {
		std::cout << "Warning: poker-chips-hover.wav not found" << std::endl;
	}
	if (!chipClickBuffer.loadFromFile("../Assets/audio/poker-chips-click.wav")) {
		std::cout << "Warning: poker-chips-click.wav not found" << std::endl;
	}
	if (!winBuffer.loadFromFile("../Assets/audio/you-win.wav")) { std::cout << "Warning: win.wav not found" << std::endl; }
	if (!loseBuffer.loadFromFile("../Assets/audio/you-lose.wav")) { std::cout << "Warning: lose.wav not found" << std::endl; }
	if (!tieBuffer.loadFromFile("../Assets/audio/tie.wav")) { std::cout << "Warning: tie.wav not found" << std::endl; }
	if (!dealBuffer.loadFromFile("../Assets/audio/taking-playing-card.wav")) { std::cout << "Warning: taking-playing-card.wav not found" << std::endl; }
	if (!moneyBuffer.loadFromFile("../Assets/audio/money.wav")) { std::cout << "Warning: money.wav not found" << std::endl; }

	// ========================================================================
	// Sound buffer assignment
	// ========================================================================
	winSound.setBuffer(winBuffer);
	loseSound.setBuffer(loseBuffer);
	tieSound.setBuffer(tieBuffer);
	dealSound.setBuffer(dealBuffer);
	globalDealBuffer = dealBuffer;
	globalDealSound.setBuffer(globalDealBuffer);

	// Setup other sounds
	clickSound.setBuffer(clickBuffer);
	chipHoverSound.setBuffer(chipHoverBuffer);
	chipClickSound.setBuffer(chipClickBuffer);
	moneySound.setBuffer(moneyBuffer);

	// Set volume levels
	clickSound.setVolume(60.f);
	chipHoverSound.setVolume(60.f);
	chipClickSound.setVolume(60.f);
	winSound.setVolume(80.f);
	loseSound.setVolume(80.f);
	tieSound.setVolume(80.f);
	dealSound.setVolume(60.f);
	moneySound.setVolume(85.f);

	betText cppbet;

	Texture backgroundTexture1, backgroundTexture2, cardBackTexture;
	Texture loseTexture, winTexture, tieTexture, exitTexture;
	Texture buttonTextures[15];

	backgroundTexture1.loadFromFile("../Assets/images/blackjack-classic-background1.jpg");
	backgroundTexture2.loadFromFile("../Assets/images/blackjack-classic-background2.jpg");
	cardBackTexture.loadFromFile("../Assets/images/cardBack.png");
	loseTexture.loadFromFile("../Assets/images/Lose.jpg");
	winTexture.loadFromFile("../Assets/images/Win.jpg");
	tieTexture.loadFromFile("../Assets/images/Tie.jpg");
	exitTexture.loadFromFile("../Assets/images/Exit2.png");

	buttonTextures[0].loadFromFile("../Assets/images/min.png");
	buttonTextures[1].loadFromFile("../Assets/images/1.png");
	buttonTextures[2].loadFromFile("../Assets/images/10.png");
	buttonTextures[3].loadFromFile("../Assets/images/100.png");
	buttonTextures[4].loadFromFile("../Assets/images/1000.png");
	buttonTextures[5].loadFromFile("../Assets/images/max.png");
	buttonTextures[6].loadFromFile("../Assets/images/double.png");
	buttonTextures[7].loadFromFile("../Assets/images/hit.png");
	buttonTextures[8].loadFromFile("../Assets/images/stand.png");
	buttonTextures[9].loadFromFile("../Assets/images/start.png");
	buttonTextures[10].loadFromFile("../Assets/images/Chancewin.png");
	buttonTextures[11].loadFromFile("../Assets/images/zero.png");
	buttonTextures[12].loadFromFile("../Assets/images/fifty.png");
	buttonTextures[13].loadFromFile("../Assets/images/hundred.png");

	Sprite background1(backgroundTexture1);
	Sprite background2(backgroundTexture2);
	Sprite cardBack(cardBackTexture);
	Sprite lose(loseTexture);
	Sprite win(winTexture);
	Sprite tie(tieTexture);
	Sprite exitSprite(exitTexture);
	Sprite betButtons[6];
	Sprite doubleButton(buttonTextures[6]);
	Sprite hitButton(buttonTextures[7]);
	Sprite standButton(buttonTextures[8]);
	Sprite startButton(buttonTextures[9]);
	Sprite chanceWin(buttonTextures[10]);
	Sprite zero(buttonTextures[11]);
	Sprite fifty(buttonTextures[12]);
	Sprite hundred(buttonTextures[13]);

	for (int i = 0; i < 6; ++i)
		betButtons[i].setTexture(buttonTextures[i]);

	// hide result sprites initially (off-screen)
	lose.setPosition(2000, 1200);
	win.setPosition(2000, 1200);
	tie.setPosition(2000, 1200);
	zero.setPosition(2000, 1200);
	fifty.setPosition(2000, 1200);
	hundred.setPosition(2000, 1200);

	// ========================================================================
	// UI text and graphics setup
	// ========================================================================
	RectangleShape bareq(Vector2f(0.f, 0.f));
	Text chanceText;
	Font font;
	if (!font.loadFromFile("../Assets/fonts/Arial.ttf"))
		return 1;

	cppbet.balanceBet.setFont(font);

	// ========================================================================
	// Game state variables
	// ========================================================================
	int balance = 1000;
	bool betPlaced = false;
	bool betBarEnabled = true;
	bool firstRound = true;
	bool gameFinish = false;
	bool ready = false;
	int playerTotal = 0;
	int dealerTotal = 0;
	int playerCount = 0;
	int dealerCount = 0;

	std::array<Card, MAX_HAND> playerHand;
	std::array<Card, MAX_HAND> dealerHand;

	// ========================================================================
	// UI interaction rectangles
	// ========================================================================
	std::array<IntRect, 6> betRects;
	std::array<int, 6> betValues = { 0, 1, 10, 100, 1000, -1 };

	IntRect startRect;
	IntRect doubleRect;
	IntRect hitRect;
	IntRect standRect;
	IntRect exitRect;

	// ========================================================================
	// Timing and animation
	// ========================================================================
	std::string balanceString;
	sf::Clock finishClock;
	float widthReal = 0.f;
	int pressedTarget = -1;
	double cachedProbability = 0.0;
	bool probDirty = true;

	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		// Ensure background music loops: if for any reason it stopped, restart playback.
		// This guards against platform/codec issues where setLoop(true) might not persist.
		if (backgroundMusic.getStatus() != sf::Music::Playing)
		{
			backgroundMusic.play();
		}
		if (gameFinish)
		{
			if (finishClock.getElapsedTime().asSeconds() >= 3.f)
			{
					cppbet.betValue = 0;
				resetRound(betPlaced, betBarEnabled, firstRound, gameFinish, ready,
					playerTotal, dealerTotal, playerCount, dealerCount, balance,
					chanceText, bareq,
					lose, win, tie, zero, fifty, hundred);
				widthReal = 0.f;

				cardDeck.init();
				cardDeck.shuffle();
					// deck / state changed after reset -> recalc probability next frame
					cachedProbability = 0.0;
					probDirty = true;
				continue;
			}
		}

		// Update interactive sprites positions used for hit-testing
		const std::array<Vector2f, 6> betButtonPositions = {
			Vector2f(630, 845), Vector2f(752, 865), Vector2f(874, 870),
			Vector2f(996, 870), Vector2f(1118, 865), Vector2f(1240, 845)
		};

		for (int i = 0; i < 6; ++i)
			betButtons[i].setPosition(betButtonPositions[i]);

		for (int i = 0; i < 6; ++i)
		{
			FloatRect fb = betButtons[i].getGlobalBounds();
			betRects[i] = IntRect(static_cast<int>(fb.left), static_cast<int>(fb.top), static_cast<int>(fb.width), static_cast<int>(fb.height));
		}

		if (!betPlaced)
			startButton.setPosition(870, 550);
		else
		{
			chanceWin.setPosition(870, 1000);
			doubleButton.setPosition(1290, 1000);
			hitButton.setPosition(1500, 1000);
			standButton.setPosition(1710, 1000);
		}

		// compute rectangles from current sprite bounds
		{
			FloatRect fb = startButton.getGlobalBounds();
			startRect = IntRect(static_cast<int>(fb.left), static_cast<int>(fb.top), static_cast<int>(fb.width), static_cast<int>(fb.height));
		}
		{
			FloatRect fb = doubleButton.getGlobalBounds();
			doubleRect = IntRect(static_cast<int>(fb.left), static_cast<int>(fb.top), static_cast<int>(fb.width), static_cast<int>(fb.height));
		}
		{
			FloatRect fb = hitButton.getGlobalBounds();
			hitRect = IntRect(static_cast<int>(fb.left), static_cast<int>(fb.top), static_cast<int>(fb.width), static_cast<int>(fb.height));
		}
		{
			FloatRect fb = standButton.getGlobalBounds();
			standRect = IntRect(static_cast<int>(fb.left), static_cast<int>(fb.top), static_cast<int>(fb.width), static_cast<int>(fb.height));
		}
		exitSprite.setPosition(30, 1000);
		{
			FloatRect fb = exitSprite.getGlobalBounds();
			exitRect = IntRect(static_cast<int>(fb.left), static_cast<int>(fb.top), static_cast<int>(fb.width), static_cast<int>(fb.height));
		}

		// hover animation: scale slightly when mouse over for chips; compute hover flags for buttons
		Vector2i mpos = Mouse::getPosition(window);
		Vector2f mposf(static_cast<float>(mpos.x), static_cast<float>(mpos.y));
		for (int i = 0; i < 6; ++i)
		{
			// chips are interactive only while bet bar enabled and round not finished
			if (!betBarEnabled || gameFinish) { betButtons[i].setScale(1.f,1.f); continue; }
			if (pressedTarget == i) continue;
			if (betButtons[i].getGlobalBounds().contains(mposf))
				betButtons[i].setScale(1.08f, 1.08f);
			else
				betButtons[i].setScale(1.f, 1.f);
		}
		// compute hover flags respecting allowed interaction states
			bool hoverStart = startButton.getGlobalBounds().contains(mposf) && betBarEnabled && !betPlaced && !gameFinish && cppbet.betValue > 0;
			bool hoverDouble = doubleButton.getGlobalBounds().contains(mposf) && betPlaced && !betBarEnabled && !gameFinish;
			bool hoverHit = hitButton.getGlobalBounds().contains(mposf) && betPlaced && !betBarEnabled && !gameFinish;
			bool hoverStand = standButton.getGlobalBounds().contains(mposf) && betPlaced && !betBarEnabled && !gameFinish;
			// exit should be hoverable even during the round finish pause so user can exit
			bool hoverExit = exitSprite.getGlobalBounds().contains(mposf);

			// Hover sound logic removed for buttons (Start, Hit, Stand, Double, Exit)
			// Only chip buttons play hover sound

			// For chip buttons play hover sound only once when entering hover
			for (int i = 0; i < 6; ++i)
			{
				if (!betBarEnabled || gameFinish) { prevHoverBet[i] = false; continue; }
				if (betButtons[i].getGlobalBounds().contains(mposf)) {
					if (!prevHoverBet[i]) chipHoverSound.play();
					prevHoverBet[i] = true;
				} else {
					prevHoverBet[i] = false;
				}
			}

		// glow color: use white for visibility on dark backgrounds
		auto glowColor = [&](const Color& fallback) -> Color { return Color(255,255,255,160); };

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				clickSound.play();
				sf::sleep(sf::milliseconds(150));
				window.close();
			}

			// keyboard entry for bets disabled (use chips only)

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
			{
				int mouseX = Mouse::getPosition(window).x;
				int mouseY = Mouse::getPosition(window).y;

				// press animation: chips scale on press, buttons only record pressedTarget to hide glow
				bool handledPress = false;
				// chips are clickable only while bet bar enabled and round not finished
				if (betBarEnabled && !gameFinish)
				{
					for (int i = 0; i < 6; ++i)
					{
						if (isClickInside(mouseX, mouseY, betRects[i]))
						{
							pressedTarget = i;
							betButtons[i].setScale(0.92f, 0.92f);
							handledPress = true;
							break;
						}
					}
				}
				if (!handledPress)
				{
					// start allowed only before round starts and while bet bar enabled
					if (isClickInside(mouseX, mouseY, startRect) && betBarEnabled && !betPlaced && !gameFinish && cppbet.betValue > 0) { pressedTarget = 6; }
					// in-round actions allowed only when betPlaced and betting disabled and round not finished
					else if (isClickInside(mouseX, mouseY, doubleRect) && betPlaced && !betBarEnabled && !gameFinish) { pressedTarget = 7; }
					else if (isClickInside(mouseX, mouseY, hitRect) && betPlaced && !betBarEnabled && !gameFinish) { pressedTarget = 8; }
					else if (isClickInside(mouseX, mouseY, standRect) && betPlaced && !betBarEnabled && !gameFinish) { pressedTarget = 9; }
					// exit allowed anytime (including during finish pause)
					else if (isClickInside(mouseX, mouseY, exitRect)) { 
						pressedTarget = 10; 
						clickSound.play();
						std::cout << "Exit clicked - playing sound and closing" << std::endl;
						for (int i = 0; i < 20; ++i) {
							window.display();
							sf::sleep(sf::milliseconds(10));
						}
						window.close();
						break;
					}
				}

				if (isClickInside(mouseX, mouseY, startRect) && cppbet.betValue > 0 && !betPlaced && !gameFinish)
				{
					clickSound.play();

					// ensure typed portion does not exceed available balance
					if (cppbet.typedValue > balance)
					{
						int diff = cppbet.typedValue - balance;
						cppbet.typedValue = balance;
						cppbet.betValue = cppbet.buttonsValue + cppbet.typedValue;
						balance = 0;
					}
					else
					{
						balance -= cppbet.typedValue; // subtract typed input; buttonsValue was removed when chips clicked
					}
					betBarEnabled = false;
					betPlaced = true;
					// new betting state -> probability depends on hands/deck; mark dirty
					probDirty = true;
				}
				else if (isClickInside(mouseX, mouseY, hitRect) && !betBarEnabled && !gameFinish)
						{
							clickSound.play();

						if (betPlaced && playerTotal < 21 && playerCount < MAX_HAND)
							dealCard(cardDeck, playerHand.data(), playerCount, playerTotal);

						// recompute totals with proper Ace handling
						playerTotal = computeHandTotal(playerHand.data(), playerCount);

						// player card changed -> recompute probability once (defer heavy work to post-event)
						probDirty = true;

						if (playerTotal > 21)
						{
							// player busted: show lose and auto-finish round
								lose.setPosition(651, 345);
								loseSound.play();
								zero.setPosition(870, 1000);
								gameFinish = true; finishClock.restart(); widthReal = 0.f;
						}
						else if (playerTotal == 21)
						{
							// Player reached natural 21: resolve immediately as a stand.
							dealToDealer(cardDeck, dealerHand.data(), dealerCount, dealerTotal);
							dealerTotal = computeHandTotal(dealerHand.data(), dealerCount);

							// dealer cards changed -> probability result finalized
							probDirty = true;

							if (playerTotal > 21 || (dealerTotal <= 21 && dealerTotal > playerTotal))
							{
								lose.setPosition(651, 345);
								loseSound.play();
								zero.setPosition(870, 1000);
								gameFinish = true; finishClock.restart(); widthReal = 0.f;
							}
							else if ((playerTotal <= 21 && playerTotal > dealerTotal) || dealerTotal > 21)
							{
								win.setPosition(651, 345);
								winSound.play(); moneySound.play();
								hundred.setPosition(870, 1000);
								balance += 2 * cppbet.betValue;
								gameFinish = true; finishClock.restart(); widthReal = 0.f;
							}
							else
							{
								tie.setPosition(651, 345);
								tieSound.play();
								fifty.setPosition(870, 1000);
								balance += cppbet.betValue;
								gameFinish = true; finishClock.restart(); widthReal = 0.f;
							}
						}
						else
						{
							// No immediate resolution: continue game normally

					}
				}
				else if (isClickInside(mouseX, mouseY, standRect) && !betBarEnabled && !gameFinish)
							{
								clickSound.play();

					dealToDealer(cardDeck, dealerHand.data(), dealerCount, dealerTotal);
					dealerTotal = computeHandTotal(dealerHand.data(), dealerCount);

					// dealer cards changed -> probability result finalized
					probDirty = true;

					if (playerTotal > 21 || (dealerTotal <= 21 && dealerTotal > playerTotal))
					{
						lose.setPosition(651, 345);
						loseSound.play();
						zero.setPosition(870, 1000);
						gameFinish = true; finishClock.restart(); widthReal = 0.f;
					}
					else if ((playerTotal <= 21 && playerTotal > dealerTotal) || dealerTotal > 21)
					{
						win.setPosition(651, 345);
						winSound.play(); moneySound.play();
						hundred.setPosition(870, 1000);
						balance += 2 * cppbet.betValue;
						gameFinish = true; finishClock.restart(); widthReal = 0.f;
					}
					else
					{
						tie.setPosition(651, 345);
						tieSound.play();
						fifty.setPosition(870, 1000);
						balance += cppbet.betValue;
						gameFinish = true; finishClock.restart(); widthReal = 0.f;
					}
				}
				else if (isClickInside(mouseX, mouseY, doubleRect) && betPlaced && playerTotal < 21 && !gameFinish)
							{
								clickSound.play();

					int stake = cppbet.betValue;
					dealCard(cardDeck, playerHand.data(), playerCount, playerTotal);
					playerTotal = computeHandTotal(playerHand.data(), playerCount);
					dealToDealer(cardDeck, dealerHand.data(), dealerCount, dealerTotal);
					dealerTotal = computeHandTotal(dealerHand.data(), dealerCount);
					// ensure player total reflects any ace adjustments after the extra card
					playerTotal = computeHandTotal(playerHand.data(), playerCount);
					cppbet.betValue *= 2;
					balance -= stake;

					// double changes both player and likely dealer cards
					probDirty = true;

					if (playerTotal > 21 || (dealerTotal <= 21 && dealerTotal > playerTotal))
					{
						lose.setPosition(651, 345);
						loseSound.play();
						zero.setPosition(870, 1000);
						gameFinish = true; finishClock.restart();
					}
					else if ((playerTotal <= 21 && playerTotal > dealerTotal) || dealerTotal > 21)
					{
						win.setPosition(651, 345);
						winSound.play(); moneySound.play();
						hundred.setPosition(870, 1000);
						balance += 2 * cppbet.betValue;
						gameFinish = true; finishClock.restart();
					}
					else
					{
						tie.setPosition(651, 345);
						tieSound.play();
						fifty.setPosition(870, 1000);
						balance += cppbet.betValue;
						gameFinish = true; finishClock.restart();
					}
				}

				// do not compute probability here per-frame; mark/proc is handled once per-loop
				//if (ready && playerTotal < 22)
				//{
				//	// keep probability ready for display; compute once after event loop if dirty
				//}

				// on mouse release, reset pressed animation (will be handled in MouseButtonReleased event)

				if (betBarEnabled)
							{
								for (int i = 0; i < static_cast<int>(betRects.size()); ++i)
								{
									if (!isClickInside(mouseX, mouseY, betRects[i]))
										continue;

									if (i == 0 && cppbet.betValue > 0)
									{
										balance += cppbet.betValue;
										cppbet.betValue = 0;
										cppbet.addButton(0);
																chipClickSound.play();
									}
									else if (i == 5) // max
									{
										balance += cppbet.betValue;
										cppbet.betValue = 0;
										cppbet.addButton(balance);
																balance = 0;
																chipClickSound.play();
									}
									else if (betValues[i] > 0 && balance >= betValues[i])
									{
										balance -= betValues[i];
										cppbet.addButton(betValues[i]);
																// bet changed -> probability depends on player/dealer state; recompute
																probDirty = true;
																chipClickSound.play();
									}
								}
							}
			}
			else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				int rx = Mouse::getPosition(window).x;
				int ry = Mouse::getPosition(window).y;
				Vector2f rpos(static_cast<float>(rx), static_cast<float>(ry));
				if (pressedTarget >= 0 && pressedTarget < 6)
				{
					if (betButtons[pressedTarget].getGlobalBounds().contains(rpos) && betBarEnabled && !gameFinish)
						betButtons[pressedTarget].setScale(1.08f, 1.08f);
					else
						betButtons[pressedTarget].setScale(1.f, 1.f);
				}
				// always clear pressedTarget on release so glows/hovers can resume
				pressedTarget = -1;
			}
		}

		window.clear(Color::Green);

		if (!betPlaced)
		{
			window.draw(background1);
			// Show start button only when a non-zero bet has been placed
			if (cppbet.betValue > 0)
			{
				startButton.setPosition(870, 550);
				// glow for start button when hovered (skip while pressedTarget == start)
				if (hoverStart && pressedTarget != 6)
				{
					FloatRect fb = startButton.getGlobalBounds();
					Color col = glowColor(Color(255,255,255,160));
					for (int k = 2; k >= 0; --k)
					{
						RectangleShape glow(Vector2f(fb.width + 12.f + k * 8.f, fb.height + 12.f + k * 8.f));
					glow.setPosition(fb.left - 6.f - k*4.f, fb.top - 6.f - k*4.f);
					glow.setFillColor(Color(col.r, col.g, col.b, static_cast<Uint8>(60/(k+1))));
					window.draw(glow);
				}
			}
						window.draw(startButton);
					}
					zero.setPosition(2000, 1200);
					hundred.setPosition(2000, 1200);
					fifty.setPosition(2000, 1200);
				}
			else
			{
				window.draw(background2);
			chanceWin.setPosition(870, 1000);
			window.draw(chanceWin);
			doubleButton.setPosition(1290, 1000);
			// glow for double button (skip while pressed)
			if (hoverDouble && pressedTarget != 7)
			{
				FloatRect fb = doubleButton.getGlobalBounds();
				Color col = glowColor(Color(255,255,255,160));
				for (int k = 2; k >= 0; --k)
				{
					RectangleShape glow(Vector2f(fb.width + 12.f + k*8.f, fb.height + 12.f + k*8.f));
					glow.setPosition(fb.left - 6.f - k*4.f, fb.top - 6.f - k*4.f);
					glow.setFillColor(Color(col.r, col.g, col.b, static_cast<Uint8>(60/(k+1))));
					window.draw(glow);
				}
			}
			window.draw(doubleButton);
			hitButton.setPosition(1500, 1000);
			if (hoverHit && pressedTarget != 8)
			{
				FloatRect fb = hitButton.getGlobalBounds();
				Color col = glowColor(Color(255,255,255,160));
				for (int k = 2; k >= 0; --k)
				{
					RectangleShape glow(Vector2f(fb.width + 12.f + k*8.f, fb.height + 12.f + k*8.f));
					glow.setPosition(fb.left - 6.f - k*4.f, fb.top - 6.f - k*4.f);
					glow.setFillColor(Color(col.r, col.g, col.b, static_cast<Uint8>(60/(k+1))));
					window.draw(glow);
				}
			}
			window.draw(hitButton);
			standButton.setPosition(1710, 1000);
			if (hoverStand && pressedTarget != 9)
			{
				FloatRect fb = standButton.getGlobalBounds();
				Color col = glowColor(Color(255,255,255,160));
				for (int k = 2; k >= 0; --k)
				{
					RectangleShape glow(Vector2f(fb.width + 12.f + k*8.f, fb.height + 12.f + k*8.f));
					glow.setPosition(fb.left - 6.f - k*4.f, fb.top - 6.f - k*4.f);
					glow.setFillColor(Color(col.r, col.g, col.b, static_cast<Uint8>(60/(k+1))));
					window.draw(glow);
				}
			}
			window.draw(standButton);
		}

		exitSprite.setPosition(30, 1000);
		// glow for exit when hovered (skip while pressed)
		// exit should glow even during gameFinish so user can click to exit
		if (hoverExit && pressedTarget != 10)
		{
			FloatRect fb = exitSprite.getGlobalBounds();
			Color col = glowColor(Color(255,255,255,160));
			for (int k = 2; k >= 0; --k)
			{
				RectangleShape glow(Vector2f(fb.width + 12.f + k*8.f, fb.height + 12.f + k*8.f));
				glow.setPosition(fb.left - 6.f - k*4.f, fb.top - 6.f - k*4.f);
				glow.setFillColor(Color(col.r, col.g, col.b, static_cast<Uint8>(60/(k+1))));
				window.draw(glow);
			}
		}
		window.draw(exitSprite);

		for (int i = 0; i < 6; ++i)
		{
			window.draw(betButtons[i]);
		}

		if (betPlaced && firstRound)
		{
			// Initial two-card deal to player and dealer
			dealCard(cardDeck, playerHand.data(), playerCount, playerTotal);
			dealCard(cardDeck, playerHand.data(), playerCount, playerTotal);
			dealCard(cardDeck, dealerHand.data(), dealerCount, dealerTotal);
			dealCard(cardDeck, dealerHand.data(), dealerCount, dealerTotal);
			playerTotal = computeHandTotal(playerHand.data(), playerCount);
			dealerTotal = computeHandTotal(dealerHand.data(), dealerCount);
			ready = true;
			firstRound = false;

			// If player has a natural blackjack (two-card 21), resolve immediately
			if (playerCount == 2 && playerTotal == 21)
			{
				// Dealer also has blackjack -> push (tie)
				if (dealerCount == 2 && dealerTotal == 21)
										{
											tie.setPosition(651, 345);
											tieSound.play();
											fifty.setPosition(870, 1000); // show tie indicator
											balance += cppbet.betValue; // return stake
											gameFinish = true; finishClock.restart(); widthReal = 0.f;
										}
										else
										{
													// Player has blackjack: pay 3:2
													win.setPosition(651, 345);
													winSound.play(); moneySound.play();
													hundred.setPosition(870, 1000);
													int payout = (cppbet.betValue * 5) / 2; // 2.5x total return
													balance += payout;
													gameFinish = true; finishClock.restart(); widthReal = 0.f;
												}
											}
											// Check for three sevens (777) jackpot
											else if (hasThreeSevens(playerHand.data(), playerCount))
											{
												win.setPosition(651, 345);
												winSound.play(); moneySound.play();
												hundred.setPosition(870, 1000);
												int jackpot = cppbet.betValue * 3; // 3:1 payout for 777
												balance += cppbet.betValue + jackpot;
												gameFinish = true; finishClock.restart(); widthReal = 0.f;
											}
		}

		Text balanceText;
		balanceText.setFont(font);
		balanceText.setOutlineColor(Color::Black);
		balanceText.setOutlineThickness(2);
		balanceText.setString("Balance: " + std::to_string(balance));
	balanceText.setPosition(30.f, 30.f);
		balanceText.setCharacterSize(45);
		window.draw(balanceText);

		int positionX = 480;
		for (int i = 0; i < playerCount; ++i)
		{
			Sprite cardSprite = cardDeck.getSprite(playerHand[i].rank, playerHand[i].suit);
			cardSprite.setPosition(static_cast<float>(positionX), 450.f);
			positionX += 160;
			window.draw(cardSprite);
		}

		positionX = 480;
		for (int i = 0; i < dealerCount; ++i)
		{
			Sprite cardSprite;
			if (i == 1 && !gameFinish)
			{
				cardSprite = cardBack;
			}
			else
			{
				cardSprite = cardDeck.getSprite(dealerHand[i].rank, dealerHand[i].suit);
			}
			cardSprite.setPosition(static_cast<float>(positionX), 100.f);
			positionX += 160;
			window.draw(cardSprite);
		}

		float realChance = 0.f;
		// compute and show probability only during active round (ready && not in finish state)
		if (ready && !gameFinish && playerTotal < 22)
		{
			if (probDirty)
			{
				// compute once when state changed
				cachedProbability = estimateStandWinProbability(cardDeck, playerHand.data(), playerCount, dealerHand.data(), dealerCount, 300);
				probDirty = false;
			}
			realChance = static_cast<float>(cachedProbability);
		}

		// target width is zero when not actively displaying probability (hide during finish/reset)
		float targetWidth = (ready && !gameFinish) ? (164.f / 100.f) * realChance : 0.f;
		// Smoothly animate widthReal towards targetWidth with a stable step.
		float diff = targetWidth - widthReal;
		if (std::abs(diff) > 0.01f)
		{
			// use a step proportional to diff but clamped to avoid 1-pixel jitter
			float step = std::clamp(diff * 0.15f, -4.f, 4.f);
			// ensure we snap to target when very close to avoid oscillation
			if (std::abs(step) < 0.2f) step = diff;
			widthReal += step;
		}

		// draw percentage bar and centered label only during active round
		if (ready && !gameFinish)
		{
			// round width to avoid subpixel jitter when converting to integer pixels
			float displayWidth = std::round(widthReal);
			bareq.setSize(Vector2f(displayWidth, 60.f));
			bareq.setFillColor(Color::Red);
			bareq.setPosition(878, 1000);
			window.draw(bareq);

			chanceText.setFont(font);
			chanceText.setOutlineColor(Color::Black);
			chanceText.setOutlineThickness(2);
			chanceText.setCharacterSize(45);
			// center chanceText above the fixed bar container for stable alignment
			std::string chanceStr = std::to_string(static_cast<int>(realChance));
			chanceText.setString(chanceStr);
			FloatRect tb = chanceText.getLocalBounds();
			chanceText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
			// fixed container for the bar is 164px wide at x=878; center text inside the bar
			const float BAR_X = 878.f;
			const float BAR_FULL_WIDTH = 164.f;
			const float BAR_Y = 1000.f;
			const float BAR_HEIGHT = 60.f;
			// place text vertically centered inside the bar so it overlays the fill
			chanceText.setPosition(BAR_X + BAR_FULL_WIDTH / 2.f, BAR_Y + BAR_HEIGHT / 2.f);
			window.draw(chanceText);
		}
		window.draw(lose);
		window.draw(win);
		window.draw(tie);
		window.draw(zero);
		window.draw(fifty);
		window.draw(hundred);
		cppbet.drawBetBar(window, font);
		window.display();
	}

	return 0;
}
