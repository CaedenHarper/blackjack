#ifndef PLAYER_H
#define PLAYER_H

// OTHER MODULE IMPORTS
#include <hand.hpp>
#include <shoe.hpp>

// STANDARD LIBRARY
#include <iostream>
#include <vector>

class Player {
    public:
    std::vector<Hand> hands;

    Player();
    Hand at(int i) const;
    std::vector<Hand>::iterator begin();
    std::vector<Hand>::iterator end();
    friend std::ostream& operator<<(std::ostream &out, const Player &player);
    int size() const;
    int add(Shoe& shoe);
    int add(Shoe& shoe, int index);
    int hit(Shoe& shoe);
    int hit(Shoe& shoe, int index);
    int stand();
    int stand(int index);
    int double_down(Shoe& shoe);
    int double_down(Shoe& shoe, int index);
    int split();
    int split(int index);
    int surrender();
    int surrender(int index);
    int get_value();
    int get_value(int index);
    int get_first_active_index();
    bool all_busted();
    bool all_blackjacks();
};

#endif