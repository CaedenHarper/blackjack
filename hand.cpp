#include <iostream>
#include <vector>
#include <hand.h>

std::ostream& operator<<(std::ostream &out, Hand &hand) {
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