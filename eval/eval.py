from eval.deck import spades, clubs, diamonds, hearts
from eval.table import nBitsTable, straightTable, topFiveCardsTable, topCardTable
from eval.handval import *


def evaluate(cards, n_cards):

    ss = spades(cards)
    sc = clubs(cards)
    sd = diamonds(cards)
    sh = hearts(cards)

    retval = 0
    ranks = ss | sc | sd | sh
    n_ranks = nBitsTable[ranks]
    n_dups = n_cards - n_ranks

    if n_ranks >= 5:
        if nBitsTable[ss] >= 5:
            if straightTable[ss]:
                return hand_type_value(HAND_TYPE_STFLUSH) + top_card_value(straightTable[ss])
            else:
                retval = hand_type_value(HAND_TYPE_FLUSH) + topFiveCardsTable[ss]
        elif nBitsTable[sc] >= 5:
            if straightTable[sc]:
                return hand_type_value(HAND_TYPE_STFLUSH) + top_card_value(straightTable[sc])
            else:
                retval = hand_type_value(HAND_TYPE_FLUSH) + topFiveCardsTable[sc]
        elif nBitsTable[sd] >= 5:
            if straightTable[sc]:
                return hand_type_value(HAND_TYPE_STFLUSH) + top_card_value(straightTable[sd])
            else:
                retval = hand_type_value(HAND_TYPE_FLUSH) + topFiveCardsTable[sd]
        elif nBitsTable[sh] >= 5:
            if straightTable[sc]:
                return hand_type_value(HAND_TYPE_STFLUSH) + top_card_value(straightTable[sh])
            else:
                retval = hand_type_value(HAND_TYPE_FLUSH) + topFiveCardsTable[sh]
        else:
            st = straightTable[ranks]
            if st > 0:
                retval = hand_type_value(HAND_TYPE_STRAIGHT) + top_card_value(st)

        if retval and n_dups < 3:
            return retval

        if n_dups == 0:
            # Старшая карта
            return hand_type_value(HAND_TYPE_NOPAIR) + topFiveCardsTable[ranks]
        elif n_dups == 1:
            # Пара
            two_mask = ranks ^ (sc ^ sd ^ sh ^ ss)
            retval = hand_type_value(HAND_TYPE_ONEPAIR) + top_card_value(topCardTable[two_mask])
            t = ranks ^ two_mask
            kickers = (topFiveCardsTable[t] >> CARD_WIDTH) & 0x0000000F
            retval += kickers
            return retval
        elif n_dups == 2:
            # Две пары или тройка
            two_mask = ranks ^ (sc ^ sd ^ sh ^ ss)
            if two_mask:
                t = ranks ^ two_mask
                retval = hand_type_value(HAND_TYPE_TWOPAIR) + topFiveCardsTable[two_mask] &





