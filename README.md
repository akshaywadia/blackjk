blackjk
=======

COMPILING AND RUNNING 
=====================

Simply run 'make', which will output the executable 'blackjk'.

DESCRIPTION OF STATES AND COMMANDS
==================================

The game is modeled as a finite automaton. The commands and player actions available depend on the current state.

1. Initial State (gameStateInitial): Initial state of the game before the hand has been dealt. Actions available:
	q : Quit game.
	d : Deal hand.
	c : Check remaining chips.
	? : Display help.
   Players start with 100 chips. On entering 'd', player is prompted to enter a bet amount. Thereafter, cards are dealt and displayed, and the game moves to state gameStatePlayer.

2. Player state (gameStatePlayer): This is the state where player is playing his hand by deciding to hit or stand. Actions available:
	h : Hit. Draw another card from dealer.
	s : Stand your hand.
	c : Your remaining chips.
	p : Print current hands.
	q : Quit game.
   Whenever player chooses 'h', a new card is drawn and updated hand displayed. Two sums of the player's cards is also displayed - one is the minimum sum, where all aces are treated as ones (player 'busts' if this is more than twenty-one), and the second is the best sum (if player stands, this sum will be compared with dealer's sum). Once the player stands of 'busts', game moves to dealer's state gameStateDealer or gameStateInitial respectively.

3. Dealer's state (gameStateDealer): This is the state where dealer's deterministic strategy is implemented. If the dealer does not go bust, then the best hands are compared and winner is determined.

4. gameStateQuit: Acts as placeholder for end of game. Whenever player chooses 'q', or exhausts it chips, game enters this state and quits.
