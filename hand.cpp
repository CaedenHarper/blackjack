#include <iostream>
#include <vector>
#include <numeric>
#include <hand.h>
// TODO: add pointer prefix to all hand pointers in file

Hand :: Hand() : Deck() {
    value = 0;
    num_cards = 0;
    // has_ace means there is at least one ace
    has_ace = false;
    // soft means at least one ace is acting as an 11
    soft = false;
    can_hit = true;
    can_double = true;
    can_split = false;
    can_surrender = false;
    can_blackjack = true;
    active = true;
}

Hand :: Hand(int _card) : Deck(_card) {
    Hand();
    num_cards = 1;
    has_ace = (_card == 11) ? 1 : 0;
}

Hand :: Hand(std::vector<int> _cards) : Deck(_cards) {
    Hand();
    int _cards_size = _cards.size();
    num_cards = _cards_size;
    has_ace = false;
    for(int i = 0; i < _cards_size; i++) {
        if(_cards[i] == 11) {
            has_ace = true;
            return;
        }
    }
}

std::ostream& operator<<(std::ostream &out, Hand const &hand) {
    if(hand.size() == 0) {
        out << "[]";
        return out;
    }

    // TODO: consider changing writing A instead of 11 method
    // write A instead of 11
    if(hand[0] == 11) {
        out << "[" << "A";
    } else {
        out << "[" << hand[0];
    }
    for(int i = 1; i < hand.size(); i++) {
        if(hand[i] == 11) {
            out << ", " << "A";
        } else {
            out << ", " << hand[i];
        }
    }
    out << "]";

    return out;
}

void Hand :: push_back(const int &val) {
    cards.push_back(val);;
    update(val);
}

int Hand::get_value() const {
    return value;
}

bool Hand::is_bust() const {
    return value > 21;
}

int Hand :: size() const {
    return num_cards;
}

bool Hand :: get_has_ace() const {
    return has_ace;
}

bool Hand :: get_soft() const {
    return soft;
}

bool Hand :: get_can_hit() const {
    return can_hit;
}

bool Hand :: get_can_double() const {
    return can_double;
}

bool Hand :: get_can_split() const {
    return can_split;
}

bool Hand :: get_can_surrender() const {
    return can_surrender;
}

bool Hand :: is_blackjack() const {
    // blackjack if value is 21, made up of only 2 cards,
    // and it is the first hand made (splitting does not count)
    return ((value == 21) && (num_cards == 2) && (can_blackjack));
}

bool Hand :: get_active() const {
    return active;
}

void Hand::set_has_ace(bool _has_ace) {
    has_ace = _has_ace;
}

void Hand::set_soft(bool _soft) {
    soft = _soft;
}

void Hand::set_can_hit(bool _can_hit) {
    can_hit = _can_hit;
}

void Hand::set_can_double(bool _can_double) {
    can_double = _can_double;
}

void Hand::set_can_split(bool _can_split) {
    can_split = _can_split;
}

void Hand::set_can_surrender(bool _can_surrender) {
    can_surrender = _can_surrender;
}

void Hand::set_can_blackjack(bool _can_blackjack) {
    can_blackjack = _can_blackjack;
}

void Hand::set_active(bool _active) {
    active = _active;
}

void Hand :: update(int card) {
    num_cards++;

    if(card != 11) {
        // card is not an ace
        value += card;
        // if bust and hand is soft, subtract 10 and make hand not soft
        if(value > 21 && soft == true) {
            value -= 10;
            soft = false;
        }
        return;
    }

    // card is an ace
    if(has_ace == false) {
        // if there are zero aces then the value can be value + 11 as long as it is under 22
        has_ace = true;
        if(value + 11 <= 21) {
            value += 11;
            soft = true;
        } else {
            value += 1;
        }
        return;
    }

    // if there is already one ace then no matter what the value of this ace must be 1
    value += 1;
}