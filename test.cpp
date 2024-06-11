#include <shoe.hpp>
#include <player.hpp>
#include <strategy.hpp>
#include <settings.hpp>
#include <map>
#include <iostream>

// verifies shoe is not empty
bool is_shoe_empty(Shoe& shoe) {
    if(shoe.size() <= 0) return true;
    return false;
}

void wait_for_input() {
    std::cout << "Press enter to continue...\n" << '\r';
    std::getchar();
}

void print_hand(Hand const player_hand, int const dealer_upcard) {
    if(player_hand.get_soft()) {
        std::cout << "Soft " << player_hand.get_value() << " vs. " << dealer_upcard << "\n";
    } else {
        std::cout << player_hand.get_value() << " vs. " << dealer_upcard << "\n";
    }
}

void test() {
    Shoe shoe = Shoe(1, true);

    // init frequency map
    std::map<int, int> card_frequency;
    for(int i = 2; i < 12; i++) {
        card_frequency[i] = 0;
    }

    std::cout<<"TEST SHOE PRINTOUT"<<"\n";
    for(int i = shoe.size()-1; i >= 0; i--) {
        int value = shoe[i];
        std::cout<<value<<"\n";
        card_frequency[value]++;
        wait_for_input();
    }
    std::cout<<"SHOE SIZE: "<<shoe.size()<<"\n";

    std::cout<<"TEST MAP: "<<"\n";
    for(auto& [key, value]: card_frequency) {
        std::cout<<"card_frequency["<< key <<"]: "<<value<<"\n";
    }

    wait_for_input();

    int index = 1;
    float player_profit = 0;
    int num_hits = 0;
    int num_stands = 0;
    int num_doubles = 0;
    int num_splits = 0;
    int num_surrenders = 0;
    int num_player_bjs = 0;
    int num_dealer_bjs = 0;
    while(shoe.size() > 0) {
        // Player variables
        Player player = Player();
        Player dealer = Player();

        // Populate initial hands
        if(is_shoe_empty(shoe)) {
            std::cout << "SHOE EMPTY; DISREGARD HAND\n";
            break;
        }
        player.add(shoe);
        if(is_shoe_empty(shoe)) {
            std::cout << "SHOE EMPTY; DISREGARD HAND\n";
            break;
        }
        dealer.add(shoe);
        if(is_shoe_empty(shoe)) {
            std::cout << "SHOE EMPTY; DISREGARD HAND\n";
            break;
        }
        player.add(shoe);
        if(is_shoe_empty(shoe)) {
            std::cout << "SHOE EMPTY; DISREGARD HAND\n";
            break;
        }
        dealer.add(shoe);
        
        // Useful dealer constants
        const int dealer_upcard = dealer[0][0];
        const int dealer_init_value = dealer.get_value();
        Hand* p_dealer_hand = &dealer.hands[0];

        // Has the dealer gotten a blackjack
        bool dealer_blackjack = false;

        std::cout << "HAND " << index << ":" << "\n";
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
                if(is_shoe_empty(shoe)) {
                    std::cout << "SHOE EMPTY; DISREGARD HAND\n";
                    break;
                }
                player.add(shoe, current_index);
                print_hand(*p_current_hand, dealer_upcard);

                p_current_hand->set_split_aces_final_card(false);
                p_current_hand->set_active(false);

                continue;
            }

            bool break_all_flag = false;
            // populate hand if necessary
            while(p_current_hand->size() < 2) {
                if(is_shoe_empty(shoe)) {
                    std::cout << "SHOE EMPTY; DISREGARD HAND\n";
                    break_all_flag = true;
                    break;
                }
                player.add(shoe, current_index);
            }
            if(break_all_flag) {
                break_all_flag = false;
                break;
            }

            int player_value = player.get_value(current_index);
            
            print_hand(*p_current_hand, dealer_upcard);

            // handle blackjacks
            if(dealer_init_value == 21) {
                dealer_blackjack = true;
                std::cout << "DEALER BJ" << "\n";
                break;
            }
            if(player_value == 21) {
                p_current_hand->set_active(false);
                std::cout << "PLAYER BJ" << "\n";
                continue;
            }
            
            // handle bust
            if(player_value > 21) {
                p_current_hand->set_active(false);
                continue;
            }

            action player_action = basic_strategy_general(*p_current_hand, dealer_upcard);
            // TODO: consider wrapping in a function
            switch (player_action) {
            case action::hit:
                std::cout << "HIT\n";
                num_hits++;
                if(is_shoe_empty(shoe)) {
                    std::cout << "SHOE EMPTY; DISREGARD HAND\n";
                    break;
                }
                player.hit(shoe, current_index);
                break;
            case action::stand:
                std::cout << "STAND\n";
                num_stands++;
                player.stand(current_index);
                break;
            case action::double_down:
                std::cout << "DOUBLE\n";
                num_doubles++;
                if(is_shoe_empty(shoe)) {
                    std::cout << "SHOE EMPTY; DISREGARD HAND\n";
                    break;
                }
                player.double_down(shoe, current_index);
                print_hand(*p_current_hand, dealer_upcard);
                break;
            case action::split:
                std::cout << "SPLIT\n";
                num_splits++;
                player.split(current_index);
                
                // reset pointer.. shouldn't need to be here, but here in case
                p_current_hand = &player.hands[current_index];

                break;
            case action::surrender:
                std::cout << "\n";
                num_surrenders++;
                player.surrender(current_index);
                break;
            default:
                std::cout << "ERROR\n";
                break;
            }
        }
        // dealer hitting loop
        bool all_busted = player.all_busted();
        bool all_blackjacks = player.all_blackjacks();
        while(true) {
            std::cout << dealer.get_value() << "\n";
            // exit loop if all players have busted or all players have blackjacks
            if(all_busted || all_blackjacks) break;

            action dealer_action = dealer_strategy(*p_dealer_hand);

            // exit loop if dealer stands
            if(dealer_action == action::stand) {
                std::cout << "DEALER STAND\n";
                break;
            }

            if(is_shoe_empty(shoe)) {
                std::cout << "SHOE EMPTY; DISREGARD HAND\n";
                break;
            }
            dealer.hit(shoe);

            // exit loop if dealer busts
            if(p_dealer_hand->is_bust()) {
                std::cout << dealer.get_value() << "\n";
                std::cout << "DEALER BUST\n";
                break;
            }
        }
        int dealer_final_value = dealer.get_value();
        // player winning/losing loop
        for(int i = 0; i < player.hands.size(); i++) {
            index++;
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
                num_dealer_bjs++;
                num_player_bjs++;
                std::cout << "Push\n\n";
                wait_for_input();
                continue;
            }
            
            // loss
            if(dealer_blackjack) {
                num_dealer_bjs++;
                
                player_profit -= money_magnitude;

                std::cout << "Dealer Wins\n\n";
                wait_for_input();
                continue;
            }

            // win
            if(player_blackjack) {
                num_player_bjs++;
        
                player_profit += money_magnitude*BLACKJACK_PAYOUT;
    
                std::cout << "Player Wins\n\n";
                wait_for_input();
                continue;
            }

            // loss
            if(player_bust) {
                player_profit -= money_magnitude;

                std::cout << "Dealer Wins\n\n";
                wait_for_input();
                continue;
            }

            // win
            if(dealer[0].is_bust()) {
                player_profit += money_magnitude;

                std::cout << "Player Wins\n\n";
                wait_for_input();
                continue;
            }

            // push
            if(dealer_final_value == value) {
                std::cout << "Push\n\n";
                wait_for_input();
                continue;
            }
            
            // win
            if(dealer_final_value < value) {
                player_profit += money_magnitude;

                std::cout << "Player Wins\n\n";
                wait_for_input();
                continue;
            }

            // loss
            if(dealer_final_value > value) {
                player_profit -= money_magnitude;

                std::cout << "Dealer Wins\n\n";
                wait_for_input();
                continue;
            }
        // should never get here
        }
    }
    std::cout << "Total count for each action:\n";
    std::cout << "Hit: " << num_hits << "\n";
    std::cout << "Stand: " << num_stands << "\n";
    std::cout << "Double: " << num_doubles << "\n";
    std::cout << "Split: " << num_splits << "\n";
    std::cout << "Surrender: " << num_surrenders << "\n";
    std::cout << "Player BJ: " << num_player_bjs << "\n";
    std::cout << "Dealer BJ: " << num_dealer_bjs << "\n";
    std::cout << "\n";
    std::cout << "Total amount of money won: " << player_profit << "\n";
}

int main() {
    test();
}
