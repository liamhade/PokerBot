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

class Poker {
public:
    Poker(std::vector<Player> players_vec, DeckHandler deck_struct) {
        players = players_vec;
        deck = deck_struct;
    }
    
    void play() {
        deal_player_cards();

        betting_round();
        if (all_but_one_folded()) {
            announce_winner();
            return;
        };
        
        add_community_cards(3);
        show_community_cards();
        betting_round();
        if (all_but_one_folded()) {
            announce_winner();
            return;
        };
        
        add_community_cards(1);
        show_community_cards();
        betting_round();
        if (all_but_one_folded()) {
            announce_winner();
            return;
        };
        
        add_community_cards(1);
        show_community_cards();
        betting_round();
        announce_winner();
    }

    void show_community_cards() {
        std::cout << "Community Cards: " << get_cards_str(community_cards) << std::endl;
    }

    void announce_winner() {
        // Winner is either because everyone folded
        if (all_but_one_folded()) {
            for (Player p : players) {
                if (!p.has_player_folded()) {
                    printf("%s has won the $%f pot!", p.get_name(), pot);
                    return;
                }
            }
        }
        // Or the person with the best hand 
        for (Player p : players) {
            if (!p.has_player_folded()) {
                printf("%s: %s", p.get_name(), get_cards_str(p.get_hole_cards()));
            }
        }
        printf("Community cards: %s", community_cards);
        printf(""); 
        // Show the person with the best hand.
        std::tuple<Player, Cards, HandRank> player_with_best_hand = HandEvaluator::player_with_best_hand(players, community_cards);
        show_winner_details(player_with_best_hand);
    }

    void show_winner_details(std::tuple<Player, Cards, HandRank> winner_details)  {
        Player player     = std::get<0>(winner_details);
        Cards cards       = std::get<1>(winner_details);
        HandRank handrank = std::get<2>(winner_details);

        printf("%s has won the $%f pot!", player.get_name(), pot);
        printf("\t They had a %s with the cards: %s", KindsOfHand_2_string(std::get<0>(handrank)), get_cards_str(cards));
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
        std::vector<KindsOfAction> possible_actions = { KindsOfAction::CHECK, KindsOfAction::BET, KindsOfAction::FOLD };

        do {
            for (Player p : players)  {
                if (p.has_player_folded()) {
                    continue;
                };

                Action player_action = p.get_action(possible_actions, minimum_bet);
                handle_player_action(&p, player_action, &minimum_bet);
                possible_actions = get_possible_actions(player_action);
            };
        } while ((!round_is_over()));
    }
    
    bool round_is_over() {
        return (all_players_bet_same_amount() || all_but_one_folded());
    }
    
    bool all_players_bet_same_amount() {
        float bet_amount = -1;
        for (Player p : players) {
            if (!p.has_player_folded()) {
                if (bet_amount < 0) {
                    // Bet amounts can only be >= 0.
                    bet_amount = p.get_amount_bet();
                } else {
                    if (p.get_amount_bet() != bet_amount) {
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

    void handle_player_action(Player* player, Action player_action, float* minimum_bet) {
        switch (std::get<0>(player_action)) {
            case KindsOfAction::CHECK:
                return;
            case KindsOfAction::FOLD:
                player->set_player_has_folded();
                return;
            case KindsOfAction::BET:
            case KindsOfAction::CALL:
            case KindsOfAction::RAISE:
                float bet_amount = std::get<1>(player_action);
                *minimum_bet = bet_amount;  
                player->add_to_amount_bet(bet_amount);
                pot += bet_amount;
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
                return { KindsOfAction::CHECK, KindsOfAction::FOLD, KindsOfAction::BET };
            case KindsOfAction::BET:
            case KindsOfAction::CALL:
            case KindsOfAction::RAISE:
                return { KindsOfAction::CALL, KindsOfAction::RAISE };
            default:
                return { KindsOfAction::CALL };
        };
    }
};

int main() {
    DeckHandler deck;
    Player p1(100, "Kenny Rogers");
    Player p2(100, "Dolly Parton");

    Poker game({ p1, p2 }, deck);
    game.play();

    // The game ends when one player folds, or, if both players bet the same amount
    return 0;
}