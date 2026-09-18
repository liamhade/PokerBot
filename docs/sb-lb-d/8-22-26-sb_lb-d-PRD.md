# Goal

Implement the mechanics of the small blind, little blind, and dealer buttons

# Non-Goal

# Requirements

1. [x] the small blind (SB) is to the right of the dealer (D)
2. [x] The big blind (BB) is to the right of the SB
3. [x] A player is obligated to bet according to their blind
4. [x] If a player doesn't have enough for their blind, they must go all in
5. [x] The D, SB, and BB chips must rotate clockwise after each hand
6. [x] A (SB), (BB), or (D) icon must appear in bold next to the name of the player 
7. [x] Minimum bet must reflect the BB and SB values
8. [x] On pre-flop, the BB doesn't have to bet again.
8. [x] On pre-flop, the SB has to bet again.

# Edge-cases

- Only two players in the game

# Open Questions

- Q: What if if a player is no longer in the game?
	+ A: For now, just assume that a player can't drop out of the game. If they do, then the game is over.	