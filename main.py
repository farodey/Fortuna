from HoldemCalculator import HoldemCalculator


def test():
    calc = HoldemCalculator()
    result = calc.CalculateMC("AhKh|Td9s|QQ+,AQs+,AQo+|JJ-88|XxXx|XxXx|XxXx", "Ks7d4d", "", 100000)
    print(result)


if __name__ == '__main__':
    test()


