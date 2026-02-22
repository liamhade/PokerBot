#include <algorithm>
#include <iterator>

#include "PotDispersal.h"

typedef tuple<Player*, float> PlayerWithBet;

bool _did_player_bet_more(PlayerWithBet player_with_bet_1, PlayerWithBet player_with_bet_2) {
    return get<1>(player_with_bet_1) > get<1>(player_with_bet_2);
}

/**
 * @brief Groups players into sidepots.
 * 
 * Creates sidepots by iteratively finding the smallest bet size from a list of players,
 * adding those players to a list, then removing players who have no money leftover from that sidepot.
 *
 * The function returns once the list of players with leftover money from the sidepots is empty. 
 */
vector<SidePot> create_sidepots(vector<Player*> players) {
    
    vector<PlayerWithBet> players_with_bets;
    vector<SidePot> sidepots;

    // Populating players_with_bets array
    for (Player* player: players) {
        PlayerWithBet player_bet = {player, player->get_total_amount_bet()};
        players_with_bets.push_back(player_bet);
    }

    // Sorting from smallest to largest bets so that we can easily find the smallest bet
    sort(players_with_bets.begin(), players_with_bets.end(), _did_player_bet_more);

    while (players_with_bets.size() > 0) {
        float smallest_bet = get<1>(players_with_bets[0]);
        vector<Player*> players_in_sidepot;
        
        // Populating players_in_side_pot
        for (PlayerWithBet player_with_bet: players_with_bets) {
            // Including player with bet in this side pot.
            players_in_sidepot.push_back(get<0>(player_with_bet));
        }

        // Adding sidepot to list of sidepot
        tuple<vector<Player*>, float> sidepot = {players_in_sidepot, smallest_bet*players_in_sidepot.size()};
        sidepots.push_back(sidepot);

        // Remove players who would have no money remaining after this sidepot
        remove_if(players_with_bets.begin(), players_with_bets.end(), [smallest_bet](PlayerWithBet pwb){return get<1>(pwb) > smallest_bet;});

        // Substracting sidepot amount from each player
        for (PlayerWithBet& pwb: players_with_bets) {
            get<1>(pwb) -= smallest_bet; 
        }
    }
    
    return sidepots;
}

void disperse_sidepot_funds_to_winners(PlayerWithHandRank player_handranks, SidePot sidepots) {
    /* TODO: DocString */
    return;
}