#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <vector>
#include <deck.hpp>

// TODO: implement was_surrendered
class Hand : public Deck {
    public:
    Hand();
    Hand(int card);
    Hand(std::vector<int> cards);
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
    // setters
    void set_has_ace(bool _has_ace);
    void set_soft(bool _soft);
    void set_can_double(bool _can_double);
    void set_can_split(bool _can_split);
    void set_can_surrender(bool _can_surrender);
    void set_can_blackjack(bool _can_blackjack);
    void set_active(bool _can_activate);
    void set_was_doubled(bool _was_doubled);

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
    // number of total hands split from
    int split_num;

    void update(int card);
};

#endif