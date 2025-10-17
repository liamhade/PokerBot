#include <random>
#include <algorithm>
#include "CardHandler.h"
#include "DeckHandler.h"

DeckHandler::DeckHandler() {
	construct_deck();
	shuffle_deck();
}
    
void DeckHandler::shuffle_deck() {    
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(deck.begin(), deck.end(), g);
}

Card DeckHandler::draw_card() {
	const Card card = deck.back();
	deck.pop_back();
	return card;
}

Cards DeckHandler::construct_deck() {
	for (int i=0; i<13; i++) {
		CardValue c = static_cast<CardValue>(i);
		for (int j=0; j<4; j++) {
			Suit s = static_cast<Suit>(j);
			deck.push_back(std::make_tuple(c, s));
		};
	};
	return deck;
}
