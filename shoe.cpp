#include <shoe.h>
#include <vector>
#include <algorithm>
#include <random>

Shoe :: Shoe(int _num_decks) {
    Deck();
    num_decks = _num_decks;
    std::vector<int> deck = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
    for(int i = 0; i < num_decks; i++) {
        cards.insert(cards.end(), deck.begin(), deck.end());
    }
}

Shoe :: Shoe(int _num_decks, std::default_random_engine rng) {
    Deck();
    int num_decks = _num_decks;
    std::vector<int> deck = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
    for(int i = 0; i < num_decks; i++) {
        cards.insert(cards.end(), deck.begin(), deck.end());
    }

    shuffle(rng);
}

void Shoe :: shuffle(std::default_random_engine rng) {
    std::shuffle(std::begin(cards), std::end(cards), rng);
}