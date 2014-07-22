#include"blackjk.h"

int main()
{
	// testing Suite_t
	/*	Suite_t suite = SPADES;
	cout << suite << endl; */

	// testing card
	/*Card card(2, CLUBS);
	cout << card.getValue() << endl;*/

	// test Player
	/*
	Player pl;
	pl.addCard(Card(2,CLUBS));
	pl.addCard(Card(7,DIAMONDS));
	cout << pl.getHandSum() << endl;
	pl.reset();
	pl.addCard(Card(12, HEARTS));
	cout << pl.getHandSum() << endl; */

	// test Deck
	/* Deck dk;
	dk.printDeck();
	cout << endl;
	dk.resetDeck();
	dk.printDeck();
	cout << endl << endl;
	for (int i=0;i<5;++i) 
		dk.drawNextCard().printCard(); */

	//test Table;
	//
	

	//test Table
	Table currentTable = Table();
	currentTable.startGame();
	
	return 0;
}
