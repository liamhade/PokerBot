#include "../include/PlayerInterface.h"
#include "../include/InputHandler.h"
#include <iostream>
#include <algorithm>
#include <limits>

class Human : public Player {
public:
	Human(float s, std::string n) : Player(s, n) {}

	void show_player_info() {
        // std::cout << "USER  : " << "\033[1m" << name << "\033[0m" << std::endl;
        std::cout << "Stack : " << stack << std::endl;
        std::cout << "Hand  : " << get_cards_str(pocket) << std::endl << std::endl;
    };

	Action decide_action( 
		float big_blind, 
		float current_bet,
		Cards community_cards,
		int total_num_players
	) {
		KindsOfAction user_action;
		float bet_amount;
		std::vector<KindsOfAction> possible_actions = get_possible_actions_for_player(current_bet);
		
		prompt_user_with_options(possible_actions,  current_bet);
		user_action = get_user_action_type(possible_actions);    
		if (actions_is_kind_of_bet(user_action)) {
			bet_amount = handle_bet_action(user_action, big_blind, current_bet);
		} else {
			bet_amount = 0;
		}
		printf("\n");
		return std::make_tuple(user_action, bet_amount);
	} 

	float handle_bet_action(KindsOfAction user_action, float big_blind, float current_bet) {
		/*
		Helper function for when the user wants to bet.
		*/
		float bet_amount = 0;

		if (user_action == KindsOfAction::CALL) {
			bet_amount = handle_call(current_bet); 
		} else if (user_action == KindsOfAction::ALL_IN) {
			bet_amount = stack;
		} else {
			bet_amount = get_bet_amount(big_blind, current_bet);
		}
		
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

	void prompt_user_with_options(std::vector<KindsOfAction> possible_actions, float minimum_bet) {
		show_player_info();
		std::cout << "Minimum total bet this round: " << minimum_bet << std::endl;
		std::cout << "Amount player bet this round: " << round_amount_bet << std::endl << std::endl;
		show_possible_actions(possible_actions);
	}

	KindsOfAction get_user_action_type(std::vector<KindsOfAction> possible_actions) {
		std::string input; 
		int user_action_num;
		
		while (1) {
			std::cout << "Enter the number of your action: ";
			std::cin >> input;

			if (!(is_valid_int(input))) {
				clear_cin();
				std::cout << "\n\t [WARNING] Input must be a valid integer! \n\n";
				continue;
			}
			user_action_num = std::stoi(input);
			if ((user_action_num < 0) || (possible_actions.size()-1 < user_action_num)) {
				clear_cin();
				std::cout << "\n\t [WARNING] Inputted action is not a valid option! \n\n";
				continue;
			} 
			break;
		}
		return possible_actions[user_action_num];
	}

	float get_bet_amount(float big_blind, float current_bet) {
		/* 
		Assumes that a player wants to bet, and retrieves that amount. 
		*/
		float bet_needed_to_stay_in_game = current_bet - round_amount_bet;

		if (bet_needed_to_stay_in_game >= stack) {
			std::cout << "\tMinimum bet is $" << bet_needed_to_stay_in_game << ", but player only has $" << stack << std::endl;
			std::cout << "\tBetting players entire stack..." << std::endl;
			return stack;
		} else {
			float user_bet;
			float min_bet = std::min({bet_needed_to_stay_in_game, big_blind});
			while (1) {
				std::cout << "How much would you like to bet: ";
				std::cin >> user_bet;
				if (user_bet < min_bet) {
					std::cout << "\tIllegal move: must bet at least $" << min_bet << "..." << std::endl; 
					continue;
				}
				return user_bet;
			}
		}
	}

	void show_possible_actions(std::vector<KindsOfAction> actions) {
		std::cout << "Possible actions:" << std::endl;
		int i = 0;
		for (KindsOfAction pos_act : actions) {
			std::cout << "\t" << i << ") " << action_enum_2_string(pos_act) << std::endl;
			i++;
		};
	}
};
