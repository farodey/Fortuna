from ocr.scraper import Scraper


def test():
    scraper = Scraper()
    scraper.screen_display()
    print(type(scraper.open_cv_image))


if __name__ == '__main__':
    test()
