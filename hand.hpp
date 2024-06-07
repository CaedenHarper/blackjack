#ifndef HAND_H
#define HAND_H

// OTHER MODULES
#include <deck.hpp>

// STANDARD LIBRARY
#include <iostream>
#include <vector>

// TODO: implement was_surrendered
class Hand : public Deck {
    public:
    Hand();
    Hand(int card);
    friend std::ostream& operator<<(std::ostream &out, const Hand &hand);
    void push_back(const int& val);
    // getters
    int get_value() const;
    bool is_bust() const;
    int size() const;
    bool get_has_ace() const;
    bool get_soft() const;
    bool get_pair() const;
    bool get_can_double() const;
    bool get_can_split() const;
    bool get_can_surrender() const;
    bool is_blackjack() const;
    bool get_active() const;
    bool get_was_doubled() const;
    bool get_was_surrendered() const;
    int get_split_num() const;
    bool get_split_aces_final_card() const;
    // setters
    void set_has_ace(bool _has_ace);
    void set_soft(bool _soft);
    void set_can_double(bool _can_double);
    void set_can_split(bool _can_split);
    void set_can_surrender(bool _can_surrender);
    void set_can_blackjack(bool _can_blackjack);
    void set_active(bool _active);
    void set_was_doubled(bool _was_doubled);
    void set_was_surrendered(bool _was_doubled);
    void increment_split_num();
    void set_split_aces_final_card(bool _split_aces_final_card);

    private:
    int value;
    int num_cards;
    // if hand contains one ace
    bool has_ace;
    // if hand is considered "soft",
    // meaning it has an ace acting as an 11
    bool soft;
    // if hand has two cards which are the same
    bool pair;
    bool can_double;
    bool can_split;
    bool can_surrender;
    bool can_blackjack;
    // if hand is still in play
    bool active;
    // if hand was previously doubled down on
    bool was_doubled;
    // if hand was previously surrendered on
    bool was_surrendered;
    // number of total hands split from
    int split_num;
    // if card needs to be drawn to exactly 2 because of no drawing to split aces rule
    bool split_aces_final_card;

    void update(int card);
};

#endif