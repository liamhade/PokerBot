## Game Mechanics 

- [x] If the minimum bet is more than the current player's total stack, then they must go all in.

- [x] Call should automatically put the minimum bet in

- [ ] Handle side-pot mechanics

	- [x] Fix issue with players stack not updating

		- [x] change sorted_eligible_players to work with Player*""

	- [ ] Allow multiple players to be awarded after game is over  

		- Right now, only one player can win the whole stack. 

- [ ] Enable hands smaller than "7-hands" to be shown and computed.

	- Right now, we can only show the hand rank if all five community cards
	have been played.

- [ ] End the betting round early depending on how players fold

- [ ] Not all pairs are created equal

	- Create a way to compare pairs using different cards.

	- Right now, we only use the high card to compare. 

- [ ] Add a simple poker engine to play against

- [ ] Handle ties

- [x] Handle multiple games

## Organization

- [ ] Simplify the logic in disperse_winnings()

## C++ Questions

- [ ] Convert action_enum_2_string to use switch statement instead

- [x] What's the proper way to use printf?