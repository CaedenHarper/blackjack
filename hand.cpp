// SELF
#include <hand.hpp>

// OTHER MODULES
#include <settings.hpp>

// STANDARD LIBRARY
#include <iostream>
#include <vector>
#include <numeric>

Hand::Hand() : Deck() {
    value = 0;
    num_cards = 0;
    has_ace = false;
    soft = false;
    pair = false;
    can_double = true;
    can_split = false;
    can_surrender = LATE_SURRENDER;
    can_blackjack = true;
    active = true;
    was_doubled = false;
    was_surrendered = false;
    split_num = 1;
    split_aces_final_card = false;
}

Hand::Hand(int _card) : Deck(_card) {
    soft = false;
    pair = false;
    can_double = true;
    can_split = false;
    can_surrender = LATE_SURRENDER;
    can_blackjack = true;
    active = true;
    was_doubled = false;
    was_surrendered = false;
    split_num = 1;
    split_aces_final_card = false;

    value = _card;
    num_cards = 1;
    has_ace = (_card == 11) ? 1 : 0;
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

void Hand::push_back(const int &val) {
    // std::cout<<"here IN IN 1"<<'\n';
    cards.push_back(val);
    // std::cout<<"here IN IN 2"<<'\n';
    update(val);
    // std::cout<<"here IN IN 3"<<'\n';
}

int Hand::get_value() const {
    return value;
}

bool Hand::is_bust() const {
    return value > 21;
}

int Hand::size() const {
    return num_cards;
}

bool Hand::get_has_ace() const {
    return has_ace;
}

bool Hand::get_soft() const {
    return soft;
}

bool Hand::get_pair() const {
    return pair;
}

bool Hand::get_can_double() const {
    return can_double;
}

bool Hand::get_can_split() const {
    return can_split;
}

bool Hand::get_can_surrender() const {
    return can_surrender;
}

bool Hand::is_blackjack() const {
    // blackjack if value is 21, made up of only 2 cards,
    // and it is the first hand made (splitting does not count)
    return ((value == 21) && (num_cards == 2) && (can_blackjack));
}

bool Hand::get_active() const {
    return active;
}

bool Hand::get_was_doubled() const {
    return was_doubled;
}

bool Hand::get_was_surrendered() const {
    return was_surrendered;
}

int Hand::get_split_num() const {
    return split_num;
}

bool Hand::get_split_aces_final_card() const {
    return split_aces_final_card;
}

void Hand::set_has_ace(bool _has_ace) {
    has_ace = _has_ace;
}

void Hand::set_soft(bool _soft) {
    soft = _soft;
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

void Hand::set_was_doubled(bool _was_doubled) {
    was_doubled = _was_doubled;
}

void Hand::set_was_surrendered(bool _was_surrendered) {
    was_surrendered = _was_surrendered;
}

void Hand::increment_split_num() {
    split_num++;
}

void Hand::set_split_aces_final_card(bool _split_aces_final_card) {
    split_aces_final_card = _split_aces_final_card;
}

void Hand::update(int card) {
    num_cards++;
    
    // update pair variable
    if(num_cards == 2) {
        // two separate if statements to ensure no errors
        if(card == cards.at(0)) {
            pair = true;
        } else {
            pair = false;
        }
    } else {
        pair = false;
    }

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