from equity.calculator import HoldemCalculator


def test():
    calc = HoldemCalculator()
    calc.calculate_mc('99|9s8s', '', '', 10000000)
    print(calc.wins)
    print(calc.ties)


if __name__ == '__main__':
    test()
