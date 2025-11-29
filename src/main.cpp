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
#include "Player.h"
using namespace std;

/*
Thank you to paparazzo from https://softwareengineering.stackexchange.com/questions/317640/poker-split-side-pots
for helping me to realize that my side pop implementation was over-complicated.
*/

class Poker {
public:
    Poker(vector<Player> players_vec, DeckHandler deck_handler) {
        players = players_vec;
        deck = deck_handler;
    }
    
    void play() {
        while (1) {
            community_cards = {};
            pot = 0;
            deck.reset();
            reset_player_fold_states();
            reset_player_bet_amounts_for_round();
    
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
            if (num_players_not_all_in() > 1) {
                betting_round();
                if (all_but_one_folded()) {
                    disperse_winnings(false);
                    continue;
                };
            }
            
            show_header("TURN");
            add_community_cards(1);
            show_community_cards();
            if (num_players_not_all_in() > 1) {
                betting_round();
                if (all_but_one_folded()) {
                    disperse_winnings(false);
                    continue;
                };
            }
            
            show_header("RIVER");
            add_community_cards(1);
            show_community_cards();
            if (num_players_not_all_in() > 1) {
                betting_round();
                disperse_winnings(true);
            }
        }
    }

    void show_community_cards() {
        cout << "Community Cards: " << get_cards_str(community_cards) << endl;
    }

    void show_header(string text) {
        cout << "############" << endl;
        cout << "## " << text << endl;
        cout << "############\n" << endl;
    }

    void reset_player_fold_states() {
        for (Player& p : players) {
            p.unfold();
        }
    }

    vector<tuple<Player*, Cards, HandRank>> sorted_non_folded_players() {
        vector<Player*> eligible_players;
        for (Player& p : players) {
            if (!p.has_player_folded()) {
                eligible_players.push_back(&p);
            }
        }
        return HandEvaluator::ranked_player_hands(eligible_players, community_cards);;
    }

    void show_winner_details(tuple<Player*, Cards, HandRank> winner_details, float winnings, bool show_hand_details)  {
        Player* player    = get<0>(winner_details);
        Cards cards       = get<1>(winner_details);
        HandRank handrank = get<2>(winner_details);

        cout << endl;
        cout << player->get_name() <<" has won $" << winnings << endl;
        if (show_hand_details) {
            cout << " -- They had a " << KindsOfHand_2_string(get<0>(handrank));
            cout << " with the cards: " << get_cards_str(cards) << endl;
        }
    }

    void disperse_winnings(bool show_hand_details) {
        vector<tuple<Player*, Cards, HandRank>> ranked_non_folded_players = sorted_non_folded_players();
        
        for (tuple<Player*, Cards, HandRank> player_tuple : ranked_non_folded_players) {
            Player* player = get<0>(player_tuple);
            float amount_player_won_overall = 0;
            float amount_player_bet = player->get_total_amount_bet();

            for (Player& paying_player : players) {
                float amount_won_from_player = min(amount_player_bet, paying_player.get_total_amount_bet()); 
                player->add_to_stack(amount_won_from_player);
                paying_player.set_total_amount_bet(paying_player.get_total_amount_bet() - amount_won_from_player);
                amount_player_won_overall += amount_won_from_player;
            }
            if (amount_player_won_overall > 0) {
                show_winner_details(player_tuple, amount_player_won_overall, show_hand_details);
            }
        }
    }

    void add_community_cards(int n) {
        for (int i=0; i<n; i++) {
            community_cards.push_back(deck.draw_card());
        }
    }

    int num_players_not_all_in() {
        int i = 0;
        for (Player p : players) {
            if (!p.has_player_folded() && !p.is_all_in()) {
                i++;
            }
        }
        return i;
    }

    void deal_player_cards() {
        for (Player& p : players) {
            p.set_cards({ deck.draw_card(), deck.draw_card() });
        }
    }

    void betting_round() {
        float minimum_bet = 0.0;
        bool all_players_have_gone = false;
        reset_player_bet_amounts_for_round();

        while (1) {
            if (all_players_have_gone && round_is_over(minimum_bet)) {
                return;
            }

            for (Player& p : players)  {
                if (p.has_player_folded()) {
                    printf("Skipping %s because they folded. \n", p.get_name().c_str());
                } else if (p.is_all_in()) {
                    printf("Skipping %s because they are all in. \n", p.get_name().c_str());
                } else {
                    Action player_action = p.get_action(minimum_bet);
                    update_game_using_player_action(&p, player_action, &minimum_bet);
                }
            };

            all_players_have_gone = true;
        }
    }
    
    void reset_player_bet_amounts_for_round() {
        for (Player& p : players) {
            p.set_round_amount_bet(0);
        }
    }

    bool round_is_over(float min_bet) {
        if (all_but_one_folded()) {
            return true;
        } else {
            for (Player p : players) {
                if (!(p.is_all_in() || (p.get_round_amount_bet() == min_bet))) {
                    return false;
                }
            }
            return true;
        }
    }

    bool all_but_one_folded() {
        int fold_count = 0;
        for (Player p : players) {
            if (p.has_player_folded()) {
                fold_count++;
            }
        }
        return (fold_count == players.size()-1);
    }

    void update_game_using_player_action(Player* player, Action player_action, float* minimum_bet) {
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

private:
    vector<Player> players;
    DeckHandler deck;
    float pot = 0.0;
    Cards community_cards;

    vector<KindsOfAction> get_possible_actions(Action last_action) {
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
};

int main() {
    DeckHandler deck;
    Player p1(50, "Kenny Rogers");
    Player p2(100, "Dolly Parton");

    Poker game({ p1, p2 }, deck);
    game.play();

    return 0;
}