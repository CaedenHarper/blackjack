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

enum action {
    hit,
    stand,
    double_down,
    split,
    surrender
};

action dealer_strategy(Hand const hand) {
    const int value = hand.get_value();
    if(value < 17) {
        return hit;
    } else {
        return stand;
    }
}

action basic_strategy(Hand const hand) {
    const int value = hand.get_value();
    const bool can_double = hand.get_can_double();
    const bool can_split = hand.get_can_split();
    const bool can_surrender = hand.get_can_surrender();

    return hit;
}

 int main() {
    constexpr int DECK_SIZE = 52;

    constexpr bool DEBUG = false;
    constexpr bool PRINT_STATS = true;

    constexpr int MAX_DECKS = 8;
    constexpr int MAX_HANDS = 1000000;      // 1000000 = 1,000,000 = 1 million
    constexpr float PENETRATON_PERCENT = 0.25;
    constexpr int PENETRATION_CARDS = DECK_SIZE * MAX_DECKS * PENETRATON_PERCENT;

    constexpr int BETTING_UNIT = 5;
    constexpr int HANDS_PER_HOUR = 250;

    auto start = std::chrono::steady_clock::now();

    // init shoe
    Shoe shoe = Shoe(MAX_DECKS);

    // init statistic vars
    int total = 0;
    int win = 0;
    int loss = 0;
    int draw = 0;
    int player_blackjacks = 0;
    int dealer_blackjacks = 0;

    float player_profit = 0;

    // TODO: Consider defining changing variables outside of the loop
    // round loops
    while(total < MAX_HANDS) {
        if(shoe.size() <= PENETRATION_CARDS) shoe = Shoe(MAX_DECKS);  
        // init players
        Player player = Player();
        Player dealer = Player();

        player.add(shoe);
        dealer.add(shoe);
        player.add(shoe);
        dealer.add(shoe);
        
        // init dealer values
        const int dealer_upcard = dealer[0][0];
        const int dealer_init_value = dealer.get_value();
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
                player_profit--;
                if(DEBUG) std::cout << "DEALER BLACKJACK" << "\n";
                continue;
            }

            // win
            if(hand->is_blackjack()) {
                total++;
                win++;
                player_blackjacks++;
                player_profit += 1.5;
                if(DEBUG) std::cout << "PLAYER BLACKJACK" << "\n";
                continue;
            }

            // loss
            if(hand->is_bust()) {
                total++;
                loss++;
                player_profit--;
                if(DEBUG) std::cout << "PLAYER BUST" << "\n";
                continue;
            }

            // win
            if(dealer[0].is_bust()) {
                total++;
                win++;
                player_profit++;
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
                player_profit++;
                if(DEBUG) std::cout << "STANDARD WIN" << "\n";
                continue;
            }

            // loss
            if(dealer_final_value > value) {
                total++;
                loss++;
                player_profit--;
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
