N_CARDS = 52

# LIST_CARDS = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
#               21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
#               41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51]

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
    """Получает маску карты по индексу\n
    Вход: индекс\n
    Выход: маска"""
    return cardMasksTable[index]


def card_is_set(mask, index):
    return mask & get_mask(index) != 0


def card_mask_or(mask1, mask2):
    return mask1 | mask2


def card_mask_set(mask, index):
    return card_mask_or(mask, get_mask(index))


def card_mask_any_set(mask1, mask2):
    """Возвращает True, если любая карта в масках совпадает"""
    return (mask1 & mask2) != 0


def num_cards(mask):
    n_cards = 0
    for i in range(N_CARDS):
        if card_is_set(mask, i):
            n_cards += 1
    return n_cards


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


def char_to_rank(char):
    if char == 'A':
        return RANK_ACE
    elif char == 'K':
        return RANK_KING
    elif char == 'Q':
        return RANK_QUEEN
    elif char == 'J':
        return RANK_JACK
    elif char == 'T':
        return RANK_TEN
    elif char == '9':
        return RANK_9
    elif char == '8':
        return RANK_8
    elif char == '7':
        return RANK_7
    elif char == '6':
        return RANK_6
    elif char == '5':
        return RANK_5
    elif char == '4':
        return RANK_4
    elif char == '3':
        return RANK_3
    elif char == '2':
        return RANK_2
    return -1


def index_to_string(index):
    rank_list = ['2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A']
    suit_list = ['h', 'd', 'c', 's']

    i_suit = index // RANK_COUNT
    i_rank = index % RANK_COUNT
    return rank_list[i_rank] + suit_list[i_suit]


def mask_to_text(mask):
    ret_text = ''
    list_index = mask_to_cards(mask)
    for index in list_index:
        ret_text = ret_text + index_to_string(index)
    return ret_text


def mask_to_cards(mask):
    cards = []
    for i in range(52):
        if card_is_set(mask, i):
            cards.append(i)
    return cards

