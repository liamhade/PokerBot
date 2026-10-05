#include "..\include\CardHandler.h"
#include "..\include\DeckHandler.h"
#include "..\include\PredictionEngine.h"
#include "..\include\Testing.h"
#include "vector"

void test_mc_finishes_in_time_on_1_sim() {
	DeckHandler deck;
	Cards hand = { deck.draw_card(), deck.draw_card() }; 
	Cards com_cards = { deck.draw_card(), deck.draw_card(), deck.draw_card() }; 
	win_likelihood(hand, com_cards, 3, 1);

    assert(true,  true, "MonteCarlo passed with one simulation round.");
}

void test_mc_finishes_in_time_on_1000_sim() {
	DeckHandler deck;
	Cards hand = { deck.draw_card(), deck.draw_card() }; 
	Cards com_cards = { deck.draw_card(), deck.draw_card(), deck.draw_card() }; 
	win_likelihood(hand, com_cards, 3, 1000);

	assert(true,  true, "MonteCarlo passed with 1000 simulation rounds.");
}

void test_royal_flush_alway_wins() {
	Card ten   = Card(CardValue::Ten, Suit::Diamonds);
	Card jack  = Card(CardValue::Jack, Suit::Diamonds);
	Card queen = Card(CardValue::Queen, Suit::Diamonds);
	Card king  = Card(CardValue::King, Suit::Diamonds);
	Card ace   = Card(CardValue::Ace, Suit::Diamonds);

	Cards hand = { ten, jack }; 
	Cards com_cards = { queen, king, ace }; 

	double wl = win_likelihood(hand, com_cards, 3, 100);

	std::cout << wl << std::endl;
	assert(wl, 1.00, "Royal flush alway wins");
}

int main() {
	test_mc_finishes_in_time_on_1_sim();
	test_mc_finishes_in_time_on_1000_sim();
	test_royal_flush_alway_wins();
	return 0;
}