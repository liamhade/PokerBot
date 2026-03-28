#ifndef CARD_HANDLER
#define CARD_HANDLER

#include <tuple>
#include <vector>
#include <string>

enum class CardValue { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

enum class Suit { Clubs, Spades, Hearts, Diamonds };

using Card = std::tuple<CardValue, Suit>; 

using Cards = std::vector<Card>;

Suit get_card_suit(Card c);

CardValue get_card_value(Card c);

std::string suit_2_string(Suit suit);

std::string card_2_string(Card card);

std::string cardvalue_2_string(CardValue value);

std::string get_cards_str(Cards cards);

#endif 