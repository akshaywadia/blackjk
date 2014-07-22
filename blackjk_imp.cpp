#include"blackjk.h"

int Card::getValue(void)
{
	return value;
}

void Card::printCard(void)
{
	if (value == 1)
		cout << 'A';
	else 
		cout << value;
	cout << suite << " ";
}

void Player::addCard(Deck &currentDeck, int numberOfCards=1)
{
	Card nextCard;
	for (int i=0;i<numberOfCards;++i) {
		nextCard = currentDeck.drawNextCard();
		hand.push_back(nextCard);
		handSum += nextCard.getValue();
	}
}

int Player::getHandSum(void)
{
	return handSum;
}

void Player::resetPlayer(void)
{
	hand = vector<Card>();
	handSum = 0;
}

void Player::printHand(bool dealer = false)
{
	if (!dealer) {
		cout << "\tYour Hand:\t\t";
		for (int i=0;i<hand.size();++i)
			hand[i].printCard();
	}
	else {
		cout << "\tDealer's Hand:\t\t" ;
		hand[0].printCard();
		cout 	<< "??" << endl;
	}
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
	resetDeck();
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
	++nextCardIndex;	
	return currentDeck[nextCardIndex-1];
}

void Deck::printDeck(void)
{
	for (int i=0;i<52;++i) 
		currentDeck[i].printCard();
}

Table::Table()
{
	clearScreen();
	tableDeck = Deck();
	userPlayer = Player();
	dealerPlayer = Player();
	//Welcome message.
	cout << "Welcome to Blackjack!" << endl << endl;
}

void Table::clearScreen(void)
{
	cout << string(100, '\n');
}

void Table::initialScreen(void)
{
	cout << "Press `d\' to deal new hand: " ;
}

void Table::displayHands(bool gameEnded = false)
{
	if (gameEnded) 
		dealerPlayer.printHand();
	else
		dealerPlayer.printHand(true);
	userPlayer.printHand();
	cout << endl << "\t    Your Sum:\t\t\t" << userPlayer.getHandSum() << endl;
}

void Table::startGame(void)
{
	initialScreen();
	char action = getNextAction();
	cout << endl;
	while (action != QUIT) {
		switch (action) {
			case DEAL: userPlayer.addCard(tableDeck, 2);
				   dealerPlayer.addCard(tableDeck, 2);
				   displayHands();
				   break;
			case HIT: userPlayer.addCard(tableDeck);
				  displayHands();
				  break;
			default: cout << endl << "Enter valid choice ('?' for help): " ;
				 break;

		}
		cout << endl << "Enter next command (? for help): " ;
		action = getNextAction();
		cout << endl;
	}
}

char Table::getNextAction(void)
{
	char action;
	cin >> action;
	if (action < 'a')
		action += 'a'-'A';
	return action;
}
