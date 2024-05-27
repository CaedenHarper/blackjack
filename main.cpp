#include <iostream>
#include <random>
// auto rd = std::random_device {}; 
// auto rng = std::default_random_engine { rd() };
#include <deck.h>
#include <hand.h>
#include <shoe.h>
#include <player.h>
#include <chrono>

int main() {
    auto start = std::chrono::steady_clock::now();
        
    // init RNG
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };

    // init shoe
    Shoe shoe = Shoe(1000000, rng);

    // init statistic vars
    int total = 0;
    int win = 0;
    int loss = 0;
    int draw = 0;
    int player_blackjacks = 0;
    int dealer_blackjacks = 0;

    // round loops
    while(shoe.size() > 52) {
        // init players
        Player player = Player();
        Player dealer = Player();

        player.add(shoe);
        dealer.add(shoe);
        player.add(shoe);
        dealer.add(shoe);
        
        // init dealer values
        int dealer_upcard = dealer[0][0];
        int dealer_value = dealer.get_value();
        bool dealer_blackjack = false;

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

            // handle blackjacks
            if(dealer_value == 21) {
                dealer_blackjack = true;
                break;
            }
            if(player_value == 21) {
                current_hand->active = false;
                continue;
            }
            
            // handle bust
            if(player_value > 21) {
                current_hand->active = false;
                continue;
            }

            // hit (stand if not available)
            int result = player.hit(shoe, current_index);
            if(result == -1) player.stand(current_index);
        }
        
        // dealer hitting loop
        bool all_busted = player.all_busted();
        while(dealer.get_value() < 17) {
            // exit loop if all player hands are busted
            if(all_busted) break;

            dealer.hit(shoe);

            // exit loop if dealer busts
            if(dealer[0].is_bust()) break;
        }
        
        // player winning/losing loop
        for(int i = 0; i < player.hands.size(); i++) {
            Hand* hand = &player.hands[i];
            int value = hand->value;

            // winner logic
            // push
            if(dealer_blackjack && hand->is_blackjack()) {
                total++;
                draw++;
                dealer_blackjacks++;
                player_blackjacks++;
                continue;
            }
            
            // loss
            if(dealer_blackjack) {
                total++;
                loss++;
                dealer_blackjacks++;
                continue;
            }

            // win
            if(hand->is_blackjack()) {
                total++;
                win++;
                player_blackjacks++;
                continue;
            }

            // loss
            if(hand->is_bust()) {
                total++;
                loss++;
                continue;
            }

            // win
            if(dealer[0].is_bust()) {
                total++;
                win++;
                continue;
            }

            // push
            if(dealer_value == value) {
                total++;
                draw++;
                continue;
            }
            
            // win
            if(dealer_value < value) {
                total++;
                win++;
                continue;
            }

            // loss
            if(dealer_value > value) {
                total++;
                loss++;
                continue;
            }

        // should never get here
        total++;
        }
    }
    
    std::cout << "TOTAL = " << total << "\n";
    std::cout << "WINS = " << win << " -> " << win*1.0/total * 100 << "%\n";
    std::cout << "LOSS = " << loss << " -> " << loss*1.0/total * 100 << "%\n";
    std::cout << "DRAWS = " << draw << " -> " << draw*1.0/total * 100 << "%\n";
    std::cout << "DEALER BJs = " << dealer_blackjacks << " -> " << dealer_blackjacks*1.0/total * 100 << "%\n";
    std::cout << "PLAYER BJs = " << player_blackjacks << " -> " << player_blackjacks*1.0/total * 100 << "%\n";

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << std::chrono::duration<double, std::milli>(diff).count() *1.0 / 1000     << " seconds" << std::endl;
}
