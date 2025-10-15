#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>

/*
Outline

Poker
    - players : *Player
    - deck    : Deck
    - pot     : float
    - community_cards : *Card  

    void run(*Player players, *Deck deck)
        - Stores our main game loop

    bool is_legal_action(Action action, *GameState game)
        - Check if a players action is legal given the current game state
        
    void betting_round(*Gamestate game, *players players)
        - Part of the game where the players make their moves
        - This function updates the gamestate, but does not return anything
        - min_bet : float
            + The minimum amount that a player has to bet to stay in the round
            + Initalized to 0 (start of the round)

    void deal_the_board(*Gamestate game, int num_cards)
        - Adds <num_cards> cards to the community cards
        - We call this on flop, turn and river

    int player_with_best_hand(Gamestate* game, Player* players)
        - Returns the index of the player with the best hand

    int best_hand(Gamestate* game, std::vector<card*> hands)
        - Returns the index of the highest ranked hand

    <type> hand_rank(card* seven_cards)
        - Gives 

Player
    - name  : *char
    - stack : float
    - hand  : Optional[*Deck]
    - is_in_the_hand : bool
        + Keeps track of whether a player has folded yet or not

    Action act()
        - Player takes an action
        
Action
    - move_type  : *char
        + Either "fold", "check", "call" or "raise"
    - bet_amount : float
        + For "fold" and "check", this value is 0.

Deck : *Card
    void shuffle_cards(Deck deck)
    - Shuffles our deck of cards in place

    Deck construct_deck()
    - Builds the deck of 52 cards

Card : *char
    - First char is the card
        + Possible ones are 234567890JQKA
    - Next char is the suite
        + CSHD
    - Examples:
        + Ten of Hearts = 0H
        + Ace of Apaces = A

*/
enum class CardValue { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
enum class Suit { Clubs, Spades, Hearts, Diamonds };
using Card = std::tuple<CardValue, Suit>; 
using Deck = std::vector<Card>;

struct DeckHandler {
public:
    DeckHandler(bool shuffle = true) {
        construct_deck();
        if (shuffle) {
            shuffle_deck();
        }
    }
    
    void shuffle_deck() {    
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(deck.begin(), deck.end(), g);
    }
    
    Card draw_card() {
        const Card card = deck.back();
        deck.pop_back();
        return card;
    }

private:
    Deck deck;

    Deck construct_deck() {
        for (int i=0; i<13; i++) {
            CardValue c = static_cast<CardValue>(i);
            for (int j=0; j<4; j++) {
                Suit s = static_cast<Suit>(j);
                deck.push_back(std::make_tuple(c, s));
            };
        };
        return deck;
    }
};

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

Suit get_card_suit(Card c) { return std::get<1>(c); } 
CardValue get_card_value(Card c) { return std::get<0>(c); } 

using HandRank = std::tuple<KindsOfHand, CardValue>;

int num_of_value_in_hand(CardValue value, std::vector<Card> cards) {
    int count = 0;
    for (Card c : cards) {
        if (get_card_value(c) == value) {
            count++;
        }
    }
    return count;
}

bool is_card_value_in_hand(std::vector<Card> cards, CardValue value) {
    return num_of_value_in_hand(value, cards) > 0;
}

bool card_value_lt(Card c1, Card c2) {
    return static_cast<int>(get_card_value(c1)) < static_cast<int>(get_card_value(c2));
}

bool card_values_ascend(std::vector<Card> cards) {
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

std::vector<std::vector<Card>> all_five_hands_from_seven(std::vector<Card> seven_card_hand) {
    std::vector<std::vector<Card>> five_hands;
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

bool all_cards_have_same_suit(std::vector<Card> cards) {
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

int main() {
 
    DeckHandler deck;
    const Card c = deck.draw_card();

    std::vector<Card> hand = {
        std::make_tuple(CardValue::Queen, Suit::Clubs),
        std::make_tuple(CardValue::Ten, Suit::Hearts),
        std::make_tuple(CardValue::Queen, Suit::Clubs),
        // std::make_tuple(CardValue::King, Suit::Hearts),
        // std::make_tuple(CardValue::Two, Suit::Hearts),
        // std::make_tuple(CardValue::Two, Suit::Clubs),
        // std::make_tuple(CardValue::Five, Suit::Hearts)
    };
    std::cout << all_cards_have_same_suit(hand) << std::endl;
    return 0;
}