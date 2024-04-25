"""Playing cards (Poker deck) encoded
as one-byte integers
"""

import enum
from typing import Tuple, List


class CardSuit(enum.Enum):
    value: int      # Hint to typechecker
    Hearts = 0
    Diamonds = 1
    Spades = 2
    Clubs = 3


class CardRank(enum.Enum):
    value: int      # Hint to typechecker
    Ace = 1
    Two = 2
    Three = 3
    Four = 4
    Five = 5
    Six = 6
    Seven = 7
    Eight = 8
    Nine = 9
    Ten = 10
    Jack = 11
    Queen = 12
    King = 13

# Note we need 6 bits total for a card
#   Bits 0..3 for CardRank (4 bits)
#   Bits 4..5 for CardSuit (2 bits)


def encode(rank: CardRank, suit: CardSuit) -> int:
    return (0b11111 & suit.value) << 4 | rank.value


def decode(card: int) -> Tuple[CardRank, CardSuit]:
    rank = 0b1111 & card
    suit = 0b11 & (card >> 4)
    return CardRank(rank), CardSuit(suit)


def desc(card: int):
    rank, suit = decode(card)
    return f"{rank.name} of {suit.name}"


def pack_hand(cards: List[Tuple[CardRank, CardSuit]]) -> int:
    """Pack a list of 5 (rank, suit) pairs into
    30 bits of an integer.
    """
    assert len(cards) == 5, "Poker hand is 5 cards"
    word = 0
    for rank, suit in cards:
        word <<= 6
        word |= encode(rank, suit)
    return word


def unpack_hand(packed: int) -> List[Tuple[CardRank, CardSuit]]:
    """Inverse of pack_hand: Unpack a single integer
    into a list of 5 (CardRank, CardSuit) pairs.
    """
    hand = []
    mask = 0b111111
    for x in range(5):
        encoded_card = (packed >> (6 * x)) & mask
        card = decode(encoded_card)
        hand.append(card)
    hand.reverse()
    return hand


def main():
    """Smoke test"""
    card = encode(CardRank.Jack, CardSuit.Hearts)
    print(f"Jack of Hearts => {card} => {desc(card)}")
    card = encode(CardRank.Ace, CardSuit.Spades)
    print(f"Ace of Spades => {card} => {desc(card)}")
    card = encode(CardRank.Four, CardSuit.Diamonds)
    print(f"Four of Diamonds => {card} => {desc(card)}")
    card = encode(CardRank.King, CardSuit.Clubs)
    print(f"King of Clubs => {card} => {desc(card)}")

    # Expected output:
    # Jack of Hearts => 11 => Jack of Hearts
    # Ace of Spades => 33 => Ace of Spades
    # Four of Diamonds => 20 => Four of Diamonds
    # King of Clubs => 61 => King of Clubs

    hand = [(CardRank.King, CardSuit.Diamonds),
            (CardRank.Queen, CardSuit.Hearts),
            (CardRank.Jack, CardSuit.Clubs),
            (CardRank.Ten, CardSuit.Spades),
            (CardRank.Ace, CardSuit.Diamonds)]
    packed = pack_hand(hand)
    # Result should be an integer, in 30 bits.
    # It should therefore be less than 2^30.
    assert packed < (1 << 30), "Should fit in 30 bits"
    unpacked = unpack_hand(packed)
    assert unpacked == hand, "Should be the same"
    print("Hand:")
    for rank, suit in unpacked:
        print(f"{rank.name} of {suit.name}")


if __name__ == "__main__":
    main()


