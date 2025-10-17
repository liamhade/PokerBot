#ifndef HAND_EVAL
#define HAND_EVAL

#include <tuple>
#include "CardHandler.h"

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
using HandRank = std::tuple<KindsOfHand, Card>;

namespace HandEvaluator {
	Card get_high_card(Cards cards);
	std::tuple<Cards, HandRank> best_five_hand_out_of_seven(Cards seven_hand);
};

#endif
