
```c++
vector<Cards> ranked_hands(vector<Cards> hands, Cards community_cards)
```

How do we extract some of the logic from `ranked_player_hands` into this function so that we can use it elsewhere?

# Extraction

input type is `vector<Players>`

(1) Loop through the players:
- (1.a) get the players_cards
- (1.b) calculate their best hand (`tuple<Cards five_hand, HandRank> players_best_hand`)
- (1.c) add to list of players associated with their best hand
(2) sort the list of players based off their 



outcome type is `vector<tuple<Player*, Cards, HandRank>>`, that is ranked order of where index [0] is the best hand, and index [-1] is the worst hand.


# Functions

## goal

get_sorted_best_hands_w_ranks: 
- IN: `vector<Cards> hands` + `Cards comunity_cards`
- OUT: `vector<tuple<Cards, HandRank>> sorted_hands`
- Will be used in `ranked_player_hands`, and in `MonteCarlo`

# bridges

get_players_seven_hand:
- IN: `Cards hand` + `Cards community_cards`
- OUT: `Cards seven_hand`

best_five_hand_out_of_seven:
- IN: `Cards seven_hand`
- OUT: `tuple<Cards, HandRank> best_five_hand_w_rank`

## helpers

get_player_hands: 
- IN: `vector<Player*> players` 
- OUT: `vector<Cards> hands`

attach_player_to_hands_with_rank_in_same_order:
- IN: `vector<Player*> players` + `vector<tuple<Cards, HandRank>> sorted_hands` + `Cards community_cards`
- OUT: `vector<tuple<Player*, Cards, HandRank>>` (GOAL)
