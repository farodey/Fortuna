from equity.HoldemCalculator import HoldemCalculator
from equity.HoldemHandDistribution import HoldemHandDistribution
from eval.deck import index_to_string


def test():
    calc = HoldemCalculator()
    # result = calc.CalculateMC("AhKh|Td9s|QQ+,AQs+,AQo+|JJ-88|XxXx|XxXx|XxXx", "Ks7d4d", "", 100000)

    hand_list = calc.create_list_hand('AhKh|Td9s|QQ+,AQs+,AQo+|JJ-88|XxXx|XxXx|XxXx')
    hand_dist = HoldemHandDistribution('AJs+')

    print(hand_dist.instantiate('65+'))


if __name__ == '__main__':
    test()

