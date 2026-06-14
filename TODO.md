## Game Mechanics 

- [ ] Add a simple poker engine to play against

	- [x] Convert the classes to Abstract Base Classes.

	- [x] Print out whether the player is a User or a Bot next to their name. 

	- [x] Print out the Players action (Action Name and bet size) after they make it.

	- [x] Add a short pause between player turns to increase flow.

	- [ ] Make it work on any user action
	
		- There are two basic kinds moves :
			1) Fold
			2) Bet
				a) At least the minimum bet
				b) All in 
		
		- [ ] Add the most basic strategy in, and expand on it later.
	

- [ ] Error handling if the user types in something that they can't do.

- [ ] Add the sound of chips when a player bets 

- [ ] End the betting round early depending on how players fold

	- [ ] Make sure that player's cards aren't revealed if they everyone else folded.

## Organization

- [ ] (In `PotDispersal.cpp`) Replace use of `float` in tuple with `.total_amount_bet()` to simplify the code. 

## C++ Questions

- [ ] Convert action_enum_2_string to use switch statement instead

## Solved

- [x] Infinite loop with three players (see `Errors` file) 

- [x] Remove `Poker` module from `main.cpp` 

- [x] Add more spaces to the UI

- [x] Handle ties

- [x] Handle multiple games

- [x] Not all pairs are created equal

	- Create a way to compare pairs using different cards.

	- Right now, we only use the high card to compare. 

- [x] What's the proper way to use printf?

- [x] If the minimum bet is more than the current player's total stack, then they must go all in.

- [x] Call should automatically put the minimum bet in

- [x] Handle side-pot mechanics

	- [x] Fix issue with players stack not updating

		- [x] change sorted_eligible_players to work with Player*""

	- [x] Allow multiple players to be awarded after game is over  

	- [x] Allow a player to go all in

- [x] Move `disperse_winnings()` to a seperate module

- [x] Simplify the logic in `disperse_winnings()`
