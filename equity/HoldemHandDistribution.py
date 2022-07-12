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
        is_plus = '+' in hand_text
        is_slice = '-' in hand_text

        if is_slice:
            pass





    def is_pair(self, hand):
        return hand[0] == hand[1]

    def is_suited(self, hand):
        return len(hand) >= 3 and hand[2] == 's'

    def is_off_suited(self, hand):
        return len(hand) >= 3 and hand[3] == 'o'

    def is_specific_hand(self, hand):
        return (hand[1] in 'shdc') and (hand[3] in 'shdc') \
               and (hand[0] in '23456789TJQKA') and (hand[2] in '23456789TJQKA')

