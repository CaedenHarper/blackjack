#include <iostream>
#include <vector>
#include <numeric>
#include <hand.h>

Hand :: Hand() : Deck() {
    value = 0;
    num_cards = 0;
    num_aces = 0;
    can_hit = true;
    can_double = true;
    can_split = false;
    can_surrender = false;
    can_blackjack = true;
    active = true;
}

Hand :: Hand(int _card) : Deck(_card) {
    Hand();
    num_cards = 1;
    num_aces = (_card == 11) ? 1 : 0;
}

Hand :: Hand(std::vector<int> _cards) : Deck(_cards) {
    Hand();
    int _cards_size = _cards.size();
    num_cards = _cards_size;
    num_aces = 0;
    for(int i = 0; i < _cards_size; i++) {
        if(_cards[i] == 11) num_aces++;
    }
}

std::ostream& operator<<(std::ostream &out, Hand const &hand) {
    if(hand.size() == 0) {
        out << "{}";
        return out;
    }

    out << "{" << hand[0];
    for(int i = 1; i < hand.size(); i++) {
        out << ", " << hand[i];
    }
    out << "}";

    return out;
}

void Hand :: push_back(const int &val) {
    cards.push_back(val);;
    update(val);
}

void Hand :: update(int card) {
    num_cards++;
    value += card;
    if(card == 11) num_aces++;
}

// int Hand :: count_aces() {
//     int num_aces = 0;
//     for(int i = 0; i < cards.size(); i++) {
//         int card = cards[i];

//         if(card != 11) continue;

//         if(num_aces == 1) {
//             cards.at(i) = 1;
//         } else {
//             num_aces += 1;
//         }
//     }
//     return num_aces;
// }

// int Hand :: get_value() {
//     int out;
//     int num_aces = count_aces();

//     // find initial sum
//     out = std::accumulate(cards.begin(), cards.end(), 0);

//     // if initial sum > 21 and number of aces > 0,
//     // we can get a lower value by removing the only remaining ace
//     // using guard clauses, if first pass <= 21 or number of aces == 0, 
//     // we can return that value as that is the best it is going to get
//     if(out <= 21 || num_aces == 0) return out;
    
//     // since we've gotten here, we know there is one ace
//     // if we turn that aces value to 1 instead of 11, 
//     // that is the same as just subtracting 10 from the total
//     return (out - 10);
// }

bool Hand :: is_bust() {
    return value > 21;
}

bool Hand :: is_blackjack() {
    // blackjack if value is 21, made up of only 2 cards,
    // and it is the first hand made (splitting does not count)
    return ((value == 21) && (num_cards == 2) && (can_blackjack));
}