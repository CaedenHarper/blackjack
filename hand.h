#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <vector>
#include <deck.h>

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
    bool get_can_hit() const;
    bool get_can_double() const;
    bool get_can_split() const;
    bool get_can_surrender() const;
    bool is_blackjack() const;
    bool get_active() const;
    // setters
    void set_has_ace(bool _has_ace);
    void set_soft(bool _soft);
    void set_can_hit(bool _can_hit);
    void set_can_double(bool _can_double);
    void set_can_split(bool _can_split);
    void set_can_surrender(bool _can_surrender);
    void set_can_blackjack(bool _can_blackjack);
    void set_active(bool _can_activate);

    private:
    int value;
    int num_cards;
    bool has_ace;
    bool soft;
    bool can_hit;
    bool can_double;
    bool can_split;
    bool can_surrender;
    bool can_blackjack;
    bool active;

    void update(int card);
};

#endif