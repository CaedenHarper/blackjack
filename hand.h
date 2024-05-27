#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <vector>
#include <deck.h>

class Hand : public Deck {
    public:
    int value;
    int num_cards;
    int num_aces;
    bool can_hit;
    bool can_double;
    bool can_split;
    bool can_surrender;
    bool can_blackjack;
    bool active;

    Hand();
    Hand(int card);
    Hand(std::vector<int> cards);
    friend std::ostream& operator<<(std::ostream &out, const Hand &hand);
    void push_back(const int& val);
    bool is_bust();
    bool is_blackjack();

    private:
    void update(int card);
};

#endif