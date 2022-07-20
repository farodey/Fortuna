import numpy as np
import win32gui
import win32ui
from ctypes import windll
from PIL import Image, ImageGrab
import cv2


class Scraper:
    def screen_window(self, name):
        hwnd = win32gui.FindWindow(None, name)

        # left, top, right, bot = win32gui.GetClientRect(hwnd)
        left, top, right, bot = win32gui.GetWindowRect(hwnd)
        w = right - left
        h = bot - top

        hwndDC = win32gui.GetWindowDC(hwnd)
        mfcDC = win32ui.CreateDCFromHandle(hwndDC)
        saveDC = mfcDC.CreateCompatibleDC()

        saveBitMap = win32ui.CreateBitmap()
        saveBitMap.CreateCompatibleBitmap(mfcDC, w, h)

        saveDC.SelectObject(saveBitMap)

        result = windll.user32.PrintWindow(hwnd, saveDC.GetSafeHdc(), 3)

        bmpinfo = saveBitMap.GetInfo()
        bmpstr = saveBitMap.GetBitmapBits(True)

        im = Image.frombuffer(
            'RGB',
            (bmpinfo['bmWidth'], bmpinfo['bmHeight']),
            bmpstr, 'raw', 'BGRX', 0, 1)

        win32gui.DeleteObject(saveBitMap.GetHandle())
        saveDC.DeleteDC()
        mfcDC.DeleteDC()
        win32gui.ReleaseDC(hwnd, hwndDC)

        if result == 1:
            print('Скриншот окна успешно сделан')
            im.save("C:\\Users\\farodey\\Desktop\\screen.png")
        else:
            print('Ошибка скриншота окна')

        return im

    def screen_display(self):
        pil_image = ImageGrab.grab()
        open_cv_image = np.array(pil_image)
        self.open_cv_image = cv2.cvtColor(open_cv_image, cv2.COLOR_BGR2RGB)
        # self.open_cv_image = open_cv_image[:, :, ::-1].copy()

    # Поиск начала первого символа, отсекает лишнюю область слева  ->|....
    def x_roi(self, image):
        height, width = image.shape
        for x in range(width):
            for y in range(height):
                if image[y, x] == 0xff:
                    return True, image[:, x:]
        return False, image

    # Поиск ширины символа, отсечение области справа  ....|<-
    def w_roi(self, image):
        height, width = image.shape
        for x in range(12):
            bel = False
            for y in range(height):
                if image[y, x] == 0xff:
                    bel = True
                    break
            if not bel:
                return True, image[:, :x]
        return False, image  # Ошибка

    # Поиск начала символа сверху, отсечение области сверху
    def y_roi(self, image):
        height, width = image.shape
        for y in range(height):
            for x in range(width):
                if image[y, x] == 0xff:
                    return True, image[y:, :]
        return False, image

    # Поиск высоты символа, отсечение области снизу
    def h_roi(self, image):
        height, width = image.shape
        for y in range(height):
            bel = False
            for x in range(width):
                if image[y, x] == 0xff:
                    bel = True
                    break
            if not bel:
                return True, image[:y, :]
        return False, image

    # перцептивный хэш
    def hash_image(self, yx_card, y, x):

        # вырезаем картинку
        roi_image = self.open_cv_image[yx_card[0]:yx_card[0] + y, yx_card[1]:yx_card[1] + x]

        # уменьшаем разрешение  до 8х8
        resize_image = cv2.resize(roi_image, (8, 8))

        # переводим в градации серого (убираем цвет)
        gray_image = cv2.cvtColor(resize_image, cv2.COLOR_BGR2GRAY)

        # Находим среднее значение всех цветов
        result = 0
        for y in range(8):
            for x in range(8):
                result += gray_image[y, x]
        average_color = result // 64

        # бинаризуем картинку (average_color - пороговая величина)
        th, bin_image = cv2.threshold(gray_image, average_color, 255, cv2.THRESH_BINARY)

        # переводим картинку в одно 64 битное значение
        i = 0
        hash = 0
        for y in range(8):
            for x in range(8):
                # 0xff - белый пиксель, 0x00 - черный пиксель
                if bin_image[y, x] == 0xff:
                    # устанавливаем бит
                    hash = hash | 1 << i
                i += 1
        return hash

    def bin_image_to_int(self, image):
        i = 0
        hash = 0
        height, width = image.shape
        for y in range(height):
            for x in range(width):
                # 0xff - белый пиксель, 0x00 - черный пиксель
                if image[y, x] == 0xff:
                    # устанавливаем бит
                    hash = hash | 1 << i
                i += 1
        return hash


def get_list_name_window():
    list_name_window = []

    # Функция обратного вызова
    def call_back(hwnd, hwnds):
        if win32gui.IsWindowVisible(hwnd):
            window_name = win32gui.GetWindowText(hwnd)
            if window_name != '':
                list_name_window.append(win32gui.GetWindowText(hwnd))

    b = win32gui.EnumWindows(call_back, 0)
    return list_name_window
