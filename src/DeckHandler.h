#ifndef DECK_HANDLER
#define DECK_HANDLER

#include "CardHandler.h"

struct DeckHandler {
public:
    DeckHandler();    
    void reset();
    void shuffle_deck();
    Card draw_card();

private:
    Cards deck;
    void construct_deck();
};

#endif