#include "stdafx.h"
#include "Fortuna.h"
#include "Scraper.h"
#include "HoldemCalculator.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Органы управления которые обновляются отдельным потоком
HWND hEdit1;
HWND hEdit2;
HWND hEdit3;
HWND hEdit4;

// Кнопки
HWND hButton1;
HWND hButton2;
HWND hButton3;

// Индикаторы для рисования
int card1;
int card2;
int card3;
int card4;
int card5;
int card6;
int card7;
// char diapazon[250] = "QQ+, AKs, A5s-A2s, 98s, 87s, 76s, 65s, AKo";

HBITMAP hBitmap[53];		// Массив хендлов загруженных карт
HBITMAP hBitmapCall[11];	// Массив хендлов картинок цифер на кнопке колла
bool stop;					// Индикатор остановки потока

// Объявления функций, включенных в этот модуль кода
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI		CalculateThread(LPVOID param);
void				ClsWind();
void				PaintDiapazon(HDC hdc, int x, int y, char* diapazon);
BOOL				CheckComb(char* ch_comb, char* diapazon);
int					SetCard(Scraper scraper, int iRound, char* pCard, char* pBoard);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FORTUNA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FORTUNA));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FORTUNA));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, 300, 210, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hCompatibleDC;
	HANDLE hOldBitmap;
	BITMAP Bitmap;

	// Параметры для потока
	int tParametr;
	DWORD tPar;
	HANDLE hThread;

	// Дескрипторы органов управления
	HWND hStatic1;
	HWND hStatic2;
	HWND hStatic3;
	HWND hStatic4;
	HWND hStatic5;

	switch (message)
	{
	case WM_CREATE:

		hStatic1 = CreateWindow("STATIC", "Эквити", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 10, 50, 150, 25, hWnd, (HMENU)ID_STATIC_1, hInst, NULL);
		hStatic2 = CreateWindow("STATIC", "Шансы банка", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 10, 80, 150, 25, hWnd, (HMENU)ID_STATIC_2, hInst, NULL);
		hStatic3 = CreateWindow("STATIC", "Мат. ожидание", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 10, 110, 150, 25, hWnd, (HMENU)ID_STATIC_3, hInst, NULL);
		hStatic4 = CreateWindow("STATIC", "Оппоненты", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 10, 140, 150, 25, hWnd, (HMENU)ID_STATIC_4, hInst, NULL);

		// Создаем текстовые окна
		hEdit1 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 120, 50, 60, 20, hWnd, (HMENU)ID_EDIT_1, hInst, NULL);
		hEdit2 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 120, 80, 60, 20, hWnd, (HMENU)ID_EDIT_2, hInst, NULL);
		hEdit3 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 120, 110, 60, 20, hWnd, (HMENU)ID_EDIT_3, hInst, NULL);
		hEdit4 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 120, 140, 60, 20, hWnd, (HMENU)ID_EDIT_4, hInst, NULL);

		hButton1 = CreateWindow("BUTTON", "Стол 1", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 190, 50, 80, 25, hWnd, (HMENU)ID_BUTTON_1, hInst, NULL);
		hButton2 = CreateWindow("BUTTON", "Стол 2", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 190, 80, 80, 25, hWnd, (HMENU)ID_BUTTON_2, hInst, NULL);
		// hButton3 = CreateWindow("BUTTON", "Настройки", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 190, 110, 80, 25, hWnd, (HMENU)ID_BUTTON_3, hInst, NULL);

		// Отображаем обложку карты
		card1 = 0;
		card2 = 0;
		card3 = 0;
		card4 = 0;
		card5 = 0;
		card6 = 0;
		card7 = 0;

		// Загружаем картинки карт
		hBitmap[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP0));

		// Бубны с туза
		hBitmap[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBitmap[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		hBitmap[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		hBitmap[4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		hBitmap[5] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		hBitmap[6] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		hBitmap[7] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		hBitmap[8] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		hBitmap[9] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP9));
		hBitmap[10] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP10));
		hBitmap[11] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP11));
		hBitmap[12] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP12));
		hBitmap[13] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP13));

		// Червы с туза
		hBitmap[14] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP14));
		hBitmap[15] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP15));
		hBitmap[16] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP16));
		hBitmap[17] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP17));
		hBitmap[18] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP18));
		hBitmap[19] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP19));
		hBitmap[20] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP20));
		hBitmap[21] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP21));
		hBitmap[22] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP22));
		hBitmap[23] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP23));
		hBitmap[24] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP24));
		hBitmap[25] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP25));
		hBitmap[26] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP26));

		// Пики с туза
		hBitmap[27] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP27));
		hBitmap[28] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP28));
		hBitmap[29] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP29));
		hBitmap[30] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP30));
		hBitmap[31] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP31));
		hBitmap[32] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP32));
		hBitmap[33] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP33));
		hBitmap[34] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP34));
		hBitmap[35] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP35));
		hBitmap[36] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP36));
		hBitmap[37] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP37));
		hBitmap[38] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP38));
		hBitmap[39] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP39));

		// Крести с туза
		hBitmap[40] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP40));
		hBitmap[41] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP41));
		hBitmap[42] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP42));
		hBitmap[43] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP43));
		hBitmap[44] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP44));
		hBitmap[45] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP45));
		hBitmap[46] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP46));
		hBitmap[47] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP47));
		hBitmap[48] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP48));
		hBitmap[49] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP49));
		hBitmap[50] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP50));
		hBitmap[51] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP51));
		hBitmap[52] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP52));

		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON_1:

			// Запустить поток, скрыть кнопки
			tParametr = 1;
			hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CalculateThread, &tParametr, NULL, &tPar);
			ShowWindow(hButton1, SW_HIDE);
			ShowWindow(hButton2, SW_HIDE);
			break;
		case ID_BUTTON_2:

			// Запустить поток, скрыть кнопки
			tParametr = 2;
			hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CalculateThread, &tParametr, NULL, &tPar);
			ShowWindow(hButton1, SW_HIDE);
			ShowWindow(hButton2, SW_HIDE);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);
		hCompatibleDC = CreateCompatibleDC(hdc);

		GetObject(hBitmap[card1], sizeof(BITMAP), &Bitmap);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap[card1]);
		StretchBlt(hdc, 10, 10, Bitmap.bmWidth, Bitmap.bmHeight, hCompatibleDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
		SelectObject(hCompatibleDC, hOldBitmap);

		GetObject(hBitmap[card2], sizeof(BITMAP), &Bitmap);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap[card2]);
		StretchBlt(hdc, 35, 10, Bitmap.bmWidth, Bitmap.bmHeight, hCompatibleDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
		SelectObject(hCompatibleDC, hOldBitmap);

		GetObject(hBitmap[card3], sizeof(BITMAP), &Bitmap);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap[card3]);
		StretchBlt(hdc, 70, 10, Bitmap.bmWidth, Bitmap.bmHeight, hCompatibleDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
		SelectObject(hCompatibleDC, hOldBitmap);

		GetObject(hBitmap[card4], sizeof(BITMAP), &Bitmap);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap[card4]);
		StretchBlt(hdc, 95, 10, Bitmap.bmWidth, Bitmap.bmHeight, hCompatibleDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
		SelectObject(hCompatibleDC, hOldBitmap);

		GetObject(hBitmap[card5], sizeof(BITMAP), &Bitmap);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap[card5]);
		StretchBlt(hdc, 120, 10, Bitmap.bmWidth, Bitmap.bmHeight, hCompatibleDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
		SelectObject(hCompatibleDC, hOldBitmap);

		GetObject(hBitmap[card6], sizeof(BITMAP), &Bitmap);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap[card6]);
		StretchBlt(hdc, 145, 10, Bitmap.bmWidth, Bitmap.bmHeight, hCompatibleDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
		SelectObject(hCompatibleDC, hOldBitmap);

		GetObject(hBitmap[card7], sizeof(BITMAP), &Bitmap);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap[card7]);
		StretchBlt(hdc, 170, 10, Bitmap.bmWidth, Bitmap.bmHeight, hCompatibleDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
		SelectObject(hCompatibleDC, hOldBitmap);

		// PaintDiapazon(hdc, 100, 100, diapazon);

		//DeleteObject(hBitmap);
		DeleteDC(hCompatibleDC);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//
// Считывает диапазон комбинаций до запятой или до конца строки
// 2 - считали, есть еще (т.е. запятая)
// 1 - считали, но больше нету (конец строки, ноль)
// 0 - не считали, ошибка
//
int CommaComb(char* inputDiapazon, char* outputChar)
{
	for (int x = 0; ; x++)
	{
		if (inputDiapazon[x] == ',' || inputDiapazon[x] == 0)
		{
			outputChar[x] = 0;
			return x;
		}
		else
		{
			outputChar[x] = inputDiapazon[x];
		}
	}
}

// Возвращает ранг карты
int RangCard(char card)
{
	int x = 0;
	switch (card)
	{
	case 'A':
		x = 14;
		break;
	case 'K':
		x = 12;
		break;
	case 'Q':
		x = 12;
		break;
	case 'J':
		x = 11;
		break;
	case 'T':
		x = 10;
		break;
	case '9':
		x = 9;
		break;
	case '8':
		x = 8;
		break;
	case '7':
		x = 7;
		break;
	case '6':
		x = 6;
		break;
	case '5':
		x = 5;
		break;
	case '4':
		x = 4;
		break;
	case '3':
		x = 3;
		break;
	case '2':
		x = 2;
		break;
	}
	return x;
}

BOOL CheckPodDiap(char* ch_comb, char* podDiap)
{
	if (lstrlen(ch_comb) == 2)
	{
		// Если проверяемая пара
		if (podDiap[0] == podDiap[1])
		{
			// Если в поддиапазоне тоже пара
			if (podDiap[2] == '+')
			{
				if (RangCard(podDiap[0]) <= RangCard(ch_comb[0]))
				{
					return TRUE;
				}
			}
			else if (podDiap[2] == ',')
			{

			}
			else if (podDiap[2] == '-')
			{

			}
		}
	}
	else if (ch_comb[2] == 's')
	{
		// Одномастная комбинация

	}
	else if (ch_comb[2] == 'o')
	{
		// Разномастная комбинация

	}
	return FALSE;
}


BOOL CheckComb(char* ch_comb, char* diapazon)
{
	char podDiap[10] = "";
	char* u_dip = diapazon;

	while (CommaComb(u_dip, podDiap) > 1)
	{
		if (CheckPodDiap(ch_comb, podDiap) == TRUE)
		{
			// Если входит в поддиапазон, то выходим
			return TRUE;
		}
		// Подготавливаем указатель для нового поиска поддиапазона
		if (*(u_dip + lstrlen(podDiap)) == ',')
		{
			u_dip = u_dip + lstrlen(podDiap) + 2;
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}

void PaintDiapazon(HDC hdc, int x, int y, char* diapazon)
{
	COLORREF color1 = RGB(54, 31, 184);
	COLORREF color2 = RGB(128, 128, 192);
	//COLORREF colorFont = RGB(0, 0, 0);
	//COLORREF colorBk = RGB(0, 0, 0);
	HBRUSH hbr1 = CreateSolidBrush(color1);
	HBRUSH hbr2 = CreateSolidBrush(color2);
	RECT rect;

	//COLORREF oldColorFont = SetTextColor(hdc, colorFont);
	SetBkMode(hdc, TRANSPARENT); // Прозрачный фон шрифта

	LOGFONT font;
	font.lfHeight = 10;						// Устанавливает высоту шрифта или символа
	font.lfWidth = 0;						// Устанавливает среднюю ширину символов в шрифте
	font.lfEscapement = 0;					// Устанавливает угол, между вектором наклона и осью X устройства
	font.lfOrientation = 0;					// Устанавливает угол, между основной линией каждого символа и осью X устройства
	font.lfWeight = 100;					// Устанавливает толщину шрифта в диапазоне от 0 до 1000
	font.lfItalic = 0;						// Устанавливает курсивный шрифт
	font.lfUnderline = 0;					// Устанавливает подчеркнутый шрифт
	font.lfStrikeOut = 0;					// Устанавливает зачеркнутый шрифт
	font.lfCharSet = RUSSIAN_CHARSET;		// Устанавливает набор символов
	font.lfOutPrecision = 0;				// Устанавливает точность вывода
	font.lfClipPrecision = 0;				// Устанавливает точность отсечения
	font.lfQuality = 0;						// Устанавливает качество вывода
	font.lfPitchAndFamily = 0;				// Устанавливает ширину символов и семейство шрифта
	strcpy_s(font.lfFaceName, "VERDANA");	// Устанавливает название шрифта

	HFONT hFont = CreateFontIndirect(&font);

	// Матрица всего диапазона
	char* char_comb[169];

	// 1
	char_comb[0] = "AA";
	char_comb[1] = "AKs";
	char_comb[2] = "AQs";
	char_comb[3] = "AJs";
	char_comb[4] = "ATs";
	char_comb[5] = "A9s";
	char_comb[6] = "A8s";
	char_comb[7] = "A7s";
	char_comb[8] = "A6s";
	char_comb[9] = "A5s";
	char_comb[10] = "A4s";
	char_comb[11] = "A3s";
	char_comb[12] = "A2s";

	// 2
	char_comb[13] = "AKo";
	char_comb[14] = "KK";
	char_comb[15] = "KQs";
	char_comb[16] = "kJs";
	char_comb[17] = "KTs";
	char_comb[18] = "K9s";
	char_comb[19] = "K9s";
	char_comb[20] = "K7s";
	char_comb[21] = "K6s";
	char_comb[22] = "K5s";
	char_comb[23] = "K4s";
	char_comb[24] = "K3s";
	char_comb[25] = "K2s";

	// 3
	char_comb[26] = "AQo";
	char_comb[27] = "KQo";
	char_comb[28] = "QQ";
	char_comb[29] = "QJs";
	char_comb[30] = "QTs";
	char_comb[31] = "Q9s";
	char_comb[32] = "Q8s";
	char_comb[33] = "Q7s";
	char_comb[34] = "Q6s";
	char_comb[35] = "Q5s";
	char_comb[36] = "Q4s";
	char_comb[37] = "Q3s";
	char_comb[38] = "Q2s";

	// 4
	char_comb[39] = "AJo";
	char_comb[40] = "KJo";
	char_comb[41] = "QJo";
	char_comb[42] = "JJ";
	char_comb[43] = "JTs";
	char_comb[44] = "J9s";
	char_comb[45] = "J8s";
	char_comb[46] = "J7s";
	char_comb[47] = "J6s";
	char_comb[48] = "J5s";
	char_comb[49] = "J4s";
	char_comb[50] = "J3s";
	char_comb[51] = "J2s";

	// 5
	char_comb[52] = "ATo";
	char_comb[53] = "KTo";
	char_comb[54] = "QTo";
	char_comb[55] = "JTo";
	char_comb[56] = "TT";
	char_comb[57] = "T9s";
	char_comb[58] = "T8s";
	char_comb[59] = "T7s";
	char_comb[60] = "T6s";
	char_comb[61] = "T5s";
	char_comb[62] = "T4s";
	char_comb[63] = "T3s";
	char_comb[64] = "T2s";

	// 6
	char_comb[65] = "A9o";
	char_comb[66] = "K9o";
	char_comb[67] = "Q9o";
	char_comb[68] = "J9o";
	char_comb[69] = "T9o";
	char_comb[70] = "99";
	char_comb[71] = "98s";
	char_comb[72] = "97s";
	char_comb[73] = "96s";
	char_comb[74] = "95s";
	char_comb[75] = "94s";
	char_comb[76] = "93s";
	char_comb[77] = "92s";

	// 7
	char_comb[78] = "A8o";
	char_comb[79] = "K8o";
	char_comb[80] = "Q8o";
	char_comb[81] = "J8o";
	char_comb[82] = "T8o";
	char_comb[83] = "98o";
	char_comb[84] = "88";
	char_comb[85] = "87s";
	char_comb[86] = "86s";
	char_comb[87] = "85s";
	char_comb[88] = "84s";
	char_comb[89] = "83s";
	char_comb[90] = "82s";

	// 8
	char_comb[91] = "A7o";
	char_comb[92] = "K7o";
	char_comb[93] = "Q7o";
	char_comb[94] = "J7o";
	char_comb[95] = "T7o";
	char_comb[96] = "97o";
	char_comb[97] = "87o";
	char_comb[98] = "77";
	char_comb[99] = "76s";
	char_comb[100] = "75s";
	char_comb[101] = "74s";
	char_comb[102] = "73s";
	char_comb[103] = "72s";

	// 9
	char_comb[104] = "A6o";
	char_comb[105] = "K6o";
	char_comb[106] = "Q6o";
	char_comb[107] = "J6o";
	char_comb[108] = "T6o";
	char_comb[109] = "96o";
	char_comb[110] = "86o";
	char_comb[111] = "76o";
	char_comb[112] = "66";
	char_comb[113] = "65s";
	char_comb[114] = "64s";
	char_comb[115] = "63s";
	char_comb[116] = "62s";

	// 10
	char_comb[117] = "A5o";
	char_comb[118] = "K5o";
	char_comb[119] = "Q5o";
	char_comb[120] = "J5o";
	char_comb[121] = "T5o";
	char_comb[122] = "95o";
	char_comb[123] = "85o";
	char_comb[124] = "75o";
	char_comb[125] = "65o";
	char_comb[126] = "55";
	char_comb[127] = "54s";
	char_comb[128] = "53s";
	char_comb[129] = "52s";

	// 11
	char_comb[130] = "A4o";
	char_comb[131] = "K4o";
	char_comb[132] = "Q4o";
	char_comb[133] = "J4o";
	char_comb[134] = "T4o";
	char_comb[135] = "94o";
	char_comb[136] = "84o";
	char_comb[137] = "74o";
	char_comb[138] = "64o";
	char_comb[139] = "54o";
	char_comb[140] = "44";
	char_comb[141] = "43s";
	char_comb[142] = "42s";

	// 12
	char_comb[143] = "A3o";
	char_comb[144] = "K3o";
	char_comb[145] = "Q3o";
	char_comb[146] = "J3o";
	char_comb[147] = "T3o";
	char_comb[148] = "93o";
	char_comb[149] = "83o";
	char_comb[150] = "73o";
	char_comb[151] = "63o";
	char_comb[152] = "53o";
	char_comb[153] = "43o";
	char_comb[154] = "33";
	char_comb[155] = "32s";

	// 13
	char_comb[156] = "A2o";
	char_comb[157] = "K2o";
	char_comb[158] = "Q2o";
	char_comb[159] = "J2o";
	char_comb[160] = "T2o";
	char_comb[161] = "92o";
	char_comb[162] = "82o";
	char_comb[163] = "72o";
	char_comb[164] = "62o";
	char_comb[165] = "52o";
	char_comb[166] = "42o";
	char_comb[167] = "32o";
	char_comb[168] = "22";

	// Индекс комбинации в массиве
	int index_comb = 0;

	// Цикл по таблице комбинаций
	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			// Устанавливаем прямоугольник
			SetRect(&rect, 250 + x * 20 + 1, 10 + y * 20 + 1, 270 + x * 20, 30 + y * 20);

			// Зарисовываем прямоугольник в зависимости от включения в диапазон
			if (CheckComb(char_comb[index_comb], diapazon) == TRUE)
			{
				// Зарисовываем его первой кистью
				FillRect(hdc, &rect, hbr1);
			}
			else
			{
				// Зарисовываем прямоугольник второй кистью
				FillRect(hdc, &rect, hbr2);
			}

			// Выводим название комбинации поверх прямоугольниука
			SelectObject(hdc, hFont);
			TextOut(hdc, 250 + x * 20 + 1, 10 + y * 20 + 1, char_comb[index_comb], lstrlen(char_comb[index_comb]));
			index_comb++;
		}
	}
}

void ClsWind()
{
	SetWindowText(hEdit1, "");
	SetWindowText(hEdit2, "");
	SetWindowText(hEdit3, "");
	SetWindowText(hEdit4, "");

	card1 = 0;
	card2 = 0;
	card3 = 0;
	card4 = 0;
	card5 = 0;
	card6 = 0;
	card7 = 0;

	InvalidateRect(NULL, NULL, FALSE);
}

//
// Функция реального времени, работает в отдельном потоке
//
//   - получает информацию со стола
//   - на основе полученной информации производит расчет
//   - выводит результат расчера в текстовом виде в окно
//
DWORD WINAPI CalculateThread(LPVOID param)
{
	// Калькулятор шансов и скрепалка
	HoldemCalculator calc;
	Scraper scraper;
	
	// Устанавливаем номер стола 
	scraper.SetTable(*(int*)param);

	// Переменные для работы калькулятора
	int numberOfTrials = 300000;
	char pCard[100] = "";
	char pBoard[20] = "";
	double results[10];
	char chResults[10] = "";
	char chPotOdds[10] = "";
	char chOpp[10] = "";
	double ev = 0;
	char chEV[20] = "";

	// Уникальный идентификатор последней расчитанной ситуации
	unsigned __int64 calcPlay = 0;
	int calcRound = 0;
	double calcPot = 0;

	// Инициализируем окно начальными данными
	ClsWind();

	// Бесконечный цикл, поток уничтожит система при закрытии приложения
	while (TRUE)
	{		
		// Задержка для снижения нагрузки на систему
		Sleep(1000);

		// Очищаем буфера
		ZeroMemory(pCard, 100);
		ZeroMemory(pBoard, 20);
		ZeroMemory(chOpp, 10);
		ZeroMemory(chResults, 10);
		ZeroMemory(chPotOdds, 10);

		// Скриншот всего экрана
		scraper.Screen();

		// Если горит кнопка фолд
		if (scraper.Fold() == TRUE)
		{
			unsigned __int64 play = scraper.Play();
			double pot = scraper.Pot();
			int round = scraper.Round();
			int player = scraper.Player();

			// Если новая ситуация, для которой расчет еще не выполнялся
			if ((play != 0 && pot != 0 && round != 0 && player != 0) && (play != calcPlay || pot != calcPot || round != calcRound))
			{
				// Считываем карты
				if (SetCard(scraper, round, pCard, pBoard) != 0)
				{				
					// Показываем число оппонентов
					_itoa_s(player, chOpp, 10);
					SetWindowText(hEdit4, chOpp);

					// Расчитываем и показываем шансы банка
					double call = scraper.Call();
					if (call != 0)
					{
						double potOdds = 100 / ((pot + call) / call);
						sprintf_s(chPotOdds, "%d%%", (int)potOdds);
						SetWindowText(hEdit2, chPotOdds);
						// double divOds = pot / call;
						// char chDivOds[20] = "";
						// sprintf_s(chDivOds, "%.1f", divOds);
						// strcat_s(chPotOdds, " или 1 к ");
						// strcat_s(chPotOdds, chDivOds);
					}

					// Добавляем диапазоны опонентов для расчета эквити
					for (int x = 0; x < player; x++)
					{
						strcat_s(pCard, 100, "|XxXx");
					}
					
					// Расчитываем  и показываем эквити
					calc.CalculateMC(pCard, pBoard, NULL, numberOfTrials, results);
					sprintf_s(chResults, "%d%%", (int)results[0]);
					SetWindowText(hEdit1, chResults);

					if (call != 0)
					{
						// Расчитываем  и показываем математическое ожидание EV
						ev = (pot / 100) * results[0] - call;
						sprintf_s(chEV, "%.2f", ev);
						strcat_s(chEV, "$");
						SetWindowText(hEdit3, chEV);
					}

					// Обновить индикаторы обработанной ситуации
					calcPot = pot;
					calcPlay = play;
					calcRound = round;
				}
			}
		}

		// Освобождаем память от скриншота
		scraper.DeleteScreen();
	}

	// Сюда никогда не попадаем
	return 0;
}

// Считываем карты в переменные для калькулятора
int SetCard(Scraper scraper, int iRound, char* pCard, char* pBoard)
{
	int iCard1 = 0;
	int iCard2 = 0;
	int iCard3 = 0;
	int iCard4 = 0;
	int iCard5 = 0;
	int iCard6 = 0;
	int iCard7 = 0;
	
	switch (iRound)
	{
	case PREFLOP:	
		iCard1 = scraper.Card(1, pCard);
		iCard2 = scraper.Card(2, pCard + 2);	
		if (iCard1 != 0 && iCard2 != 0)
		{
			// Успех
			card1 = iCard1;
			card2 = iCard2;
			card3 = 0;
			card4 = 0;
			card5 = 0;
			card6 = 0;
			card7 = 0;

			SetWindowText(hEdit1, "");
			SetWindowText(hEdit2, "");
			SetWindowText(hEdit3, "");
			SetWindowText(hEdit4, "");
			
			// Обновляем отображаемые карты
			InvalidateRect(NULL, NULL, FALSE);
			return 1;
		}
		else
		{
			// Ошибка
			ClsWind;
			return 0;
		}

	case FLOP:
		iCard1 = scraper.Card(1, pCard);
		iCard2 = scraper.Card(2, pCard + 2);
		iCard3 = scraper.Card(3, pBoard);
		iCard4 = scraper.Card(4, pBoard + 2);
		iCard5 = scraper.Card(5, pBoard + 4);

		if (iCard1 != 0 && iCard2 != 0 && iCard3 != 0 && iCard4 != 0 && iCard5 != 0)
		{
			// Успех
			card1 = iCard1;
			card2 = iCard2;
			card3 = iCard3;
			card4 = iCard4;
			card5 = iCard5;
			card6 = 0;
			card7 = 0;

			SetWindowText(hEdit1, "");
			SetWindowText(hEdit2, "");
			SetWindowText(hEdit3, "");
			SetWindowText(hEdit4, "");
			
			// Обновляем отображаемые карты
			InvalidateRect(NULL, NULL, FALSE);
			return 1;
		}
		else
		{
			// Ошибка
			ClsWind;
			return 0;
		}

	case TERN:
		iCard1 = scraper.Card(1, pCard);
		iCard2 = scraper.Card(2, pCard + 2);
		iCard3 = scraper.Card(3, pBoard);
		iCard4 = scraper.Card(4, pBoard + 2);
		iCard5 = scraper.Card(5, pBoard + 4);
		iCard6 = scraper.Card(6, pBoard + 6);

		if (iCard1 != 0 && iCard2 != 0 && iCard3 != 0 && iCard4 != 0 && iCard5 != 0 && iCard6 != 0)
		{
			// Успех
			card1 = iCard1;
			card2 = iCard2;
			card3 = iCard3;
			card4 = iCard4;
			card5 = iCard5;
			card6 = iCard6;
			card7 = 0;

			SetWindowText(hEdit1, "");
			SetWindowText(hEdit2, "");
			SetWindowText(hEdit3, "");
			SetWindowText(hEdit4, "");

			// Обновляем отображаемые карты
			InvalidateRect(NULL, NULL, FALSE);
			return 1;
		}
		else
		{
			// Ошибка
			ClsWind;
			return 0;
		}
	case RIVER:
		iCard1 = scraper.Card(1, pCard);
		iCard2 = scraper.Card(2, pCard + 2);
		iCard3 = scraper.Card(3, pBoard);
		iCard4 = scraper.Card(4, pBoard + 2);
		iCard5 = scraper.Card(5, pBoard + 4);
		iCard6 = scraper.Card(6, pBoard + 6);
		iCard7 = scraper.Card(7, pBoard + 8);

		if (iCard1 != 0 && iCard2 != 0 && iCard3 != 0 && iCard4 != 0 && iCard5 != 0 && iCard6 != 0 && iCard7 != 0)
		{
			// Успех
			card1 = iCard1;
			card2 = iCard2;
			card3 = iCard3;
			card4 = iCard4;
			card5 = iCard5;
			card6 = iCard6;
			card7 = iCard7;

			SetWindowText(hEdit1, "");
			SetWindowText(hEdit2, "");
			SetWindowText(hEdit3, "");
			SetWindowText(hEdit4, "");

			// Обновляем отображаемые карты
			InvalidateRect(NULL, NULL, FALSE);
			return 1;
		}
		else
		{
			// Ошибка
			ClsWind;
			return 0;
		}
	}
	return 0;
}

