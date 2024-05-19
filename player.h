#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <hand.h>

class Player {
    public:
    std::vector<Hand> hands;

    Player();
    Hand operator [](int i);
    friend std::ostream& operator<<(std::ostream &out, const Player &player);
    int size();
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
    bool is_active();
    bool all_busted(); 
};

#endif