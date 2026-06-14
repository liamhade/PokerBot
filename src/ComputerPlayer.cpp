#include "../include/PlayerInterface.h"
#include <iostream>


class Bot : public Player {
public:
	Bot(float s, std::string n) : Player(s, n) {}

	void show_player_info() {
        std::cout << "BOT   : " << "\033[1m" << name << "\033[0m" << std::endl;
        std::cout << "Stack : " << stack << std::endl;
        std::cout << "Hand  : " << get_cards_str(pocket) << std::endl << std::endl;
    };

	Action get_action(float min_total_bet) {
		show_player_info();

		// std::vector<KindsOfAction> possible_actions = get_possible_actions_for_player(min_total_bet);
		// KindsOfAction action;
		float bet_delta = min_total_bet - round_amount_bet;
		
		if (bet_delta > 0) {
			// We haven't bet the minimum amount
			return std::make_tuple(KindsOfAction::CALL, bet_delta);
		} else if (bet_delta == 0) {
			// We've already bet the minimum amount
			return std::make_tuple(KindsOfAction::CHECK, 0);
		} else {
			// We can't bet the minimum amount.
			return std::make_tuple(KindsOfAction::FOLD, 0);
		}
 	} 
	
	float get_bet_amount(float current_min_bet) {
		return current_min_bet;
	}

	float handle_betting(KindsOfAction user_action, float min_total_bet) {
		float bet_amount = 0;

		if (user_action == KindsOfAction::CALL) {
			bet_amount = handle_call(min_total_bet); 
		} else if (user_action == KindsOfAction::ALL_IN) {
			bet_amount = stack;
		} else {
			bet_amount = 1;
		}
		
		add_to_amount_bet(bet_amount);
		remove_from_stack(bet_amount);
		return bet_amount;
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
};
