// SELF
#include <hand.hpp>

// OTHER MODULES
#include <settings.hpp>

// STANDARD LIBRARY
#include <iostream>
#include <vector>
#include <numeric>

Hand::Hand() : Deck() {
    this->value = 0;
    this->num_cards = 0;
    this->has_ace = false;
    this->soft = false;
    this->pair = false;
    this->can_double = true;
    this->can_split = false;
    this->can_surrender = LATE_SURRENDER;
    this->can_blackjack = true;
    this->active = true;
    this->was_doubled = false;
    this->was_surrendered = false;
    this->split_num = 1;
    this->split_aces_final_card = false;
}

Hand::Hand(int card) : Deck(card) {
    this->soft = false;
    this->pair = false;
    this->can_double = true;
    this->can_split = false;
    this->can_surrender = LATE_SURRENDER;
    this->can_blackjack = true;
    this->active = true;
    this->was_doubled = false;
    this->was_surrendered = false;
    this->split_num = 1;
    this->split_aces_final_card = false;

    this->value = card;
    this->num_cards = 1;
    this->has_ace = (card == 11) ? 1 : 0;
}

std::ostream& operator<<(std::ostream &out, Hand const &hand) {
    if(hand.size() == 0) {
        out << "[]";
        return out;
    }

    // TODO: consider changing writing A instead of 11 method
    // write A instead of 11
    if(hand.at(0) == 11) {
        out << "[" << "A";
    } else {
        out << "[" << hand.at(0);
    }
    for(int i = 1; i < hand.size(); i++) {
        if(hand.at(i) == 11) {
            out << ", " << "A";
        } else {
            out << ", " << hand.at(i);
        }
    }
    out << "]";

    return out;
}

void Hand::push_back(const int &val) {
    this->cards.push_back(val);
    update(val);
}

int Hand::get_value() const {
    return this->value;
}

bool Hand::is_bust() const {
    return this->value > 21;
}

int Hand::size() const {
    return this->num_cards;
}

bool Hand::get_has_ace() const {
    return this->has_ace;
}

bool Hand::get_soft() const {
    return this->soft;
}

bool Hand::get_pair() const {
    return this->pair;
}

bool Hand::get_can_double() const {
    return this->can_double;
}

bool Hand::get_can_split() const {
    return this->can_split;
}

bool Hand::get_can_surrender() const {
    return this->can_surrender;
}

bool Hand::is_blackjack() const {
    // blackjack if value is 21, made up of only 2 cards,
    // and it is the first hand made (splitting does not count)
    return ((this->value == 21) && (this->num_cards == 2) && (this->can_blackjack));
}

bool Hand::get_active() const {
    return this->active;
}

bool Hand::get_was_doubled() const {
    return this->was_doubled;
}

bool Hand::get_was_surrendered() const {
    return this->was_surrendered;
}

int Hand::get_split_num() const {
    return this->split_num;
}

bool Hand::get_split_aces_final_card() const {
    return this->split_aces_final_card;
}

void Hand::set_has_ace(bool has_ace) {
    this->has_ace = has_ace;
}

void Hand::set_soft(bool soft) {
    this->soft = soft;
}

void Hand::set_can_double(bool can_double) {
    this->can_double = can_double;
}

void Hand::set_can_split(bool can_split) {
    this->can_split = can_split;
}

void Hand::set_can_surrender(bool can_surrender) {
    this->can_surrender = can_surrender;
}

void Hand::set_can_blackjack(bool can_blackjack) {
    this->can_blackjack = can_blackjack;
}

void Hand::set_active(bool active) {
    this->active = active;
}

void Hand::set_was_doubled(bool was_doubled) {
    this->was_doubled = was_doubled;
}

void Hand::set_was_surrendered(bool was_surrendered) {
    this->was_surrendered = was_surrendered;
}

void Hand::increment_split_num() {
    this->split_num++;
}

void Hand::set_split_aces_final_card(bool split_aces_final_card) {
    this->split_aces_final_card = split_aces_final_card;
}

void Hand::update(int card) {
    this->num_cards++;
    
    // update pair variable
    if(this->num_cards == 2) {
        // two separate if statements to ensure no errors
        if(card == this->cards.at(0)) {
            this->pair = true;
        } else {
            this->pair = false;
        }
    } else {
        this->pair = false;
    }

    if(card != 11) {
        // card is not an ace
        this->value += card;
        // if bust and hand is soft, subtract 10 and make hand not soft
        if(this->value > 21 && this->soft == true) {
            this->value -= 10;
            this->soft = false;
        }
        return;
    }

    // card is an ace
    if(this->has_ace == false) {
        // if there are zero aces then the value can be value + 11 as long as it is under 22
        this->has_ace = true;
        if(this->value + 11 <= 21) {
            this->value += 11;
            this->soft = true;
        } else {
            this->value += 1;
        }
        return;
    }

    // if there is already one ace then no matter what the value of this ace must be 1
    this->value += 1;
}