// TODO: generalize to an arbitrary file input
// SELF
#include <strategy.hpp>

// OTHER MODULES
#include <settings.hpp>

// TODO: properly comment
action dealer_strategy(Hand const hand) {
    const int value = hand.get_value();

    // S17/H17 distinction
    // Note: this also affects how players using naive dealer strategy play
    // This is not necessarily a bug, so I did not make a distinction
    if(value == 17 && hand.get_soft() && HIT_SOFT_SEVENTEEN) {
        return action::hit;
    }

    if(value < 17) {
        return action::hit;
    }

    return action::stand;
}

// TODO: properly comment
// TODO: currently automatically assumes DAS; fix
// TODO: refactor
action basic_strategy_general(Hand const hand, int const dealer_card) {
    int player_value = hand.get_value();
    bool can_split = hand.get_can_split();
    bool can_double = hand.get_can_double();
    bool can_surrender = hand.get_can_surrender();

    // Every hand is either soft, a pair, or hard
    if(hand.get_soft()) {
        // soft
        switch (player_value) {
            case 12:
                // same as a pair of aces
                if(can_split) return action::split;
                if(dealer_card == 6 && can_double) return action::double_down;
                return action::hit;
            case 13:
            case 14:
                switch (dealer_card) {
                    case 2:
                    case 3:
                    case 4:
                        return action::hit;
                    case 5:
                    case 6:
                        if(can_double) return action::double_down;
                        return action::hit;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return action::hit;
                    default:
                        return action::hit;
                }
            case 15:
            case 16:
                switch (dealer_card) {
                    case 2:
                    case 3:
                        return action::hit;
                    case 4:
                    case 5:
                    case 6:
                        if(can_double) return action::double_down;
                        return action::hit;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return action::hit;
                    default:
                        return action::hit;
                }
            case 17:
                switch (dealer_card) {
                    case 2:
                        return action::hit;
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if(can_double) return action::double_down;
                        return action::hit;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return action::hit;
                    default:
                        return action::hit;
                }
            case 18:
                switch (dealer_card) {
                    case 2:
                        if(HIT_SOFT_SEVENTEEN && can_double) return action::double_down;
                        return action::stand;
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if(can_double) return action::double_down;
                        return action::stand;
                    case 7:
                    case 8:
                        return action::stand;
                    case 9:
                    case 10:
                    case 11:
                        return action::hit;
                    default:
                        return action::hit;
                }
            case 19:
                if(HIT_SOFT_SEVENTEEN && can_double && dealer_card == 6) return action::double_down;
                return action::stand;
            case 20:
            case 21:
                return action::stand;
            default:
                return action::hit;
        }
    } else if(hand.get_pair()) {
        // pair
        int pair_card = hand.at(0);
        switch (pair_card) {
        case 2:
        case 3:
            switch (dealer_card) {
            case 2:
            case 3:
                if(can_split && DOUBLE_AFTER_SPLIT) return action::split;
                return action::hit;
            case 4:
            case 5:
            case 6:
            case 7:
                if(can_split) return action::split;
                return action::hit;
            case 8:
            case 9:
            case 10:
            case 11:
                return action::hit;
            default:
                return action::hit;
            }
        case 4:
            switch (dealer_card) {
                case 2:
                case 3:
                case 4:
                    return action::hit;
                case 5:
                case 6:
                    if(can_split && DOUBLE_AFTER_SPLIT) return action::split;
                    return action::hit;
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                    return action::hit;
                default:
                    return action::hit;
            }
        case 5:
            switch (dealer_card) {
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                    if(can_double) return action::double_down;
                    return action::hit;
                case 10:
                case 11:
                    return action::hit;
                default:
                    return action::hit;
            }
        case 6:
            switch (dealer_card) {
                case 2:
                    if(can_split && DOUBLE_AFTER_SPLIT) return action::split;
                    return action::hit;
                case 3:
                case 4:
                case 5:
                case 6:
                    if(can_split) return action::split;
                    return action::hit;
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                    return action::hit;
                default:
                    return action::hit;
            }
        case 7:
            switch (dealer_card) {
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    if(can_split) return action::split;
                    return action::hit;
                case 8:
                case 9:
                case 10:
                case 11:
                    return action::hit;
                default:
                    return action::hit;
            }
        case 8:
            switch (dealer_card) {
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    if(can_split) return action::split;
                    return action::stand;
                case 7:
                case 8:
                    if(can_split) return action::split;
                    return action::hit;
                case 9:
                case 10:
                    if(can_split) return action::split;
                    if(can_surrender && LATE_SURRENDER) return action::surrender;
                    return action::hit;
                case 11:
                    if(HIT_SOFT_SEVENTEEN && can_surrender && LATE_SURRENDER) return action::surrender;
                    if(can_split) return action::split;
                    if(can_surrender && LATE_SURRENDER) return action::surrender;
                    return action::hit;
                default:
                    return action::hit;
            }
        case 9:
            switch (dealer_card) {
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    if(can_split) return action::split;
                    return action::stand;
                case 7:
                    return action::stand;
                case 8:
                case 9:
                    if(can_split) return action::split;
                    return action::stand;
                case 10:
                case 11:
                    return action::stand;
                default:
                    return action::stand;
            }
        case 10:
            return action::stand;
        case 11:
            if(can_split) return action::split;
            if(dealer_card == 6 && can_double) return action::double_down;
            return action::hit;
        default:
            return action::hit;
        }
    } else {
        // hard
        switch (player_value) {
            case 5:
            case 6:
            case 7:
            case 8:
                return action::hit;
            case 9:
                switch (dealer_card) {
                    case 2:
                        return action::hit;
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if(can_double) return action::double_down;
                        return action::hit;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return action::hit;
                    default:
                        return action::hit;
                }
            case 10:
                switch (dealer_card) {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        if(can_double) return action::double_down;
                        return action::hit;
                    case 10:
                        return action::hit;
                    case 11:
                        return action::hit;
                    default:
                        return action::hit;
                }
            case 11:
                if(!HIT_SOFT_SEVENTEEN && dealer_card == 11) return action::hit;
                if(can_double) return action::double_down;
                return action::hit;
            case 12:
                switch (dealer_card) {
                    case 2:
                    case 3:
                        return action::hit;
                    case 4:
                    case 5:
                    case 6:
                        return action::stand;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return action::hit;
                    default:
                        return action::hit;
                }
            case 13:
            case 14:
                switch (dealer_card) {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        return action::stand;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return action::hit;
                    default:
                        return action::hit;
                }
            case 15:
                switch (dealer_card) {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        return action::stand;
                    case 7:
                    case 8:
                    case 9:
                        return action::hit;
                    case 10:
                        if(can_surrender && LATE_SURRENDER) return action::surrender;
                        return action::hit;
                    case 11:
                        if(HIT_SOFT_SEVENTEEN && can_surrender && LATE_SURRENDER) return action::surrender;
                        return action::hit;
                    default:
                        return action::hit;
                }
            case 16:
                switch (dealer_card) {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if(can_split) return action::split;
                        return action::stand;
                    case 7:
                    case 8:
                        if(can_split) return action::split;
                        return action::hit;
                    case 9:
                    case 10:
                        if(can_split) return action::split;
                        if(can_surrender && LATE_SURRENDER) return action::surrender;
                        return action::hit;
                    case 11:
                        if(HIT_SOFT_SEVENTEEN && can_surrender && LATE_SURRENDER) return action::surrender;
                        if(can_split) return action::split;
                        if(can_surrender && LATE_SURRENDER) return action::surrender;
                        return action::hit;
                    default:
                        return action::hit;
                }
            case 17:
                if(HIT_SOFT_SEVENTEEN && can_surrender && LATE_SURRENDER) return action::surrender;
                return action::stand;
            case 18:
            case 19:
            case 20:
            case 21:
                return action::stand;
            default:
                return action::hit;
        }
    }
    // should never get here
    return action::hit;
}

// TODO: properly comment
action basic_strategy_one_deck(Hand const hand, int const dealer_card) {
    return action::hit;
}

// TODO: properly comment
action basic_strategy_two_deck(Hand const hand, int const dealer_card) {
    int player_value = hand.get_value();
    bool is_soft = hand.get_soft();
    bool is_pair = hand.get_pair();
    bool can_split = hand.get_can_split();
    bool can_double = hand.get_can_double();

    // hard
    if(player_value == 9 && dealer_card == 2 && can_double) return action::double_down;
    if(player_value == 11 && dealer_card == 11 && can_double) return action::double_down;
    if(player_value == 11 && dealer_card == 11) return action::hit;
    if(player_value == 16 && dealer_card == 9) return action::hit;

    // soft
    if(is_soft && player_value == 13 && dealer_card == 4 && HIT_SOFT_SEVENTEEN && can_double) return action::double_down;

    // pair
    if(is_pair && player_value == 12 && dealer_card == 7 && DOUBLE_AFTER_SPLIT && can_split) return action::split;
    if(is_pair && player_value == 14 && dealer_card == 8 && DOUBLE_AFTER_SPLIT && can_split) return action::split;

    return basic_strategy_general(hand, dealer_card);
}

// TODO: properly comment
action basic_strategy(Hand const hand, int const dealer_card) {
    switch (MAX_DECKS) {
    case 1:
        return basic_strategy_one_deck(hand, dealer_card);
    case 2:
        return basic_strategy_two_deck(hand, dealer_card);
    default:
        return basic_strategy_general(hand, dealer_card);
    }
}