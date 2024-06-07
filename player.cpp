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

    if(hand->get_pair()) hand->set_can_split(true);
    // disallow split depending on split aces rule
    if((*hand)[0] == 11 && !SPLIT_ACES) hand->set_can_split(false);
    // disallow split depending on max number of splits
    if(hand->get_split_num() >= MAX_SPLIT_NUM) hand->set_can_split(false);
    // disallow split depending on max number of aces splits
    if((*hand)[0] == 11 && hand->get_split_num() >= MAX_SPLIT_ACES_NUM) hand->set_can_split(false);

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

    // std::cout<<"here IN 1"<<'\n';
    int card = shoe.pop_back();
    // std::cout<<"here IN 2"<<'\n';

    hand->push_back(card);
    // std::cout<<"here IN 3"<<'\n';

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
    
    // std::cout<<"POST ERASE VECTOR; SIZE = "<<hands.size()<<"\n";
    // for(int i = 0; i < hands.size(); i++) {
        // std::cout<<"INDEX: "<<i<<"; hands[" <<i<<"]: "<<hands[i]<<"@ pointer: "<<&hands[i]<<"\n";
    // }

    Hand _hand1 = Hand(pair_card);
    Hand _hand2 = Hand(pair_card);

    hands.insert(hands.begin() + index, _hand1);
    // std::cout<<"POST INSERT HAND 1 VECTOR; SIZE = "<<hands.size()<<"\n";
    // for(int i = 0; i < hands.size(); i++) {
        // std::cout<<"INDEX: "<<i<<"; hands[" <<i<<"]: "<<hands[i]<<"@ pointer: "<<&hands[i]<<"\n";
    // }
    hands.insert(hands.begin() + index + 1, _hand2);
    // std::cout<<"POST INSERT HAND 2 VECTOR; SIZE = "<<hands.size()<<"\n";
    // for(int i = 0; i < hands.size(); i++) {
        // std::cout<<"INDEX: "<<i<<"; hands[" <<i<<"]: "<<hands[i]<<"@ pointer: "<<&hands[i]<<"\n";
    // }

    Hand* hand1 = &hands[index];
    // std::cout<<"HAND 1 POINTER: "<<hand1<<"\n";
    Hand* hand2 = &hands[index + 1];
    // std::cout<<"HAND 2 POINTER: "<<hand2<<"\n";

    // if split ace with no drawing, set special flag to only draw one more card
    if(pair_card == 11 && !DRAW_TO_SPLIT_ACES) {
        hand1->increment_split_num();
        hand1->set_can_double(false);
        hand1->set_can_split(false);
        hand1->set_can_surrender(false);
        hand1->set_can_blackjack(false);
        hand1->set_active(true);
        hand1->set_split_aces_final_card(true);

        hand2->increment_split_num();
        hand2->set_can_double(false);
        hand2->set_can_split(false);
        hand2->set_can_surrender(false);
        hand2->set_can_blackjack(false);
        hand2->set_active(true);
        hand2->set_split_aces_final_card(true);

        return 0;
    }

    hand1->increment_split_num();
    hand1->set_can_double(DOUBLE_AFTER_SPLIT);       // depends on double after split rules
    hand1->set_can_split(false);
    hand1->set_can_surrender(false);
    hand1->set_can_blackjack(false);
    hand1->set_active(true);

    hand2->increment_split_num();
    hand2->set_can_double(DOUBLE_AFTER_SPLIT);       // depends on double after split rules
    hand2->set_can_split(false);
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

    hand->set_was_surrendered(true);

    return 0;
}

int Player::get_value() {
    return get_value(0);
}

int Player::get_value(int index) {
    return hands[index].get_value();
}

// Returns list of all active hands indices
// TODO: consider transforming into get_first_active_index() for speed increase,
// as only the first active index is ever actually needed in main.cpp
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