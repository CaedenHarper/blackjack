#ifndef DECK_H
#define DECK_H

#include <iostream>
#include <vector>

class Deck {
    public:
    std::vector<int> cards;
    
    Deck();
    Deck(int card);
    Deck(std::vector<int> cards);
    int operator [](int i);
    friend std::ostream& operator<<(std::ostream &out, const Deck &deck);
    int size();
    void clear();
    void push_back(const int& val);
    int pop_back();
};

#endif