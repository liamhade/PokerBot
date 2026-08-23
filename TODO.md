## Game Mechanics 

- [ ] (**Design + Implement SB and LB**) Design archicture for + add Small Blind and Little Blind into the game mechanics. Make sure that `SB` and `LB` are written next to the names of the players who have those chips. 

- [ ] (**option to buy back / quit if no more money**) If a player has no more money, the game shouldn't continue as usual. We should give the option for them to buy back in for more.

- [ ] (**add/remove players**) A player should be able to leave / join the game any time they want.

- [ ] (**Antes**) Make sure that the players in the beginning of the game have to throw in their antes.
	- ANTE is set at startup as `Game` parameter.
	- `game.py`: LN24 becomes `Poker game({ &p1, &p2 }, deck, ant: float)`
	- `Poker.h`: 
		- LN24 becomes `Poker(vector<Player*> players, DeckHandler deck, float ante);`
		- Below LN60, add `float ante;`

- [ ] (**Split get_action**) The `get_action` function as defined in `PlayerInterface.h` is too general. What we really need are two specialized `get_action` functions. One will be `Action get_bot_action(float min_bet, Cards table_cards, Players players)`, and the other will be `Action get_user_action(float min_bet)`.

- [ ] (**Add minimum bet interval mechanics**) 
	*Setup*       : Any game
	*Action*      : Player bets by 0.00001
	*Expectation* : Message stating: "I'm sorry you must bet atleast <the_minimum_bet>"
	*Reality*	  : Bet is allowed.

- [~] Add a simple poker engine to play against

	- [ ] (*BUG*, **against loop**) 
		*Setup*       : Only two players in the game, the user and the bot.
		*Action*      : User folds.
		*Expectation* : The bot wins.
		*Reality*	  : Bot makes the move that they previously played, the game engine skips the user because they folded, and the loops continues. 

	- [ ] (**Monte Carlo win function**) Implement the function `float montecarlo_winrate(Cards my_hand, Cards board_cards, int n_other_players, int s);` that runs `s` MonteCarlo simulations to see how often we would win.

	- [ ] (**bruteforce win function**) Implement the function `float bruteforce_winrate(Cards my_hand, Cards board_cards, int n_other_players)`.

	- [ ] (**bluffing**) Currently, the bot `Player` has no conception of lying, or of how to lie well. In order to bluff well, it needs to know three things. When to bluff, how much to bet when bluffing, and when to stop bluffing. Implement these functions to decide wheher the player should bluff:

		- [ ] `bool will_bluff()`: Probabilistic function that decides whether to start bluffing or not. Even if all the criteria are met, we need to introduce some randomness into the gameplay, otherwise we'll be too predicatable

		- [ ] `bool is_almost_good_hand(Cards hand, Cards table)`: Checks if the players current best hand is almost a flush, or a straight, or if there are any okay pairs. This information is important, because when the bot bluffs, it should have some hand that it can fall back on.

		- [ ] `bool players_have_folded_before(vector<Player*> players)`: If none of the other players in the game have ever folded, then bluffing against them is dangerous, since they may just call out. 

		- [ ] `float get_bluff_amount(Player* bluffer, Game game)`: This function determines how much money the bluffer should throw into the pot. If you bet too little, then no one gets scared by your bet. But if the bet is too large, it looks suspicious and people may call you.

		- [ ] `bool any_big_raises_on_bluff(Player* bluffer, Game game)`: This function determines whether anybody did a large raise on the bluffer's bet. If they did, they we're probably better of folding.

- [ ] (**folding mechanics**) End the betting round early depending on how players fold

- [ ] (**card reveals**) Make sure that player's cards aren't revealed if they everyone else folded.

- [ ] (**chip sounds**)  Add the sound of chips falling when a player bets. 
