#include<vector>
#include<algorithm>

using namespace std;

enum Suite_t {HEARTS, CLUBS, DIAMONDS, SPADES};

class Card {
	public:
		// Three of clubs will have `value' 3 and `suite' 'C'
		int value;
		Suite_t suite;

		Card(int _value, Suite_t _suite) : value(_value), suite(_suite) {}

		// get value
		int getValue(void);
};

class Player {
	private:
		vector<Card> hand;
		int handSum;
	public:
		Player(void) { handSum = 0;}
		void addCard(Card nextCard);
		int getHandSum(void);

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
};

class Table {
	private:
		Player userPlayer;
		Player dealerPlayer;
		Deck tableDeck;
	public:
		Table(void);
		startGame();
};
