#ifndef PRED_ENG
#define PRED_ENG

#include "CardHandler.h"
#include "DeckHandler.h"
#include "HandEvaluator.h"

using namespace std;

Cards get_simulated_com_cards(DeckHandler* deck, Cards known_community_cards);

vector<Cards> get_simulated_player_hands(DeckHandler* deck, int num_players);

bool hand_in_first_winning_group(Cards hand, vector<vector<Cards>> winning_groups);

double win_likelihood(Cards hand, Cards community_cards, int num_players, int num_sims);

#endif