from equity.calculator import HoldemCalculator


def test():
    calc = HoldemCalculator()
    calc.calculate_mc('AA|XxXx', '', '', 100000)


if __name__ == '__main__':
    test()
