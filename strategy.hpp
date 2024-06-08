#ifndef STRATEGY_H
#define STRATEGY_H

#include <hand.hpp>

// TODO: properly comment
enum class action {
    hit = 0,
    stand = 1,
    double_down = 2,
    split = 3,
    surrender = 4
};

// TODO: properly comment
action dealer_strategy(Hand const hand);

// TODO: properly comment
// TODO: refactor
// TODO: generalize to a file
action basic_strategy_general(Hand const hand, int const dealer_card);

// TODO: properly comment
action basic_strategy_one_deck(Hand const hand, int const dealer_card);

// TODO: properly comment
action basic_strategy_two_deck(Hand const hand, int const dealer_card);

// TODO: properly comment
action basic_strategy(Hand const hand, int const dealer_card);

#endif