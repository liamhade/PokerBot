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

/*
Thank you to paparazzo from https://softwareengineering.stackexchange.com/questions/317640/poker-split-side-pots
for helping me to realize that my side pop implementation was over-complicated.
*/

class Poker {
public:
    Poker(std::vector<Player> players_vec, DeckHandler deck_handler) {
        players = players_vec;
        deck = deck_handler;
    }
    
    void play() {
        while (1) {
            community_cards = {};
            pot = 0;
            deck.reset();
    
            deal_player_cards();
            show_header("PRE-FLOP");
            betting_round();
            if (all_but_one_folded()) {
                declare_winner();
                continue;
            };
            
            show_header("FLOP");
            add_community_cards(3);
            show_community_cards();
            betting_round();
            if (all_but_one_folded()) {
                declare_winner();
                continue;
            };
            
            show_header("TURN");
            add_community_cards(1);
            show_community_cards();
            betting_round();
            if (all_but_one_folded()) {
                declare_winner();
                continue;
            };
            
            show_header("RIVER");
            add_community_cards(1);
            show_community_cards();
            betting_round();
            declare_winner();
        }
    }

    void show_community_cards() {
        std::cout << "Community Cards: " << get_cards_str(community_cards) << std::endl;
    }

    void show_header(std::string text) {
        std::cout << "############" << std::endl;
        std::cout << "## " << text << std::endl;
        std::cout << "############\n" << std::endl;
    }

    void reset_player_fold_states() {
        for (Player& p : players) {
            p.unfold();
        }
    }

    void declare_winner() {
        if (all_but_one_folded()) {
            // Winner won either because everyone folded...
            for (Player& p : players) {
                if (!p.has_player_folded()) {
                    std::cout << p.get_name() << " has won the $" << pot << " pot!"  << std::endl;
                    p.add_to_stack(pot);
                }
            }
        }
        else {
            // Or because they had the best hand
            // among all of the other winners.
            for (Player p : players) {
                if (!p.has_player_folded()) {
                    std::cout << p.get_name() << ": " << get_cards_str(p.get_hole_cards()) << std::endl;
                }
            }
            std::cout << "Community cards: " << get_cards_str(community_cards) << std::endl;
            // Show the person with the best hand.
            std::tuple<Player, Cards, HandRank> player_with_best_hand = HandEvaluator::player_with_best_hand(players, community_cards);
            show_winner_details(player_with_best_hand);
            std::get<0>(player_with_best_hand).add_to_stack(pot);
        }

        reset_player_fold_states();
        reset_player_bet_amounts_for_round();
    }

    void show_winner_details(std::tuple<Player, Cards, HandRank> winner_details)  {
        Player player     = std::get<0>(winner_details);
        Cards cards       = std::get<1>(winner_details);
        HandRank handrank = std::get<2>(winner_details);

        std::cout << std::endl;
        std::cout << player.get_name() <<" has won the $" << pot << " pot!" << std::endl;
        std::cout << " --> They had a " << KindsOfHand_2_string(std::get<0>(handrank));
        std::cout << " with the cards: " << get_cards_str(cards) << std::endl;
    }

    void add_community_cards(int n) {
        for (int i=0; i<n; i++) {
            community_cards.push_back(deck.draw_card());
        }
    }

    void deal_player_cards() {
        for (Player& p : players) {
            p.set_cards({ deck.draw_card(), deck.draw_card() });
        }
    }

    void betting_round() {
        float minimum_bet = 0.0;
        bool all_players_have_gone = false;
        std::vector<KindsOfAction> possible_actions = { KindsOfAction::CHECK, KindsOfAction::BET, KindsOfAction::FOLD };
        reset_player_bet_amounts_for_round();
        while (1) {
            for (Player& p : players)  {
                if (enough_players_are_all_in()) {
                   return;
                }
                if (all_players_have_gone) {
                    // If every players has gone,
                    // then the round can end on any player.
                    if (round_is_over()) {
                        return;
                    }
                }
                if (p.has_player_folded()) {
                    continue;
                };
                Action player_action = p.get_action(possible_actions, minimum_bet);
                update_game_using_player_action(&p, player_action, &minimum_bet);
                possible_actions = get_possible_actions(player_action);
            };
            all_players_have_gone = true;
        }
    }
    
    bool round_is_over() {
        return (all_players_bet_same_amount() || all_but_one_folded());
    }

    void reset_player_bet_amounts_for_round() {
        for (Player& p : players) {
            p.set_round_amount_bet(0);
        }
    }
    
    bool all_players_bet_same_amount() {
        float bet_amount = -1;
        for (Player p : players) {
            if (!p.has_player_folded()) {
                if (bet_amount < 0) {
                    // Bet amounts can only be >= 0.
                    bet_amount = p.get_round_amount_bet();
                } else {
                    if (p.get_round_amount_bet() != bet_amount) {
                        return false;
                    }
                }
            }
        }
        return true;
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
        switch (std::get<0>(player_action)) {
            case KindsOfAction::CHECK:
                return;
            case KindsOfAction::FOLD:
                player->set_player_has_folded();
                return;
            case KindsOfAction::BET:
            case KindsOfAction::CALL:
            case KindsOfAction::RAISE:
                pot += std::get<1>(player_action);
                *minimum_bet = player->get_round_amount_bet();
        };
    }

private:
    std::vector<Player> players;
    DeckHandler deck;
    float pot = 0.0;
    Cards community_cards;

    std::vector<KindsOfAction> get_possible_actions(Action last_action) {
        KindsOfAction last_kind_of_action = std::get<0>(last_action);
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

    bool enough_players_are_all_in() {
        int num_all_in_players = 0;
        for (Player p : players) {
            if (p.player_is_all_in()) {
                num_all_in_players++;
            }
        }
        /*
        In a two person game, if one player has more money than another player, then
        the player with more money can't do anything because they have nobody to
        play against.
        */
        return (players.size() - num_all_in_players) <= 1;
    }
};

int main() {
    DeckHandler deck;
    Player p1(100, "Kenny Rogers");
    Player p2(100, "Dolly Parton");
    Player p3(100, "Travis Kelce");

    p1.set_cards({ deck.draw_card(), deck.draw_card() });
    p2.set_cards({ deck.draw_card(), deck.draw_card() });
    p3.set_cards({ deck.draw_card(), deck.draw_card() });

    Cards community_cards = {
        deck.draw_card(),
        deck.draw_card(),
        deck.draw_card(),
        deck.draw_card(),
        deck.draw_card(),

    };
    // Poker game({ p1, p2 }, deck);
    // game.play();

    std::vector<std::tuple<Player, Cards, HandRank>> rankings;
    rankings = HandEvaluator::ranked_player_hands({ p1, p2, p3 }, community_cards);

    return 0;
}