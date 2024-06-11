// SELF
#include <deck.hpp>

// STANDARD LIBRARY
#include <iostream>
#include <vector>

Deck::Deck() {
    this->cards = std::vector<int>();
}

Deck::Deck(int card) {
    this->cards = {card};
}

int Deck::operator [](int i) const {
    return this->cards.at(i);
}

std::ostream& operator<<(std::ostream &out, const Deck &deck) {
    if(deck.size() == 0) {
        out << "[]";
        return out;
    }

    // TODO: consider refactoring
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

int Deck::size() const {
    return this->cards.size();
}

void Deck::clear() {
    this->cards.clear();
}

void Deck::insert(int index, const int& val) {
    std::vector<int>::iterator it = this->cards.begin();
    this->cards.insert((it + index), val);
}

void Deck::push_back(const int& val) {
    this->cards.push_back(val);
}

int Deck::pop_back() {
    int out = this->cards.back();
    this->cards.pop_back();
    return out;
}