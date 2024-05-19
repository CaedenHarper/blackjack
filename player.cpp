#include <iostream>
#include <vector>
#include <player.h>
#include <hand.h>
#include <shoe.h>

Player :: Player() {
    std::vector<Hand> hands = {Hand()};
}

Hand Player :: operator [](int i) {
    return hands[i];
}

std::ostream& operator<<(std::ostream &out, Player &hand) {
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

int Player :: size() {
    return hands.size();
}

int Player :: add(Shoe& shoe) {
    return add(shoe, 0);
}

// Add card to hand without modifying attributes
int Player :: add(Shoe& shoe, int index) {
    int card = shoe.pop_back();

    Hand hand = hands[index];
    if(hand.active == false) return -1;

    hand.push_back(card);

    // hand.can_hit = true;
    // hand.can_double = true;
    if( (hand.size() == 2) && (hand[0] == hand[1]) ) hand.can_split = true;
    // hand.can_surrender = true;
    // hand.can_blackjack = true;
    // hand.active = true;

    return 0;
}

int Player :: hit(Shoe& shoe) {
    return hit(shoe, 0);
}

int Player :: hit(Shoe& shoe, int index) {
    int card = shoe.pop_back();

    Hand hand = hands[index];
    if(hand.can_hit == false) return -1;

    hand.push_back(card);

    // hand.can_hit = true;
    hand.can_double = false;
    hand.can_split = false;
    hand.can_surrender = false;
    hand.can_blackjack = false;
    // hand.active = true;

    return 0;
}

int Player :: stand() {
    return stand(0);
}

int Player :: stand(int index) {
    Hand hand = hands[index];
    if(hand.active == false) return -1;

    hand.can_hit = false;
    hand.can_double = false;
    hand.can_split = false;
    hand.can_surrender = false;
    // hand.can_blackjack = true;
    hand.active = false;

    return 0;
}

int Player :: double_down(Shoe& shoe) {
    return double_down(shoe, 0);
}

int Player :: double_down(Shoe& shoe, int index) {
    int card = shoe.pop_back();

    Hand hand = hands[index];
    if(hand.can_double == false) return -1;

    hand.push_back(card);

    hand.can_hit = false;
    hand.can_double = false;
    hand.can_split = false;
    hand.can_surrender = false;
    hand.can_blackjack = false;
    hand.active = false;

    return 0;
}

int Player :: split() {
    return split(0);
}

int Player :: split(int index) {
    Hand hand = hands[index];
    if(hand.can_split == false) return -1;

    // create new hands
    Hand hand1 = Hand(hand[0]);
    Hand hand2 = Hand(hand[1]);

    // hand1.can_hit = true;
    // hand1.can_double = true;
    // hand1.can_split = true;
    hand1.can_surrender = false;
    hand1.can_blackjack = false;
    // hand1.active = true;

    // hand2.can_hit = true;
    // hand2.can_double = true;
    // hand2.can_split = true;
    hand2.can_surrender = false;
    hand2.can_blackjack = false;
    // hand2.active = true;

    return 0;
}

int Player :: surrender() {
    return surrender(0);
}

int Player :: surrender(int index) {
    Hand hand = hands[index];
    if(hand.can_surrender == false) return -1;

    hand.can_hit = false;
    hand.can_double = false;
    hand.can_split = false;
    hand.can_surrender = false;
    hand.can_blackjack = false;
    hand.active = false;

    return 0;
}

int Player :: get_value() {
    return get_value(0);
}

int Player :: get_value(int index) {
    return hands[index].get_value();
}

// Returns true if any hand is active
bool Player :: is_active() {
    for(int i = 0; i < hands.size(); i++) {
        Hand hand = hand[i];
        if(hand.active) return true;
    }

    return false;
}

// Returns true if all hands have busted
bool Player :: all_busted() {
    for(int i = 0; i < hands.size(); i++) {
        Hand hand = hand[i];
        if(hand.get_value() <= 21) return false;
    }

    return true;
} 