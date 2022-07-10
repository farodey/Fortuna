from eval.deck import string_to_card, text_to_mask, get_mask
from eval.eval import eval_7_hand
from eval.handval import *
import time


def test_string_to_card():
    print(string_to_card('2h'))


def test_text_to_mask():
    print(text_to_mask('AcKcQcJcTc2s5h'))


def test_eval_7_hand():
    print(eval_7_hand(text_to_mask('2h3h4h5h2d3d7d')))


def enum_test_eval_7_hand():
    sf = 0
    q = 0
    fh = 0
    f = 0
    st = 0
    t = 0
    two = 0
    pair = 0
    nopair = 0

    start_time = time.time()
    for c1 in range(52):
        for c2 in range(c1 + 1, 52):
            for c3 in range(c2 + 1, 52):
                for c4 in range(c3 + 1, 52):
                    for c5 in range(c4 + 1, 52):
                        for c6 in range(c5 + 1, 52):
                            for c7 in range(c6 + 1, 52):
                                ret = eval_7_hand(get_mask(c1) | get_mask(c2) | get_mask(c3) | get_mask(c4) | get_mask(c5) | get_mask(c6) | get_mask(c7))
                                r = ret >> HAND_TYPE_SHIFT
                                if r == HAND_TYPE_STFLUSH:
                                    sf += 1
                                elif r == HAND_TYPE_QUADS:
                                    q += 1
                                elif r == HAND_TYPE_FULLHOUSE:
                                    fh += 1
                                elif r == HAND_TYPE_FLUSH:
                                    f += 1
                                elif r == HAND_TYPE_STRAIGHT:
                                    st += 1
                                elif r == HAND_TYPE_TRIPS:
                                    t += 1
                                elif r == HAND_TYPE_TWOPAIR:
                                    two += 1
                                elif r == HAND_TYPE_ONEPAIR:
                                    pair += 1
                                elif r == HAND_TYPE_NOPAIR:
                                    nopair += 1
                                else:
                                    print(c1, c2, c3, c4, c5, c6, c7)

    print("--- %s seconds ---" % (time.time() - start_time))

    print(nopair)
    print(pair)
    print(two)
    print(t)
    print(st)
    print(f)
    print(fh)
    print(q)
    print(sf)


if __name__ == '__main__':
    enum_test_eval_7_hand()
