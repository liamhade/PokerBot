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
using Card  = std::tuple<CardValue, Suit>; 
using Cards = std::vector<Card>;

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
    Cards deck;

    Cards construct_deck() {
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


/*
Still working on:
*/


int main() {
 
    DeckHandler deck;
    const Card c = deck.draw_card();
     
    Cards all_cards = {
        std::make_tuple(CardValue::Two,   Suit::Diamonds),
        std::make_tuple(CardValue::Two ,  Suit::Clubs),
        std::make_tuple(CardValue::Three, Suit::Diamonds),
        std::make_tuple(CardValue::Three, Suit::Clubs),
        std::make_tuple(CardValue::Three, Suit::Spades),
        std::make_tuple(CardValue::Two, Suit::Hearts),
        std::make_tuple(CardValue::Five, Suit::Hearts)
    };
    std::tuple<Cards, HandRank> best_hand = best_five_hand_out_of_seven(all_cards);
    // std::cout << is_two_pair(hand) << std::endl;
    return 0;
}