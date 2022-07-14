from equity.handrange import HoldemHandRange
from eval.deck import text_to_mask, card_mask_or, num_cards, mask_to_text
from eval.enumerate import deck_montecarlo_n_cards_d
from eval.eval import eval_7_hand


class HoldemCalculator:

    def __init__(self):
        self.list_hands = []            # Список диапазонов в строковом представлении (типа Aks+ и т.д.)
        self.list_range = []            # Список масок диапазонов
        self.dead_mask = 0              # Маска метрвых карт
        self.board_mask = 0             # Маска доски
        self.indicated_trials = 0       # Нобходимое число итераций симуляции
        self.actual_trials = 0          # Число фактически выполненных итерации симуляции
        self.collisions = 0
        self.number_of_board_cards = 0
        self.player_count = 0

        self.wins = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        self.ties = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        self.tied_player_indexes = []

    def calculate_mc(self, hands, board, dead, number_of_trials):
        self.precalculate(hands, board, dead, number_of_trials)
        self.calculate_monte_carlo()

    def precalculate(self, hands, board, dead, number_of_trials):
        self.store(hands, board, dead, number_of_trials)
        self.create_list_range(hands)

    def store(self, hands, board, dead, number_of_trials):
        self.board_mask = text_to_mask(board)
        self.dead_mask = text_to_mask(dead)
        self.dead_mask = card_mask_or(self.dead_mask, self.board_mask)
        self.number_of_board_cards = num_cards(self.board_mask)

        self.indicated_trials = number_of_trials

    def create_list_range(self, hands):
        self.list_hands = hands.split('|')
        self.list_range = []

        static_player_cards = 0

        # Сначала найдем все конкретные руки, добавим их в список и зарезервируем
        # место в списке для диапазонов. Это нужно для соблюдения последоватедьлности рук в списке
        for hand in self.list_hands:
            if HoldemHandRange.is_specific_hand(hand):
                cur_range = HoldemHandRange(hand, self.dead_mask)
                self.list_range.append(cur_range)
                cur_range.choose(self.dead_mask)
                static_player_cards = card_mask_or(static_player_cards, cur_range.current_hand)
            else:
                # Зарезервируем место в списке
                self.list_range.append('reserved')

        self.dead_mask = card_mask_or(self.dead_mask, static_player_cards)

        for i_hand in range(len(self.list_hands)):
            if not HoldemHandRange.is_specific_hand(self.list_hands[i_hand]):
                cur_range = HoldemHandRange(self.list_hands[i_hand], self.dead_mask)
                self.list_range[i_hand] = cur_range

    def calculate_monte_carlo(self):

        # Цикл симуляции
        while self.actual_trials < self.indicated_trials:
            used_cards_this_trial = self.dead_mask
            collision_error = False
            for cur_range in self.list_range:
                collision_error = cur_range.choose(used_cards_this_trial)
                if collision_error:
                    self.collisions += 1
                    break
                if not cur_range.is_unary():
                    used_cards_this_trial = card_mask_or(used_cards_this_trial, cur_range.current())
            if collision_error:
                continue
            deck_montecarlo_n_cards_d(used_cards_this_trial, 5 - self.number_of_board_cards, 1, self.eval_one_trial)

    def eval_one_trial(self, bord_fragment):
        self.actual_trials += 1
        win_index_player = -1
        is_tie = False
        num_ties = 0
        max_handval = 0

        board_mask = card_mask_or(bord_fragment, self.board_mask)
        for player in range(len(self.list_hands)):
            temp_hand = card_mask_or(board_mask, self.list_range[player].current())
            cur_handval = eval_7_hand(temp_hand)

            if cur_handval > max_handval:
                max_handval = cur_handval
                is_tie = False
                win_index_player = player
                num_ties = 0
                self.tied_player_indexes.clear()

                # Отладка
                # if player == 0:
                #     print(mask_to_text(temp_hand))

            elif cur_handval == max_handval:
                if num_ties == 0:
                    self.tied_player_indexes.append(win_index_player)
                    self.tied_player_indexes.append(player)
                else:
                    self.tied_player_indexes[num_ties + 1] = player
                is_tie = True
                num_ties += 1

            temp_hand = 0
        if not is_tie:
            self.wins[win_index_player] += 1
        else:
            for i in self.tied_player_indexes:
                self.ties[i] += 1
