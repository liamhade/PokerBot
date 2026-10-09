#include "../include/PlayerInterface.h"
#include "../include/PredictionEngine.h"
#include <iostream>
#include <algorithm>


class Bot : public Player {
public:
	Bot(float s, std::string n) : Player(s, n) {}

	void show_player_info() {
        // std::cout << "BOT   : " << "\033[1m" << name << "\033[0m" << std::endl;
        std::cout << "Stack : " << stack << std::endl;
        std::cout << "Hand  : " << get_cards_str(pocket) << std::endl << std::endl;
    };

	float _get_bet_size(float big_blind) {
		float stack_fraction = 0.5;
		float bet_size = stack*stack_fraction;
		if (stack < big_blind) {
			return stack;
		} else if (bet_size < big_blind) {
			return big_blind;
		} 
		return bet_size;
	}

	Action decide_action(
		float big_blind, 
		float current_bet,
		Cards community_cards,
		int total_num_players
	) {
		show_player_info();

		float bet_delta = current_bet - round_amount_bet;

		if (bet_delta > 0) {
			// We haven't bet the minimum amount
			return std::make_tuple(KindsOfAction::CALL, bet_delta);
		} else if (bet_delta == 0) {
			double wl = win_likelihood(pocket, community_cards, total_num_players, 250);
			std::cout << "[the bot believes it has a " << wl << "\% chance of winning]\n\n";												
			if (wl > 0.6) {
				return std::make_tuple(KindsOfAction::BET, _get_bet_size(big_blind));
			}
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
