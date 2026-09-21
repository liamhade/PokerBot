#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>
#include "../include/DeckHandler.h"
#include "../include/Poker.h"
#include "../src/HumanPlayer.cpp"
#include "../src/ComputerPlayer.cpp"
using namespace std;

/*
Thank you to paparazzo from https://softwareengineering.stackexchange.com/questions/317640/poker-split-side-pots
for helping me to realize that my side pop implementation was over-complicated.
*/

int main() {
    DeckHandler deck;
    Human p1(5, "Kenny Rogers");
    Bot p2(100, "James Dean");
    // Bot p3(100, "Merle Haggard");

    // Poker game({ &p1, &p2, &p3 }, deck, 0.5, 1);
    Poker game({ &p1, &p2 }, deck, 0.5, 1);
    game.play();

    return 0;
}