#include <algorithm>
#include <iterator>
#include <iostream>
#include <unordered_map>

#include "../include/PotDispersal.h"

typedef tuple<Player*, float> PlayerWithBet;

bool _did_player_bet_less(PlayerWithBet player_with_bet_1, PlayerWithBet player_with_bet_2) {
    return get<1>(player_with_bet_1) < get<1>(player_with_bet_2);
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
    sort(players_with_bets.begin(), players_with_bets.end(), _did_player_bet_less);

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
        players_with_bets.erase(remove_if(players_with_bets.begin(), players_with_bets.end(), [smallest_bet](PlayerWithBet pwb){return get<1>(pwb) == smallest_bet;}), players_with_bets.end());

        // Substracting sidepot amount from each player
        for (PlayerWithBet& pwb: players_with_bets) {
            get<1>(pwb) -= smallest_bet; 
        }
    }
    
    return sidepots;
}


bool _sidepot_player_and_playerwithhandrank_are_same(Player* sidepot_p, PlayerWithHandRank p_handrank) {
    return sidepot_p->get_name() == get<0>(p_handrank)->get_name();
}

/**
 * @brief Finds the playersthat are in both the sidepot and the winning_group.
 * @return Vector of Player* coupled with their cards and handrank.
 */
vector<PlayerWithHandRank> _players_in_sidepot_and_winning_group(SidePot sidepot, vector<PlayerWithHandRank> winning_group) {
    vector<PlayerWithHandRank> player_handranks;
    for (Player* sidepot_p: get<0>(sidepot)) {
        for (PlayerWithHandRank p_handrank: winning_group) {
            if (_sidepot_player_and_playerwithhandrank_are_same(sidepot_p, p_handrank)) {
                player_handranks.push_back(p_handrank);
            }
        }
    }
    return player_handranks;
}

/**
 * @brief Displays details of each pots winner to the user.
 */
void show_winner_details(float winnings, PlayerWithHandRank winner_details, bool show_hand_details)  {
    Player* player    = get<0>(winner_details);
    Cards cards       = get<1>(winner_details);
    HandRank handrank = get<2>(winner_details);

    cout << endl;
    cout << player->get_name() <<" has won $" << winnings << endl;
    if (show_hand_details) {
        cout << " -- They had a " << KindsOfHand_2_string(handrank.kind_of_hand);
        cout << " with the cards: " << get_cards_str(cards) << endl;
    }
}

/**
 * @brief Disperses each sidepot depending on player handranks. 
 */
void disperse_sidepot_funds_to_winners(vector<vector<PlayerWithHandRank>> winning_groups, vector<SidePot> sidepots, bool show_outcome) {
    unordered_map<string, tuple<float, PlayerWithHandRank>> player_total_winnings;
    float sidepot_winnings;
    Player* player;

    for (SidePot sp: sidepots) {
        float sidepot_amount = get<1>(sp);

        // Dispering funds from this sidepot
        for (vector<PlayerWithHandRank> winning_group: winning_groups) {
            // Checking if any players from this winning group are present here
            vector<PlayerWithHandRank> player_handranks = _players_in_sidepot_and_winning_group(sp, winning_group);
            if (player_handranks.size() == 0) {
                // No players in this winning group that are in this side pot.
                continue;
            }
            // There are players in this winning group who can win the side-pot
            for (PlayerWithHandRank player_w_handrank: player_handranks) {
                // Splitting this pot evenly amonst the winning group.
                sidepot_winnings = sidepot_amount / player_handranks.size();
                
                // Parsing out player.
                player = get<0>(player_w_handrank); 

                // Updating player stack.
                player->add_to_stack(sidepot_winnings);

                // Updating winnings map
                if (player_total_winnings.find(player->get_name()) == player_total_winnings.end()) {
                    // Initializing new map entry.
                    player_total_winnings[player->get_name()] = {sidepot_winnings, player_w_handrank};
                } else {
                    // Updating map entry.
                    get<0>(player_total_winnings[player->get_name()]) += sidepot_winnings;
                }
            }
            break;
        }
    }

    // Showing outcome details for each winner.
    if (show_outcome) {
        for (const auto& item: player_total_winnings) {
            show_winner_details(get<0>(item.second), get<1>(item.second), true);
        }
    }
}