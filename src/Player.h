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
    Player(float starting_stack, std::string player_name);

    void show_player_info();

    Action get_action(std::vector<KindsOfAction> possible_actions, float current_min_bet);

	void set_cards(Cards cards);

    void remove_from_stack(float amount);
    
    void prompt_user_with_options(std::vector<KindsOfAction> possible_actions,float minimum_bet);

    bool has_player_folded();

    void set_player_has_folded();

    void set_round_amount_bet(float amount);

    void add_to_amount_bet(float bet);

    float get_total_amount_bet();

    float get_round_amount_bet();

    std::string get_name();

    Cards get_hole_cards();

private:
    std::string name;
    float stack;
    float total_amount_bet = 0;
    float round_amount_bet = 0;
    Cards pocket;
    bool has_folded = false;
    
    KindsOfAction get_user_action_type(std::vector<KindsOfAction> possible_actions);

    bool actions_is_kind_of_bet(KindsOfAction action);

    float get_user_bet_amount(float current_min_bet);

    void show_possible_actions(std::vector<KindsOfAction> actions);
};

#endif