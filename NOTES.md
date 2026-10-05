# 9/30/26

I should have defined a Hand as a `tuple<Card, Card>`, rather a `vector<Card>`.  

# 9/8/26

PROBLEM: The betting design for the player is overly complicated. I'll probably need to go in their and fine-tune some of the abstractions.

PROBLEM: The minimum bet for the round isn't being reset. It keep's defaulting to the `big bling`.
SOLUTION: That's because I set it that way. I conflated the minimum amount to bet with the big blind amount.

PROBLEM: For some reason, when I play against the bot, I'm getting stuck in an infinite loop on the preflop.
- Does it happen when it's human vs human?
    + No, it only happens when it's human vs bot. Which implies that this is an issue with the Bot implementation.
- SOLUTION: The problem was that `ComputerPlayer` wasn't updating it's `round_amount_bet` and `total_amount_bet` values like it should have been. That functionality should be taken factored out so that that logic is applied, regardless of the `Player` implementation.

Just learned about `GoogleTest`, Google's C++ testing framework. I should migrate over to that when I get the chance.

# 4/26/26

I got the Bot player to respond to user bets with calls. However, we're having some issues with the bets not registering, so I'll need to fix that in the next iteration. I also adjusted some of the formatting for the outputs to make them more aesthetic and easier to look at.

# 2/24/26

We fixed the issue with the money not being dispersed correctly.

# 2/23/26

The code is compiling, which is good. But now we're running into issues where the money is not being dispersed correctly. Sometimes by the end of the round
there's more money in circulation then we started the game.

# 2/22/26

Made some good progress today. Even though I moved the PotDispersal logic into a seperate function, it can still be made more modular.

It's always a get idea to keep complicated logic in a seperate module. It makes the system easier to upgrade if you ever want to change anything. Plus, 

# 2/1/26

Why can't I get VS-Code Debugger to work?

I think the implementation I was working on previously was for side pots where the players tie.
Essentially, everyone in the same group should get the same amount.

Having to rework the logic for side-pots. My previous logic couldn't handle ties between playes who
bet different amounts.

For the future, I should have handled the disperse winnings function in a seperate file / module.
Right now, the logic is enmeshed with my main.cpp, which is making this logic change interfere
with the entire pipeline of the game.

Also, having a seperate module would make testing easier.

Pick the right data-structure, and the solution will reveal itself.

The main problem that I'm having right now is how to handle ties between
players of different amounts using my current side-pot mechanic.

# 11/28/25

## Problem 1

(?) Why is main.cpp not even compiling?

    --> It works when I manualy type out the g++ function, so the issue was something with the built in command.

## Problem 2

(?) Why is the players stack not updating when they win a hand?

    - I think it could be because we are updating a copy of the Player, instead  
    of the actual player.

    --> Made it so that we only deal with the points of players, not the actual player objects.

## Problem 3

(?) Why are we getting an error with stl_vector.h

    - The error happens after a winner is declared. 