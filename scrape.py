import win32gui
import win32ui
from ctypes import windll
from PIL import Image


def screen_window(name):
    hwnd = win32gui.FindWindow(None, name)

    # Измените строку ниже в зависимости от того, хотите ли вы использовать все окно целиком
    # или просто в клиентской области.
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

    # Измените строку ниже в зависимости от того, хотите ли вы использовать все окно целиком
    # или просто в клиентской области.
    # result = windll.user32.PrintWindow(hwnd, saveDC.GetSafeHdc(), 1)
    # Если не работает, то можно использовать следующий код
    # windll.user32.PrintWindow(hwnd, saveDC.GetSafeHdc(), 3)
    result = windll.user32.PrintWindow(hwnd, saveDC.GetSafeHdc(), 0)
    print(result)

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
        # PrintWindow Succeeded
        im.save("test.png")


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


def main():
    list_name_window = get_list_name_window()
    screen_window(list_name_window[0])


if __name__ == '__main__':
    main()
