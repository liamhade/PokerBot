using namespace std;

#ifndef HAND_EVAL
#define HAND_EVAL

#include <tuple>
#include "CardHandler.h"
#include "PlayerInterface.h"


enum class KindsOfHand { 
    High_Card, 
    Pair,
    Two_Pair, 
    Three_Of_a_Kind, 
    Straight,
    Flush,
    Full_House,
    Four_Of_a_Kind,
    Straight_Flush,
    Royal_Flush 
};

enum class COMP_TYPE {
    GT,
    LT,
    EQ
};


string KindsOfHand_2_string(KindsOfHand hand);

struct HandRank {
    KindsOfHand kind_of_hand;
    Cards sorted_cards_inside_hand;
    Cards sorted_cards_outside_hand;
};

typedef tuple<Player*, Cards, HandRank> PlayerWithHandRank;

namespace HandEvaluator {
    Card get_high_card(Cards cards);
    HandRank get_cards_handrank(Cards five_hand);
    bool handrank_lte(HandRank hr1, HandRank hr2);
	tuple<Cards, HandRank> best_five_hand_out_of_seven(Cards seven_hand);
    tuple<Player, Cards, HandRank> player_with_best_hand(vector<Player> players, Cards community_cards);
    vector<vector<PlayerWithHandRank>> ranked_player_hands(vector<Player*> players, Cards community_cards);
};

#endif
