#include "../include/PlayerInterface.h"
#include <iostream>


class Bot : public Player {
public:
	Bot(float s, std::string n) : Player(s, n) {}

	void show_player_info() {
        // std::cout << "BOT   : " << "\033[1m" << name << "\033[0m" << std::endl;
        std::cout << "Stack : " << stack << std::endl;
        std::cout << "Hand  : " << get_cards_str(pocket) << std::endl << std::endl;
    };

	Action decide_action(float big_blind, float current_bet) {
		show_player_info();

		// std::vector<KindsOfAction> possible_actions = get_possible_actions_for_player(min_total_bet);
		// KindsOfAction action;
		float bet_delta = current_bet - round_amount_bet;
		
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
