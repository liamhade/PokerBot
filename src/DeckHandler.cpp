#include <random>
#include <algorithm>
#include "../include/CardHandler.h"
#include "../include/DeckHandler.h"

DeckHandler::DeckHandler() {
	construct_deck();
	shuffle_deck();
}
    
void DeckHandler::reset() {
	deck = {};
	construct_deck();
	shuffle_deck();
}

void DeckHandler::shuffle_deck() {    
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(deck.begin(), deck.end(), g);
}

void DeckHandler::remove_cards(Cards cards) {
	for (int i=deck.size()-1; i>=0; i--) {
		for (int j=0; j<cards.size(); j++) {
			if (are_cards_same(deck[i], cards[j])) {
				deck.erase(deck.begin()+i);
				cards.erase(cards.begin()+j);
				break;
			}
		}
	}
}

Card DeckHandler::draw_card() {
	const Card card = deck.back();
	deck.pop_back();
	return card;
}

void DeckHandler::construct_deck() {
	for (int i=0; i<13; i++) {
		CardValue c = static_cast<CardValue>(i);
		for (int j=0; j<4; j++) {
			Suit s = static_cast<Suit>(j);
			deck.push_back(std::make_tuple(c, s));
		};
	};
}
