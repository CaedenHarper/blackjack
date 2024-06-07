// TODO in ALL files: add "this" to all classes
// IO
#include <iostream>
#include <iomanip>

// Runtime time calculation
#include <chrono>

// Game
#include <random>

// Classes
#include <deck.hpp>
#include <hand.hpp>
#include <shoe.hpp>
#include <player.hpp>

// Program Settings
#include <settings.hpp>

// TODO: properly comment
enum action {
    hit,
    stand,
    double_down,
    split,
    surrender
};

// TODO: properly comment
action dealer_strategy(Hand const hand) {
    const int value = hand.get_value();

    // S17/H17 distinction
    // Note: this also affects how players using naive dealer strategy play
    // This is not necessarily a bug, so I did not make a distinction
    if(value == 17 && hand.get_soft() && HIT_SOFT_SEVENTEEN) {
        return hit;
    }

    if(value < 17) {
        return hit;
    }

    return stand;
}

// TODO: properly comment
action basic_strategy_one_deck(Hand const hand, int const dealer_card) {
    return hit;
}

// TODO: properly comment
action basic_strategy_two_deck(Hand const hand, int const dealer_card) {
    return hit;
}

// TODO: properly comment
// TODO: currently automatically assumes DAS; fix
// TODO: refactor
// TODO: generalize to a file
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
                if(can_split) return split;
                if(dealer_card == 6 && can_double) return double_down;
                return hit;
            case 13:
            case 14:
                switch (dealer_card) {
                    case 2:
                    case 3:
                    case 4:
                        return hit;
                    case 5:
                    case 6:
                        if(can_double) return double_down;
                        return hit;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return hit;
                    default:
                        return hit;
                }
            case 15:
            case 16:
                switch (dealer_card) {
                    case 2:
                    case 3:
                        return hit;
                    case 4:
                    case 5:
                    case 6:
                        if(can_double) return double_down;
                        return hit;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return hit;
                    default:
                        return hit;
                }
            case 17:
                switch (dealer_card) {
                    case 2:
                        return hit;
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if(can_double) return double_down;
                        return hit;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return hit;
                    default:
                        return hit;
                }
            case 18:
                switch (dealer_card) {
                    case 2:
                        if(HIT_SOFT_SEVENTEEN && can_double) return double_down;
                        return stand;
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if(can_double) return double_down;
                        return stand;
                    case 7:
                    case 8:
                        return stand;
                    case 9:
                    case 10:
                    case 11:
                        return hit;
                    default:
                        return hit;
                }
            case 19:
                if(HIT_SOFT_SEVENTEEN && can_double && dealer_card == 6) return double_down;
                return stand;
            case 20:
            case 21:
                return stand;
            default:
                return hit;
        }
    } else if(hand.get_pair()) {
        // pair
        int pair_card = hand[0];
        switch (pair_card) {
        case 2:
        case 3:
            switch (dealer_card) {
            case 2:
            case 3:
                if(can_split && DOUBLE_AFTER_SPLIT) return split;
                return hit;
            case 4:
            case 5:
            case 6:
            case 7:
                if(can_split) return split;
                return hit;
            case 8:
            case 9:
            case 10:
            case 11:
                return hit;
            default:
                return hit;
            }
        case 4:
            switch (dealer_card) {
                case 2:
                case 3:
                case 4:
                    return hit;
                case 5:
                case 6:
                    if(can_split && DOUBLE_AFTER_SPLIT) return split;
                    return hit;
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                    return hit;
                default:
                    return hit;
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
                    if(can_double) return double_down;
                    return hit;
                case 10:
                case 11:
                    return hit;
                default:
                    return hit;
            }
        case 6:
            switch (dealer_card) {
                case 2:
                    if(can_split && DOUBLE_AFTER_SPLIT) return split;
                    return hit;
                case 3:
                case 4:
                case 5:
                case 6:
                    if(can_split) return split;
                    return hit;
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                    return hit;
                default:
                    return hit;
            }
        case 7:
            switch (dealer_card) {
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    if(can_split) return split;
                    return hit;
                case 8:
                case 9:
                case 10:
                case 11:
                    return hit;
                default:
                    return hit;
            }
        case 8:
            switch (dealer_card) {
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    if(can_split) return split;
                    return stand;
                case 7:
                case 8:
                    if(can_split) return split;
                    return hit;
                case 9:
                case 10:
                    if(can_split) return split;
                    if(can_surrender && LATE_SURRENDER) return surrender;
                    return hit;
                case 11:
                    if(HIT_SOFT_SEVENTEEN && can_surrender && LATE_SURRENDER) return surrender;
                    if(can_split) return split;
                    if(can_surrender && LATE_SURRENDER) return surrender;
                    return hit;
                default:
                    return hit;
            }
        case 9:
            switch (dealer_card) {
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    if(can_split) return split;
                    return stand;
                case 7:
                    return stand;
                case 8:
                case 9:
                    if(can_split) return split;
                    return stand;
                case 10:
                case 11:
                    return stand;
                default:
                    return stand;
            }
        case 10:
            return stand;
        case 11:
            if(can_split) return split;
            if(dealer_card == 6 && can_double) return double_down;
            return hit;
        default:
            return hit;
        }
    } else {
        // hard
        switch (player_value) {
            case 5:
            case 6:
            case 7:
            case 8:
                return hit;
            case 9:
                switch (dealer_card) {
                    case 2:
                        return hit;
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if(can_double) return double_down;
                        return hit;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return hit;
                    default:
                        return hit;
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
                        if(can_double) return double_down;
                        return hit;
                    case 10:
                        return hit;
                    case 11:
                        return hit;
                    default:
                        return hit;
                }
            case 11:
                if(!HIT_SOFT_SEVENTEEN && dealer_card == 11) return hit;
                if(can_double) return double_down;
                return hit;
            case 12:
                switch (dealer_card) {
                    case 2:
                    case 3:
                        return hit;
                    case 4:
                    case 5:
                    case 6:
                        return stand;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return hit;
                    default:
                        return hit;
                }
            case 13:
            case 14:
                switch (dealer_card) {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        return stand;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return hit;
                    default:
                        return hit;
                }
            case 15:
                switch (dealer_card) {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        return stand;
                    case 7:
                    case 8:
                    case 9:
                        return hit;
                    case 10:
                        if(can_surrender && LATE_SURRENDER) return surrender;
                        return hit;
                    case 11:
                        if(HIT_SOFT_SEVENTEEN && can_surrender && LATE_SURRENDER) return surrender;
                        return hit;
                    default:
                        return hit;
                }
            case 16:
                switch (dealer_card) {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if(can_split) return split;
                        return stand;
                    case 7:
                    case 8:
                        if(can_split) return split;
                        return hit;
                    case 9:
                    case 10:
                        if(can_split) return split;
                        if(can_surrender && LATE_SURRENDER) return surrender;
                        return hit;
                    case 11:
                        if(HIT_SOFT_SEVENTEEN && can_surrender && LATE_SURRENDER) return surrender;
                        if(can_split) return split;
                        if(can_surrender && LATE_SURRENDER) return surrender;
                        return hit;
                    default:
                        return hit;
                }
            case 17:
                if(HIT_SOFT_SEVENTEEN && can_surrender && LATE_SURRENDER) return surrender;
                return stand;
            case 18:
            case 19:
            case 20:
            case 21:
                return stand;
            default:
                return hit;
        }
    }
    // should never get here
    return hit;
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

 int main() {
    auto start = std::chrono::steady_clock::now();

    // Shoe for simulation
    Shoe shoe = Shoe(MAX_DECKS);
    // Total amount of cards in one deck -- (almost) always constant
    const int ONE_DECK_SIZE = shoe.one_deck.size();
    // Amount of cards to stop dealing when reached
    const int PENETRATION_CARDS = ONE_DECK_SIZE * MAX_DECKS * PENETRATON_PERCENT;

    // Statistic variables
    int total_hands = 0;
    int total = 0;
    int win = 0;
    int loss = 0;
    int draw = 0;
    int player_blackjacks = 0;
    int dealer_blackjacks = 0;

    // Total player profit
    // TODO: replace with bankroll, bet spread, true count, etc.
    float player_profit = 0;


    // Simulation loop
    while(total_hands < MAX_HANDS) {
        if(shoe.size() <= PENETRATION_CARDS) shoe = Shoe(MAX_DECKS);  
        // Player variables
        Player player = Player();
        Player dealer = Player();

        // Populate initial hands
        player.add(shoe);
        dealer.add(shoe);
        player.add(shoe);
        dealer.add(shoe);
        
        // Useful dealer constants
        const int dealer_upcard = dealer[0][0];
        const int dealer_init_value = dealer.get_value();
        Hand* p_dealer_hand = &dealer.hands[0];

        // Has the dealer gotten a blackjack
        bool dealer_blackjack = false;

        if(DEBUG) std::cout << "--PLAYER LOOP--" << "\n";
        // main round loop
        while(true) {
            // exit loop if no active hands
            std::vector<int> active_indices = player.get_active_indices();

            if(active_indices.size() == 0) break;

            // init current values
            int current_index = active_indices.at(0);
            // TODO:
            // For some reason player.hands[index] is required because
            // player[index] throws an error even though player[index] simply returns
            // player.hands[index]... eventually fix this
            Hand* p_current_hand = &player.hands[current_index];

            // populate hand if necessary
            while(p_current_hand->size() < 2) player.add(shoe, current_index);

            int player_value = player.get_value(current_index);
            
            if(DEBUG) {
                std::cout << "Dealer: " << dealer_upcard << "\n";
                std::cout << "Player: " << player_value << " -> " << player << "\n";
            }

            // handle blackjacks
            if(dealer_init_value == 21) {
                dealer_blackjack = true;
                break;
            }
            if(player_value == 21) {
                p_current_hand->set_active(false);
                continue;
            }
            
            // handle bust
            if(player_value > 21) {
                p_current_hand->set_active(false);
                continue;
            }

            // action player_action = dealer_strategy(*p_current_hand);
            action player_action = basic_strategy(*p_current_hand, dealer_upcard);
            // TODO: consider wrapping in a function
            switch (player_action) {
            case hit:
                if(DEBUG) std::cout << "HIT! " << player << " -> ";
                player.hit(shoe, current_index);
                if(DEBUG) std::cout << player << "\n";
                break;
            case stand:
                if(DEBUG) std::cout << "STAND! " << player << " -> ";
                player.stand(current_index);
                if(DEBUG) std::cout << player << "\n";
                break;
            case double_down:
                if(DEBUG) std::cout << "DOUBLE! " << player << " -> ";
                player.double_down(shoe, current_index);
                if(DEBUG) std::cout << player << "\n";
                break;
            case split:
                if(DEBUG) std::cout << "SPLIT! " << player << " -> ";
                player.split(current_index);
                if(DEBUG) std::cout << player << "\n";
                total_hands--;
                break;
            case surrender:
                if(DEBUG) std::cout << "SURRENDER! " << player << " -> ";
                player.surrender(current_index);
                if(DEBUG) std::cout << player << "\n";
                break;
            default:
                if(DEBUG) std::cout << "ERROR! " << player << " -> ";
                break;
                if(DEBUG) std::cout << player << "\n";
            }
        }
        if(DEBUG) std::cout << "--PLAYER LOOP END--" << "\n\n";
        
        if(DEBUG) std::cout << "--DEALER LOOP--" << "\n";
        if(DEBUG) std::cout << dealer.get_value() << " -> " << dealer << "\n";
        // dealer hitting loop
        bool all_busted = player.all_busted();
        bool all_blackjacks = player.all_blackjacks();
        while(true) {
            // exit loop if all players have busted or all players have blackjacks
            if(all_busted || all_blackjacks) break;

            action dealer_action = dealer_strategy(*p_dealer_hand);

            // exit loop if dealer stands
            if(dealer_action == stand) break;

            dealer.hit(shoe);

            // exit loop if dealer busts
            if(p_dealer_hand->is_bust()) break;

            if(DEBUG) std::cout << dealer.get_value() << " -> " << dealer << "\n";
        }
        if(DEBUG) std::cout << dealer.get_value() << " -> " << dealer << "\n";
        if(DEBUG) std::cout << "--DEALER LOOP END--" << "\n\n";
        
        if(DEBUG) std::cout << "--WINNING LOOP--" << "\n";
        int dealer_final_value = dealer.get_value();
        // player winning/losing loop
        for(int i = 0; i < player.hands.size(); i++) {
            total_hands++;
            total++;
            // TODO:
            // For some reason player.hands[index] is required because
            // player[index] throws an error even though player[index] simply returns
            // player.hands[index]... eventually fix this
            Hand* p_hand = &player.hands[i];
            int value = p_hand->get_value();
            // define magnitude of money based on if double
            float double_multiplier = p_hand->get_was_doubled() ? 2 : 1;
            float money_magnitude = 1*double_multiplier;

            // winner logic
            // push
            if(dealer_blackjack && p_hand->is_blackjack()) {
                draw++;
                dealer_blackjacks++;
                player_blackjacks++;
                if(DEBUG) std::cout << "TWO BLACKJACKS" << "\n";
                continue;
            }
            
            // loss
            if(dealer_blackjack) {
                loss++;
                dealer_blackjacks++;
                
                player_profit -= money_magnitude;

                if(DEBUG) std::cout << "DEALER BLACKJACK" << "\n";
                if(DEBUG) std::cout << "-" << money_magnitude << "\n";
                continue;
            }

            // win
            if(p_hand->is_blackjack()) {
                win++;
                player_blackjacks++;
        
                player_profit += money_magnitude*BLACKJACK_PAYOUT;
    
                if(DEBUG) std::cout << "PLAYER BLACKJACK" << "\n";
                if(DEBUG) std::cout << money_magnitude*BLACKJACK_PAYOUT << "\n";
                continue;
            }

            // loss
            if(p_hand->is_bust()) {
                loss++;

                player_profit -= money_magnitude;

                if(DEBUG) std::cout << "PLAYER BUST" << "\n";
                if(DEBUG) std::cout << "-" << money_magnitude << "\n";
                continue;
            }

            // win
            if(dealer[0].is_bust()) {
                win++;

                player_profit += money_magnitude;

                if(DEBUG) std::cout << "DEALER BUST" << "\n";
                if(DEBUG) std::cout << money_magnitude << "\n";
                continue;
            }

            // push
            if(dealer_final_value == value) {
                draw++;
                if(DEBUG) std::cout << "STANDARD PUSH" << "\n";
                continue;
            }
            
            // win
            if(dealer_final_value < value) {
                win++;

                player_profit += money_magnitude;

                if(DEBUG) std::cout << "STANDARD WIN" << "\n";
                if(DEBUG) std::cout << money_magnitude << "\n";
                continue;
            }

            // loss
            if(dealer_final_value > value) {
                loss++;

                player_profit -= money_magnitude;

                if(DEBUG) std::cout << "STANDARD LOSS" << "\n";
                if(DEBUG) std::cout << "-" << money_magnitude << "\n";
                continue;
            }

        // should never get here
        }
        if(DEBUG) std::cout << "--WINNING LOOP END--" << "\n\n";
    }
    
    if(PRINT_STATS) {
        // calculate program time early to avoid time from printing (even though it is negligable)
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;

        // prevent scientific notation in printout
        std::cout << std::fixed;
        std::cout << std::setprecision(2);
        
        std::cout << "TOTAL = " << total << "\n\n";
        
        std::cout << "WINS = " << win << " -> " << win*1.0/total * 100 << "%\n";
        std::cout << "LOSS = " << loss << " -> " << loss*1.0/total * 100 << "%\n";
        std::cout << "DRAWS = " << draw << " -> " << draw*1.0/total * 100 << "%\n\n";

        std::cout << "DEALER BJs = " << dealer_blackjacks << " -> " << dealer_blackjacks*1.0/total * 100 << "%\n";
        std::cout << "PLAYER BJs = " << player_blackjacks << " -> " << player_blackjacks*1.0/total * 100 << "%\n\n";
        
        float profit = BETTING_UNIT * player_profit;
        float profit_per_hour = (profit) / (total*1.0 / HANDS_PER_HOUR);

        // define sign chars to print dollar sign prettier
        // e.g., -$400 instead of $-400
        char profit_sign;
        if(profit < 0) {
            // if negative, turn positive and assign negative sign
            profit *= -1;
            profit_sign = '-';
        } else {
            // if non-negative, assign no sign
            profit_sign = '\0';
        }

        char profit_per_hour_sign;
        if(profit_per_hour < 0) {
            // if negative, turn positive and assign negative sign
            profit_per_hour *= -1;
            profit_per_hour_sign = '-';
        } else {
            // if non-negative, assign no sign
            profit_per_hour_sign = '\0';
        }

        std::cout << "PLAYER PROFIT:\n";
        std::cout << "-> " << player_profit << " betting units\n";
        std::cout << "-> " << profit_sign << "$" << profit << "\n";
        std::cout << "-> " << profit_per_hour_sign << "$" << profit_per_hour << "/hr\n\n";

        std::cout << std::chrono::duration<double, std::milli>(diff).count() *1.0 / 1000     << " seconds" << std::endl;
    }
}
