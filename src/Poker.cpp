#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <thread>
#include <chrono>
#include <random>
#include "../include/HandEvaluator.h"
#include "../include/CardHandler.h"
#include "../include/DeckHandler.h"
#include "../include/PotDispersal.h"
#include "../include/PlayerInterface.h"
#include "../include/Poker.h"
using namespace std;

/*
Thank you to paparazzo from https://softwareengineering.stackexchange.com/questions/317640/poker-split-side-pots
for helping me to realize that my side pop implementation was over-complicated.
*/

Poker::Poker(vector<Player*> players, DeckHandler deck) {
    this->players = players;
    this->deck    = deck;
}

void Poker::play() {
    while (1) {
        community_cards = {};
        pot = 0;
        deck.reset();
        reset_player_fold_states();
        reset_player_bet_amounts_for_round();
        reset_player_total_bet_amounts();

        deal_player_cards();
        show_header("PRE-FLOP");
        betting_round();
        if (all_but_one_folded()) {
            disperse_winnings(false);
            continue;
        };
        
        show_header("FLOP");
        add_community_cards(3);
        show_community_cards();
        betting_round();
        if (all_but_one_folded()) {
            disperse_winnings(false);
            continue;
        };
        
        show_header("TURN");
        add_community_cards(1);
        show_community_cards();
        betting_round();
        if (all_but_one_folded()) {
            disperse_winnings(false);
            continue;
        };
        
        show_header("RIVER");
        add_community_cards(1);
        show_community_cards();
        betting_round();
        disperse_winnings(true);
    }
}

void Poker::show_header(string text) {
    cout << "############" << endl;
    cout << "## " << text << endl;
    cout << "############\n" << endl;
}

void Poker::show_community_cards() {
    cout << "Community Cards: " << get_cards_str(community_cards) << endl;
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
                cout << player->get_name() << " won $" << pot << " because they were the only one not to fold." << endl;
                pot = 0;
                return;
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

void Poker::betting_round() {
    float minimum_bet = 0.0;
    bool all_players_have_gone = false;
    reset_player_bet_amounts_for_round();

    while (1) {
        if ((all_players_have_gone && round_is_over(minimum_bet))) {
            return;
        }
        for (Player* p : players)  {
            if (p->has_player_folded()) {
                cout << "Skipping " << p->get_name() << " because they folded." << endl << endl;
            } else if (p->is_all_in()) {
                cout << "Skipping " << p->get_name() << " because they are all in." << endl << endl;
            } else if (p->has_no_money()) {
                cout << "Skipping " << p->get_name() << " because they have no money." << endl << endl;
            } else {
                Action player_action = p->get_action(minimum_bet);
                show_action(player_action);
                update_game_using_player_action(p, player_action, &minimum_bet);
            }
            this_thread::sleep_for(chrono::milliseconds(1000));
        };
        all_players_have_gone = true;
    }
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

bool Poker::round_is_over(float min_bet) {
    for (Player* p : players) {
        if (!(p->has_player_folded())) {
            // Only examining players who haven't folded.
            if (!(p->is_all_in() || (p->get_round_amount_bet() == min_bet))) {
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

void Poker::update_game_using_player_action(Player* player, Action player_action, float* minimum_bet) {
    switch (get<0>(player_action)) {
        case KindsOfAction::CHECK:
            return;
        case KindsOfAction::FOLD:
            player->set_player_has_folded();
            return;
        case KindsOfAction::BET:
        case KindsOfAction::CALL:
        case KindsOfAction::RAISE:
        case KindsOfAction::ALL_IN:
            pot += get<1>(player_action);

            if (*minimum_bet < player->get_round_amount_bet()) {
                *minimum_bet = player->get_round_amount_bet();
            }
    };
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