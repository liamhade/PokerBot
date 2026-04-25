#include <algorithm>
#include <stdexcept>
#include <tuple>
#include "../include/HandEvaluator.h"
#include "../include/PlayerInterface.h"

using namespace std;

string KindsOfHand_2_string(KindsOfHand hand) {
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
	bool card_value_gt(Card c1, Card c2) {
		return static_cast<int>(get_card_value(c1)) > static_cast<int>(get_card_value(c2));
	}

	bool card_value_lt(Card c1, Card c2) {
		return static_cast<int>(get_card_value(c1)) < static_cast<int>(get_card_value(c2));
	}

	void sort_cards(Cards cards) {
		sort(cards.begin(), cards.end(), card_value_gt);
	}

	Cards cards_with_same_value_in_hand(CardValue value, Cards hand) {
		Cards cards;
		for (Card c : hand) {
			if (get_card_value(c) == value) {
				cards.push_back(c);
			}
		} 
		return cards;
	}
	
	bool is_card_value_in_hand(Cards cards, CardValue value) {
		return cards_with_same_value_in_hand(value, cards).size() > 0;
	}
	
	bool card_values_ascend(Cards cards) {
		sort(cards.begin(), cards.end(), card_value_lt);
		for (int i=0; i < cards.size()-1; i++) {
			CardValue v1 = get_card_value(cards[i]);
			CardValue v2 = get_card_value(cards[i+1]);
			if (static_cast<int>(v1)+1 != static_cast<int>(v2)) {
				return false;
			}
		}
		return true;
	}
	
	vector<Cards> all_five_hands_from_seven(Cards seven_card_hand) {
		vector<Cards> five_hands;
		vector<int> indexes = {0,0,1,1,1,1,1};
		do {
			vector<Card> five_hand;
			for (int i=0; i<indexes.size(); i++) {
				if (indexes[i]) five_hand.push_back(seven_card_hand[i]);
			} 
			five_hands.push_back(five_hand);
		} while (next_permutation(indexes.begin(), indexes.end()));
		return five_hands;
	}

	COMP_TYPE compare_cards(Cards c1s, Cards c2s) {
		if (c1s.size() != c2s.size()) {
			throw invalid_argument("Card vectors must have the same size!");
		}

		for (int i=0; i < c1s.size(); i++) {
			if (card_value_lt(c1s[i], c2s[i])) {
				return COMP_TYPE::LT;
			} else if (card_value_gt(c1s[i], c2s[i])) {
				return COMP_TYPE::GT;
			}
		}
		return COMP_TYPE::EQ;
	}

	COMP_TYPE compare_hands(KindsOfHand h1, KindsOfHand h2) {
		if (static_cast<int>(h1) < static_cast<int>(h2)) {
			return COMP_TYPE::LT;
		} else if (static_cast<int>(h1) > static_cast<int>(h2)) {
			return COMP_TYPE::GT;
		} else {
			return COMP_TYPE::EQ;
		}
	}

	COMP_TYPE compare_handrank(HandRank hr1, HandRank hr2) {
		// Comparing the kinds of hand
		switch (compare_hands(hr1.kind_of_hand, hr2.kind_of_hand)) {
			case COMP_TYPE::LT:
				return COMP_TYPE::LT;
			case COMP_TYPE::GT:
				return COMP_TYPE::GT;
			default:
				switch (compare_cards(hr1.sorted_cards_inside_hand, hr2.sorted_cards_inside_hand)) {
					case COMP_TYPE::LT:
						return COMP_TYPE::LT;
					case COMP_TYPE::GT:
						return COMP_TYPE::GT;
					default:
						// Comparing cards OUTSIDE the hand to break the tie
						switch (compare_cards(hr1.sorted_cards_outside_hand, hr2.sorted_cards_outside_hand)) {
							case COMP_TYPE::LT:
								return COMP_TYPE::LT;
							case COMP_TYPE::GT:
								return COMP_TYPE::GT;
							default:
								return COMP_TYPE::EQ;
						};
				};
		}
	}

	bool handrank_lte(HandRank hr1, HandRank hr2) {
		switch (compare_handrank(hr1, hr2)) {
			case COMP_TYPE::GT:
				return false;
			default:
				return true;
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

	Cards cards_not_in_hand(Cards five_hand, Cards cards_in_hand) {
		Cards cards_not_in_hand;
		for (Card c : five_hand) {
			if (find(cards_in_hand.begin(), cards_in_hand.end(), c) == cards_in_hand.end()) {
				cards_not_in_hand.push_back(c);
			} 
		} 
		return cards_not_in_hand;
	}

	tuple<bool, HandRank> get_royal_flush_handrank(Cards five_hand) {
		HandRank royal_flush_handrank;
		
		if (!is_card_value_in_hand(five_hand, CardValue::Ten)) {
			return make_tuple(false, royal_flush_handrank);
		}
		if (!is_card_value_in_hand(five_hand, CardValue::Jack)) {
			return make_tuple(false, royal_flush_handrank);
		}
		if (!is_card_value_in_hand(five_hand, CardValue::Queen)) {
			return make_tuple(false, royal_flush_handrank);
		}
		if (!is_card_value_in_hand(five_hand, CardValue::King)) {
			return make_tuple(false, royal_flush_handrank);
		}
		if (!is_card_value_in_hand(five_hand, CardValue::Ace)) {
			return make_tuple(false, royal_flush_handrank);
		}
		if (!all_cards_have_same_suit(five_hand)){
			return make_tuple(false, royal_flush_handrank);
		}
		
		royal_flush_handrank.kind_of_hand = KindsOfHand::Royal_Flush;
		royal_flush_handrank.sorted_cards_inside_hand = five_hand;
		royal_flush_handrank.sorted_cards_outside_hand = {};

		return make_tuple(true, royal_flush_handrank);
	}
	
	tuple<bool, HandRank> get_straight_flush_handrank(Cards five_hand) {
		HandRank straight_flush_handrank;

		if (card_values_ascend(five_hand) && all_cards_have_same_suit(five_hand)) {
			straight_flush_handrank.kind_of_hand = KindsOfHand::Straight_Flush;
			straight_flush_handrank.sorted_cards_inside_hand = five_hand;
			straight_flush_handrank.sorted_cards_outside_hand = {};
			return make_tuple(true, straight_flush_handrank);
		}
		return make_tuple(false, straight_flush_handrank);
	}
	
	tuple<bool, HandRank> get_four_of_a_kind_handrank(Cards five_hand) {
		HandRank four_of_a_kind_handrank;
		Cards cards_in_hand;
		Cards cards_with_same_value;

		for (Card c: five_hand) {
			cards_with_same_value = cards_with_same_value_in_hand(get_card_value(c), five_hand);
			if (cards_with_same_value.size() == 4) {
				for (Card c_ : cards_with_same_value) {cards_in_hand.push_back(c_);}
				four_of_a_kind_handrank.kind_of_hand = KindsOfHand::Four_Of_a_Kind;
				four_of_a_kind_handrank.sorted_cards_inside_hand = cards_in_hand;
				four_of_a_kind_handrank.sorted_cards_outside_hand = cards_not_in_hand(five_hand, cards_in_hand);
				return make_tuple(true, four_of_a_kind_handrank);
			}
		}
		return make_tuple(false, four_of_a_kind_handrank);
	}
	
	tuple<bool, HandRank> get_full_house_handrank(Cards five_hand) {
		HandRank full_house_handrank;
		Cards cards_in_hand;
		Cards cards_with_same_value;

		bool has_pair = false;
		bool has_three_of_a_kind = false;
			
		for (Card c: five_hand) {
			cards_with_same_value = cards_with_same_value_in_hand(get_card_value(c), five_hand);
			if (cards_with_same_value.size() == 2) {
				for (Card c_ : cards_with_same_value) {cards_in_hand.push_back(c_);}
				has_pair = true;
			} else if (cards_with_same_value.size() == 3) {
				for (Card c_ : cards_with_same_value) {cards_in_hand.push_back(c_);}
				has_three_of_a_kind = true;
			}
		}
		full_house_handrank.kind_of_hand = KindsOfHand::Full_House;
		full_house_handrank.sorted_cards_inside_hand = cards_in_hand;
		full_house_handrank.sorted_cards_outside_hand = cards_not_in_hand(five_hand, cards_in_hand);
		return make_tuple(has_pair && has_three_of_a_kind, full_house_handrank);
	}
	
	tuple<bool, HandRank> get_flush_handrank(Cards five_hand) {
		HandRank flush_handrank;
		if (all_cards_have_same_suit(five_hand)) {
			flush_handrank.kind_of_hand = KindsOfHand::Flush;
			flush_handrank.sorted_cards_inside_hand = five_hand;
			flush_handrank.sorted_cards_outside_hand = {};
			return make_tuple(true, flush_handrank);
		}
		return make_tuple(false, flush_handrank);
	}
	
	bool card_values_in_hand(vector<CardValue> card_values, Cards hand) {
		for (CardValue cv : card_values) {
			if (!is_card_value_in_hand(hand, cv)) {
				return false;
			}
		}
		return true;
	}
	
	tuple<bool, HandRank> get_straight_handrank(Cards five_hand) {
		HandRank straight_handrank;
		vector<CardValue> values = { CardValue::Ace, CardValue::Two, CardValue::Three, CardValue::Four, CardValue::Five };
		if (card_values_ascend(five_hand) || card_values_in_hand(values, five_hand)) {
			straight_handrank.kind_of_hand = KindsOfHand::Straight;
			straight_handrank.sorted_cards_inside_hand = five_hand;
			straight_handrank.sorted_cards_outside_hand = {};
			return make_tuple(true, straight_handrank);
		}
		return make_tuple(false, straight_handrank);
	}
	
	tuple<bool, HandRank> get_three_of_a_kind_handrank(Cards five_hand) {
		HandRank three_of_a_kind_handrank;
		Cards cards_in_hand;
		Cards cards_with_same_value;

		for (Card c : five_hand) {
			cards_with_same_value = cards_with_same_value_in_hand(get_card_value(c), five_hand);
			if (cards_with_same_value.size() == 3) {
				for (Card c_ : cards_with_same_value) {cards_in_hand.push_back(c_);}
				three_of_a_kind_handrank.kind_of_hand = KindsOfHand::Three_Of_a_Kind;
				three_of_a_kind_handrank.sorted_cards_inside_hand = cards_in_hand;
				three_of_a_kind_handrank.sorted_cards_outside_hand = cards_not_in_hand(five_hand, cards_in_hand);
				return make_tuple(true, three_of_a_kind_handrank);
			}
		}
		return make_tuple(false, three_of_a_kind_handrank);
	}
	
	tuple<bool, HandRank> get_two_pair_handrank(Cards five_hand) {
		HandRank two_pair_handrank;
		Cards cards_in_hand;
		Cards cards_with_same_value;
		
		for (Card c : five_hand) {
			cards_with_same_value = cards_with_same_value_in_hand(get_card_value(c), five_hand);
			if (cards_with_same_value.size() == 2) {
				// Found a pair.
				if (cards_in_hand.size() == 0) {
					// First pair we've found. 
					for (Card c_ : cards_with_same_value) {cards_in_hand.push_back(c_);}
				} else {
					if (get_card_value(c) == get_card_value(cards_in_hand[0])) {
						// Found the other card in the first pair.
						continue;
					} else {
						// Found second pair.
						for (Card c_ : cards_with_same_value) {cards_in_hand.push_back(c_);}
						two_pair_handrank.kind_of_hand = KindsOfHand::Two_Pair;
						two_pair_handrank.sorted_cards_inside_hand = cards_in_hand;
						two_pair_handrank.sorted_cards_outside_hand = cards_not_in_hand(five_hand, cards_in_hand);
						return make_tuple(true, two_pair_handrank);
					}
				}
			}
		}
		return make_tuple(false, two_pair_handrank);
	} 
	
	tuple<bool, HandRank> get_pair_handrank(Cards five_hand) {
		HandRank pair_handrank;
		Cards cards_in_hand;
		Cards cards_with_same_value;

		for (Card c : five_hand) {
			cards_with_same_value = cards_with_same_value_in_hand(get_card_value(c), five_hand);
			if (cards_with_same_value.size() == 2) {
				for (Card c_ : cards_with_same_value) {cards_in_hand.push_back(c_);}
				pair_handrank.kind_of_hand = KindsOfHand::Pair;
				pair_handrank.sorted_cards_inside_hand = cards_in_hand;
				pair_handrank.sorted_cards_outside_hand = cards_not_in_hand(five_hand, cards_in_hand);
				return make_tuple(true, pair_handrank);
			}
		}
		return make_tuple(false, pair_handrank);
	}
	
	tuple<bool, HandRank> get_high_card_handrank(Cards five_hand) {
		HandRank high_card_handrank;
		Cards cards_in_hand;
		
		// High-card will be the last card in the sorted five_hand
		sort_cards(five_hand);
		Card high_card = five_hand[five_hand.size()-1];

		high_card_handrank.kind_of_hand = KindsOfHand::High_Card;
		high_card_handrank.sorted_cards_inside_hand = { high_card };
		high_card_handrank.sorted_cards_outside_hand = cards_not_in_hand(five_hand, { high_card });

		return make_tuple(true, high_card_handrank);
	}

	HandRank get_cards_handrank(Cards five_cards) {
		sort(five_cards.begin(), five_cards.end(), card_value_gt); // sort in descending order.

		tuple<bool, HandRank> get_handrank;

		get_handrank = get_royal_flush_handrank(five_cards);
		if (get<0>(get_handrank)) {
			return get<1>(get_handrank);
		}

		get_handrank = get_straight_flush_handrank(five_cards);
		if (get<0>(get_handrank)) {
			return get<1>(get_handrank);
		}

		get_handrank = get_four_of_a_kind_handrank(five_cards);
		if (get<0>(get_handrank)) {
			return get<1>(get_handrank);
		}

		get_handrank = get_full_house_handrank(five_cards);
		if (get<0>(get_handrank)) {
			return get<1>(get_handrank);
		}

		get_handrank = get_flush_handrank(five_cards);
		if (get<0>(get_handrank)) {
			return get<1>(get_handrank);
		}

		get_handrank = get_straight_handrank(five_cards);
		if (get<0>(get_handrank)) {
			return get<1>(get_handrank);
		}

		get_handrank = get_three_of_a_kind_handrank(five_cards);
		if (get<0>(get_handrank)) {
			return get<1>(get_handrank);
		}

		get_handrank = get_two_pair_handrank(five_cards);
		if (get<0>(get_handrank)) {
			return get<1>(get_handrank);
		}
		
		get_handrank = get_pair_handrank(five_cards);
		if (get<0>(get_handrank)) {
			return get<1>(get_handrank);
		}
		
		return get<1>(get_high_card_handrank(five_cards));
	}

	tuple<Cards, HandRank> best_five_hand_out_of_seven(Cards seven_hand) {
		vector<Cards> all_five_hands = all_five_hands_from_seven(seven_hand);
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

	Cards get_players_seven_hand(Cards player_cards, Cards community_cards) {
		Cards seven_hand = community_cards;
		for (Card c : player_cards) {
			seven_hand.push_back(c);
		}
		return seven_hand;
	}

	tuple<Player*, Cards, HandRank> player_with_best_hand(vector<Player*> players, Cards community_cards) {
		// Since we set our own constructors for the Player,
		// we can't rely on the compiler to choose a default constructor.
		int tuple_initialized = false;

		// TODO: We need to change the Player type to a Player pointer type.
		// That way we can store both Human and Bot child classes. 

		Player* best_player;
		HandRank best_handrank;
		Cards best_cards;

		for (Player* p : players) {
			if (!p->has_player_folded()) {
				Cards seven_hand = get_players_seven_hand(p->get_hole_cards(), community_cards);
				tuple<Cards, HandRank> players_handrank = best_five_hand_out_of_seven(seven_hand);
			
				if (!tuple_initialized) {
					// Setting up baseline of comparison
					best_player = p;
					best_cards = get<0>(players_handrank);
					best_handrank = get<1>(players_handrank);
					tuple_initialized = true;
				} else {
					// Found a new best hand
					if (handrank_lte(best_handrank, get<1>(players_handrank))) {
						best_player = p;
						best_cards = get<0>(players_handrank);
						best_handrank = get<1>(players_handrank);
					}
				}
			}
		}
		return make_tuple(best_player, best_cards, best_handrank);
	}

	bool _compare_player_hand_tuple(tuple<Player*, Cards, HandRank> hand_tuple1, tuple<Player*, Cards, HandRank> hand_tuple2) {
		HandRank handrank1 = get<2>(hand_tuple1);
		HandRank handrank2 = get<2>(hand_tuple2);
		return compare_handrank(handrank1, handrank2) == COMP_TYPE::GT;
	}

	vector<vector<tuple<Player*, Cards, HandRank>>> ranked_player_hands(vector<Player*> players, Cards community_cards) {
		/*
		Returns:
			- All of the players in a "winner-group" have the same handrank, which means that they are tied.
		*/
		vector<tuple<Player*, Cards, HandRank>> rank_hands;
		Cards players_seven_hand;
		tuple<Cards, HandRank> players_best_hand;
		HandRank players_best_handrank;
		Cards    players_best_cards;

		for (Player* p: players) {
			printf("");

			if (!p->has_player_folded()) {
				players_seven_hand = get_players_seven_hand(p->get_hole_cards(), community_cards);
				players_best_hand  = best_five_hand_out_of_seven(players_seven_hand);

				players_best_handrank = get<1>(players_best_hand); 
				players_best_cards    = get<0>(players_best_hand);
				rank_hands.push_back(make_tuple(p, players_best_cards, players_best_handrank));
			}
		}
		// Sorting in descending order.
		sort(rank_hands.begin(), rank_hands.end(), _compare_player_hand_tuple);

		vector<vector<tuple<Player*, Cards, HandRank>>> sorted_winner_groups;
		for (tuple<Player*, Cards, HandRank> &ranked_hand : rank_hands) {
			if (sorted_winner_groups.size() == 0) {
				// Initialize list.
				sorted_winner_groups.push_back({ranked_hand});
			} else {
				// winner-groups is partially populated.
				HandRank last_handrank    = get<2>(sorted_winner_groups[sorted_winner_groups.size()-1][0]);
				HandRank current_handrank = get<2>(ranked_hand);

				if (compare_handrank(current_handrank, last_handrank) == COMP_TYPE::EQ) {
					// This ranked-hand belongs in the same winner-group as the previous ranked-hand
					sorted_winner_groups[sorted_winner_groups.size()-1].push_back(ranked_hand);
				} else {
					// This ranked-hand is of a lower rank than the previous ranked-hand
					sorted_winner_groups.push_back({ranked_hand});
				}
			}
		}
		return sorted_winner_groups;
	}
};
