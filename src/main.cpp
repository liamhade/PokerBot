#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>
#include "../include/DeckHandler.h"
#include "../include/Player.h"
#include "../include/Poker.h"
using namespace std;

/*
Thank you to paparazzo from https://softwareengineering.stackexchange.com/questions/317640/poker-split-side-pots
for helping me to realize that my side pop implementation was over-complicated.
*/

int main() {
    DeckHandler deck;
    Player p1(100, "Kenny Rogers");
    Player p2(100, "Dolly Parton");
    Player p3(100, "Jimmy Buffet");

    Poker game({ p1, p2, p3 }, deck);
    game.play();

    return 0;
}