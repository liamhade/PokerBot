#include "../include/Action.h"


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