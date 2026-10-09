
#include "../include/PredictionEngine.h"

using namespace std;

Cards get_simulated_com_cards(DeckHandler* deck, Cards known_community_cards) {
	Cards simmed_community_cards = known_community_cards;
	for (int i=0; i<(5-known_community_cards.size()); i++) {
		simmed_community_cards.push_back(deck->draw_card());
	}
	return simmed_community_cards;
}

/**
 * @brief Simulates the possible other player hands
 */
vector<Cards> get_simulated_player_hands(DeckHandler* deck, int num_players) {
	vector<Cards> player_cards = {};
	for (int i=0; i<num_players; i++) {
		player_cards.push_back({ deck->draw_card(), deck->draw_card() });
	}
	return player_cards;
}

bool hand_in_first_winning_group(Cards hand, vector<vector<Cards>> winning_groups) {
	for (Cards winning_hand : winning_groups[0]) {
		if (are_group_of_cards_same(hand, winning_hand)) {
			return true;
		}
	}
	return false;
}

double win_likelihood(
	Cards hand, 
	Cards community_cards, 
	int num_players,
	int num_sims) {
	
	double times_won = 0.0;

	for (int i=0; i<num_sims; i++) {
		// Removing the cards that we know from the deck
		DeckHandler deck = DeckHandler();
		deck.remove_cards(hand);
		deck.remove_cards(community_cards);

		Cards simulated_com_cards = get_simulated_com_cards(&deck, community_cards);
		vector<Cards> sim_player_hands = get_simulated_player_hands(&deck, num_players-1);  
		sim_player_hands.push_back(hand);
		
		// Calculating simulated outcome
		vector<vector<Cards>> winning_groups = HandEvaluator::get_winning_groups_for_hands(sim_player_hands, simulated_com_cards);
		if (hand_in_first_winning_group(hand, winning_groups)) {
			times_won++;
		}
	}
	return times_won / num_sims;
}