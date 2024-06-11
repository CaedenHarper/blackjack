// TODO: test everything EXTENSIVELY!!!
// TODO: fix all for loops with auto
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
#include <strategy.hpp>

// Program Settings
#include <settings.hpp>

int main() {
    auto start = std::chrono::steady_clock::now();

    // Shoe for simulation
    Shoe shoe = Shoe(MAX_DECKS);
    // Total amount of cards in one deck -- (almost) always constant
    const int ONE_DECK_SIZE = shoe.one_deck.size();
    // Amount of cards to stop dealing when reached
    const int PENETRATION_CARDS = ONE_DECK_SIZE * MAX_DECKS * PENETRATON_PERCENT;

    // Statistic variables
    int num_rounds = 0;
    // num_hands includes hands created from splits; num_rounds does not
    int num_hands = 0;
    int num_wins = 0;
    int num_losses = 0;
    int num_draws = 0;
    int num_player_blackjacks = 0;
    int num_dealer_blackjacks = 0;

    // Total player profit
    // TODO: replace with bankroll, bet spread, true count, etc.
    float player_profit = 0;


    // Simulation loop
    while(num_rounds < MAX_HANDS) {
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

            // if card was split aces, only allow one more card before de-activating
            if(p_current_hand->get_split_aces_final_card()) {
                player.add(shoe, current_index);

                p_current_hand->set_split_aces_final_card(false);
                p_current_hand->set_active(false);

                continue;
            }

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
            case action::hit:
                if(DEBUG) std::cout << "HIT! " << player << " -> ";
                player.hit(shoe, current_index);
                if(DEBUG) std::cout << player << "\n";
                break;
            case action::stand:
                if(DEBUG) std::cout << "STAND! " << player << " -> ";
                player.stand(current_index);
                if(DEBUG) std::cout << player << "\n";
                break;
            case action::double_down:
                if(DEBUG) std::cout << "DOUBLE! " << player << " -> ";
                player.double_down(shoe, current_index);
                if(DEBUG) std::cout << player << "\n";
                break;
            case action::split:
                if(DEBUG) std::cout << "SPLIT! " << player << " -> ";
                player.split(current_index);
                if(DEBUG) std::cout << player << "\n";
                
                // reset pointer.. shouldn't need to be here, but here in case
                p_current_hand = &player.hands[current_index];

                num_rounds--;
                break;
            case action::surrender:
                if(DEBUG) std::cout << "SURRENDER! " << player << " -> ";
                player.surrender(current_index);
                if(DEBUG) std::cout << player << "\n";
                break;
            default:
                if(DEBUG) std::cout << "ERROR! " << player << " -> ";
                if(DEBUG) std::cout << player << "\n";
                break;
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
            if(dealer_action == action::stand) break;

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
            num_rounds++;
            num_hands++;
            // TODO:
            // For some reason player.hands[index] is required because
            // player[index] throws an error even though player[index] simply returns
            // player.hands[index]... eventually fix this
            Hand* p_hand = &player.hands[i];
            int value = p_hand->get_value();
            bool player_blackjack = p_hand->is_blackjack();
            bool player_bust = p_hand->is_bust();
            // define magnitude of money based on double/surrender
            float double_multiplier = p_hand->get_was_doubled() ? 2 : 1;
            float surrender_multiplier = p_hand->get_was_surrendered() ? 0.5 : 1;
            float money_magnitude = 1*double_multiplier*surrender_multiplier;
            // std::cout<<"here14"<<"\n";
            // std::cout<<*p_hand<<"\n";

            // winner logic
            // push
            if(dealer_blackjack && player_blackjack) {
                num_draws++;
                num_dealer_blackjacks++;
                num_player_blackjacks++;
                if(DEBUG) std::cout << "TWO BLACKJACKS" << "\n";
                continue;
            }
            
            // num_losses
            if(dealer_blackjack) {
                num_losses++;
                num_dealer_blackjacks++;
                
                player_profit -= money_magnitude;

                if(DEBUG) std::cout << "DEALER BLACKJACK" << "\n";
                if(DEBUG) std::cout << "-" << money_magnitude << "\n";
                continue;
            }

            // num_wins
            if(player_blackjack) {
                num_wins++;
                num_player_blackjacks++;
        
                player_profit += money_magnitude*BLACKJACK_PAYOUT;
    
                if(DEBUG) std::cout << "PLAYER BLACKJACK" << "\n";
                if(DEBUG) std::cout << money_magnitude*BLACKJACK_PAYOUT << "\n";
                continue;
            }

            // num_losses
            if(player_bust) {
                num_losses++;

                player_profit -= money_magnitude;

                if(DEBUG) std::cout << "PLAYER BUST" << "\n";
                if(DEBUG) std::cout << "-" << money_magnitude << "\n";
                continue;
            }

            // num_wins
            if(dealer[0].is_bust()) {
                num_wins++;

                player_profit += money_magnitude;

                if(DEBUG) std::cout << "DEALER BUST" << "\n";
                if(DEBUG) std::cout << money_magnitude << "\n";
                continue;
            }

            // push
            if(dealer_final_value == value) {
                num_draws++;
                if(DEBUG) std::cout << "STANDARD PUSH" << "\n";
                continue;
            }
            
            // num_wins
            if(dealer_final_value < value) {
                num_wins++;

                player_profit += money_magnitude;

                if(DEBUG) std::cout << "STANDARD WIN" << "\n";
                if(DEBUG) std::cout << money_magnitude << "\n";
                continue;
            }

            // num_losses
            if(dealer_final_value > value) {
                num_losses++;

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
        
        std::cout << "TOTAL = " << num_hands << "\n\n";
        
        std::cout << "WINS = " << num_wins << " -> " << num_wins*1.0/num_hands * 100 << "%\n";
        std::cout << "LOSS = " << num_losses << " -> " << num_losses*1.0/num_hands * 100 << "%\n";
        std::cout << "DRAWS = " << num_draws << " -> " << num_draws*1.0/num_hands * 100 << "%\n\n";

        std::cout << "DEALER BJs = " << num_dealer_blackjacks << " -> " << num_dealer_blackjacks*1.0/num_hands * 100 << "%\n";
        std::cout << "PLAYER BJs = " << num_player_blackjacks << " -> " << num_player_blackjacks*1.0/num_hands * 100 << "%\n\n";
        
        float profit = BETTING_UNIT * player_profit;
        float profit_per_hour = (profit) / (num_hands*1.0 / HANDS_PER_HOUR);

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
