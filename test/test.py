from eval.deck import string_to_card, text_to_mask
from eval.eval import eval_7_hand


def test_string_to_card():
    print(string_to_card('2h'))
    print(string_to_card('3h'))
    print(string_to_card('4h'))
    print(string_to_card('5h'))
    print(string_to_card('6h'))
    print(string_to_card('7h'))
    print(string_to_card('8h'))
    print(string_to_card('9h'))
    print(string_to_card('Th'))
    print(string_to_card('Jh'))
    print(string_to_card('Qh'))
    print(string_to_card('Kh'))
    print(string_to_card('Ah'))
    print(string_to_card('2d'))
    print(string_to_card('3d'))
    print(string_to_card('4d'))
    print(string_to_card('5d'))
    print(string_to_card('6d'))
    print(string_to_card('7d'))
    print(string_to_card('8d'))
    print(string_to_card('9d'))
    print(string_to_card('Td'))
    print(string_to_card('Jd'))
    print(string_to_card('Qd'))
    print(string_to_card('Kd'))
    print(string_to_card('Ad'))


def test_text_to_mask():
    print(text_to_mask('AcKcQcJcTc2s5h'))


def test_eval_7_hand():
    print(eval_7_hand(text_to_mask('AcKcQcJcTc2s5h')))


if __name__ == '__main__':
    # test_string_to_card()
    #test_text_to_mask()
    test_eval_7_hand()
