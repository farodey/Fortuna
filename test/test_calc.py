from equity.calculator import HoldemCalculator


def test():
    calc = HoldemCalculator()
    calc.calculate_mc('98|9s8s', 'KsQhJs', '', 100000)


if __name__ == '__main__':
    test()
