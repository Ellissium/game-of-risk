#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include "betText.h"
#include "menu.h"
#include <Time.h>
#include <vector>
#include <stdlib.h>
using namespace sf;

 
float chance;
float realChance;
string c;
float widthGoal = 0.f;
float widthReal = 0.f;
int semen;
bool ready = 0;
int balance = 1000;
bool betPlaced = false, betBarEnable = true, first = true, gameFinish = false;
int playerNrOfCards, dealerNrOfCards;
int playerCards, dealerCards;
int gameNum = 0;

Text chanceText;

struct card
{
	int color, value;
};

sf::RectangleShape bareq(sf::Vector2f(0.f, 0.f));
card player[6], dealer[6];

vector<int>deck;
void initdeck()
{
	deck = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
}
void shufflercard()
{
	srand((unsigned)time(0));
	for (int i = 0; i < 52; i++)
		swap(deck[i], deck[rand() % 52]);

}
int randomgenerator()
{
	int card = deck.front();
	deck.erase(deck.begin());
	return card;
}
vector<int>mast;
void initmast()
{
	mast = { 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4,
			1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4,
			1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4,
			1, 2, 3, 4, };
}
void shufflermast()
{
	srand((unsigned)time(0));
	for (int i = 0; i < 52; i++)
		swap(mast[i], mast[rand() % 52]);

}

int randomColor()
{
	int suit = mast.front();
	mast.erase(mast.begin());
	return suit;
}



int main()
{
	initdeck();
	shufflercard();
	initmast();
	shufflermast();
	RenderWindow menuwindow(VideoMode(1900, 1080), "Game of Risk", Style::Titlebar | Style::Fullscreen);
	menu(menuwindow);
	RenderWindow window(VideoMode(1900, 1080), "Game of Risk", Style::Titlebar | Style::Fullscreen);
	textBox bet;
	Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t18, t19, t20, t21, t22, t23, t24, t25, t17;
	Texture t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38, t39, t40, t41, t42, t43, t44, t45, t46, t47;
	Texture t48, t49, t50, t51, t52;
	Texture t53, t54, t55, t56, t57, t58, t59, t60, t61, t62, t63, t64, t65, t66, t67, t68, t69, t70, t71, t72, t73,t74;

	t1.loadFromFile("../Assets/images/2_of_clubs.png");
	t2.loadFromFile("../Assets/images/2_of_diamonds.png");
	t3.loadFromFile("../Assets/images/2_of_hearts.png");
	t4.loadFromFile("../Assets/images/2_of_spades.png");

	t5.loadFromFile("../Assets/images/3_of_clubs.png");
	t6.loadFromFile("../Assets/images/3_of_diamonds.png");
	t7.loadFromFile("../Assets/images/3_of_hearts.png");
	t8.loadFromFile("../Assets/images/3_of_spades.png");

	t9.loadFromFile("../Assets/images/4_of_clubs.png");
	t10.loadFromFile("../Assets/images/4_of_diamonds.png");
	t11.loadFromFile("../Assets/images/4_of_hearts.png");
	t12.loadFromFile("../Assets/images/4_of_spades.png");

	t13.loadFromFile("../Assets/images/5_of_clubs.png");
	t14.loadFromFile("../Assets/images/5_of_diamonds.png");
	t15.loadFromFile("../Assets/images/5_of_hearts.png");
	t16.loadFromFile("../Assets/images/5_of_spades.png");

	t17.loadFromFile("../Assets/images/6_of_clubs.png");
	t18.loadFromFile("../Assets/images/6_of_diamonds.png");
	t19.loadFromFile("../Assets/images/6_of_hearts.png");
	t20.loadFromFile("../Assets/images/6_of_spades.png");

	t21.loadFromFile("../Assets/images/7_of_clubs.png");
	t22.loadFromFile("../Assets/images/7_of_diamonds.png");
	t23.loadFromFile("../Assets/images/7_of_hearts.png");
	t24.loadFromFile("../Assets/images/7_of_spades.png");

	t25.loadFromFile("../Assets/images/8_of_clubs.png");
	t26.loadFromFile("../Assets/images/8_of_diamonds.png");
	t27.loadFromFile("../Assets/images/8_of_hearts.png");
	t28.loadFromFile("../Assets/images/8_of_spades.png");

	t29.loadFromFile("../Assets/images/9_of_clubs.png");
	t30.loadFromFile("../Assets/images/9_of_diamonds.png");
	t31.loadFromFile("../Assets/images/9_of_hearts.png");
	t32.loadFromFile("../Assets/images/9_of_spades.png");

	t33.loadFromFile("../Assets/images/10_of_clubs.png");
	t34.loadFromFile("../Assets/images/10_of_diamonds.png");
	t35.loadFromFile("../Assets/images/10_of_hearts.png");
	t36.loadFromFile("../Assets/images/10_of_spades.png");

	t37.loadFromFile("../Assets/images/ace_of_clubs.png");
	t38.loadFromFile("../Assets/images/ace_of_diamonds.png");
	t39.loadFromFile("../Assets/images/ace_of_hearts.png");
	t40.loadFromFile("../Assets/images/ace_of_spades.png");

	t41.loadFromFile("../Assets/images/jack_of_clubs.png");
	t42.loadFromFile("../Assets/images/jack_of_diamonds.png");
	t43.loadFromFile("../Assets/images/jack_of_hearts.png");
	t44.loadFromFile("../Assets/images/jack_of_spades.png");

	t45.loadFromFile("../Assets/images/king_of_clubs.png");
	t46.loadFromFile("../Assets/images/king_of_diamonds.png");
	t47.loadFromFile("../Assets/images/king_of_hearts.png");
	t48.loadFromFile("../Assets/images/king_of_spades.png");

	t49.loadFromFile("../Assets/images/queen_of_clubs.png");
	t50.loadFromFile("../Assets/images/queen_of_diamonds.png");
	t51.loadFromFile("../Assets/images/queen_of_hearts.png");
	t52.loadFromFile("../Assets/images/queen_of_spades.png");

	t53.loadFromFile("../Assets/images/min.png");
	t54.loadFromFile("../Assets/images/1.png");
	t55.loadFromFile("../Assets/images/10.png");
	t56.loadFromFile("../Assets/images/100.png");
	t57.loadFromFile("../Assets/images/1000.png");
	t58.loadFromFile("../Assets/images/max.png");

	t59.loadFromFile("../Assets/images/double.png");
	t60.loadFromFile("../Assets/images/hit.png");
	t61.loadFromFile("../Assets/images/stand.png");
	t62.loadFromFile("../Assets/images/start.png");

	t63.loadFromFile("../Assets/images/cardBack.png");
	t64.loadFromFile("../Assets/images/blackjack-classic-background1.jpg");
	t65.loadFromFile("../Assets/images/blackjack-classic-background2.jpg");

	t66.loadFromFile("../Assets/images/Lose.jpg");
	t67.loadFromFile("../Assets/images/Win.jpg");
	t68.loadFromFile("../Assets/images/Tie.jpg");
	t69.loadFromFile("../Assets/images/Exit1.png");
	t70.loadFromFile("../Assets/images/Exit2.png");

	t71.loadFromFile("../Assets/images/Chancewin.png");
	t72.loadFromFile("../Assets/images/zero.png");
	t73.loadFromFile("../Assets/images/fifty.png");
	t74.loadFromFile("../Assets/images/hundred.png");

	Sprite two_of_clubs(t1);
	Sprite two_of_diamonds(t2);
	Sprite two_of_hearts(t3);
	Sprite two_of_spades(t4);

	Sprite three_of_clubs(t5);
	Sprite three_of_diamonds(t6);
	Sprite three_of_hearts(t7);
	Sprite three_of_spades(t8);

	Sprite four_of_clubs(t9);
	Sprite four_of_diamonds(t10);
	Sprite four_of_hearts(t11);
	Sprite four_of_spades(t12);

	Sprite five_of_clubs(t13);
	Sprite five_of_diamonds(t14);
	Sprite five_of_hearts(t15);
	Sprite five_of_spades(t16);

	Sprite six_of_clubs(t17);
	Sprite six_of_diamonds(t18);
	Sprite six_of_hearts(t19);
	Sprite six_of_spades(t20);

	Sprite seven_of_clubs(t21);
	Sprite seven_of_diamonds(t22);
	Sprite seven_of_hearts(t23);
	Sprite seven_of_spades(t24);

	Sprite eight_of_clubs(t25);
	Sprite eight_of_diamonds(t26);
	Sprite eight_of_hearts(t27);
	Sprite eight_of_spades(t28);

	Sprite nine_of_clubs(t29);
	Sprite nine_of_diamonds(t30);
	Sprite nine_of_hearts(t31);
	Sprite nine_of_spades(t32);

	Sprite ten_of_clubs(t33);
	Sprite ten_of_diamonds(t34);
	Sprite ten_of_hearts(t35);
	Sprite ten_of_spades(t36);

	Sprite ace_of_clubs(t37);
	Sprite ace_of_diamonds(t38);
	Sprite ace_of_hearts(t39);
	Sprite ace_of_spades(t40);

	Sprite jack_of_clubs(t41);
	Sprite jack_of_diamonds(t42);
	Sprite jack_of_hearts(t43);
	Sprite jack_of_spades(t44);

	Sprite king_of_clubs(t45);
	Sprite king_of_diamonds(t46);
	Sprite king_of_hearts(t47);
	Sprite king_of_spades(t48);

	Sprite queen_of_clubs(t49);
	Sprite queen_of_diamonds(t50);
	Sprite queen_of_hearts(t51);
	Sprite queen_of_spades(t52);

	Sprite betmin(t53);
	Sprite bet1(t54);
	Sprite bet10(t55);
	Sprite bet100(t56);
	Sprite bet1000(t57);
	Sprite betmax(t58);

	Sprite doubleButton(t59);
	Sprite hitButton(t60);
	Sprite standButton(t61);
	Sprite startButton(t62);

	Sprite cardBack(t63);
	Sprite background1(t64);
	Sprite background2(t65);

	Sprite lose(t66);
	Sprite win(t67);
	Sprite tie(t68);
	Sprite exit1(t69);
	Sprite exit2(t70);

	Sprite chancewin(t71);
	Sprite zero(t72);
	Sprite fifty(t73);
	Sprite hundred(t74);
	Sprite cardd;

	lose.setPosition(2000, 1200);
	win.setPosition(2000, 1200);
	tie.setPosition(2000, 1200);

	while (window.isOpen())
	{	
		if (!gameFinish) {
			window.setFramerateLimit(60);
			Event e;
			while (window.pollEvent(e))
			{
				if (e.type == Event::Closed)
				{
					window.close();
				}
				if (e.type == Event::TextEntered)
				{
					bet.input(e, betBarEnable);
				}
				if (e.type == Event::MouseButtonPressed)
				{
					if (e.key.code == Mouse::Left)
					{
						if (IntRect(30, 990, 300, 50).contains(Mouse::getPosition(window))) { exit2.setColor(Color::Black); gameNum = 1; }
						{
							if (gameNum == 1) { exit(0); }

						}
						float x = Mouse::getPosition(window).x;
						float y = Mouse::getPosition(window).y;
						// start button 
						if (x >= 870 && y >= 550 && x <= 1050 && y <= 610 && bet.betValue > 0 && betPlaced == false)
						{
							betBarEnable = false;
							betPlaced = true;
						}
						// hit button
						else if (x >= 1500 && y >= 1000 && x <= 1680 && y <= 1060 && betBarEnable == false)
						{
							
							if (betPlaced == true && playerCards < 21 && playerNrOfCards < 5)
							{
								playerNrOfCards++;
								int card = randomgenerator();
								if (card > 9)
								{
									playerCards += 10;
								}
								else
								{
									playerCards += card;
								}
								player[playerNrOfCards].value = card;
								card = randomColor();
								player[playerNrOfCards].color = card;
							}
							if (playerCards > 21) {
								zero.setPosition(870, 1000);
							}

						}
						// stand button
						else if (x >= 1710 && y >= 1000 && x <= 1890 && y <= 1060 && betBarEnable == false)
						{
							while (dealerCards < 17)
							{
								dealerNrOfCards++;
								int card = randomgenerator();
								if (card > 9)
								{
									dealerCards += 10;
								}
								else
								{
									dealerCards += card;
								}
								dealer[dealerNrOfCards].value = card;
								card = randomColor();
								dealer[dealerNrOfCards].color = card;

							}
							
							if (playerCards > 21 || dealerCards > playerCards && dealerCards <= 21)
							{
								cout << "Lose\n";
								cout << "Player card=" << playerCards << " Dealer cards=" << dealerCards << "\n";
								lose.setPosition(651, 345);
								zero.setPosition(870, 1000);
								gameFinish = true;

							}
							else if (playerCards > dealerCards && playerCards <= 21 || dealerCards > 21)
							{
								cout << "Win\n";
								cout << "Player card=" << playerCards << " Dealer cards=" << dealerCards << "\n";
								win.setPosition(651, 345);
								hundred.setPosition(870, 1000);
								balance += 2 * bet.betValue;
								gameFinish = true;
							}
							else
							{
								cout << "Tie\n";
								cout << "Player card=" << playerCards << " Dealer cards=" << dealerCards << "\n";
								tie.setPosition(651, 345);
								fifty.setPosition(870, 1000);
								balance += bet.betValue;
								gameFinish = true;

							}
						}
						// double button
						else if (x >= 1290 && y >= 1000 && y <= 1060 && x <= 1470 && betPlaced == true && playerCards < 21)
						{
							playerNrOfCards++;
							int card = randomgenerator();
							if (card > 9)
							{
								playerCards += 10;
							}
							else
							{
								playerCards += card;
							}
							player[playerNrOfCards].value = card;
							card = randomColor();
							player[playerNrOfCards].color = card;
							while (dealerCards < 17)
							{
								dealerNrOfCards++;
								int card = randomgenerator();
								if (card > 9)
								{
									dealerCards += 10;
								}
								else
								{
									dealerCards += card;
								}
								dealer[dealerNrOfCards].value = card;
								card = randomColor();
								dealer[dealerNrOfCards].color = card;
							}
							bet.betValue *= 2;
							balance -= bet.betValue;
							if (playerCards > 21 || dealerCards > playerCards && dealerCards <= 21)
							{
								cout << "Lose\n";
								cout << "Player card=" << playerCards << " Dealer cards=" << dealerCards << "\n";
								lose.setPosition(651, 345);
								zero.setPosition(870, 1000);
								gameFinish = true;
							}
							else if (playerCards > dealerCards && playerCards <= 21 || dealerCards > 21)
							{
								cout << "Win\n";
								cout << "Player card=" << playerCards << " Dealer cards=" << dealerCards << "\n";
								win.setPosition(651, 345);
								hundred.setPosition(870, 1000);
								balance += 2 * bet.betValue;
								gameFinish = true;
							}
							else
							{
								cout << "Tie\n";
								cout << "Player card=" << playerCards << " Dealer cards=" << dealerCards << "\n";
								tie.setPosition(651, 345);
								fifty.setPosition(870, 1000);
								balance += bet.betValue;
								gameFinish = true;

							}
						}
						if (ready == 1 && playerCards < 22) {
							chance = (playerCards + dealerCards);
							realChance = (100 * playerCards) / chance;
						}
						// bet min
						if (x >= 630 && y >= 845 && x <= 680 && y <= 895 && betBarEnable == true && bet.betValue > 0)
						{
							balance += bet.betValue;
							bet.betValue = 0;
							bet.addButton(0);
						}
						// bet 1
						if (x >= 752 && y >= 865 && x <= 802 && y <= 915 && betBarEnable == true && balance > 0 && balance >= 1)
						{
							balance -= 1;
							bet.addButton(1);
						}
						// bet 10
						if (x >= 874 && y >= 870 && x <= 924 && y <= 920 && betBarEnable == true && balance > 0 && balance >= 10)
						{
							balance -= 10;
							bet.addButton(10);
						}
						// bet 100
						if (x >= 996 && y >= 870 && x <= 1046 && y <= 920 && betBarEnable == true && balance > 0 && balance >= 100)
						{
							balance -= 100;
							bet.addButton(100);
						}
						// bet 1000
						if (x >= 1118 && y >= 865 && x <= 1168 && y <= 915 && betBarEnable == true && balance > 0 && balance >= 1000)
						{
							balance -= 1000;
							bet.addButton(1000);
						}
						// bet max
						if (x >= 1240 && y >= 845 && x <= 1290 && y <= 895 && betBarEnable == true && balance > 0)
						{
							balance += bet.betValue;
							bet.betValue = 0;
							bet.addButton(balance);
							balance = 0;
						}
					}
				}
			}
			window.clear(Color::Green);
			if (betPlaced == false)
			{
				window.draw(background1);
				startButton.setPosition(870, 550);
				window.draw(startButton);
				chanceText.setPosition(1200, 2000);
				zero.setPosition(2000, 1200);
				hundred.setPosition(2000, 1200);
				fifty.setPosition(2000, 1200);
				widthGoal = 0;
				widthReal = 0;
				realChance = 0;
			}
			else
			{
				window.draw(background2);
				chancewin.setPosition(870, 1000);
				window.draw(chancewin);
				doubleButton.setPosition(1290, 1000);
				window.draw(doubleButton);
				hitButton.setPosition(1500, 1000);
				window.draw(hitButton);
				standButton.setPosition(1710, 1000);
				window.draw(standButton);
				chanceText.setPosition(935, 1000);

			}
			exit2.setPosition(30, 1000);
			window.draw(exit2);
			betmin.setPosition(630, 845);
			window.draw(betmin);
			bet1.setPosition(752, 865);
			window.draw(bet1);
			bet10.setPosition(874, 870);
			window.draw(bet10);
			bet100.setPosition(996, 870);
			window.draw(bet100);
			bet1000.setPosition(1118, 865);
			window.draw(bet1000);
			betmax.setPosition(1240, 845);
			window.draw(betmax);
			if (first == true && betPlaced == true)
			{
				playerNrOfCards++;
				int card = randomgenerator();
				if (card > 9)
				{
					playerCards += 10;
				}
				else
				{
					playerCards += card;
				}
				player[playerNrOfCards].value = card;
				card = randomColor();
				player[playerNrOfCards].color = card;
				playerNrOfCards++;
				card = randomgenerator();
				if (card > 9)
				{
					playerCards += 10;
				}
				else
				{
					playerCards += card;
				}
				player[playerNrOfCards].value = card;
				card = randomColor();
				player[playerNrOfCards].color = card;
				dealerNrOfCards++;
				card = randomgenerator();
				if (card > 9)
				{
					dealerCards += 10;
				}
				else
				{
					dealerCards += card;
				}
				dealer[dealerNrOfCards].value = card;
				card = randomColor();
				dealer[dealerNrOfCards].color = card;
				dealerNrOfCards++;
				card = randomgenerator();
				if (card > 9)
				{
					dealerCards += 10;
				}
				else
				{
					dealerCards += card;
				}
				dealer[dealerNrOfCards].value = card;
				card = randomColor();
				dealer[dealerNrOfCards].color = card;
	
				chance = (playerCards + dealerCards);
				realChance = (100 * playerCards) / chance;

				ready = 1;
				first = false;
			}
			string b;
			b = "Balance: " + to_string(balance);
			Text balanceText;
			Font font;
			font.loadFromFile("../Assets/fonts/Arial.ttf");
			balanceText.setFont(font);
			balanceText.setOutlineColor(Color::Black);
			balanceText.setOutlineThickness(2);
			balanceText.setString(b);
			balanceText.setPosition(30, 30);
			balanceText.setCharacterSize(45);
			window.draw(balanceText);
			int position = 480;
			for (int i = 1; i <= playerNrOfCards; i++)
			{
				if (player[i].value == 1 || player[i].value == 11)
				{
					if (player[i].color == 1)
					{
						cardd = ace_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = ace_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = ace_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = ace_of_spades;
					}
				}
				if (player[i].value == 2)
				{
					if (player[i].color == 1)
					{
						cardd = two_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = two_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = two_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = two_of_spades;
					}
				}
				if (player[i].value == 3)
				{
					if (player[i].color == 1)
					{
						cardd = three_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = three_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = three_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = three_of_spades;
					}
				}
				if (player[i].value == 4)
				{
					if (player[i].color == 1)
					{
						cardd = four_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = four_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = four_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = four_of_spades;
					}
				}
				if (player[i].value == 5)
				{
					if (player[i].color == 1)
					{
						cardd = five_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = five_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = five_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = five_of_spades;
					}
				}
				if (player[i].value == 6)
				{
					if (player[i].color == 1)
					{
						cardd = six_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = six_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = six_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = six_of_spades;
					}
				}
				if (player[i].value == 7)
				{
					if (player[i].color == 1)
					{
						cardd = seven_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = seven_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = seven_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = seven_of_spades;
					}
				}
				if (player[i].value == 8)
				{
					if (player[i].color == 1)
					{
						cardd = eight_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = eight_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = eight_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = eight_of_spades;
					}
				}
				if (player[i].value == 9)
				{
					if (player[i].color == 1)
					{
						cardd = nine_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = nine_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = nine_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = nine_of_spades;
					}
				}
				if (player[i].value == 10)
				{
					if (player[i].color == 1)
					{
						cardd = ten_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = ten_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = ten_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = ten_of_spades;
					}
				}
				if (player[i].value == 12)
				{
					if (player[i].color == 1)
					{
						cardd = jack_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = jack_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = jack_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = jack_of_spades;
					}
				}
				if (player[i].value == 13)
				{
					if (player[i].color == 1)
					{
						cardd = queen_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = queen_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = queen_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = queen_of_spades;
					}
				}
				if (player[i].value == 14)
				{
					if (player[i].color == 1)
					{
						cardd = king_of_clubs;
					}
					if (player[i].color == 2)
					{
						cardd = king_of_diamonds;
					}
					if (player[i].color == 3)
					{
						cardd = king_of_hearts;
					}
					if (player[i].color == 4)
					{
						cardd = king_of_spades;
					}
				}
				cardd.setPosition(position, 450);
				position += 160;
				window.draw(cardd);
			}
			position = 480;
			for (int i = 1; i <= dealerNrOfCards; i++)
			{
				if (i == 2 && gameFinish == false)
				{
					cardd = cardBack;
				}
				else if (i != 2 || gameFinish == true)
				{
					if (dealer[i].value == 1 || dealer[i].value == 11)
					{
						if (dealer[i].color == 1)
						{
							cardd = ace_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = ace_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = ace_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = ace_of_spades;
						}
					}
					if (dealer[i].value == 2)
					{
						if (dealer[i].color == 1)
						{
							cardd = two_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = two_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = two_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = two_of_spades;
						}
					}
					if (dealer[i].value == 3)
					{
						if (dealer[i].color == 1)
						{
							cardd = three_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = three_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = three_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = three_of_spades;
						}
					}
					if (dealer[i].value == 4)
					{
						if (dealer[i].color == 1)
						{
							cardd = four_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = four_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = four_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = four_of_spades;
						}
					}
					if (dealer[i].value == 5)
					{
						if (dealer[i].color == 1)
						{
							cardd = five_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = five_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = five_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = five_of_spades;
						}
					}
					if (dealer[i].value == 6)
					{
						if (dealer[i].color == 1)
						{
							cardd = six_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = six_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = six_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = six_of_spades;
						}
					}
					if (dealer[i].value == 7)
					{
						if (dealer[i].color == 1)
						{
							cardd = seven_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = seven_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = seven_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = seven_of_spades;
						}
					}
					if (dealer[i].value == 8)
					{
						if (dealer[i].color == 1)
						{
							cardd = eight_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = eight_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = eight_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = eight_of_spades;
						}
					}
					if (dealer[i].value == 9)
					{
						if (dealer[i].color == 1)
						{
							cardd = nine_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = nine_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = nine_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = nine_of_spades;
						}
					}
					if (dealer[i].value == 10)
					{
						if (dealer[i].color == 1)
						{
							cardd = ten_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = ten_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = ten_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = ten_of_spades;
						}
					}
					if (dealer[i].value == 12)
					{
						if (dealer[i].color == 1)
						{
							cardd = jack_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = jack_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = jack_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = jack_of_spades;
						}
					}
					if (dealer[i].value == 13)
					{
						if (dealer[i].color == 1)
						{
							cardd = queen_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = queen_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = queen_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = queen_of_spades;
						}
					}
					if (dealer[i].value == 14)
					{
						if (dealer[i].color == 1)
						{
							cardd = king_of_clubs;
						}
						if (dealer[i].color == 2)
						{
							cardd = king_of_diamonds;
						}
						if (dealer[i].color == 3)
						{
							cardd = king_of_hearts;
						}
						if (dealer[i].color == 4)
						{
							cardd = king_of_spades;
						}
					}
				}
				cardd.setPosition(position, 100);
				position += 160;
				window.draw(cardd);
			}
			widthGoal = (164.f / 100.f) * realChance;
			int widthGuy = widthGoal;
			if (widthReal < widthGuy) {
				widthReal += 1;
			}
			else if (widthReal > widthGuy) {
				widthReal -= 1;
			}
			bareq.setSize(sf::Vector2f(widthReal, 60.f));
			bareq.setFillColor(sf::Color::Red);
			bareq.setPosition(878, 1000);
			window.draw(bareq);
			c = to_string((int)(realChance));
			chanceText.setFont(font);
			chanceText.setOutlineColor(Color::Black);
			chanceText.setOutlineThickness(2);
			chanceText.setString(c);
			chanceText.setCharacterSize(45);
			window.draw(chanceText);
			window.draw(lose);
			window.draw(win);
			window.draw(tie);
			window.draw(zero);
			window.draw(fifty);
			window.draw(hundred);
			bet.drawBetBar(window);
			window.display();
		
		} else {
			lose.setPosition(2000, 1200);
			zero.setPosition(2000, 1200);
			win.setPosition(2000, 1200);
			hundred.setPosition(2000, 1200);
			tie.setPosition(2000, 1200);
			fifty.setPosition(2000, 1200);
			bareq.setPosition(2000, 1200);
			bet.betValue = 0;
			sleep(seconds(5));
			chance = 0.f;
			ready = 0;
			betBarEnable = true;
			betPlaced = false;
			playerCards = 0;
			playerNrOfCards = 0;
			dealerCards = 0;
			dealerNrOfCards = 0;
			first = true;
			chanceText.setPosition(1200, 2000);
			gameFinish = false;
		}
	}

	return 0;
}