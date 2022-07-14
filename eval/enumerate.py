import random
from eval.deck import card_is_set, card_mask_set, N_CARDS, get_mask, card_mask_any_set, card_mask_or, mask_to_text


#
# -------- Симуляция Монте-Карло ---------------------
# deck -
# cards_var -
# dead_cards -
# num_cards - количество карт
# num_iter - число итераций
# action - функция вызываемая на каждой итерации
#
def deck_montecarlo_n_cards_d(dead_cards, num_cards, num_iter, action):
    for i in range(num_iter):
        cards_var = 0
        used = dead_cards
        for j in range(num_cards):
            c = random.randrange(0, 52, 1)
            while card_is_set(used, c):
                c = random.randrange(0, 52, 1)
            cards_var = card_mask_set(cards_var, c)
            used = card_mask_set(used, c)
        # print('deck_montecarlo_n_cards_d ' + 'cards_var=' + mask_to_text(cards_var) + ' dead_cards=' +  mask_to_text(dead_cards))
        action(cards_var)


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



