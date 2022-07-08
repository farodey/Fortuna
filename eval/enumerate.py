import random
from eval.deck import card_is_set, card_mask_set


#
# -------- Симуляция Монте-Карло ---------------------
# deck -
# cards_var -
# dead_cards -
# num_cards - количество карт
# num_iter - число итераций
# action - функция вызываемая на каждой итерации
#
def DECK_MONTECARLO_N_CARDS_D(cards_var, dead_cards, num_cards, num_iter, action):
    for i in range(num_iter):
        for j in range(num_cards):
            c = random.randrange(0, 52, 1)
            while card_is_set(dead_cards, c):
                c = random.randrange(0, 52, 1)
            card_mask_set(cards_var, c)
            card_mask_set(dead_cards, c)
        action()


