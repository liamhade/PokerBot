#include <iostream>
#include <vector>

#include "C:\Users\11061985\OneDrive - Dover Corporation\Desktop\PROJECTS\PokerBot\src\CardHandler.h"
#include "C:\Users\11061985\OneDrive - Dover Corporation\Desktop\PROJECTS\PokerBot\src\Player.h"
#include "C:\Users\11061985\OneDrive - Dover Corporation\Desktop\PROJECTS\PokerBot\src\PotDispersal.h"

template<typename T>
void assert(T actual, T expected, string test_name) {
    if (actual != expected) {
        cout << "\033[31m" << "FAILED" << "\033[0m" << ":  " << test_name << "\n";
    } else{
        cout << "\033[32m" << "PASSED" << "\033[0m" << ":  " << test_name << "\n";
    }
}

void test_create_sidepots() {
    Player p1(100, "Alice");
    Player p2(100, "Bob");
    Player p3(100, "Carl");

    // Simulating betting
    p1.add_to_amount_bet(50);
    p2.add_to_amount_bet(30);
    p3.add_to_amount_bet(50);

    vector<Player*> players = {&p1, &p2, &p3}; 
    vector<SidePot> sidepots = create_sidepots(players);

    assert(get<0>(sidepots[0]), {&p2, &p1, &p3}, "Main sidepot ($90)");
    assert(get<0>(sidepots[1]), {&p1, &p3}, "Smaller sidepot ($40)");
}

void test_disperse_sidepot_funds_to_winners() {
    Player p1(0, "Alice");
    Player p2(0, "Bob");
    Player p3(0, "Carl");
    
    //////////////////////////////////
    /* Creating PlayerWithHandRanks */
    //////////////////////////////////

    // Creating HandRanks
    HandRank p1_handrank;
    p1_handrank.kind_of_hand = KindsOfHand::Pair;
    p1_handrank.sorted_cards_inside_hand = {};
    p1_handrank.sorted_cards_outside_hand = {};

    HandRank p2_handrank;
    p2_handrank.kind_of_hand = KindsOfHand::Pair;
    p2_handrank.sorted_cards_inside_hand = {};
    p2_handrank.sorted_cards_outside_hand = {};
    
    HandRank p3_handrank;
    p3_handrank.kind_of_hand = KindsOfHand::Pair;
    p3_handrank.sorted_cards_inside_hand = {};
    p3_handrank.sorted_cards_outside_hand = {};

    // Creating PlayersWithHandRank
    PlayerWithHandRank p1_hr = {&p1, {}, p1_handrank};
    PlayerWithHandRank p2_hr = {&p2, {}, p2_handrank};
    PlayerWithHandRank p3_hr = {&p3, {}, p3_handrank};

    vector<vector<PlayerWithHandRank>> winning_groups = {{p1_hr, p2_hr}, {p3_hr}};

    ///////////////////////
    /* Creating sidepots */
    ///////////////////////

    // Simulating betting
    p1.add_to_amount_bet(50);
    p2.add_to_amount_bet(30);
    p3.add_to_amount_bet(50);

    // Setting up players and sidepots
    vector<Player*> players = {&p1, &p2, &p3}; 
    vector<SidePot> sidepots = create_sidepots(players);

    /**
     * Expectation is that because the side pots are the following: {({p1, p2, p3}, 90), ({p1, p3}, 40)}
     * And because the winning groups are: {{p1, p2}, {p3}}
     * 
     * That the winnings will be split in the following way:
     *  - p1: 45 + 40
     *  - p2: 45
     *  - p3: 0
     */

    // Dispering funds
    disperse_sidepot_funds_to_winners(winning_groups, sidepots, false);

    assert(p1.get_stack(), 85.0f, "Player 1 stack (should be $85)");
    assert(p2.get_stack(), 45.0f, "Player 2 stack (should be $45)");
    assert(p3.get_stack(),  0.0f, "Player 3 stack (should be $0)");
}

void test_disperse_side_funds_0_bet() {
    assert(1, 2, "TODO: complete this function.");
}

int main() {
    test_create_sidepots();
    test_disperse_sidepot_funds_to_winners();
    test_disperse_side_funds_0_bet();
}