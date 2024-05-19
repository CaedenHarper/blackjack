#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <vector>
#include <deck.h>

class Hand : public Deck {
    public:
    bool can_hit;
    bool can_double;
    bool can_split;
    bool can_surrender;
    bool can_blackjack;
    bool active;
    
    int count_aces();
    int get_value();
    bool is_bust();
    bool is_blackjack();
};

#endif