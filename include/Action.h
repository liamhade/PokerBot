#ifndef ACTION
#define ACTIOn
#include <iostream>

enum class KindsOfAction {
    CHECK,
    BET,
    CALL,
    ALL_IN,
    RAISE,
    FOLD,
};

using Action = std::tuple<KindsOfAction, float>;

void show_action(Action action);

std::string action_enum_2_string(KindsOfAction action);

#endif