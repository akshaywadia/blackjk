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
	if (action < 'a' && action != '?')
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
	switch (value) {
		case 1: cout << 'A';
			break;
		case 11: cout << 'J';
			 break;
		case 12: cout << 'Q';
			 break;
		case 13: cout << 'K';
			 break;
		default: cout << value;
			 break;
	}
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
		if (nextCard.getValue() < 10)
			handSum += nextCard.getValue();
		else
			handSum += 10;
	}
}

int Player::getHandSum(void)
{
	return handSum;
}

int Player::getBestHandSum(void)
{
	int lowSum = handSum;
	int noOfAces = 0;
	for (int i=0;i<hand.size();++i)
		if (hand[i].getValue() == 1)
			++noOfAces;
	while (lowSum + 10 <= 21 && noOfAces > 0) {
		lowSum += 10;
		--noOfAces;
	}
	return lowSum;
}

bool Player::bust(void)
{
	return (handSum > 21);
}

void Player::resetPlayer(void)
{
	hand = vector<Card>();
	handSum = 0;
}

void Player::printHand(bool dealer = false, bool shadow = true)
{
	if (!dealer) {
		cout << "\tYour Hand:\t\t";
		for (int i=0;i<hand.size();++i)
			hand[i].printCard();
	}
	else {
		cout << "\tDealer's Hand:\t\t" ;
		hand[0].printCard();
		if (shadow)
			cout 	<< "??" << endl;
		else {
			for (int i=1;i<hand.size();++i)
				hand[i].printCard();
			cout << endl;
		}
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

int randomGen(int i)
{
	srand(time(NULL));
	return rand()%i;
}

void Deck::resetDeck(void)
{
	random_shuffle(currentDeck.begin(), currentDeck.end(), randomGen);
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
		dealerPlayer.printHand(true, false);
	else
		dealerPlayer.printHand(true, true);
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
	userPlayer.resetPlayer();
	dealerPlayer.resetPlayer();
	switch (action) {
		case '?': displayHelp(allowedActions);
			  break;
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
		case '?': displayHelp(allowedActions);
			  break;
		case HIT: userPlayer.addCard(gameDeck);
			  displayHands();
			  if (userPlayer.bust())
				  cout << "Bust!!" << endl;
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
		case HIT:   if (userPlayer.bust())
				    return new gameStateInitial();
			    else
				    return new gameStatePlayer();
			    break;
		case STAND: return new gameStateDealer();
			    break;
		case QUIT:  return new gameStateQuit();
			    break;
		default:    return new gameStatePlayer();
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
	// dealer's strategy
	while (dealerPlayer.getBestHandSum() < 17)
		dealerPlayer.addCard(gameDeck);

	displayHands(true);
	// check who won
	if (dealerPlayer.getBestHandSum() > userPlayer.getBestHandSum() && dealerPlayer.getBestHandSum() <= 21) 
		cout << "Dealer wins!!" << endl;
	else
		cout << "You win!!" << endl;
	displayPrompt();
}

gameState * gameStateDealer::transition(char action)
{
	return new gameStateInitial();
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
	gameState *temp;
	char action;
	do {
		if (currentState->getStateName() == "Dealer State")
			action = '\0';
		else
			action = getNextAction();
		currentState->exec(action);
		temp = currentState;
		currentState = currentState->transition(action);
		delete temp;

	} while (currentState->getStateName() != "Quit State");
}
