#ifndef PLAYER_H
#define PLAYER_H

#include <tuple>
#include <vector>
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

    std::vector<KindsOfAction> get_possible_actions_for_player(float minimum_bet) {
        if (get_round_amount_bet() == minimum_bet) {
            // No bets to meet
            return { KindsOfAction::CHECK, KindsOfAction::BET, KindsOfAction::ALL_IN, KindsOfAction::FOLD };
        } else if (get_round_amount_bet() < minimum_bet) {
            // Player needs to atleast match the minimum bet, or fold.
            float bet_delta = minimum_bet - get_round_amount_bet();
            if (stack > bet_delta) {
                return { KindsOfAction::CALL, KindsOfAction::RAISE, KindsOfAction::ALL_IN, KindsOfAction::FOLD };
            } else {
                return { KindsOfAction::ALL_IN, KindsOfAction::FOLD };
            }
        } else {
            // Should never happen
            throw 400;
        }
    }

    // Must be defined by the child class.
    virtual Action get_action(float current_min_bet) = 0;

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

    float virtual get_bet_amount(float min_total_bet) = 0;

    float handle_call(float call_amount) {
		float amount_to_bet = call_amount - round_amount_bet;
		if (amount_to_bet >= stack) {
			std::cout << " --> Player is going all in!" << std::endl;
			return stack;
		} else {
			return amount_to_bet;
		}
	}

   	float handle_betting(KindsOfAction action, float min_total_bet) {
		float bet_amount = 0;

		if (action == KindsOfAction::CALL) {
			bet_amount = handle_call(min_total_bet); 
		} else if (action == KindsOfAction::ALL_IN) {
			bet_amount = stack;
		} else {
			bet_amount = get_bet_amount(min_total_bet);
		}
		
		add_to_amount_bet(bet_amount);
		remove_from_stack(bet_amount);
		return bet_amount;
	}
};

#endif