#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>
#include "HandEvaluator.h"
#include "CardHandler.h"
#include "DeckHandler.h"
#include "Player.h"

std::vector<KindsOfAction> get_possible_actions(Action last_action) {
    KindsOfAction last_kind_of_action = std::get<0>(last_action);
    switch (last_kind_of_action) {
        case KindsOfAction::CHECK:
        case KindsOfAction::FOLD:
            return { KindsOfAction::CHECK, KindsOfAction::FOLD, KindsOfAction::BET };
        case KindsOfAction::BET:
        case KindsOfAction::CALL:
        case KindsOfAction::RAISE:
            return { KindsOfAction::CALL, KindsOfAction::RAISE };
    }
}

int main() {
    DeckHandler deck;

    const Card c1 = deck.draw_card();
    const Card c2 = deck.draw_card();
    const Card c3 = deck.draw_card();
    const Card c4 = deck.draw_card();
    
    Player p1(100, "Kenny Rogers", { c1, c2 });
    Player p2(100, "Dolly Parton", { c3, c4 });

    std::vector<KindsOfAction> possible_actions = { 
        KindsOfAction::CHECK,
        KindsOfAction::BET,         
        KindsOfAction::FOLD,
    };

    float current_min_bet = 0;

    // First player goes
    Action p1_action = p1.get_action(possible_actions, current_min_bet);

    // Updating the minimum bet
    current_min_bet = std::get<1>(p1_action);
    possible_actions = get_possible_actions(p1_action);

    // Next player goes
    Action p2_action = p2.get_action(possible_actions, current_min_bet);
    
    // The game ends when one player folds, or, if both players bet the same amount


    return 0;
}