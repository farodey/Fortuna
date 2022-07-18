import sys

from PIL.ImageQt import ImageQt, QPixmap, QImage
from PyQt6 import uic
from PyQt6.QtCore import QSize, QObject, pyqtSignal
from PyQt6.QtWidgets import QApplication, QMainWindow, QPushButton, QVBoxLayout, QWidget, QListWidget, QHBoxLayout, \
    QLabel

from scrape import get_list_name_window, screen_window

import design

window = 0


class SignalPainScreen(QObject):

    # Настроить сигнал
    paint_screen = pyqtSignal()

    def __init__(self):
        super().__init__()

    def run(self):
        self.paint_screen.emit()


# Виджет списка
class MyListWidget(QWidget):
    def __init__(self):
        super().__init__()

        # Получаем список окон
        self.list_win = get_list_name_window()

        # Создаем виджет со списком окон и подключаем обработчик клика
        self.q_list_widget = QListWidget()
        self.q_list_widget.addItems(self.list_win)
        self.q_list_widget.itemClicked.connect(self.select_window)

        vbox = QVBoxLayout()
        vbox.addWidget(self.q_list_widget)
        self.setLayout(vbox)

    def select_window(self, item):
        print(f"Вы кликнули: {item.text()}")


# Окно со списком окон для захвата
class SelectWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle('Выберите окно')
        self.buttonOK = QPushButton('OK')
        self.buttonCancel = QPushButton('Cancel')
        layoutH = QHBoxLayout()
        layoutH.addWidget(self.buttonOK)
        layoutH.addWidget(self.buttonCancel)

        self.mlw = MyListWidget()
        layoutV = QVBoxLayout()
        layoutV.addWidget(self.mlw)
        layoutV.addLayout(layoutH)

        widget = QWidget()
        widget.setLayout(layoutV)
        self.setCentralWidget(widget)

        self.buttonOK.clicked.connect(self.select_window)

    # Обработчик нажатия на кнопку OK
    def select_window(self):

        # Получаем название окна
        window_name = self.mlw.list_win[self.mlw.q_list_widget.currentRow()]

        # Делаем скрин окна
        image = screen_window(window_name)

        # Преобразуем полученное изображение в формат QT
        window.q_image = ImageQt(image)

        # Закрываем это окно
        window.sw.close()

        # Посылаем сигнал в главное окно для рисования скрина
        window.sig.run()


# Главное окно
class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.q_image = 0

        # Название и размер окна
        self.setWindowTitle("Fortuna")
        self.setFixedSize(QSize(800, 700))

        self.button = QPushButton('Захватить окно')
        self.layout = QVBoxLayout()
        self.layout.addWidget(self.button)

        widget = QWidget()
        widget.setLayout(self.layout)

        # Устанавливаем центральный виджет окна. Виджет будет расширяться по умолчанию,
        # заполняя всё пространство окна.
        self.setCentralWidget(widget)

        self.button.clicked.connect(self.show_new_window)

        self.sig = SignalPainScreen()
        self.sig.paint_screen.connect(self.paint_screen)

    def show_new_window(self, checked):
        self.sw = SelectWindow()
        self.sw.show()

    def paint_screen(self):

        # Получем изображение
        self.pixmap = QPixmap(QImage(self.q_image))

        # Подгоняем размер окна под размер изображения
        size = self.pixmap.size()
        size.setHeight(size.height() + 50)
        size.setWidth(size.width() + 50)

        label = QLabel()
        label.setPixmap(self.pixmap)

        # Удаляем кнопку
        self.layout.removeWidget(self.button)
        self.button.deleteLater()
        self.button = None

        # Меняем размер окна
        window.setFixedSize(size)

        # Устанавливаем изображение
        self.layout.addWidget(label)

        # Создаем окно редактирования карты стола
        self.etmw = EditTableMapWindow()
        self.etmw.show()


# Окно редактирования карты стола
class EditTableMapWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = design.Ui_MainWindow()
        self.ui.setupUi(self)




if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    app.exec()
