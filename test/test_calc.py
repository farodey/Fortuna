from equity.HoldemCalculator import HoldemCalculator
from equity.HoldemHandDistribution import HoldemHandDistribution


def test():
    calc = HoldemCalculator()
    # result = calc.CalculateMC("AhKh|Td9s|QQ+,AQs+,AQo+|JJ-88|XxXx|XxXx|XxXx", "Ks7d4d", "", 100000)

    hand_list = calc.create_list_hand('AhKh|Td9s|QQ+,AQs+,AQo+|JJ-88|XxXx|XxXx|XxXx')

    hand_dist = HoldemHandDistribution(hand_list[2])

    print(hand_dist)

    print(hand_list)
    print(calc.is_specific_hand(hand_list[5]))


if __name__ == '__main__':
    test()


