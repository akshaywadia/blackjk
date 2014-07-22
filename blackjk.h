#include<vector>
#include<string>
#include<algorithm>
#include<iostream>
#include<string>
#include<map>

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


/********************* 
 * UI and IO Functions
 */
void clearScreen(void);
void initialScreen(void);
char getNextAction(void);
void displayHelp(vector<char> allowedActions);
void displayPrompt(void);


/******************** 
 * Card class.
 */
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

/********************
 * Deck class.
 */

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

/********************
 * Player class.
 */

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




/********************
 * Game state classes.
 */
class gameState {
	public:
		string stateName;
		vector<char> allowedActions;
		static Player userPlayer;
		static Player dealerPlayer;
		static Deck gameDeck;

		gameState() {}

		string getStateName(void) {return stateName;}

		// Game state transition function. 
		virtual void exec(char input) {}
		virtual gameState * transition(char input) {}
		void displayHands(bool gameEnded);
};



class gameStateInitial : public gameState {
	public:
		gameStateInitial();
		void exec(char input);
		gameState * transition(char input);
};

class gameStatePlayer : public gameState {
	public:
		gameStatePlayer();
		void exec(char input);
		gameState * transition(char input);
};

class gameStateDealer : public gameState {
	public:
		gameStateDealer();
		void exec(char input);
		gameState * transition(char input);
};


class gameStateQuit : public gameState {
	public:
		gameStateQuit();
		void exec(char input) {}
		gameState * transition(char input) {}
};

/********************
 * Game class.
 */
class Game {
	public:
		Game() {}
		// UI functions
		void playGame(void);

};  
