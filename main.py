import sys
import threading
from time import sleep

from PyQt6 import QtWidgets
from PyQt6.QtCore import pyqtSignal, QObject
from PyQt6.QtWidgets import QFileDialog

from gui import main_window, table_list_window
from ocr.scraper import get_list_name_window, screen_window

window = 0


class SignalStartCalculate(QObject):

    # Настроить сигнал
    start_calculate = pyqtSignal(str)

    def __init__(self):
        super().__init__()

    def run(self, window_name):
        self.start_calculate.emit(window_name)


class MainWindow(QtWidgets.QMainWindow, main_window.Ui_MainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setupUi(self)
        self.statusBar().showMessage('Карта стола: не выбрана / Окно: не выбрано / Захват: остановлен')
        self.pushButton_2.clicked.connect(self.select_table_map)
        self.pushButton_3.clicked.connect(self.select_window)
        self.sig = SignalStartCalculate()
        self.sig.start_calculate.connect(self.thread_calculate)

    def select_table_map(self):
        fname = QFileDialog.getOpenFileName(self, 'Open file', '/home')[0]

    def select_window(self):
        self.tlw = TableListWindow()
        self.tlw.show()

    def thread_calculate(self, window_name):
        thread = threading.Thread(target=thread_calculate, args=(window_name, ))
        thread.start()


def thread_calculate(window_name):

    while True:
        screen_window(window_name)
        sleep(1)


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
        window.sig.run(window_name)


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    app.exec()
