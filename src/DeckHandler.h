#ifndef DECK_HANDLER
#define DECK_HANDLER

#include "PokerDatatypes.h"

struct DeckHandler {
public:
    DeckHandler();    
    void shuffle_deck();
    Card draw_card();

private:
    Cards deck;
    Cards construct_deck();
};

#endif