from eval.deck import SUIT_FIRST, SUIT_LAST, char_to_rank, RANK_ACE, RANK_KING, get_mask, make_card, card_mask_or, \
    index_to_string


class HoldemHandDistribution:

    def __init__(self, hand, dead_cards=0):

        self.str_hand = hand
        self.dead_cards = dead_cards
        self.list_hand = []

        list_range = self.str_hand.split(',')
        for range in list_range:
            if self.is_specific_hand(range):
                self.list_hand.append(range)
            else:
                self.instantiate(range)

    def instantiate(self, hand_text):

        list_hand = []
        list_str_hand = []

        is_plus = '+' in hand_text
        is_slice = '-' in hand_text

        rank = []
        ceil = []

        if is_slice:

            index = hand_text.find('-')

            rank.append(char_to_rank(hand_text[index + 1]))
            rank.append(char_to_rank(hand_text[index + 2]))
            ceil.append(char_to_rank(hand_text[0]))
            ceil.append(char_to_rank(hand_text[1]))
        else:
            rank.append(char_to_rank(hand_text[0]))
            rank.append(char_to_rank(hand_text[1]))
            ceil.append(RANK_ACE if is_plus else rank[0])
            ceil.append(RANK_KING)

        # Пары типа AA или QQ+ или JJ-88
        if self.is_pair(hand_text):
            for rank0 in range(rank[0], ceil[0] + 1):
                for suit1 in range(SUIT_FIRST, SUIT_LAST + 1):
                    for suit2 in range(suit1 + 1, SUIT_LAST + 1):
                        card1 = get_mask(make_card(rank0, suit1))
                        card2 = get_mask(make_card(rank0, suit2))
                        list_hand.append(card_mask_or(card1, card2))

                        # Для удобства отладки наполняем отдельный список строковым представлением рук
                        list_str_hand.append(index_to_string(make_card(rank0, suit1)) +
                                             index_to_string(make_card(rank0, suit2)))

        # Одномастные типа A2s или T9s+ или QJs-65s
        elif self.is_suited(hand_text):
            rank0_increment = 1
            if (is_plus or is_slice) and (rank[0] == RANK_ACE):
                rank0_increment = 0
            rank0 = rank[0]
            rank1 = rank[1]
            while rank0 <= ceil[0] and rank1 <= ceil[1]:
                for suit in range(SUIT_FIRST, SUIT_LAST + 1):
                    card1 = get_mask(make_card(rank0, suit))
                    card2 = get_mask(make_card(rank1, suit))
                    list_hand.append(card_mask_or(card1, card2))

                    # Для удобства отладки наполняем отдельный список строковым представлением рук
                    list_str_hand.append(index_to_string(make_card(rank0, suit)) +
                                         index_to_string(make_card(rank1, suit)))

                rank0 += rank0_increment
                rank1 += 1

        # Разномастные типа A2o или T9o+ или QJo-65o
        elif self.is_off_suited(hand_text):
            rank0_increment = 1
            if (is_plus or is_slice) and (rank[0] == RANK_ACE):
                rank0_increment = 0
            rank0 = rank[0]
            rank1 = rank[1]
            while rank0 <= ceil[0] and rank1 <= ceil[1]:
                for suit1 in range(SUIT_FIRST, SUIT_LAST + 1):
                    for suit2 in range(SUIT_FIRST, SUIT_LAST + 1):
                        if suit1 == suit2:
                            continue
                        card1 = get_mask(make_card(rank0, suit1))
                        card2 = get_mask(make_card(rank1, suit2))
                        list_hand.append(card_mask_or(card1, card2))

                        # Для удобства отладки наполняем отдельный список строковым представлением рук
                        list_str_hand.append(index_to_string(make_card(rank0, suit1)) +
                                             index_to_string(make_card(rank1, suit2)))

                rank0 += rank0_increment
                rank1 += 1

        # Всё вместе, разномастные, одномастные
        else:
            rank0_increment = 1
            if (is_plus or is_slice) and (rank[0] == RANK_ACE):
                rank0_increment = 0
            rank0 = rank[0]
            rank1 = rank[1]
            while rank0 <= ceil[0] and rank1 <= ceil[1]:
                for suit1 in range(SUIT_FIRST, SUIT_LAST + 1):
                    for suit2 in range(SUIT_FIRST, SUIT_LAST + 1):
                        card1 = get_mask(make_card(rank0, suit1))
                        card2 = get_mask(make_card(rank1, suit2))
                        list_hand.append(card_mask_or(card1, card2))

                        # Для удобства отладки наполняем отдельный список строковым представлением рук
                        list_str_hand.append(index_to_string(make_card(rank0, suit1)) +
                                             index_to_string(make_card(rank1, suit2)))

                rank0 += rank0_increment
                rank1 += 1

        return list_str_hand

    # Пары типа AA или QQ+ или JJ-88
    def is_pair(self, hand):
        return hand[0] == hand[1]

    # Одномастные типа A2s или T9s+ или QJs-65s
    def is_suited(self, hand):
        return len(hand) >= 3 and hand[2] == 's'

    def is_off_suited(self, hand):
        return len(hand) >= 3 and hand[2] == 'o'

    def is_specific_hand(self, hand):
        return (hand[1] in 'shdc') and (hand[3] in 'shdc') \
               and (hand[0] in '23456789TJQKA') and (hand[2] in '23456789TJQKA')

