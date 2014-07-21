#include<vector>
#include<algorithm>
#include<iostream>

using namespace std;

enum Suite_t {HEARTS, CLUBS, DIAMONDS, SPADES};

class Card {
	private:
		int value;
		Suite_t suite;
	public:
		Card() {}
		Card(int _value, Suite_t _suite) : value(_value), suite(_suite) {}

		int getValue(void);
		void printCard(void);
};

class Player {
	private:
		vector<Card> hand;
		int handSum;
	public:
		Player(void) : handSum(0) {}
		void addCard(Card nextCard);
		int getHandSum(void);
		void reset(void);

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

/*

class Table {
	private:
		Player userPlayer;
		Player dealerPlayer;
		Deck tableDeck;
	public:
		Table(void);
		void startGame(void);
}; */ 

