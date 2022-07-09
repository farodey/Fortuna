RANK_2 = 0
RANK_3 = 1
RANK_4 = 2
RANK_5 = 3
RANK_6 = 4
RANK_7 = 5
RANK_8 = 6
RANK_9 = 7
RANK_TEN = 8
RANK_JACK = 9
RANK_QUEEN = 10
RANK_KING = 11
RANK_ACE = 12
RANK_COUNT = 13
RANK_FIRST = RANK_2
RANK_LAST = RANK_ACE

SUIT_HEARTS = 0
SUIT_DIAMONDS = 1
SUIT_CLUBS = 2
SUIT_SPADES = 3
SUIT_COUNT = 4
SUIT_FIRST = SUIT_HEARTS
SUIT_LAST = SUIT_SPADES


cardMasksTable = [
    0x0001000000000000,
    0x0002000000000000,
    0x0004000000000000,
    0x0008000000000000,
    0x0010000000000000,
    0x0020000000000000,
    0x0040000000000000,
    0x0080000000000000,
    0x0100000000000000,
    0x0200000000000000,
    0x0400000000000000,
    0x0800000000000000,
    0x1000000000000000,
    0x0000000100000000,
    0x0000000200000000,
    0x0000000400000000,
    0x0000000800000000,
    0x0000001000000000,
    0x0000002000000000,
    0x0000004000000000,
    0x0000008000000000,
    0x0000010000000000,
    0x0000020000000000,
    0x0000040000000000,
    0x0000080000000000,
    0x0000100000000000,
    0x0000000000010000,
    0x0000000000020000,
    0x0000000000040000,
    0x0000000000080000,
    0x0000000000100000,
    0x0000000000200000,
    0x0000000000400000,
    0x0000000000800000,
    0x0000000001000000,
    0x0000000002000000,
    0x0000000004000000,
    0x0000000008000000,
    0x0000000010000000,
    0x0000000000000001,
    0x0000000000000002,
    0x0000000000000004,
    0x0000000000000008,
    0x0000000000000010,
    0x0000000000000020,
    0x0000000000000040,
    0x0000000000000080,
    0x0000000000000100,
    0x0000000000000200,
    0x0000000000000400,
    0x0000000000000800,
    0x0000000000001000]


def make_card(rank, suit):
    return (suit * RANK_COUNT) + rank


def get_mask(index):
    return cardMasksTable[index]


def card_is_set(mask, index):
    return mask & get_mask(index) != 0


def card_mask_or(mask1, mask2):
    return mask1 | mask2


def card_mask_set(mask, index):
    return card_mask_or(mask, get_mask(index))


def spades(mask):
    return mask & 0b0000000000000000000000000000000000000000000000000001111111111111


def clubs(mask):
    return (mask & 0b0000000000000000000000000000000000011111111111110000000000000000) >> 16


def diamonds(mask):
    return (mask & 0b0000000000000000000111111111111100000000000000000000000000000000) >> 32


def hearts(mask):
    return (mask & 0b0001111111111111000000000000000000000000000000000000000000000000) >> 48


def string_to_card(string):
    rank_list = ['2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A']
    suit_list = ['h', 'd', 'c', 's']

    for rank in range(RANK_FIRST, RANK_LAST + 1):
        if rank_list[rank] == string[0]:
            for suit in range(SUIT_FIRST, SUIT_LAST + 1):
                if suit_list[suit] == string[1]:
                    return make_card(rank, suit)


def text_to_mask(str_hand):
    ret_hand = 0
    if str_hand and len(str_hand):
        for i in range(0, len(str_hand), 2):
            card_index = string_to_card(str_hand[i:i+2:1])
            card_mask = get_mask(card_index)
            ret_hand = card_mask_or(ret_hand, card_mask)
    return ret_hand
