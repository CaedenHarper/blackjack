#ifndef DECK_H
#define DECK_H

// STANDARD LIBRARY
#include <iostream>
#include <vector>

class Deck {
    public:
    std::vector<int> cards;
    
    Deck();
    Deck(int card);
    Deck(std::vector<int> cards);
    int operator [](int i) const;
    friend std::ostream& operator<<(std::ostream &out, const Deck &deck);
    int size() const;
    void clear();
    void insert(int index, const int& val);
    void push_back(const int& val);
    int pop_back();
};

#endif