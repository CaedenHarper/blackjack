// IO
#include <iostream>
#include <iomanip>

// Runtime time calculation
#include <chrono>

// Game
#include <random>

// Classes
#include <deck.h>
#include <hand.h>
#include <shoe.h>
#include <player.h>

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
    if(value < 17) {
        return hit;
    }

    return stand;
}

// TODO: properly comment
action basic_strategy(Hand const hand) {
    const int value = hand.get_value();
    const bool can_double = hand.get_can_double();
    const bool can_split = hand.get_can_split();
    const bool can_surrender = hand.get_can_surrender();

    return hit;
}

 int main() {
    // PROGRAM SETTINGS

    // print debug information throughout the simulation
    constexpr bool DEBUG = false;
    // print stats at the end of the simulation
    constexpr bool PRINT_STATS = true;


    // BLACKJACK RULE VARIATIONS

    // Amount to payout when player gets a blackjack
    // (e.g., if 1.5, 3:2 payout)
    constexpr float BLACKJACK_PAYOUT = 1.5;
    // Number of decks in play
    // (e.g., if 2, Double Deck)
    constexpr int MAX_DECKS = 8;
    // If player can late surrender
    // note: early surrender takes precedence over late surrender
    // (e.g., if early and late are both true, player can early surrender)
    // TODO: implement
    constexpr bool LATE_SURRENDER = false;
    // If player can early surrender
    // note: early surrender takes precedence over late surrender
    // (e.g., if early and late are both true, player can early surrender)
    // TODO: implement
    constexpr bool EARLY_SURRENDER = false;
    // If dealer hits soft seventeen
    // (e.g., if false, dealer stands on A-6)
    // TODO: implement
    constexpr bool HIT_SOFT_SEVENTEEN = true;
    // If player can double after split
    constexpr bool DOUBLE_AFTER_SPLIT = true;
    // If player can split aces
    // TODO: implement
    constexpr bool SPLIT_ACES = true;
    // If player can act on split aces
    // (e.g., if false, player will be given one card and can not act further)
    // TODO: implement
    constexpr bool DRAW_TO_SPLIT_ACES = false;
    // Total number of hands aces can be split to
    // (e.g., if 2, player can split aces to 2 hands total)
    // TODO: implement
    constexpr int SPLIT_ACES_NUM = 2;
    // Total number of hands any non-ace can be split to
    // (e.g., if 4, player can split tens to 4 hands total)
    // TODO: implement
    constexpr int SPLIT_NUM = 4;


    // SIMULATION SETTINGS

    // Total number of hands to play in simulation
    // (e.g., 1000000 = 1,000,000 = 1 million, 1 million hands are played)
    constexpr int MAX_HANDS = 1000000;
    // Percentage of deck to stop dealing at
    constexpr float PENETRATON_PERCENT = 0.25;


    // MONEY SETTINGS

    // Amount of dollars per smallest bet
    constexpr int BETTING_UNIT = 5;
    // Number of hands played per hour
    constexpr int HANDS_PER_HOUR = 250;

    auto start = std::chrono::steady_clock::now();

    // Shoe for simulation
    Shoe shoe = Shoe(MAX_DECKS);
    // Total amount of cards in one deck -- (almost) always constant
    const int ONE_DECK_SIZE = shoe.one_deck.size();
    // Amount of cards to stop dealing when reached
    const int PENETRATION_CARDS = ONE_DECK_SIZE * MAX_DECKS * PENETRATON_PERCENT;

    // Statistic variables
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
    while(total < MAX_HANDS) {
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
            Hand* current_hand = &player.hands[current_index];

            // populate hand if necessary
            while(current_hand->size() < 2) player.add(shoe, current_index);

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
                current_hand->set_active(false);
                continue;
            }
            
            // handle bust
            if(player_value > 21) {
                current_hand->set_active(false);
                continue;
            }

            action act = dealer_strategy(player[current_index]);
            // action act = basic_strategy(player[current_index]);
            switch (act) {
            case hit:
                player.hit(shoe, current_index);
                break;
            case stand:
                player.stand(current_index);
                break;
            case double_down:
                player.double_down(shoe, current_index);
                break;
            case split:
                player.split(current_index);
                break;
            case surrender:
                player.surrender(current_index);
                break;
            default:
                break;
            }
        }
        if(DEBUG) std::cout << "--PLAYER LOOP END--" << "\n\n";
        
        if(DEBUG) std::cout << "--DEALER LOOP--" << "\n";
        if(DEBUG) std::cout << dealer.get_value() << " -> " << dealer << "\n";
        // dealer hitting loop
        bool all_busted = player.all_busted();
        while(dealer.get_value() < 17) {
            // exit loop if all player hands are busted
            if(all_busted) break;

            dealer.hit(shoe);

            // exit loop if dealer busts
            if(dealer[0].is_bust()) break;
            if(DEBUG) std::cout << dealer.get_value() << " -> " << dealer << "\n";
        }
        if(DEBUG) std::cout << "--DEALER LOOP END--" << "\n\n";

        if(DEBUG) std::cout << "--WINNING LOOP--" << "\n";
        int dealer_final_value = dealer.get_value();
        // player winning/losing loop
        for(int i = 0; i < player.hands.size(); i++) {
            // TODO:
            // For some reason player.hands[index] is required because
            // player[index] throws an error even though player[index] simply returns
            // player.hands[index]... eventually fix this
            Hand* hand = &player.hands[i];
            int value = hand->get_value();

            // winner logic
            // push
            if(dealer_blackjack && hand->is_blackjack()) {
                total++;
                draw++;
                dealer_blackjacks++;
                player_blackjacks++;
                if(DEBUG) std::cout << "TWO BLACKJACKS" << "\n";
                continue;
            }
            
            // loss
            if(dealer_blackjack) {
                total++;
                loss++;
                dealer_blackjacks++;
                
                player_profit -= 1;

                if(DEBUG) std::cout << "DEALER BLACKJACK" << "\n";
                continue;
            }

            // win
            if(hand->is_blackjack()) {
                total++;
                win++;
                player_blackjacks++;
        
                player_profit += 1*BLACKJACK_PAYOUT;
    
                if(DEBUG) std::cout << "PLAYER BLACKJACK" << "\n";
                continue;
            }

            // loss
            if(hand->is_bust()) {
                total++;
                loss++;

                player_profit -= 1;

                if(DEBUG) std::cout << "PLAYER BUST" << "\n";
                continue;
            }

            // win
            if(dealer[0].is_bust()) {
                total++;
                win++;

                player_profit += 1;

                if(DEBUG) std::cout << "DEALER BUST" << "\n";
                continue;
            }

            // push
            if(dealer_final_value == value) {
                total++;
                draw++;
                if(DEBUG) std::cout << "STANDARD PUSH" << "\n";
                continue;
            }
            
            // win
            if(dealer_final_value < value) {
                total++;
                win++;

                player_profit += 1;

                if(DEBUG) std::cout << "STANDARD WIN" << "\n";
                continue;
            }

            // loss
            if(dealer_final_value > value) {
                total++;
                loss++;

                player_profit -= 1;

                if(DEBUG) std::cout << "STANDARD LOSS" << "\n";
                continue;
            }

        // should never get here
        total++;
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
