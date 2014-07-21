#include"blackjk.h"

int Card::getValue(void)
{
	return value;
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

Deck::Deck(void)
{
	nextCardIndex = 0;
	currentDeck = vector<Card>(52);
	for (int i=0;i<13;i=i+4) {
		currentDeck[i] = Card(i, HEARTS);
		currentDeck[i+1] = Card(i+1, CLUBS);
		currentDeck[i+2] = Card(i+2, DIAMONDS);
	       	currentDeck[i+3] = Card(i+3, SPADES);
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
