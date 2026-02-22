## Game Mechanics 

- [ ] End the betting round early depending on how players fold

- [ ] Add a simple poker engine to play against

- [ ] Handle ties


## Organization

- [ ] (In `PotDispersal.cpp`) Replace use of `float` in tuple with `.total_amount_bet()` to simplify the code. 

- [ ] Move `disperse_winnings()` to a seperate module

- [ ] Simplify the logic in `disperse_winnings()`

## C++ Questions

- [ ] Convert action_enum_2_string to use switch statement instead

## Solved

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