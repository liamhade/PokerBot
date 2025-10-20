#include <algorithm>
#include <tuple>
#include "HandEvaluator.h"
#include "Player.h"

std::string KindsOfHand_2_string(KindsOfHand hand) {
    switch (hand) {
        case KindsOfHand::High_Card: 
            return "High Card";
        case KindsOfHand::Pair:
            return "Pair";
        case KindsOfHand::Two_Pair:
            return "Two Pair";
        case KindsOfHand::Three_Of_a_Kind: 
            return "Three Of a Kind";
        case KindsOfHand::Straight:
            return "Straight";
        case KindsOfHand::Flush:
            return "Flush";
        case KindsOfHand::Full_House:
            return "Full House";
        case KindsOfHand::Four_Of_a_Kind:
            return "Four Of a Kind";
        case KindsOfHand::Straight_Flush:
            return "Straight Flush";
        case KindsOfHand::Royal_Flush:
            return "Royal Flush";
        default:
            return "None";
    }
}

namespace HandEvaluator {
	bool card_value_lt(Card c1, Card c2) {
		return static_cast<int>(get_card_value(c1)) < static_cast<int>(get_card_value(c2));
	}

	int num_of_value_in_hand(CardValue value, Cards cards) {
		int count = 0;
		for (Card c : cards) {
			if (get_card_value(c) == value) {
				count++;
			}
		}
		return count;
	}
	
	bool is_card_value_in_hand(Cards cards, CardValue value) {
		return num_of_value_in_hand(value, cards) > 0;
	}
	
	bool card_values_ascend(Cards cards) {
		std::sort(cards.begin(), cards.end(), card_value_lt);
		for (int i=0; i < cards.size()-1; i++) {
			CardValue v1 = get_card_value(cards[i]);
			CardValue v2 = get_card_value(cards[i+1]);
			if (static_cast<int>(v1)+1 != static_cast<int>(v2)) {
				return false;
			}
		}
		return true;
	}
	
	std::vector<Cards> all_five_hands_from_seven(Cards seven_card_hand) {
		std::vector<Cards> five_hands;
		std::vector<int> indexes = {0,0,1,1,1,1,1};
		do {
			std::vector<Card> five_hand;
			for (int i=0; i<indexes.size(); i++) {
				if (indexes[i]) five_hand.push_back(seven_card_hand[i]);
			} 
			five_hands.push_back(five_hand);
		} while (std::next_permutation(indexes.begin(), indexes.end()));
		return five_hands;
	}
	
	bool handrank_lte(HandRank hr1, HandRank hr2) {
		KindsOfHand k1 = std::get<0>(hr1);
		KindsOfHand k2 = std::get<0>(hr2);
		
		if (static_cast<int>(k1) < static_cast<int>(k2)) {
			// First hand is ranked lower than second.
			return true;
		} else if (static_cast<int>(k1) == static_cast<int>(k2)) {
			// Both hands are of the same kind
			CardValue high_value1 = std::get<0>(std::get<1>(hr1));
			CardValue high_value2 = std::get<0>(std::get<1>(hr2));
	
			if (static_cast<int>(high_value1) <= static_cast<int>(high_value2)) {
				// High card in the first hand is less than or equal to the high card in the second hand
				return true;
			}  else {
				// High card in the first hand is more than the high card in the second hand
				return false;
			}
		} else {
			// First hand is ranked higher than second
			return false;
		}
	}
	
	bool all_cards_have_same_suit(Cards cards) {
		if (cards.empty() || cards.size() == 1) {
			return true;
		}
		Suit s = get_card_suit(cards[0]);
		for (int i=1; i<cards.size(); i++) {
			if (get_card_suit(cards[i]) != s) {
				return false;
			}
		}
		return true;
	}

	bool is_royal_flush(Cards five_hand) {
		if (!is_card_value_in_hand(five_hand, CardValue::Ten)) {
			return false;
		}
		if (!is_card_value_in_hand(five_hand, CardValue::Jack)) {
			return false;
		}
		if (!is_card_value_in_hand(five_hand, CardValue::Queen)) {
			return false;
		}
		if (!is_card_value_in_hand(five_hand, CardValue::King)) {
			return false;
		}
		if (!is_card_value_in_hand(five_hand, CardValue::Ace)) {
			return false;
		}

		if (!all_cards_have_same_suit(five_hand)){
			return false;
		}
		return true;
	}
	
	bool is_straight_flush(Cards five_hand) {
		if (card_values_ascend(five_hand) && all_cards_have_same_suit(five_hand)) {
			return true;
		}
		return false;
	}
	
	bool is_four_of_a_kind(Cards five_hand) {
		for (Card c: five_hand) {
			if (num_of_value_in_hand(get_card_value(c), five_hand) == 4) {
				return true;
			}
		}
		return false;
	}
	
	bool is_full_house(Cards five_hand) {
		bool has_pair = false;
		bool has_three_of_a_kind = false;
		
		for (Card c: five_hand) {
			if (num_of_value_in_hand(get_card_value(c), five_hand) == 2) {
				has_pair = true;
			} else if (num_of_value_in_hand(get_card_value(c), five_hand) == 3) {
				has_three_of_a_kind = true;
			}
		}
		return has_pair && has_three_of_a_kind;
	}
	
	bool is_flush(Cards five_hand) {
		return all_cards_have_same_suit(five_hand);
	}
	
	bool card_values_in_hand(std::vector<CardValue> card_values, Cards hand) {
		for (CardValue cv : card_values) {
			if (!is_card_value_in_hand(hand, cv)) {
				return false;
			}
		}
		return true;
	}
	
	bool is_straight(Cards five_hand) {
		if (card_values_ascend(five_hand)) {
			return true;
		}
	
		// Need to check if we have a straight starting from Ace.
		std::vector<CardValue> values = { CardValue::Ace, CardValue::Two, CardValue::Three, CardValue::Four, CardValue::Five };
		if (card_values_in_hand(values, five_hand)) {
			return true;
		}
		
		return false;
	}
	
	bool is_three_of_a_kind(Cards five_hand) {
		for (Card c : five_hand) {
			if (num_of_value_in_hand(get_card_value(c), five_hand) == 3) {
				return true;
			}
		}
		return false;
	}
	
	bool is_two_pair(Cards five_hand) {
		for (Card c1 : five_hand) {
			if (num_of_value_in_hand(get_card_value(c1), five_hand) == 2) {
				// Found first distinct pair
				for (Card c2 : five_hand) {
					if (num_of_value_in_hand(get_card_value(c2), five_hand) == 2 && get_card_suit(c1) != get_card_suit(c2)) {
						// Found second distinct pair
						return true;
					}
				}
			}
		}
		return false;
	} 
	
	bool is_pair(Cards five_hand) {
		for (Card c : five_hand) {
			if (num_of_value_in_hand(get_card_value(c), five_hand) == 2) {
				return true;
			}
		}
		return false;
	}
	
	Card get_high_card(Cards cards) {
		std::sort(cards.begin(), cards.end(), card_value_lt);
		return cards[cards.size()-1];
	}

		HandRank get_cards_handrank(Cards five_cards) {
		KindsOfHand kind_of_hand;
		Card high_card;

		if (is_royal_flush(five_cards)) {
			kind_of_hand = KindsOfHand::Royal_Flush;
		} else if (is_straight_flush(five_cards)) {
			kind_of_hand = KindsOfHand::Straight_Flush;
		} else if (is_four_of_a_kind(five_cards)) {
			kind_of_hand = KindsOfHand::Four_Of_a_Kind;
		} else if (is_full_house(five_cards)) {
			kind_of_hand = KindsOfHand::Full_House;
		} else if (is_flush(five_cards)) {
			kind_of_hand = KindsOfHand::Flush;
		} else if (is_straight(five_cards)) {
			kind_of_hand = KindsOfHand::Straight;
		} else if (is_three_of_a_kind(five_cards)) {
			kind_of_hand = KindsOfHand::Three_Of_a_Kind;
		} else if (is_two_pair(five_cards)) {
			kind_of_hand = KindsOfHand::Two_Pair;
		} else if (is_pair(five_cards)) {
			kind_of_hand = KindsOfHand::Pair;
		} else {
			kind_of_hand = KindsOfHand::High_Card;
		}
		high_card = get_high_card(five_cards);
		return { kind_of_hand, high_card };
	}

	std::tuple<Cards, HandRank> best_five_hand_out_of_seven(Cards seven_hand) {
		std::vector<Cards> all_five_hands = all_five_hands_from_seven(seven_hand);
		Cards    best_five_hand = all_five_hands[0]; // Initializing the best five card hand
		HandRank best_hand_rank = get_cards_handrank(best_five_hand); 
		HandRank new_handrank;
		for (Cards hand : all_five_hands) {
			new_handrank = get_cards_handrank(hand);
			if (handrank_lte(best_hand_rank, new_handrank)) {
				// New best hand found
				best_five_hand = hand;
				best_hand_rank = new_handrank;
			}
		}
		return {best_five_hand, best_hand_rank};
	}

	std::tuple<Player, Cards, HandRank> player_with_best_hand(std::vector<Player> players, Cards community_cards) {
		// Since we set our own constructors for the Player,
		// we can't rely on the compiler to choose a default constructor.
		int tuple_initialized = false;
		Player best_player = Player(0, "none");
		HandRank best_handrank;
		Cards best_cards;

		for (Player p : players) {
			if (!p.has_player_folded()) {
				Cards seven_hand = community_cards;
				for (Card c : p.get_hole_cards()) { seven_hand.push_back(c); };
				std::tuple<Cards, HandRank> players_handrank = best_five_hand_out_of_seven(seven_hand);
			
				if (!tuple_initialized) {
					// Setting up baseline of comparison
					best_player = p;
					best_cards = std::get<0>(players_handrank);
					best_handrank = std::get<1>(players_handrank);
					tuple_initialized = true;
				} else {
					// Found a new best hand
					if (handrank_lte(best_handrank, std::get<1>(players_handrank))) {
						best_player = p;
						best_cards = std::get<0>(players_handrank);
						best_handrank = std::get<1>(players_handrank);
					}
				}
			}
		}
		return std::make_tuple(best_player, best_cards, best_handrank);
	}
};
