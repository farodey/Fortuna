from equity.handrange import HoldemHandRange
from eval.deck import text_to_mask, card_mask_or


class HoldemCalculator:

    def __init__(self):
        self.list_hands = []
        self.list_range = []
        self.dead_mask = 0

    def create_list_range(self, hands):
        self.list_hands = hands.split('|')
        self.list_range = []
        for hand in self.list_hands:
            if HoldemHandRange.is_specific_hand(hand):
                self.list_range.append(HoldemHandRange(hand))

    def calculate_mc(self, hands, board, dead, number_of_trials):
        self.precalculate(self, hands, board, dead, number_of_trials)
        self.calculate_monte_carlo()

    def precalculate(self, hands, board, dead, number_of_trials):
        self.store(self, hands, board, dead, number_of_trials)
        self.create_list_range(hands)

    def store(self, hands, board, dead, number_of_trials):
        self.dead_mask = text_to_mask(dead)
        self.dead_mask = card_mask_or(self.dead_mask, text_to_mask(board))

    def calculate_monte_carlo(self):
        pass
