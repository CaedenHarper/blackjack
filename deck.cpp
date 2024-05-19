#include <iostream>
#include <vector>
#include <numeric>
#include <deck.h>

Deck :: Deck() {
    std::vector<int> cards;
    bool can_hit = true;
    bool can_double = true;
    bool can_split = false;
    bool can_surrender = false;
    bool can_blackjack = true;
    bool active = true;
}

Deck :: Deck(int card) {
    std::vector<int> cards = {card};
    bool can_hit = true;
    bool can_double = true;
    bool can_split = false;
    bool can_surrender = false;
    bool can_blackjack = true;
    bool active = true;
}

Deck :: Deck(std::vector<int> c) {
    std::vector<int> cards(c);
    bool can_hit = true;
    bool can_double = true;
    bool can_split = false;
    bool can_surrender = false;
    bool can_blackjack = true;
    bool active = true;
}

int Deck :: operator [](int i) {
    return cards[i];
}

std::ostream& operator<<(std::ostream &out, Deck &deck) {
    if(deck.size() == 0) {
        out << "[]";
        return out;
    }

    out << "[" << deck[0];
    for(int i = 1; i < deck.size(); i++) {
        out << ", " << deck[i];
    }
    out << "]";

    return out;
}

int Deck :: size() {
    return cards.size();
}

void Deck :: clear() {
    cards.clear();
}

void Deck :: push_back(const int& val) {
    cards.push_back(val);
}

int Deck :: pop_back() {
    int out = cards.back();
    cards.pop_back();
    return out;
}