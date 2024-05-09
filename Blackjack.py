from random import shuffle

def card_to_str(card: int) -> str:
    if(card == 11 or card == 1): return "A"
    return str(card)

# Collection of Decks (Collection of Collection of Cards)
class Hand:
    def __init__(self) -> None:
        self.hands: list[Deck] = [Deck()]
    
    def __getitem__(self, index) -> "Deck":
        return self.hands[index]

    def __len__(self) -> int:
        return len(self.hands)

    # Hits without disabling can_double, can_split
    # used for creating a hand/populating hands after split
    def add(self, shoe: "Shoe", index: int = None) -> int:
        if(index is None): index = 0
        card = shoe.deal()

        hand = self.hands[index]
        hand.append(card)

        # update can_split
        if(len(hand) == 2 and hand[0] == hand[1]): hand.can_split = True

        return 0

    # Returns resulting total
    def hit(self, shoe: "Shoe", index: int = None) -> int:
        if(index is None): index = 0

        hand = self.hands[index]
        if(hand.can_hit == False): return -1
        card = shoe.deal()

        hand.append(card)

        # hand.can_hit = True
        hand.can_double = False
        hand.can_split = False
        hand.can_surrender = False
        hand.can_blackjack = False
        # hand.active = True

        return 0
    
    # Returns resulting total
    def stand(self, index: int = None) -> int:
        if(index is None): index = 0

        hand = self.hands[index]

        hand.can_hit = False
        hand.can_double = False
        hand.can_split = False
        hand.can_surrender = False
        # hand.can_blackjack = True
        hand.active = False

        return 0
    
    # Returns resulting total
    def double(self, shoe: "Shoe", index: int = None) -> int:
        if(index is None): index = 0

        hand = self.hands[index]
        if(hand.can_double == False): return -1
        card = shoe.deal()

        hand.append(card)

        hand.can_hit = False
        hand.can_double = False
        hand.can_split = False
        hand.can_surrender = False
        hand.can_blackjack = False
        hand.active = False

        return 0
    
    # Splits into two Deck objects
    def split(self, index: int = None) -> None:
        if(index is None): index = 0

        hand = self.hands[index]
        if(hand.can_split == False): return -1

        # create new hands
        hand = self.hands[index]
        hand1 = Deck(hand[0])
        hand2 = Deck(hand[1])

        # split hands can not surrender
        # hand1.can_hit = True
        # hand1.can_double = False
        # hand1.can_split = False
        hand1.can_surrender = False
        hand1.can_blackjack = False
        # hand1.active = True

        # hand2.can_hit = True
        # hand2.can_double = False
        # hand2.can_split = False
        hand2.can_surrender = False
        hand2.can_blackjack = False
        # hand2.active = True

        # remove old hand
        self.hands.pop(index)

        # insert new hands
        self.hands.insert(index, hand2)
        self.hands.insert(index, hand1)

        return 0

    # Returns resulting total
    def surrender(self, index: int = None) -> None:
        if(index is None): index = 0

        hand = self.hands[index]
        if(hand.can_surrender == False): return -1

        hand.can_hit = False
        hand.can_double = False
        hand.can_split = False
        hand.can_surrender = False
        hand.can_blackjack = False
        hand.active = False

        return 0
    
    def get_value(self, index: int = None) -> int:
        if(index is None): index = 0
 
        return self.hands[index].get_value()
    
    def is_active(self) -> tuple[bool, list[int]]:
        out1 = False
        out2 = []

        for index, hand in enumerate(self.hands):
            if(hand.active):
                out1 = True
                out2.append(index)

        return (out1, out2)
    
    def all_busted(self) -> bool:
        for hand in self.hands:
            if(not hand.is_bust()): return False

        return True
    
    def __str__(self) -> str:
        if(len(self) == 0): return "{}"

        out = f"{{{self.hands[0]}"

        for hand in self.hands[1:]:
            out += ", "
            out += str(hand)

        return out + "}"

# Collection of Cards (Collection of ints)
class Deck:
    def __init__(self, *cards: int) -> None:
        self.original_order_cards: list[int] = []
        self.cards: list[int] = []

        self.can_hit: bool = True
        self.can_double: bool = True
        self.can_split: bool = False
        self.can_surrender: bool = False
        self.can_blackjack: bool = True
        self.active: bool = True

        self.original_order_cards: list[int] = []
        self.cards.extend(cards)
        self.sort()

    def __getitem__(self, index) -> int:
        return self.cards[index]

    def __len__(self) -> int:
        return len(self.cards)

    def sort(self) -> None:
        self.cards = sorted(self.cards, reverse = True)
    
    def clear(self) -> None:
        self.cards.clear()
        self.original_order_cards.clear()

    def append(self, card: int) -> None:
        self.cards.append(card)
        self.sort()
        self.original_order_cards.append(card)

    def count_aces(self) -> int:
        out = 0

        for card in self.cards:
            if(card == 11):
                out += 1
            else:
                # we can return because list is sorted by high cards, any non-ace is always after all other aces
                return out
        return out

    # removes all but one ace
    def remove_aces(self, n: int) -> None:
        for i in range(0, n-1):
            self.cards[i] = 1

        self.sort()

    def get_value(self) -> int:
        num_aces = self.count_aces()
        out = 0

        # Only one ace can be an 11 in any given blackjack hand
        if(num_aces > 1):
            self.remove_aces(num_aces)
            num_aces = 1

        # first pass
        for card in self.cards:
            out += card

        # if first pass > 21 and number of aces > 0, we can get a lower (non bust) value by removing the only remaining ace
        # using guard clauses, if first pass <= 21 or number of aces == 0, we can return that value as that is the best it is going to get
        if(out <= 21 or num_aces == 0):
            return out
        
        # since we've gotten here, we know there is one ace
        # if we turn that aces value to 1 instead of 11, that is the same as just subtracting 10 from the total

        return (out - 10)
    
    def is_bust(self) -> bool:
        # bust if value > 21
        return self.get_value() > 21
    
    def is_blackjack(self) -> bool:
        # blackjack if value is 21, made up of only 2 cards (ace and 10), and it is the first hand made (splitting does not count)
        return ((len(self) == 2) and (self.get_value() == 21) and (self.can_blackjack))
    
    def __str__(self) -> str:
        if(len(self) == 0): return "[]"

        out = f"[{card_to_str(self.original_order_cards[0])}"

        for card in self.original_order_cards[1:]:
            out += ", "
            out += card_to_str(card)

        return out + "]"

# Collection of cards, specifically decks of cards
class Shoe:
    def __init__(self, num_decks: int = None, shuffled: bool = True) -> None:
        if(num_decks is None): num_decks = 1
        if(num_decks < 0): raise ValueError

        self.cards: list[int] = []
        self.num_decks: int = num_decks

        # add new deck order deck "num_decks" times
        for _ in range(0, num_decks):
            # A->K of Hearts (11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10)
            self.cards.extend([11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10])

            # A->K of Clubs
            self.cards.extend([11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10])

            # K->A of Diamonds
            self.cards.extend([10, 10, 10, 10, 9, 8, 7, 6, 5, 4, 3, 2, 11])

            # K->A of Spades
            self.cards.extend([10, 10, 10, 10, 9, 8, 7, 6, 5, 4, 3, 2, 11])

        if(shuffled): shuffle(self.cards)

    def __getitem__(self, index) -> int:
        return self.cards[index]

    def __len__(self) -> int:
        return len(self.cards)
    
    def deal(self) -> int:
        return self.cards.pop()

    def pop(self, index: int = None) -> int:
        if(index is None): return self.cards.pop()
        return self.cards.pop(index)
    
    def clear(self) -> None:
        self.cards.clear()
    
    def __str__(self) -> str:
        return str(self.cards)