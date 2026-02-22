#include "C:\Users\11061985\OneDrive - Dover Corporation\Desktop\PROJECTS\PokerBot\src\HandEvaluator.h"
#include "C:\Users\11061985\OneDrive - Dover Corporation\Desktop\PROJECTS\PokerBot\src\CardHandler.h"
#include "C:\Users\11061985\OneDrive - Dover Corporation\Desktop\PROJECTS\PokerBot\src\Player.h"
#include <iostream>
#include <stdio.h>
using namespace std;

// g++ -g "src/CardHandler.cpp" "src/HandEvaluator.cpp" "src/Player.cpp" "tests/test_HandEvaluator.cpp" -o "tests/test.exe"; tests/test.exe

template<typename T>
void assert(T actual, T expected, string test_name) {
    if (actual != expected) {
        cout << "\033[31m" << "FAILED" << "\033[0m" << ":  " << test_name << "\n";
    } else{
        cout << "\033[32m" << "PASSED" << "\033[0m" << ":  " << test_name << "\n";
    }
}

void test_all_cards_in_two_pair() {
    /* Making sure that the handrank function propely handles a pair. */
    Card c1 = make_tuple(CardValue::Seven, Suit::Clubs); 
    Card c2 = make_tuple(CardValue::Ace,   Suit::Clubs);
    Card c3 = make_tuple(CardValue::Three, Suit::Hearts); 
    Card c4 = make_tuple(CardValue::Seven, Suit::Spades); 
    Card c5 = make_tuple(CardValue::Three, Suit::Clubs);

    HandRank h1 = HandEvaluator::get_cards_handrank({c1, c2, c3, c4, c5});

    assert(h1.kind_of_hand, KindsOfHand::Two_Pair,         "test_all_cards_in_two_pair() - kind of hand");
    assert(h1.sorted_cards_inside_hand, {c1, c4, c3, c5},  "test_all_cards_in_two_pair() - inside hand");
    assert(h1.sorted_cards_outside_hand, {c2},             "test_all_cards_in_two_pair() - outside hand");
}

void test_higher_pair() {
    /* Both players have pairs, but one pair is better than the other. */
    Card c1 = make_tuple(CardValue::Seven, Suit::Clubs); 
    Card c2 = make_tuple(CardValue::Ace,   Suit::Clubs);
    Card c3 = make_tuple(CardValue::Three, Suit::Hearts); 
    Card c4 = make_tuple(CardValue::Seven, Suit::Spades); 
    Card c5 = make_tuple(CardValue::Three, Suit::Clubs);
    Card c6 = make_tuple(CardValue::Four,  Suit::Clubs);

    HandRank h1 = HandEvaluator::get_cards_handrank({c1, c2, c3, c6, c5}); // Pair of 3s
    HandRank h2 = HandEvaluator::get_cards_handrank({c1, c2, c6, c4, c5}); // Pair of 7s

    assert(h1.kind_of_hand, KindsOfHand::Pair,         "test_higher_pair() - kind of hand (1)");
    assert(h2.kind_of_hand, KindsOfHand::Pair,         "test_higher_pair() - kind of hand (2)");
    assert(HandEvaluator::handrank_lte(h1, h2), true,  "test_higher_pair() - hand lte");
    assert(HandEvaluator::handrank_lte(h1, h1), true,  "test_higher_pair() - hands eq");
    assert(HandEvaluator::handrank_lte(h2, h1), false, "test_higher_pair() - hands gt");
}

void test_ranked_player_hands() {
    /* Alice has a better hand than Bob. */
    Player p1(100, "Alice");
    Player p2(100, "Bob");

    Card p1_c1 = make_tuple(CardValue::Seven, Suit::Clubs); 
    Card p1_c2 = make_tuple(CardValue::Ace,   Suit::Clubs);
    p1.set_cards({p1_c1, p1_c2});

    Card p2_c1 = make_tuple(CardValue::Three, Suit::Hearts); 
    Card p2_c2 = make_tuple(CardValue::Seven, Suit::Spades); 
    p2.set_cards({p2_c1, p2_c2});
    
    Card c1 = make_tuple(CardValue::Seven, Suit::Diamonds);
    Card c2 = make_tuple(CardValue::Two,   Suit::Clubs);
    Card c3 = make_tuple(CardValue::Five,  Suit::Hearts);
    Card c4 = make_tuple(CardValue::Ten,   Suit::Clubs);
    Card c5 = make_tuple(CardValue::Jack,  Suit::Spades);

    vector<vector<tuple<Player*, Cards, HandRank>>> ranked_player_hands = HandEvaluator::ranked_player_hands({&p1, &p2}, {c1, c2, c3, c4, c5});

    assert(get<0>(ranked_player_hands[0][0]), &p1, "test_ranked_player_hands()");
}

void test_three_players_two_tie() {
    /* Alice and Carl tie with the top hands. */
    Player p1(100, "Alice");
    Player p2(100, "Bob");
    Player p3(100, "Carl");

    Card p1_c1 = make_tuple(CardValue::Seven, Suit::Clubs); 
    Card p1_c2 = make_tuple(CardValue::Ace,   Suit::Clubs);
    p1.set_cards({p1_c1, p1_c2});

    Card p2_c1 = make_tuple(CardValue::Three, Suit::Hearts); 
    Card p2_c2 = make_tuple(CardValue::Seven, Suit::Spades); 
    p2.set_cards({p2_c1, p2_c2});

    Card p3_c1 = make_tuple(CardValue::Seven, Suit::Hearts); 
    Card p3_c2 = make_tuple(CardValue::Ace,   Suit::Diamonds);
    p3.set_cards({p3_c1, p3_c2});
    
    Card c1 = make_tuple(CardValue::Seven, Suit::Diamonds);
    Card c2 = make_tuple(CardValue::Two,   Suit::Clubs);
    Card c3 = make_tuple(CardValue::Five,  Suit::Hearts);
    Card c4 = make_tuple(CardValue::Ten,   Suit::Clubs);
    Card c5 = make_tuple(CardValue::Jack,  Suit::Spades);

    vector<vector<tuple<Player*, Cards, HandRank>>> ranked_player_hands = HandEvaluator::ranked_player_hands({&p1, &p2, &p3}, {c1, c2, c3, c4, c5});

    assert(get<0>(ranked_player_hands[0][0]), &p1, "test_three_players_two_tie() - Alice in winning group");
    assert(get<0>(ranked_player_hands[1][0]), &p2, "test_three_players_two_tie() - Bob in losing group");
    assert(get<0>(ranked_player_hands[0][1]), &p3, "test_three_players_two_tie() - Carl in winning group");
}       

int main() {
    test_all_cards_in_two_pair();
    test_higher_pair();
    test_ranked_player_hands();
    test_three_players_two_tie();
}