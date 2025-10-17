#ifndef PLAYER_H
#define PLAYER_H

#include <tuple>
#include <vector>
#include "CardHandler.h"

enum class KindsOfAction {
    CHECK,
    BET,
    CALL,
    RAISE,
    FOLD,
};

std::string action_enum_2_string(KindsOfAction action);

using Action = std::tuple<KindsOfAction, float>;

class Player {
public:
    Player(float starting_stack, std::string player_name, Cards random_two_cards);

    void show_player_info();

    Action get_action(std::vector<KindsOfAction> possible_actions, float current_min_bet);
    
    void remove_from_stack(float amount);
    
    void prompt_user_with_options(std::vector<KindsOfAction> possible_actions,float minimum_bet);

private:
    std::string name;
    float stack;
    Cards pocket;
    bool has_folded;

    KindsOfAction get_user_action_type(std::vector<KindsOfAction> possible_actions);

    bool actions_is_kind_of_bet(KindsOfAction action);

    void get_user_bet_amount(float* bet_amount, float current_min_bet);

    void show_possible_actions(std::vector<KindsOfAction> actions);
};

#endif