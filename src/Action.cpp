#include <tuple>
#include <iostream>
#include "../include/Action.h"
using namespace std;

std::string action_enum_2_string(KindsOfAction action) {
    switch (action) {
		case KindsOfAction::CHECK:
			return "CHECK";
		case KindsOfAction::BET:
			return "BET";
		case KindsOfAction::CALL:
			return "CALL";
		case KindsOfAction::RAISE:
			return "RAISE";
		case KindsOfAction::ALL_IN:
			return "ALL IN";
		case KindsOfAction::FOLD:
			return "FOLD";
		default:
			return "ERROR";
	}
}

void show_action(Action action) {
	/* Prints the Action to the screen. */
	KindsOfAction action_kind = get<0>(action);
	float bet_amount          = get<1>(action);
	cout << "\t Action: " << action_enum_2_string(action_kind) << ", Bet amount: " << bet_amount << "\n" << endl; 
}
