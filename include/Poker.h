#ifndef POKER
#define POKER

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>
#include "HandEvaluator.h"
#include "CardHandler.h"
#include "DeckHandler.h"
#include "PotDispersal.h"
using namespace std;

/*
Thank you to paparazzo from https://softwareengineering.stackexchange.com/questions/317640/poker-split-side-pots
for helping me to realize that my side pop implementation was over-complicated.
*/

class Poker {
public:
    Poker(vector<Player*> players, DeckHandler deck, float sb_amount, float bb_amount);
    
    bool all_players_have_money();

    void play();

    void show_community_cards();

    void show_header(string text);

    void reset_player_fold_states();

    Player* get_dealer();

    Player* get_small_blind();

    Player* get_big_blind();

    int _rotated_button_index(int button_index);

    string get_player_header(Player* player);

    vector<vector<tuple<Player*, Cards, HandRank>>> sorted_non_folded_players();

    void disperse_winnings(bool show_hand_details);

    void add_community_cards(int n);

    int num_players_not_all_in();

    void deal_player_cards();

    void blinds_bet();

    float player_turn(Player* player, float current_bet);

    void betting_round(bool is_preflop = false);
    
    void reset_player_total_bet_amounts(); 

    void reset_player_bet_amounts_for_round(); 

    bool round_is_over(float current_bet);

    bool all_but_one_folded();

    void rotate_buttons();

private:    
    vector<Player*> players;
    DeckHandler deck;
    float sb_amount;
    float bb_amount;
    float pot = 0.0;
    Cards community_cards;

    int dealer_index;
    int sb_index;
    int bb_index;
    int starting_player_index;

    vector<KindsOfAction> get_possible_actions(Action last_action);
};

#endif