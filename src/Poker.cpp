#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <thread>
#include <chrono>
#include <random>
#include <typeinfo>
#include "../include/HandEvaluator.h"
#include "../include/CardHandler.h"
#include "../include/DeckHandler.h"
#include "../include/PotDispersal.h"
#include "../include/PlayerInterface.h"
#include "../include/Poker.h"
#include "./ComputerPlayer.cpp"
#include "./HumanPlayer.cpp"
using namespace std;

/*
Thank you to paparazzo from https://softwareengineering.stackexchange.com/questions/317640/poker-split-side-pots
for helping me to realize that my side pop implementation was over-complicated.
*/

Poker::Poker(vector<Player*> players, DeckHandler deck, float sb_amount, float bb_amount) {
    this->players   = players;
    this->deck      = deck;
    this->sb_amount = sb_amount;
    this->bb_amount = bb_amount; 

    this->dealer_index = 0;
    this->sb_index     = 1;
    this->bb_index     = 2 % players.size();
    this->starting_player_index = 3 % players.size(); 
}

void Poker::play() {
    bool is_first_round = true;

    while (1) {

        if (!is_first_round) {
            rotate_buttons();
            ++starting_player_index %= players.size();  
        } else {
            is_first_round = false;
        }

        community_cards = {};
        pot = 0;
        deck.reset();
        reset_player_fold_states();
        reset_player_total_bet_amounts();
        
        show_header("PRE-FLOP");
        deal_player_cards();
        reset_player_bet_amounts_for_round();
        betting_round(true);
        if (all_but_one_folded()) {
            disperse_winnings(false);
            continue;
        };
        
        show_header("FLOP");
        add_community_cards(3);
        show_community_cards();
        reset_player_bet_amounts_for_round();
        betting_round();
        if (all_but_one_folded()) {
            disperse_winnings(false);
            continue;
        };
        
        show_header("TURN");
        add_community_cards(1);
        show_community_cards();
        reset_player_bet_amounts_for_round();
        betting_round();
        if (all_but_one_folded()) {
            disperse_winnings(false);
            continue;
        };
        
        show_header("RIVER");
        add_community_cards(1);
        show_community_cards();
        reset_player_bet_amounts_for_round();
        betting_round();

        disperse_winnings(true);
    }
}

void Poker::show_header(string text) {
    cout << "############\n";
    cout << "## " << text << "\n";
    cout << "############\n\n";
}

void Poker::show_community_cards() {
    cout << "Community Cards: " << get_cards_str(community_cards) << "\n\n";
    this_thread::sleep_for(chrono::milliseconds(1000));
}


void Poker::reset_player_fold_states() {
    for (Player* p : players) {
        p->unfold();
    }
}

vector<vector<tuple<Player*, Cards, HandRank>>> Poker::sorted_non_folded_players() {
    vector<Player*> eligible_players;
    for (Player* p : players) {
        if (!p->has_player_folded()) {
            eligible_players.push_back(p);
        }
    }
    return HandEvaluator::ranked_player_hands(eligible_players, community_cards);
}

void Poker::disperse_winnings(bool show_hand_details) {
    if (all_but_one_folded()) {
        // Only one non-folding player, which means we may not have reach the river (5 community cards).
        for (Player* player: players) {
            // Grabbing only player who hasn't folded.
            if (!player->has_player_folded()) {
                player->add_to_stack(pot);
                cout << player->get_name() << " won $" << pot << " because they were the only one not to fold.\n" << endl;
                pot = 0;
                break;
            }
        }
    } else {
        // Converting list of players to list of pointers to players.
        vector<Player*> player_pointers;
        for (Player* player: players) {
            player_pointers.push_back(player);
        }
        // More than one non-folding players, which means we reached the river card.
        vector<vector<PlayerWithHandRank>> ranked_non_folded_players = sorted_non_folded_players();
        vector<SidePot> sidepots = create_sidepots(player_pointers);
        disperse_sidepot_funds_to_winners(ranked_non_folded_players, sidepots, true);
    }
    // Delay for aesthetic affects.
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void Poker::add_community_cards(int n) {
    for (int i=0; i<n; i++) {
        community_cards.push_back(deck.draw_card());
    }
}

int Poker::num_players_not_all_in() {
    int i = 0;
    for (Player* p : players) {
        if (!p->has_player_folded() && !p->is_all_in()) {
            i++;
        }
    }
    return i;
}

void Poker::deal_player_cards() {
    for (Player* p : players) {
        p->set_cards({ deck.draw_card(), deck.draw_card() });
    }
}

Player* Poker::get_dealer() {
    return players[dealer_index];
}

Player* Poker::get_small_blind() {
    return players[sb_index];
}

Player* Poker::get_big_blind() {
    return players[bb_index];
}

int Poker::_rotated_button_index(int button_index) {
    return (button_index + 1) % players.size();
}

void Poker::rotate_buttons() {
    dealer_index = _rotated_button_index(dealer_index);
    sb_index = _rotated_button_index(sb_index);
    bb_index = _rotated_button_index(bb_index);
}

string Poker::get_player_header(Player* player) {
    string header;
    if (typeid(*player) == typeid(Bot)) {
        header = player->name + " (bot)";
    } else if (typeid(*player) == typeid(Human)) {
        header = player->name + " (human)";
    }

    if (get_dealer() == player) {
        header = "[D] " + header;
    }
     if (get_small_blind() == player) {
        header = "[SB] " + header;
    }
    if (get_big_blind() == player) {
        header = "[BB] " + header;
    }

    return header;
}

bool are_all_other_players_folded(vector<Player*> all_players, Player* player) {
    int num_folded = 0;
    for (Player* p : all_players) {
        if (p->has_folded) {
            num_folded++;
        }
    }
    return (num_folded == (all_players.size()-1)) && (!player->has_folded);
}

void Poker::blinds_bet() {
    get_small_blind()->bet(sb_amount);
    cout << get_small_blind()->get_name() << " bet $" << sb_amount << " for the small blind.\n"; 
    pot += sb_amount;
    this_thread::sleep_for(chrono::milliseconds(500));
    
    get_big_blind()->bet(bb_amount);
    cout << get_big_blind()->get_name() << " bet $" << bb_amount << " for the big blind.\n\n"; 
    pot += bb_amount;
    this_thread::sleep_for(chrono::milliseconds(500));
}

float Poker::player_turn(Player* player, float current_bet) {
    cout << get_player_header(player) << "\n";
    Action player_action = player->act(bb_amount, current_bet);
    show_action(player_action);
    pot += get<1>(player_action);
    return max({ current_bet, player->get_round_amount_bet() });
}

void Poker::betting_round(bool is_preflop) {
    float current_bet;

    if (is_preflop) {
        blinds_bet();
        current_bet = bb_amount;
    } else {
        current_bet = 0;
    }

    do {
        for (int i=0; i<players.size(); i++)  {
            Player* p = players[(i+starting_player_index) % players.size()]; 

            if (p->has_player_folded()) {
                cout << "Skipping " << p->get_name() << " because they folded.\n\n";
            } else if (p->is_all_in()) {
                cout << "Skipping " << p->get_name() << " because they are all in.\n\n";
            } else if (p->has_no_money()) {
                cout << "Skipping " << p->get_name() << " because they have no money.\n\n";
            } else if (are_all_other_players_folded(players, p)) {
                cout << "All players except " << p->get_name() << " have folded.\n\n";
                return;
            } else {
              current_bet = player_turn(p, current_bet);
            }
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
    } while (!round_is_over(current_bet));
}
    
void Poker::reset_player_total_bet_amounts() {
    for (Player* p : players) {
        p->set_total_amount_bet(0);
    }
}

void Poker::reset_player_bet_amounts_for_round() {
    for (Player* p : players) {
        p->set_round_amount_bet(0);
    }
}

bool Poker::round_is_over(float current_bet) {
    for (Player* p : players) {
        if (!(p->has_player_folded())) {
            // Only examining players who haven't folded.
            if (!(p->is_all_in() || (p->get_round_amount_bet() == current_bet))) {
                return false;
            }
        }
    }
    return true;
}

bool Poker::all_but_one_folded() {
    int fold_count = 0;
    for (Player* p : players) {
        if (p->has_player_folded()) {
            fold_count++;
        }
    }
    return (fold_count == players.size()-1);
}

vector<KindsOfAction> Poker::get_possible_actions(Action last_action) {
    KindsOfAction last_kind_of_action = get<0>(last_action);
    switch (last_kind_of_action) {
        case KindsOfAction::CHECK:
        case KindsOfAction::FOLD:
            return { KindsOfAction::CHECK, KindsOfAction::BET, KindsOfAction::FOLD};
        case KindsOfAction::BET:
        case KindsOfAction::CALL:
        case KindsOfAction::RAISE:
            return { KindsOfAction::CALL, KindsOfAction::RAISE, KindsOfAction::FOLD };
        default:
            // This is here so that the compiler doesn't throw a warning,
            // though techinically we don't need it. 
            return { KindsOfAction::CALL };
    };
}