#include<vector>
#include<algorithm>
#include<iostream>
#include<string>

using namespace std;

// Named constants for suites.
const char HEARTS = 'H';
const char CLUBS = 'C';
const char DIAMONDS = 'D';
const char SPADES = 'S';

// Named constants for user actions.
const char QUIT='q';
const char DEAL='d';
const char HIT='h';
const char STAND='s';

class Card {
	private:
		int value;
		char suite;
	public:
		Card() {}
		Card(int _value, char _suite) : value(_value), suite(_suite) {}

		int getValue(void);
		void printCard(void);
};

class Deck {
	private:
		vector<Card> currentDeck;
		int nextCardIndex;
	public:
		Deck(void);

		// shuffles the deck. Deck should be initialized first, in the constructor.
		void resetDeck(void);
		Card drawNextCard(void);

		// debug
		void printDeck(void);
};

class Player {
	private:
		vector<Card> hand;
		int handSum;
	public:
		Player(void) : handSum(0) {}
		void addCard(Deck &currentDeck, int numberOfCards);
		int getHandSum(void);
		void resetPlayer(void);
		void printHand(bool dealer);

};

//////////////////////////////////////////////////////////////////////////////

class gameState {
	public:
		static Player userPlayer;
		static Player dealerPlayer;
		static Deck currentDeck;
};

class Table {
	private:
		Player userPlayer;
		Player dealerPlayer;
		Deck tableDeck;
	public:
		Table();
		// UI functions
		void startGame(void);
		void clearScreen(void);
		void initialScreen(void);
		void displayHands(bool gameEnded);
		char getNextAction(void);
};  
