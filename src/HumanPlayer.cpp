#include "../include/PlayerInterface.h"
#include <iostream>


class Human : public Player {
public:
	Human(float s, std::string n) : Player(s, n) {}

	void show_player_info() {
        std::cout << "USER  : " << "\033[1m" << name << "\033[0m" << std::endl;
        std::cout << "Stack : " << stack << std::endl;
        std::cout << "Hand  : " << get_cards_str(pocket) << std::endl << std::endl;
    };

	Action get_action(float min_total_bet) {
		std::vector<KindsOfAction> possible_actions = get_possible_actions_for_player(min_total_bet);
		KindsOfAction user_action;
		float bet_amount;
		
		prompt_user_with_options(possible_actions,  min_total_bet);
		user_action = get_user_action_type(possible_actions);    
		if (actions_is_kind_of_bet(user_action)) {
			bet_amount = handle_betting(user_action, min_total_bet);
		} else {
			bet_amount = 0;
		}
		printf("\n");
		return std::make_tuple(user_action, bet_amount);
	} 

	float handle_betting(KindsOfAction user_action, float min_total_bet) {
		float bet_amount = 0;

		if (user_action == KindsOfAction::CALL) {
			bet_amount = handle_call(min_total_bet); 
		} else if (user_action == KindsOfAction::ALL_IN) {
			bet_amount = stack;
		} else {
			bet_amount = get_bet_amount(min_total_bet);
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

	void prompt_user_with_options(std::vector<KindsOfAction> possible_actions, float minimum_bet) {
		show_player_info();
		std::cout << "Minimum total bet this round: " << minimum_bet << std::endl;
		std::cout << "Amount player bet this round: " << round_amount_bet << std::endl << std::endl;
		show_possible_actions(possible_actions);
	}

	KindsOfAction get_user_action_type(std::vector<KindsOfAction> possible_actions) {
		int user_action_num;
		std::cout << "Enter the number of your action: ";
		std::cin >> user_action_num;
		return possible_actions[user_action_num];
	}

	float get_bet_amount(float current_min_bet) {
		float players_min_bet = current_min_bet - round_amount_bet;

		if (players_min_bet >= stack) {
			// If the current minimum bet is >= players stack, then we just
			// make the bet everything the player has
			std::cout << "\tMinimum bet is $" << players_min_bet << ", but player only has $" << stack << std::endl;
			std::cout << "\tBetting players entire stack..." << std::endl;
			return stack;
		} else {
			// The player has more money in their stack the current minimum bet
			float user_bet;
			while (1) {
				std::cout << "How much would you like to bet ";
				std::cout << "(must be at least $" << players_min_bet << "): ";
				std::cin >> user_bet;
				
				if (user_bet < players_min_bet) {
					std::cout << "\tPlayer must bet at least $" << players_min_bet << "..." << std::endl; 
					continue;
				} else {
					break;
				}
			}
			return user_bet;
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
