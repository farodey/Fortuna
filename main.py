import sys
import threading
from time import sleep

from PyQt6 import QtWidgets
from PyQt6.QtCore import pyqtSignal, QObject
from PyQt6.QtWidgets import QLabel

from equity.calculator import HoldemCalculator
from gui import main_window, table_list_window, select_ocr_window
from ocr.ps import PokerStars
from ocr.scraper import get_list_name_window

window = 0


class SignalSelectWindow(QObject):
    # Настроить сигнал
    select_window = pyqtSignal(str)

    def __init__(self):
        super().__init__()

    def run(self, window_name):
        self.select_window.emit(window_name)


class SignalSelectOCR(QObject):
    select_ocr = pyqtSignal(str)

    def __init__(self):
        super().__init__()

    def run(self, ocr_name):
        self.select_ocr.emit(ocr_name)


class MainWindow(QtWidgets.QMainWindow, main_window.Ui_MainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setupUi(self)

        # Статусы
        self.status_ocr = QLabel('OCR: не выбран')
        self.status_window = QLabel('Окно: не выбрано')
        self.status_capture = QLabel('Захват: остановлен')
        self.statusBar().addPermanentWidget(self.status_ocr)
        self.statusBar().addPermanentWidget(self.status_window)
        self.statusBar().addPermanentWidget(self.status_capture)

        # Кнопки
        self.pushButton.setEnabled(False)
        self.pushButton_3.setEnabled(False)
        self.pushButton_2.clicked.connect(self.click_select_ocr)
        self.pushButton_3.clicked.connect(self.click_select_window)

        # Пользовательские сигналы
        self.sig_ocr = SignalSelectOCR()
        self.sig_ocr.select_ocr.connect(self.select_ocr)
        self.sig_win = SignalSelectWindow()
        self.sig_win.select_window.connect(self.select_window)

        # Переменные
        self.ocr_name = None
        self.window_name = None

    def click_select_ocr(self):
        self.socrw = SelectOCRWindow()
        self.socrw.show()

    def select_ocr(self, ocr_name):
        if ocr_name:
            self.ocr_name = ocr_name
            self.pushButton_3.setEnabled(True)

    def click_select_window(self):
        self.tlw = TableListWindow()
        self.tlw.show()

    def select_window(self, window_name):
        if window_name:
            self.window_name = window_name
            thread = threading.Thread(target=thread_calculate, args=(self.ocr_name, self.window_name))
            thread.start()


class TableListWindow(QtWidgets.QMainWindow, table_list_window.Ui_MainWindow):
    def __init__(self):
        super(TableListWindow, self).__init__()
        self.setupUi(self)
        self.list_win = get_list_name_window()
        self.listWidget.addItems(self.list_win)
        self.pushButton_2.clicked.connect(self.click_capture)

    def click_capture(self):
        # Получаем название окна
        window_name = self.list_win[self.listWidget.currentRow()]

        # Закрываем текущее окно
        self.close()

        # Посылаем сигнал в главное окно
        window.sig_win.run(window_name)


class SelectOCRWindow(QtWidgets.QMainWindow, select_ocr_window.Ui_MainWindow):
    def __init__(self):
        super(SelectOCRWindow, self).__init__()
        self.setupUi(self)

        self.list_ocr = ['PokerStars']
        self.listWidget.addItems(self.list_ocr)
        self.pushButton_2.clicked.connect(self.click_select_ocr)

    def click_select_ocr(self):
        ocr_name = self.list_ocr[self.listWidget.currentRow()]
        window.sig_ocr.run(ocr_name)
        self.close()


def thread_calculate(ocr_name, window_name):
    scraper = PokerStars()
    calc = HoldemCalculator()

    unic_play = (0, 0, 0)

    while True:

        # Задержка для снижения нагрузки на систему
        sleep(1)

        # Скриншот окна
        scraper.screen_window(window_name)

        # Если горит кнопка Фолд
        if scraper.fold():

            pot = scraper.pot()
            round = scraper.round()
            player = scraper.player()

            if (pot != 0 and round != 0 and player != 0) and unic_play != (pot, round, player):

                # Считываем карты
                hands, board = get_card()

                # Добавляем оппонентов
                for i in range(player):
                    hands += '|XxXx'

                calc.calculate_mc(hands, board, '', 100000)


def get_card():
    return '9s8s', 'KsQhJs'


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    app.exec()
