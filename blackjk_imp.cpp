#include"blackjk.h"

int Card::getValue(void)
{
	return value;
}

void Card::printCard(void)
{
	cout << "(" <<  value << "," << suite << ") " ;
}

void Player::addCard(Card nextCard)
{
	hand.push_back(nextCard);
	handSum += nextCard.getValue();
}

int Player::getHandSum(void)
{
	return handSum;
}

void Player::reset(void)
{
	hand = vector<Card>();
	handSum = 0;
}



Deck::Deck(void)
{
	nextCardIndex = 0;
	currentDeck = vector<Card>(52);
	for (int i=1;i<=13;++i) {
		currentDeck[i-1] = Card(i, HEARTS);
		currentDeck[13+i-1] = Card(i, CLUBS);
		currentDeck[2*13+i-1] = Card(i, DIAMONDS);
	       	currentDeck[3*13+i-1] = Card(i, SPADES);
	}
}

void Deck::resetDeck(void)
{
	random_shuffle(currentDeck.begin(), currentDeck.end());
	nextCardIndex = 0;
}

Card Deck::drawNextCard(void)
{
	if (nextCardIndex == 52) {
		// io `new deck' message
		resetDeck();
	}	
	return currentDeck[nextCardIndex++];
}

void Deck::printDeck(void)
{
	for (int i=0;i<52;++i) 
		currentDeck[i].printCard();
}



