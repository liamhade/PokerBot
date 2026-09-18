#ifndef PLAYER_H
#define PLAYER_H

#include <tuple>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "CardHandler.h"
#include "Action.h"

class Player {
public:
    std::string name;
    float stack;
    float total_amount_bet = 0;
    float round_amount_bet = 0;
    Cards pocket;
    bool has_folded = false;

    Player(float starting_stack, std::string player_name)
        : stack(starting_stack), name(player_name) {}

    virtual ~Player() {};

    virtual void show_player_info() = 0;

    std::vector<KindsOfAction> get_possible_actions_for_player(float largest_bet_this_round) {
        /* Returns the legal kinds of actions that a play can take this turn. */
        if (get_round_amount_bet() == largest_bet_this_round) {
            // No bets to meet
            return { KindsOfAction::CHECK, KindsOfAction::BET, KindsOfAction::ALL_IN, KindsOfAction::FOLD };
        } else if (get_round_amount_bet() < largest_bet_this_round) {
            // Player needs to atleast match the minimum bet, or fold.
            float bet_delta = largest_bet_this_round - get_round_amount_bet();
            if (stack > bet_delta) {
                return { KindsOfAction::CALL, KindsOfAction::RAISE, KindsOfAction::ALL_IN, KindsOfAction::FOLD };
            } else {
                return { KindsOfAction::ALL_IN, KindsOfAction::FOLD };
            }
        } else {
            // Should never happen
            throw std::invalid_argument("Player has bet more than the largest_bet_this_round.");
        }
    }

    // Must be defined by the child class.
    virtual Action decide_action(float big_blind, float current_bet) = 0;

    void apply_action(Action action) {
        add_to_amount_bet(std::get<1>(action));
		remove_from_stack(std::get<1>(action));
    }

    Action act(float min_bet, float current_bet) {
        Action action = decide_action(min_bet, current_bet);
		apply_action(action);
        return action;
    }

    void set_cards(Cards cards) {
        pocket = cards;
    }

    void add_to_stack(float amount) {
        stack += amount;
    }

    void remove_from_stack(float amount) {
        if (amount > stack) {
            std::cout << "WARNING: removing more than " << name << " has in their stack..." << std::endl;
        }
        stack -= amount;
    }

    bool is_all_in() {
        return (stack == 0) && (has_folded == false) && (total_amount_bet > 0);
    }

    bool has_no_money() {
        return (stack == 0) && (total_amount_bet == 0);
    }

    void set_player_has_folded() {
        has_folded = true;
    }

    void unfold() {
        has_folded = false;
    }

    bool has_player_folded() {
        return has_folded;
    }

    void set_round_amount_bet(float amount) {
        round_amount_bet = amount;
    }

    void set_total_amount_bet(float amount) {
        total_amount_bet = amount;
    }

    void add_to_amount_bet(float bet) {
        total_amount_bet += bet;
        round_amount_bet += bet;
    }
    float get_stack() {
        return stack;
    }

    float get_total_amount_bet() {
        return total_amount_bet;
    }

    float get_round_amount_bet() {
        return round_amount_bet;
    }

    std::string get_name() const {
        return name;
    }

    Cards get_hole_cards() {
        return pocket;
    }

    bool actions_is_kind_of_bet(KindsOfAction action) {
        return action == KindsOfAction::CALL || action == KindsOfAction::RAISE || action == KindsOfAction::BET || action == KindsOfAction::ALL_IN;
    }

    void show_possible_actions(std::vector<KindsOfAction> actions) {
		std::cout << "Possible actions:" << std::endl;
		int i = 0;
		for (KindsOfAction pos_act : actions) {
			std::cout << "\t" << i << ") " << action_enum_2_string(pos_act) << std::endl;
			i++;
		};
    }

    float handle_call(float call_amount) {
		float amount_to_bet = call_amount - round_amount_bet;
		if (amount_to_bet >= stack) {
			std::cout << " --> Player is going all in!" << std::endl;
			return stack;
		} else {
			return amount_to_bet;
		}
	}

    void bet(float amount) {
        /* 
        Bets a certain amount for the player, 
        not allowing them to bet more than their stack.
        */
        float adjusted_amount = std::min(amount, stack);
        add_to_amount_bet(adjusted_amount);
		remove_from_stack(adjusted_amount);
    }
};

#endif