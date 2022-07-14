from equity.calculator import HoldemCalculator


def test():
    calc = HoldemCalculator()
    calc.calculate_mc('KcKs|XxXx', '', '', 1000000)
    print(calc.wins)


if __name__ == '__main__':
    test()
