#include <iostream>
#include <vector>
#include <player.hpp>
#include <hand.hpp>
#include <shoe.hpp>

Player::Player() {
    hands = {Hand()};
}

Hand Player::operator [](int i) const {
    if(hands.size() < 1) {
        std::cout << "EMPTY PLAYER ERROR" << "\n";
    }
    return hands[i];
}

std::ostream& operator<<(std::ostream &out, const Player &player) {
    if(player.size() == 0) {
        out << "{}";
        return out;
    }

    out << "{" << player[0];
    for(int i = 1; i < player.size(); i++) {
        out << ", " << player[i];
    }
    out << "}";

    return out;
}

int Player::size() const {
    return hands.size();
}

int Player::add(Shoe& shoe) {
    return add(shoe, 0);
}

// Add card to hand without modifying attributes
int Player::add(Shoe& shoe, int index) {
    Hand* hand = &hands[index];
    if(hand->get_active() == false) return -1;

    int card = shoe.pop_back();

    hand->push_back(card);

    // hand->set_can_double(true);
    if( (hand->size() == 2) && ((*hand)[0] == (*hand)[1]) ) hand->set_can_split(true);
    // hand->set_can_surrender(true);
    // hand->set_can_blackjack(true);
    // hand->set_active(true);

    return 0;
}

int Player::hit(Shoe& shoe) {
    return hit(shoe, 0);
}

int Player::hit(Shoe& shoe, int index) {
    Hand* hand = &hands[index];
    if(hand->get_active() == false) return -1;

    int card = shoe.pop_back();

    hand->push_back(card);

    hand->set_can_double(false);
    hand->set_can_split(false);
    hand->set_can_surrender(false);
    hand->set_can_blackjack(false);
    // hand->set_active(true);

    return 0;
}

int Player::stand() {
    return stand(0);
}

int Player::stand(int index) {
    Hand* hand = &hands[index];
    if(hand->get_active() == false) return -1;

    hand->set_can_double(false);
    hand->set_can_split(false);
    hand->set_can_surrender(false);
    // hand->set_can_blackjack(true);
    hand->set_active(false);

    return 0;
}

int Player::double_down(Shoe& shoe) {
    return double_down(shoe, 0);
}

int Player::double_down(Shoe& shoe, int index) {
    int card = shoe.pop_back();

    Hand* hand = &hands[index];
    if(hand->get_can_double() == false) return -1;

    hand->push_back(card);

    hand->set_can_double(false);
    hand->set_can_split(false);
    hand->set_can_surrender(false);
    hand->set_can_blackjack(false);
    hand->set_active(false);

    hand->set_was_doubled(true);

    return 0;
}

int Player::split() {
    return split(0);
}

int Player::split(int index) {
    Hand* hand = &hands[index];
    if(hand->get_can_split() == false) return -1;

    // create new hands and remove old hand
    int pair_card = (*hand)[0];
    hands.erase(hands.begin() + index);

    Hand _hand1 = Hand(pair_card);
    hands.insert(hands.begin() + index, _hand1);
    Hand* hand1 = &hands[index];
    
    Hand _hand2 = Hand(pair_card);
    hands.insert(hands.begin() + index, _hand2);
    Hand* hand2 = &hands[index];

    hand1->set_can_double(true);       // depends on double after split rules
    hand1->set_can_split(true);
    hand1->set_can_surrender(false);
    hand1->set_can_blackjack(false);
    hand1->set_active(true);

    hand2->set_can_double(true);       // depends on double after split rules
    hand2->set_can_split(true);
    hand2->set_can_surrender(false);
    hand2->set_can_blackjack(false);
    hand2->set_active(true);

    return 0;
}

int Player::surrender() {
    return surrender(0);
}

int Player::surrender(int index) {
    Hand* hand = &hands[index];
    if(hand->get_can_surrender() == false) return -1;

    hand->set_can_split(false);
    hand->set_can_double(false);
    hand->set_can_split(false);
    hand->set_can_surrender(false);
    hand->set_can_blackjack(false);
    hand->set_active(false);

    return 0;
}

int Player::get_value() {
    return get_value(0);
}

int Player::get_value(int index) {
    return hands[index].get_value();
}

// Returns list of all active hands indices
std::vector<int> Player::get_active_indices() {
    std::vector<int> out;
    for(int i = 0; i < hands.size(); i++) {
        Hand hand = hands[i];
        if(hand.get_active()) out.push_back(i);
    }
    return out;
}

// Returns true if all hands have busted
bool Player::all_busted() {
    for(int i = 0; i < hands.size(); i++) {
        Hand hand = hands[i];
        if(!hand.is_bust()) return false;
    }

    return true;
}

// Returns true if all hands are blackjacks
bool Player::all_blackjacks() {
    for(int i = 0; i < hands.size(); i++) {
        Hand hand = hands[i];
        if(!hand.is_blackjack()) return false;
    }

    return true;
}