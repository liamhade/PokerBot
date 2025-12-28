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