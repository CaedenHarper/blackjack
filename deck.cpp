#include <iostream>
#include <vector>
#include <deck.h>

Deck :: Deck() {
    cards;
}

Deck :: Deck(int _card) {
    cards = {_card};
}

Deck :: Deck(std::vector<int> _cards) {
    cards = _cards;
}

int Deck :: operator [](int i) const {
    if(cards.size() < 1) {
        std::cout << "EMPTY DECK ERROR" << "\n";
    }
    return cards[i];
}

std::ostream& operator<<(std::ostream &out, const Deck &deck) {
    if(deck.size() == 0) {
        out << "[]";
        return out;
    }

    // TODO: consider changing writing A instead of 11 method
    // write A instead of 11
    if(deck[0] == 11) {
        out << "[" << "A";
    } else {
        out << "[" << deck[0];
    }
    for(int i = 1; i < deck.size(); i++) {
        // write A instead of 11
        if(deck[i] == 11) {
            out << ", " << "A";
        } else {
            out << ", " << deck[i];
        }
    }
    out << "]";

    return out;
}

int Deck :: size() const {
    return cards.size();
}

void Deck :: clear() {
    cards.clear();
}

void Deck :: insert(int index, const int& val) {
    std::vector<int>::iterator it = cards.begin();
    cards.insert((it + index), val);
}

void Deck :: push_back(const int& val) {
    cards.push_back(val);
}

int Deck :: pop_back() {
    int out = cards.back();
    cards.pop_back();
    return out;
}