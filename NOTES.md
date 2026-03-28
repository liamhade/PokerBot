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