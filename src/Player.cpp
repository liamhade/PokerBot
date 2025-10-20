#include "Player.h"
#include <iostream>

std::string action_enum_2_string(KindsOfAction action) {
    if (action == KindsOfAction::CHECK) {
        return "CHECK";
    } else if (action == KindsOfAction::BET) {
        return "BET";
    } else if (action == KindsOfAction::CALL) {
        return "CALL";
    } else if (action == KindsOfAction::RAISE) {
        return "RAISE";
    } else {
        return "FOLD";
    };
}

Player::Player(float starting_stack, std::string player_name) {
	stack = starting_stack;
	name = player_name;
	has_folded = false;
}

void Player::show_player_info() {
	std::cout << "Player: " << name << std::endl;
	std::cout << "Stack: " << stack << std::endl;
	std::cout << "Hand : " << get_cards_str(pocket) << std::endl;
}

Action Player::get_action(std::vector<KindsOfAction> possible_actions, float current_min_bet) {
	float bet_amount = 0;
	KindsOfAction user_action;
	
	prompt_user_with_options(possible_actions,  current_min_bet);
	user_action = get_user_action_type(possible_actions);            
	if (actions_is_kind_of_bet(user_action)) {
		get_user_bet_amount(&bet_amount, current_min_bet);
	}
	printf("\n");
	remove_from_stack(bet_amount);
	return std::make_tuple(user_action, bet_amount);
} 

void Player::set_player_has_folded() {
	has_folded = true;
}

bool Player::has_player_folded() {
	return has_folded;
}

void Player::remove_from_stack(float amount) {
	if (amount > stack) {
		std::cout << "WARNING: removing more than " << name << " has in their stack..." << std::endl;
	}
	stack -= amount;
}

void Player::set_cards(Cards cards) {
	pocket = cards;
}

void Player::add_to_amount_bet(float bet) {
	amount_bet += bet;
}

float Player::get_amount_bet() {
	return amount_bet;
}

std::string Player::get_name() {
	return name;
}

Cards Player::get_hole_cards() {
	return pocket;
}

void Player::prompt_user_with_options(std::vector<KindsOfAction> possible_actions,float minimum_bet) {
	Player::show_player_info();
	std::cout << "Minimum bet: " << minimum_bet << std::endl;
	show_possible_actions(possible_actions);
}

KindsOfAction Player::get_user_action_type(std::vector<KindsOfAction> possible_actions) {
	int user_action_num;
	std::cout << "Enter the number of your action: ";
	std::cin >> user_action_num;
	return possible_actions[user_action_num];
}

bool Player::actions_is_kind_of_bet(KindsOfAction action) {
	return action == KindsOfAction::CALL || action == KindsOfAction::RAISE || action == KindsOfAction::BET;
}

void Player::get_user_bet_amount(float* bet_amount, float current_min_bet) {
	if (current_min_bet >= stack) {
		// If the current minimum bet is >= players stack, then we just
		// make the bet everything the player has
		std::cout << "\tMinimum bet is " << current_min_bet << ", but player only has " << stack << std::endl;
		std::cout << "\tBetting players entire stack..." << std::endl;
		*bet_amount = stack;
	} else {
		// The player has more money in their stack the current minimum bet
		float user_bet;
		while (1) {
			std::cout << "How much would you like to bet: ";
			std::cin >> user_bet;
			
			if (user_bet < current_min_bet) {
				std::cout << "\tUse must bet atleast " << current_min_bet << " dollars..." << std::endl; 
				continue;
			} else {
				break;
			}
		}
		*bet_amount = user_bet;
	}
}

void Player::show_possible_actions(std::vector<KindsOfAction> actions) {
	std::cout << "Possible actions:" << std::endl;
	int i = 0;
	for (KindsOfAction pos_act : actions) {
		std::cout << "\t" << i << ") " << action_enum_2_string(pos_act) << std::endl;
		i++;
	};
}