#include"blackjk.h"

// Initialize static members
Player gameState::userPlayer = Player();
Player gameState::dealerPlayer = Player();
Deck gameState::gameDeck = Deck();

/********************* 
 * UI and IO Functions
 */
char getNextAction(void)
{
	char action;
	cin >> action;
	if (action < 'a')
		action += 'a'-'A';
	return action;
}

void clearScreen(void)
{
	cout << string(100, '\n');
}

void initialScreen(void)
{
	cout << "Welcome to Blackjack!" << endl;
	cout << "Press `d\' to deal new hand: " ;
}

void displayHelp(vector<char> allowedActions)
{
	// initialize help messages.
	map<char,string> actionDescription;
	actionDescription[QUIT] = "Quit game.";
	actionDescription[HIT] = "Hit. Draw another card from dealer.";
	actionDescription[DEAL] = "Deal a new game.";
	actionDescription[STAND] = "Stand your hand.";

	vector<char>::iterator it;
	for (it = allowedActions.begin();it != allowedActions.end(); ++it)
		cout << "\t\t" << *it << "   :   " << actionDescription[*it] << endl;
	cout << "Enter next action: ";

}

void displayPrompt(void)
{
	cout << "Enter action ('?' for help) : " ;
}

/********************
 * Card class.
 */
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

/********************
 * Player class.
 */

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

/********************
 * Deck class.
 */
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

/********************
 * Game state classes. 
 */
void gameState::displayHands(bool gameEnded = false)
{
	if (gameEnded) 
		dealerPlayer.printHand();
	else
		dealerPlayer.printHand(true);
	userPlayer.printHand();
	cout << endl << "\t    Your Sum:\t\t\t" << userPlayer.getHandSum() << endl;
}

// Initial state.
gameStateInitial::gameStateInitial()
{
	stateName = "Initial State";
	char temp[] = {DEAL, QUIT};
	allowedActions = vector<char>(temp, temp + sizeof(temp)/sizeof(char));
}

void gameStateInitial::exec(char action)
{
	switch (action) {
		case DEAL: userPlayer.addCard(gameDeck, 2);
			   dealerPlayer.addCard(gameDeck, 2);
			   displayHands();
			   displayPrompt();
			   break;
		case QUIT: break;
		default:   cout << "Invalid input." << endl;
			   displayHelp(allowedActions);
			   break;
	}
}

gameState * gameStateInitial::transition(char action)
{
	switch (action) {
		case DEAL: return new gameStatePlayer();
			   break;
		case QUIT: return new gameStateQuit();
			   break;
		default: return new gameStateInitial();
	}
}

// Player state.
gameStatePlayer::gameStatePlayer()
{
	stateName = "Player State";
	char temp[] = {HIT, STAND, QUIT};
	allowedActions = vector<char>(temp, temp + sizeof(temp)/sizeof(char));
}

void gameStatePlayer::exec(char action)
{
	switch (action) {
		case HIT: userPlayer.addCard(gameDeck);
			  displayHands();
			  displayPrompt();
			  break;
		case STAND: break;
		case QUIT: break;
		default:   cout << "Invalid input." << endl;
			   displayHelp(allowedActions);
			   break;
	}
}

gameState * gameStatePlayer::transition(char action)
{
	switch (action) {
		case HIT: return new gameStatePlayer();
			  break;
		case STAND: return new gameStateDealer();
			    break;
		case QUIT: return new gameStateQuit();
			   break;
		default: return new gameStatePlayer();
			 break;
	}
}

// Dealer state.

gameStateDealer::gameStateDealer()
{
	stateName = "Dealer State";
}

void gameStateDealer::exec(char action)
{
	// dealer strategy
}

gameState * gameStateDealer::transition(char action)
{
}

// Quit state.
gameStateQuit::gameStateQuit()
{
	stateName = "Quit State";
}

/********************
 * Game class.
 */
void Game::playGame(void)
{
	clearScreen();
	initialScreen();
	gameState * currentState = new gameStateInitial;
	char action;
	do {
		if (currentState->getStateName() == "Dealer State")
			action = '\0';
		else
			action = getNextAction();
		currentState->exec(action);
		currentState = currentState->transition(action);

	} while (currentState->getStateName() != "Quit State");
}


/*void Table::startGame(void)
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
} */


