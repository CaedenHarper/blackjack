#ifndef SHOE_H
#define SHOE_H

#include <vector>
#include <deck.h>
#include <random>

class Shoe : public Deck {
    public:
    int num_decks;
    static std::default_random_engine rng;

    const static std::vector<int> one_deck;
    const static std::vector<int> two_decks;
    const static std::vector<int> four_decks;
    const static std::vector<int> six_decks;
    const static std::vector<int> eight_decks;
    
    Shoe(int n_decks);
    void shuffle();
};

#endif