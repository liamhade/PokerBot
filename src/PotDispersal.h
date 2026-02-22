#ifndef POT_DISP_H
#define POT_DISP_H

#include "HandEvaluator.h"
#include "Player.h"
#include <vector>
#include <tuple>

typedef tuple<vector<Player*>, float> SidePot; 

vector<SidePot> create_sidepots(vector<Player*> players); 

void disperse_sidepot_funds_to_winners(PlayerWithHandRank player_handranks, vector<SidePot> sidepots);

#endif