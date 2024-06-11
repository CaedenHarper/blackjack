// SELF
#include <player.hpp>

// OTHER MODULES
#include <hand.hpp>
#include <shoe.hpp>
#include <settings.hpp>

// STANDARD LIBRARY
#include <iostream>
#include <vector>

Player::Player() {
    this->hands = {Hand()};
}

Hand Player::at(int i) const {
    return this->hands.at(i);
}

std::vector<Hand>::iterator Player::begin() {
    return this->hands.begin();
}

std::vector<Hand>::iterator Player::end() {
    return this->hands.end();
}

std::ostream& operator<<(std::ostream &out, const Player &player) {
    if(player.size() == 0) {
        out << "{}";
        return out;
    }

    out << "{" << player.at(0);
    for(int i = 1; i < player.size(); i++) {
        out << ", " << player.at(i);
    }
    out << "}";

    return out;
}

int Player::size() const {
    return this->hands.size();
}

int Player::add(Shoe& shoe) {
    return this->add(shoe, 0);
}

// Add card to hand without modifying attributes
int Player::add(Shoe& shoe, int index) {
    Hand* p_hand = &hands.at(index);
    if(p_hand->get_active() == false) return -1;

    int card = shoe.pop_back();

    p_hand->push_back(card);

    // p_hand->set_can_double(true);

    if(p_hand->get_pair()) p_hand->set_can_split(true);
    // disallow split depending on split aces rule
    if(p_hand->at(0) == 11 && !SPLIT_ACES) p_hand->set_can_split(false);
    // disallow split depending on max number of splits
    if(p_hand->get_split_num() >= MAX_SPLIT_NUM) p_hand->set_can_split(false);
    // disallow split depending on max number of aces splits
    if(p_hand->at(0) == 11 && p_hand->get_split_num() >= MAX_SPLIT_ACES_NUM) p_hand->set_can_split(false);

    // p_hand->set_can_surrender(true);
    // p_hand->set_can_blackjack(true);
    // p_hand->set_active(true);

    return 0;
}

int Player::hit(Shoe& shoe) {
    return this->hit(shoe, 0);
}

int Player::hit(Shoe& shoe, int index) {
    Hand* p_hand = &hands.at(index);
    if(p_hand->get_active() == false) return -1;

    int card = shoe.pop_back();

    p_hand->push_back(card);

    p_hand->set_can_double(false);
    p_hand->set_can_split(false);
    p_hand->set_can_surrender(false);
    p_hand->set_can_blackjack(false);
    // p_hand->set_active(true);

    return 0;
}

int Player::stand() {
    return this->stand(0);
}

int Player::stand(int index) {
    Hand* p_hand = &hands.at(index);
    if(p_hand->get_active() == false) return -1;

    p_hand->set_can_double(false);
    p_hand->set_can_split(false);
    p_hand->set_can_surrender(false);
    // p_hand->set_can_blackjack(true);
    p_hand->set_active(false);

    return 0;
}

int Player::double_down(Shoe& shoe) {
    return this->double_down(shoe, 0);
}

int Player::double_down(Shoe& shoe, int index) {
    int card = shoe.pop_back();

    Hand* p_hand = &hands.at(index);
    if(p_hand->get_can_double() == false) return -1;

    p_hand->push_back(card);

    p_hand->set_can_double(false);
    p_hand->set_can_split(false);
    p_hand->set_can_surrender(false);
    p_hand->set_can_blackjack(false);
    p_hand->set_active(false);

    p_hand->set_was_doubled(true);

    return 0;
}

int Player::split() {
    return this->split(0);
}

int Player::split(int index) {
    Hand* p_hand = &hands.at(index);
    if(p_hand->get_can_split() == false) return -1;

    // create new hands and remove old hand
    int pair_card = p_hand->at(0);
    this->hands.erase(this->hands.begin() + index);

    Hand hand1 = Hand(pair_card);
    Hand hand2 = Hand(pair_card);

    hands.insert(hands.begin() + index, hand1);
    hands.insert(hands.begin() + index + 1, hand2);

    Hand* p_hand1 = &hands.at(index);
    Hand* p_hand2 = &hands.at(index + 1);

    // if split ace with no drawing, set special flag to only draw one more card
    if(pair_card == 11 && !DRAW_TO_SPLIT_ACES) {
        p_hand1->increment_split_num();
        p_hand1->set_can_double(false);
        p_hand1->set_can_split(false);
        p_hand1->set_can_surrender(false);
        p_hand1->set_can_blackjack(false);
        p_hand1->set_active(true);
        p_hand1->set_split_aces_final_card(true);

        p_hand2->increment_split_num();
        p_hand2->set_can_double(false);
        p_hand2->set_can_split(false);
        p_hand2->set_can_surrender(false);
        p_hand2->set_can_blackjack(false);
        p_hand2->set_active(true);
        p_hand2->set_split_aces_final_card(true);

        return 0;
    }

    p_hand1->increment_split_num();
    p_hand1->set_can_double(DOUBLE_AFTER_SPLIT);       // depends on double after split rules
    p_hand1->set_can_split(false);
    p_hand1->set_can_surrender(false);
    p_hand1->set_can_blackjack(false);
    p_hand1->set_active(true);

    p_hand2->increment_split_num();
    p_hand2->set_can_double(DOUBLE_AFTER_SPLIT);       // depends on double after split rules
    p_hand2->set_can_split(false);
    p_hand2->set_can_surrender(false);
    p_hand2->set_can_blackjack(false);
    p_hand2->set_active(true);

    return 0;
}

int Player::surrender() {
    return this->surrender(0);
}

int Player::surrender(int index) {
    Hand* p_hand = &hands.at(index);
    if(p_hand->get_can_surrender() == false) return -1;

    p_hand->set_can_split(false);
    p_hand->set_can_double(false);
    p_hand->set_can_split(false);
    p_hand->set_can_surrender(false);
    p_hand->set_can_blackjack(false);
    p_hand->set_active(false);

    p_hand->set_was_surrendered(true);

    return 0;
}

int Player::get_value() {
    return this->get_value(0);
}

int Player::get_value(int index) {
    return this->hands.at(index).get_value();
}

// Returns list of all active hands indices
// TODO: consider transforming into get_first_active_index() for speed increase,
// as only the first active index is ever actually needed in main.cpp
// std::vector<int> Player::get_active_indices() {
//     std::vector<int> out;
//     for(int i = 0; i < this->hands.size(); i++) {
//         Hand hand = this->hands.at(i);
//         if(hand.get_active()) out.push_back(i);
//     }
//     return out;
// }

// Returns first active index or -1 if there are no active indeces
int Player::get_first_active_index() {
    for(int i = 0; i < this->hands.size(); i++) {
        Hand hand = this->hands.at(i);
        if(hand.get_active()) {
            return i;
        }
    }
    return -1;
}

// Returns true if all hands have busted
bool Player::all_busted() {
    for(int i = 0; i < this->hands.size(); i++) {
        Hand hand = this->hands.at(i);
        if(!hand.is_bust()) return false;
    }

    return true;
}

// Returns true if all hands are blackjacks
bool Player::all_blackjacks() {
    for(int i = 0; i < this->hands.size(); i++) {
        Hand hand = this->hands.at(i);
        if(!hand.is_blackjack()) return false;
    }

    return true;
}