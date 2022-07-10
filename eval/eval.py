from eval.deck import spades, clubs, diamonds, hearts
from eval.table import nBitsTable, straightTable, topFiveCardsTable, topCardTable
from eval.handval import *


def eval_7_hand(cards, n_cards=7):
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
            if straightTable[sd]:
                return hand_type_value(HAND_TYPE_STFLUSH) + top_card_value(straightTable[sd])
            else:
                retval = hand_type_value(HAND_TYPE_FLUSH) + topFiveCardsTable[sd]
        elif nBitsTable[sh] >= 5:
            if straightTable[sh]:
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
            retval = hand_type_value(HAND_TYPE_TWOPAIR) + (topFiveCardsTable[two_mask]
                                & (TOP_CARD_MASK | SECOND_CARD_MASK) + third_card_value(topCardTable[t]))
            return retval
        else:
            # Тройка. В three_mask устанавливаем бит карты из которой состоит тройка
            three_mask = ((sc & sd) | (sh & ss)) & ((sc & sh) | (sd & ss))
            retval = hand_type_value(HAND_TYPE_TRIPS) + top_card_value(topCardTable[three_mask])
            t = ranks ^ three_mask  # Выключаем ранг тройки в маске
            second = topCardTable[t]
            retval += second_card_value(second)
            t ^= (1 << second)      # Выключаем ранг кикера
            retval += third_card_value(topCardTable[t])
            return retval
    else:
        four_mask = sh & sd & sc & ss
        if four_mask:
            tc = topCardTable[four_mask]
            retval = hand_type_value(HAND_TYPE_QUADS) + top_card_value(tc) + \
                second_card_value(topCardTable[ranks ^ (1 << tc)])
            return retval
        two_mask = ranks ^ (sc ^ sd ^ sh ^ ss)
        if nBitsTable[two_mask] != n_dups:
            # Фулл-Хаус
            three_mask = ((sc & sd) | (sh & ss)) & ((sc & sh) | (sd & ss))
            retval = hand_type_value(HAND_TYPE_FULLHOUSE)
            tc = topCardTable[three_mask]
            retval += top_card_value(tc)
            t = (two_mask | three_mask) ^ (1 << tc)
            retval += second_card_value(topCardTable[t])
            return retval
        if retval:
            return retval
        else:
            # Две пары
            retval = hand_type_value(HAND_TYPE_TWOPAIR)
            top = topCardTable[two_mask]
            retval += top_card_value(top)
            second = topCardTable[two_mask ^ (1 << top)]
            retval += second_card_value(second)
            retval += third_card_value(topCardTable[ranks ^ (1 << top) ^ (1 << second)])
            return retval
