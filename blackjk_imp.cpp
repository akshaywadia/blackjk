#include"blackjk.h"

// Initialize static members
Player gameState::userPlayer = Player();
Player gameState::dealerPlayer = Player();
Deck gameState::gameDeck = Deck();

/********************* 
 * UI and IO Functions
 */

/*
 * Returns next action as specified by the user. Minimal input sanitation done. 
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
	cout << "Enter next action ('?' for help): ";
}

void displayHelp(vector<char> allowedActions)
{
	// initialize help messages.
	map<char,string> actionDescription;
	actionDescription[QUIT] = "Quit game.";
	actionDescription[HIT] = "Hit. Draw another card from dealer.";
	actionDescription[DEAL] = "Deal a new game.";
	actionDescription[STAND] = "Stand your hand.";
	actionDescription[CHIPS] = "Your remaining chips.";
	actionDescription[PRINT] = "Print current hands.";

	vector<char>::iterator it;
	cout << endl;

	// display help for all actions in 'allowedActions'
	for (it = allowedActions.begin();it != allowedActions.end(); ++it)
		cout << "\t\t" << *it << "   :   " << actionDescription[*it] << endl;
	cout << endl;

}

void displayPrompt(void)
{
	cout << "Enter next action ('?' for help): ";
}

/*
 * Prompts and returns user's betting amount, subject to limit set by 'chipsRemaining'. Minimal input sanitation.
 */
int getBet(int chipsRemaining)
{
	int bet = 0;
	cout << "Chips remaining: " << chipsRemaining << endl;
	while (!(bet > 0 && bet <= chipsRemaining)) {
		cout << "Enter bet amount (min 1 chip): " ;
		if (!(cin >> bet)) {
			cin.ignore(100, '\n');
			cin.clear();
		}
	}
	return bet;
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

/*
 * Adds 'numberOfCards' from the 'currentDeck' to userPlayer's hand. Updates 'handSum'.
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

/*
 * Returns the sum of the best hand so far. Invariant: if any ace is used as 10 instead of 1, then bestHandSum exceeds 21.
 */
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

bool Player::blackjack(void)
{
	return (getBestHandSum() == 21);
}

void Player::resetPlayer(void)
{
	hand = vector<Card>();
	handSum = 0;
}

/*
 * Displays header according to value of 'dealer'. Argument 'shadow' determines if hole card is displayed as '??' or open.
 */
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
	// fill up deck sequentially
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

/*
 * For debug only.
 */
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
	cout << endl;
	if (gameEnded) 
		dealerPlayer.printHand(true, false);
	else
		dealerPlayer.printHand(true, true);
	userPlayer.printHand();
	cout << endl << "\t    Your Sum:\t\t\t" << userPlayer.getHandSum() << "  (Best Hand: " << userPlayer.getBestHandSum() << ")" <<  endl << endl;
}

// Initial state.
gameStateInitial::gameStateInitial()
{
	stateName = "Initial State";
	char temp[] = {DEAL, CHIPS, QUIT};
	allowedActions = vector<char>(temp, temp + sizeof(temp)/sizeof(char));
}

void gameStateInitial::exec(char action)
{
	// reset players at start of initial state
	userPlayer.resetPlayer();
	dealerPlayer.resetPlayer();

	switch (action) {
		case '?': displayHelp(allowedActions);
			  break;
		case DEAL: userPlayer.currentBet = getBet(userPlayer.chipsRemaining); 
			   userPlayer.addCard(gameDeck, 2);
			   dealerPlayer.addCard(gameDeck, 2);
			   displayHands();
			   if (userPlayer.blackjack())
				   cout << "Blackjack!!" << endl;
			   break;
		case QUIT: break;
		case CHIPS: cout << "Chips Remaining: " << userPlayer.chipsRemaining << endl;
			    break;
		default:   cout << "*Invalid input.*" << endl;
			   displayHelp(allowedActions);
			   break;
	}
}

gameState * gameStateInitial::transition(char action)
{
	switch (action) {
		case DEAL: if (userPlayer.blackjack())
				   return new gameStateDealer();
			   else 
				   return new gameStatePlayer();
			   break;
		case QUIT: return new gameStateQuit();
			   break;
		default: return new gameStateInitial();
	}
}

void gameStateInitial::printPrompt(void)
{
	cout << "Enter next action ('?' for help): ";
}

// Player state.
gameStatePlayer::gameStatePlayer()
{
	stateName = "Player State";
	char temp[] = {HIT, STAND, CHIPS, PRINT, QUIT};
	allowedActions = vector<char>(temp, temp + sizeof(temp)/sizeof(char));
}

void gameStatePlayer::exec(char action)
{
	switch (action) {
		case '?': displayHelp(allowedActions);
			  break;
		case HIT: userPlayer.addCard(gameDeck);
			  displayHands();
			  if (userPlayer.bust()) {
				  cout << "Bust!!" << endl;
				  userPlayer.chipsRemaining -= userPlayer.currentBet;
			  }
			  else if (userPlayer.blackjack())
				  cout << "Blackjack!!" << endl;
			  //displayPrompt();
			  break;
		case STAND: break;
		case CHIPS: cout << "Chips Remaining: " << userPlayer.chipsRemaining << endl;
			    break;
		case PRINT: displayHands();
			    break;
		case QUIT: userPlayer.chipsRemaining -= userPlayer.currentBet;
			    break;
		default:   cout << "*Invalid input.*" << endl;
			   displayHelp(allowedActions);
			   break;
	}
}

gameState * gameStatePlayer::transition(char action)
{
	switch (action) {
		case HIT:   if (userPlayer.bust()) {
				    if (userPlayer.chipsRemaining > 0)
					    return new gameStateInitial();
				    else
					    return new gameStateQuit();
			    }
			    else if (userPlayer.blackjack())
				    return new gameStateDealer();
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

void gameStatePlayer::printPrompt(void)
{
	cout << "Enter next action ('?' for help): ";
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
	if (dealerPlayer.blackjack() && userPlayer.blackjack()) 
		cout << "Push!!" << endl;
	else if (dealerPlayer.getBestHandSum() > userPlayer.getBestHandSum() && dealerPlayer.getBestHandSum() <= 21) {
		cout << "Dealer wins!!" << endl;
		userPlayer.chipsRemaining -= userPlayer.currentBet;
	}
	else {
		cout << "You win!!" << endl;
		userPlayer.chipsRemaining += userPlayer.currentBet;
	}
	userPlayer.currentBet = 0;
	//displayPrompt();
}

gameState * gameStateDealer::transition(char action)
{
	if (userPlayer.chipsRemaining > 0)
		return new gameStateInitial();
	else
		return new gameStateQuit;
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
	gameState *currentState = new gameStateInitial;
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
		currentState->printPrompt();
		delete temp;

	} while (currentState->getStateName() != "Quit State");
	cout << "Chips Remaining: " << currentState->userPlayer.chipsRemaining << endl;
}
