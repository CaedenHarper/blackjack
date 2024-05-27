#ifndef SHOE_H
#define SHOE_H

#include <vector>
#include <deck.h>
#include <random>

class Shoe : public Deck {
    public:
    int num_decks;
    
    Shoe(int n_decks);
    Shoe(int n_decks, std::default_random_engine rng);
    void shuffle(std::default_random_engine rng);
};

#endif