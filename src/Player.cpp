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

Action Player::get_action(std::vector<KindsOfAction> possible_actions, float min_total_bet) {
	float bet_amount = 0;
	KindsOfAction user_action;
	
	prompt_user_with_options(possible_actions,  min_total_bet);
	user_action = get_user_action_type(possible_actions);            
	if (actions_is_kind_of_bet(user_action)) {
		if (user_action == KindsOfAction::CALL) {
			bet_amount = min_total_bet - round_amount_bet;
			std::cout << " --> Calling by betting $" << bet_amount << std::endl;
		} else {
			bet_amount = get_user_bet_amount(min_total_bet);
		}
        add_to_amount_bet(bet_amount);
		remove_from_stack(bet_amount);
	}
	printf("\n");
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

void Player::set_round_amount_bet(float amount) {
	round_amount_bet = amount;
}

void Player::add_to_amount_bet(float bet) {
	total_amount_bet += bet;
	round_amount_bet += bet;
}

float Player::get_total_amount_bet() {
	return total_amount_bet;
}

float Player::get_round_amount_bet() {
	return round_amount_bet;
}

std::string Player::get_name() {
	return name;
}

Cards Player::get_hole_cards() {
	return pocket;
}

void Player::prompt_user_with_options(std::vector<KindsOfAction> possible_actions,float minimum_bet) {
	Player::show_player_info();
	std::cout << "Minimum total bet this round: " << minimum_bet << std::endl;
	std::cout << "Amount player bet this round: " << round_amount_bet << std::endl;
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

float Player::get_user_bet_amount(float current_min_bet) {
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

void Player::show_possible_actions(std::vector<KindsOfAction> actions) {
	std::cout << "Possible actions:" << std::endl;
	int i = 0;
	for (KindsOfAction pos_act : actions) {
		std::cout << "\t" << i << ") " << action_enum_2_string(pos_act) << std::endl;
		i++;
	};
}