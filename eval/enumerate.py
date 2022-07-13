import random
from eval.deck import card_is_set, card_mask_set, N_CARDS, get_mask, card_mask_any_set, card_mask_or


#
# -------- Симуляция Монте-Карло ---------------------
# deck -
# cards_var -
# dead_cards -
# num_cards - количество карт
# num_iter - число итераций
# action - функция вызываемая на каждой итерации
#
def deck_montecarlo_n_cards_d(cards_var, dead_cards, num_cards, num_iter, action):
    for i in range(num_iter):
        for j in range(num_cards):
            c = random.randrange(0, 52, 1)
            while card_is_set(dead_cards, c):
                c = random.randrange(0, 52, 1)
            card_mask_set(cards_var, c)
            card_mask_set(dead_cards, c)
        action()


def deck_enumerate_2_cards_d(dead_cards, action):
    for i1 in reversed(range(N_CARDS)):
        card1 = get_mask(i1)
        if card_mask_any_set(dead_cards, card1):
            continue
        for i2 in reversed(range(i1)):
            card2 = get_mask(i2)
            if card_mask_any_set(dead_cards, card2):
                continue
            cards_var = card_mask_or(card1, card2)
            action(cards_var)



