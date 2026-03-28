#ifndef PLAYER_H
#define PLAYER_H

#include <tuple>
#include <vector>
#include "CardHandler.h"

enum class KindsOfAction {
    CHECK,
    BET,
    CALL,
    ALL_IN,
    RAISE,
    FOLD,
};

std::string action_enum_2_string(KindsOfAction action);

using Action = std::tuple<KindsOfAction, float>;

class Player {
public:
    Player(float starting_stack, std::string player_name);

    void show_player_info();

    std::vector<KindsOfAction> get_possible_actions_for_player(float minimum_bet);

    Action get_action(float current_min_bet);

	void set_cards(Cards cards);

    void add_to_stack(float amount);

    void remove_from_stack(float amount);
    
    void prompt_user_with_options(std::vector<KindsOfAction> possible_actions,float minimum_bet);

    bool is_all_in();

    bool has_no_money();

    bool has_player_folded();

    void unfold();

    void set_player_has_folded();

    void set_round_amount_bet(float amount);

    void set_total_amount_bet(float amount);

    void add_to_amount_bet(float bet);

    float get_stack();

    float get_total_amount_bet();

    float get_round_amount_bet();

    std::string get_name() const;
    
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

    float handle_betting(KindsOfAction user_action, float min_total_bet);

    float handle_call(float call_amount);
};

#endif