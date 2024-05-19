#ifndef SHOE_H
#define SHOE_H

#include <iostream>
#include <vector>
#include <deck.h>

class Shoe : public Deck {
    public:
    int num_decks;
    
    Shoe();
    void shuffle();
};

#endif