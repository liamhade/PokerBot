#include "CardHandler.h"
#include <iostream>

Suit get_card_suit(Card c) { 
	return std::get<1>(c); 
} 

CardValue get_card_value(Card c) { 
	return std::get<0>(c); 
} 

std::string cardvalue_2_string(CardValue value) {
	switch (value) {
		case CardValue::Two:
			return "Two";
		case CardValue::Three:
			return "Three";
		case CardValue::Four:
			return "Four";
		case CardValue::Five:
			return "Five";
		case CardValue::Six:
			return "Six";
		case CardValue::Seven:
			return "Seven";
		case CardValue::Eight:
			return "Eight";
		case CardValue::Nine:
			return "Nine";
		case CardValue::Ten:
			return "Ten";
		case CardValue::Jack:
			return "Jack";
		case CardValue::Queen:
			return "Queen";
		case CardValue::King:
			return "King";
		case CardValue::Ace:
			return "Ace";
	}
	return "Not Sure";
}

std::string suit_2_string(Suit suit) {
	switch (suit) {
		case Suit::Clubs:
			return "Clubs";
		case Suit::Diamonds:
			return "Diamonds";
		case Suit::Hearts:
			return "Hearts";
		case Suit::Spades:
			return "Spades";
	}
	return "Not Sure";
}

std::string card_2_string(Card card) {
	CardValue value = get_card_value(card);
	Suit suit = get_card_suit(card);
	return  cardvalue_2_string(value) + " of " + suit_2_string(suit);
}

std::string get_cards_str(Cards cards) {
	std::string cards_str = "";
	for (int i=0; i<cards.size(); i++) {
		cards_str += card_2_string(cards[i]);
		if (i<cards.size()-1) {
			cards_str += ", ";
		}
	}
	return cards_str;
}