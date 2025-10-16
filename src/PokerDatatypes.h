#ifndef POKER_TYPES
#define POKER_TYPES

#include <tuple>
#include <vector>

enum class CardValue { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
enum class Suit { Clubs, Spades, Hearts, Diamonds };

using Card = std::tuple<CardValue, Suit>; 
using Cards = std::vector<Card>;

#endif 