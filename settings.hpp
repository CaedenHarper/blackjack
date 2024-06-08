#ifndef SETTINGS_H
#define SETTINGS_H

// BLACKJACK RULE VARIATIONS

// Amount to payout when player gets a blackjack
// (e.g., if 1.5, 3:2 payout)
constexpr inline float BLACKJACK_PAYOUT = 1.5;
// Number of decks in play
// (e.g., if 2, Double Deck)
constexpr inline int MAX_DECKS = 8;
// If player can late surrender
// note: early surrender takes precedence over late surrender
// (e.g., if early and late are both true, player can early surrender)
constexpr inline bool LATE_SURRENDER = false;
// If player can early surrender
// note: early surrender takes precedence over late surrender
// (e.g., if early and late are both true, player can early surrender)
// TODO: consider implementing
// constexpr inline bool EARLY_SURRENDER = false;

// If dealer hits soft seventeen
// (e.g., if false, dealer stands on A-6)
constexpr inline bool HIT_SOFT_SEVENTEEN = true;
// If player can double after split
constexpr inline bool DOUBLE_AFTER_SPLIT = true;
// If player can split aces
constexpr inline bool SPLIT_ACES = true;
// If player can act on split aces
// (e.g., if false, player will be given one card and can not act further)
constexpr inline bool DRAW_TO_SPLIT_ACES = false;
// Total number of hands aces can be split to
// (e.g., if 2, player can split aces to 2 hands total)
constexpr inline int MAX_SPLIT_ACES_NUM = 2;
// Total number of hands any non-ace can be split to
// (e.g., if 4, player can split tens to 4 hands total)
constexpr inline int MAX_SPLIT_NUM = 4;


// SIMULATION SETTINGS

// print debug information throughout the simulation
constexpr inline bool DEBUG = false;
// print stats at the end of the simulation
constexpr inline bool PRINT_STATS = true;
// Total number of hands to play in simulation
// (e.g., 1000000 = 1,000,000 = 1 million, 1 million hands are played)
constexpr inline int MAX_HANDS = 1000000;
// Percentage of deck to stop dealing at
constexpr inline float PENETRATON_PERCENT = 0.25;


// MONEY SETTINGS

// Amount of dollars per smallest bet
constexpr inline float BETTING_UNIT = 100;
// Number of hands played per hour
constexpr inline int HANDS_PER_HOUR = 100;

#endif