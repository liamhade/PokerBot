#include "../include/PlayerInterface.h"
#include <iostream>


class Bot : public Player {
public:
	Bot(float s, std::string n) : Player(s, n) {}

	Action get_action(float min_total_bet) {
		Player::show_player_info();

		std::vector<KindsOfAction> possible_actions = get_possible_actions_for_player(min_total_bet);
		KindsOfAction action;
		float bet_amount = 0;
		
		// if (actions_is_kind_of_bet(action)) {
        //     // For testing purposes
        //     bet_amount = 1;
		// } else {
		// 	bet_amount = 0;
		// }
		return std::make_tuple(KindsOfAction::CHECK, bet_amount);
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
