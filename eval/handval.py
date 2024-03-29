HAND_TYPE_NOPAIR = 0
HAND_TYPE_ONEPAIR = 1
HAND_TYPE_TWOPAIR = 2
HAND_TYPE_TRIPS = 3
HAND_TYPE_STRAIGHT = 4
HAND_TYPE_FLUSH = 5
HAND_TYPE_FULLHOUSE = 6
HAND_TYPE_QUADS = 7
HAND_TYPE_STFLUSH = 8

HAND_TYPE_SHIFT = 24
HAND_TYPE_MASK = 0x0F000000
CARDS_SHIFT = 0
CARDS_MASK = 0x000FFFFF
TOP_CARD_SHIFT = 16
TOP_CARD_MASK = 0x000F0000
SECOND_CARD_SHIFT = 12
SECOND_CARD_MASK = 0x0000F000
THIRD_CARD_SHIFT = 8
THIRD_CARD_MASK = 0x00000F00
FOURTH_CARD_SHIFT = 4
FOURTH_CARD_MASK = 0x000000F0
FIFTH_CARD_SHIFT = 0
FIFTH_CARD_MASK = 0x0000000F
CARD_WIDTH = 4
CARD_MASK = 0x0F


def hand_type_value(handtype):
    return handtype << HAND_TYPE_SHIFT


def top_card_value(top_card):
    return top_card << TOP_CARD_SHIFT


def third_card_value(handtype):
    return handtype << THIRD_CARD_SHIFT


def second_card_value(handtype):
    return handtype << SECOND_CARD_SHIFT
