#include <Windows.h>
#include <mutex>
#include "Window.h"

void Window(HANDLE hModule)
{
	HANDLE hThread;
	DWORD idThread;
	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WindowThread, &hModule, NULL, &idThread);
}

DWORD WINAPI WindowThread(LPVOID param)
{
	HINSTANCE hInstance = *(HINSTANCE *)param;
	// Зарегистрировать класс окна
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowCallback;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "FORTUNA";
	// wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassEx(&wcex);

	// Создать окно
	hWnd = CreateWindow("FORTUNA", "Fortuna", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, 335, 400, NULL, NULL, hInstance, NULL);

	// Показываем и обновляем окно
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// Цикл обработки сообщений
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hCompatibleDC;
	HANDLE hOldBitmap;
	BITMAP Bitmap;

	switch (message)
	{
	case WM_CREATE:

		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);
		hCompatibleDC = CreateCompatibleDC(hdc);
		
		Paint(hdc);

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

void InitHand169(char* hand169[])
{
	// 1
	hand169[0] = "AA";
	hand169[1] = "AKs";
	hand169[2] = "AQs";
	hand169[3] = "AJs";
	hand169[4] = "ATs";
	hand169[5] = "A9s";
	hand169[6] = "A8s";
	hand169[7] = "A7s";
	hand169[8] = "A6s";
	hand169[9] = "A5s";
	hand169[10] = "A4s";
	hand169[11] = "A3s";
	hand169[12] = "A2s";

	// 2
	hand169[13] = "AKo";
	hand169[14] = "KK";
	hand169[15] = "KQs";
	hand169[16] = "KJs";
	hand169[17] = "KTs";
	hand169[18] = "K9s";
	hand169[19] = "K9s";
	hand169[20] = "K7s";
	hand169[21] = "K6s";
	hand169[22] = "K5s";
	hand169[23] = "K4s";
	hand169[24] = "K3s";
	hand169[25] = "K2s";

	// 3
	hand169[26] = "AQo";
	hand169[27] = "KQo";
	hand169[28] = "QQ";
	hand169[29] = "QJs";
	hand169[30] = "QTs";
	hand169[31] = "Q9s";
	hand169[32] = "Q8s";
	hand169[33] = "Q7s";
	hand169[34] = "Q6s";
	hand169[35] = "Q5s";
	hand169[36] = "Q4s";
	hand169[37] = "Q3s";
	hand169[38] = "Q2s";

	// 4
	hand169[39] = "AJo";
	hand169[40] = "KJo";
	hand169[41] = "QJo";
	hand169[42] = "JJ";
	hand169[43] = "JTs";
	hand169[44] = "J9s";
	hand169[45] = "J8s";
	hand169[46] = "J7s";
	hand169[47] = "J6s";
	hand169[48] = "J5s";
	hand169[49] = "J4s";
	hand169[50] = "J3s";
	hand169[51] = "J2s";

	// 5
	hand169[52] = "ATo";
	hand169[53] = "KTo";
	hand169[54] = "QTo";
	hand169[55] = "JTo";
	hand169[56] = "TT";
	hand169[57] = "T9s";
	hand169[58] = "T8s";
	hand169[59] = "T7s";
	hand169[60] = "T6s";
	hand169[61] = "T5s";
	hand169[62] = "T4s";
	hand169[63] = "T3s";
	hand169[64] = "T2s";

	// 6
	hand169[65] = "A9o";
	hand169[66] = "K9o";
	hand169[67] = "Q9o";
	hand169[68] = "J9o";
	hand169[69] = "T9o";
	hand169[70] = "99";
	hand169[71] = "98s";
	hand169[72] = "97s";
	hand169[73] = "96s";
	hand169[74] = "95s";
	hand169[75] = "94s";
	hand169[76] = "93s";
	hand169[77] = "92s";

	// 7
	hand169[78] = "A8o";
	hand169[79] = "K8o";
	hand169[80] = "Q8o";
	hand169[81] = "J8o";
	hand169[82] = "T8o";
	hand169[83] = "98o";
	hand169[84] = "88";
	hand169[85] = "87s";
	hand169[86] = "86s";
	hand169[87] = "85s";
	hand169[88] = "84s";
	hand169[89] = "83s";
	hand169[90] = "82s";

	// 8
	hand169[91] = "A7o";
	hand169[92] = "K7o";
	hand169[93] = "Q7o";
	hand169[94] = "J7o";
	hand169[95] = "T7o";
	hand169[96] = "97o";
	hand169[97] = "87o";
	hand169[98] = "77";
	hand169[99] = "76s";
	hand169[100] = "75s";
	hand169[101] = "74s";
	hand169[102] = "73s";
	hand169[103] = "72s";

	// 9
	hand169[104] = "A6o";
	hand169[105] = "K6o";
	hand169[106] = "Q6o";
	hand169[107] = "J6o";
	hand169[108] = "T6o";
	hand169[109] = "96o";
	hand169[110] = "86o";
	hand169[111] = "76o";
	hand169[112] = "66";
	hand169[113] = "65s";
	hand169[114] = "64s";
	hand169[115] = "63s";
	hand169[116] = "62s";

	// 10
	hand169[117] = "A5o";
	hand169[118] = "K5o";
	hand169[119] = "Q5o";
	hand169[120] = "J5o";
	hand169[121] = "T5o";
	hand169[122] = "95o";
	hand169[123] = "85o";
	hand169[124] = "75o";
	hand169[125] = "65o";
	hand169[126] = "55";
	hand169[127] = "54s";
	hand169[128] = "53s";
	hand169[129] = "52s";

	// 11
	hand169[130] = "A4o";
	hand169[131] = "K4o";
	hand169[132] = "Q4o";
	hand169[133] = "J4o";
	hand169[134] = "T4o";
	hand169[135] = "94o";
	hand169[136] = "84o";
	hand169[137] = "74o";
	hand169[138] = "64o";
	hand169[139] = "54o";
	hand169[140] = "44";
	hand169[141] = "43s";
	hand169[142] = "42s";

	// 12
	hand169[143] = "A3o";
	hand169[144] = "K3o";
	hand169[145] = "Q3o";
	hand169[146] = "J3o";
	hand169[147] = "T3o";
	hand169[148] = "93o";
	hand169[149] = "83o";
	hand169[150] = "73o";
	hand169[151] = "63o";
	hand169[152] = "53o";
	hand169[153] = "43o";
	hand169[154] = "33";
	hand169[155] = "32s";

	// 13
	hand169[156] = "A2o";
	hand169[157] = "K2o";
	hand169[158] = "Q2o";
	hand169[159] = "J2o";
	hand169[160] = "T2o";
	hand169[161] = "92o";
	hand169[162] = "82o";
	hand169[163] = "72o";
	hand169[164] = "62o";
	hand169[165] = "52o";
	hand169[166] = "42o";
	hand169[167] = "32o";
	hand169[168] = "22";
}

void Paint(HDC hdc)
{
	// Цвета
	COLORREF color[8];
	color[0] = RGB(122, 183, 90);	// Зеленый	
	color[1] = RGB(90, 141, 168);	// Синий
	color[2] = RGB(216, 118, 117);	// Красный
	color[3] = RGB(245, 245, 245);	// Белый
	color[4] = RGB(0, 0, 0);		// Черный
	color[5] = RGB(0, 255, 0);		// Ярко-зеленый
	color[6] = RGB(0, 0, 255);		// Ярко-синий
	color[7] = RGB(255, 0, 0);		// Ярко-красный

	// Кисти
	HBRUSH hbr[8];
	hbr[0] = CreateSolidBrush(color[0]);
	hbr[1] = CreateSolidBrush(color[1]);
	hbr[2] = CreateSolidBrush(color[2]);
	hbr[3] = CreateSolidBrush(color[3]);
	hbr[4] = CreateSolidBrush(color[4]);
	hbr[5] = CreateSolidBrush(color[5]);
	hbr[6] = CreateSolidBrush(color[6]);
	hbr[7] = CreateSolidBrush(color[7]);

	// Шрифт
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

	// Шрифт
	LOGFONT font1;
	font1.lfHeight = 30;						// Устанавливает высоту шрифта или символа
	font1.lfWidth = 0;						// Устанавливает среднюю ширину символов в шрифте
	font1.lfEscapement = 0;					// Устанавливает угол, между вектором наклона и осью X устройства
	font1.lfOrientation = 0;					// Устанавливает угол, между основной линией каждого символа и осью X устройства
	font1.lfWeight = 100;					// Устанавливает толщину шрифта в диапазоне от 0 до 1000
	font1.lfItalic = 0;						// Устанавливает курсивный шрифт
	font1.lfUnderline = 0;					// Устанавливает подчеркнутый шрифт
	font1.lfStrikeOut = 0;					// Устанавливает зачеркнутый шрифт
	font1.lfCharSet = RUSSIAN_CHARSET;		// Устанавливает набор символов
	font1.lfOutPrecision = 0;				// Устанавливает точность вывода
	font1.lfClipPrecision = 0;				// Устанавливает точность отсечения
	font1.lfQuality = 0;						// Устанавливает качество вывода
	font1.lfPitchAndFamily = 0;				// Устанавливает ширину символов и семейство шрифта
	strcpy_s(font.lfFaceName, "VERDANA");	// Устанавливает название шрифта
	HFONT hFont1 = CreateFontIndirect(&font1);

	RECT rect;
	//COLORREF oldColorFont = SetTextColor(hdc, colorFont);
	SetBkMode(hdc, TRANSPARENT); // Прозрачный фон шрифта

	// Создаем и иннициализируем матрицу всего диапазона 13 x 13 =  169
	char* hand169[169];
	InitHand169(hand169);

	// Блокируем мьютекс для доступа к данным
	mutex.lock();

	if (cls)
	{
		// Рисуем прямоугольник (очистка окна)
		SetRect(&rect, 0, 0, 335, 400);
		FillRect(hdc, &rect, hbr[3]);
	}
	else
	{
		// Рисуем прямоугольник (очистка окна)
		SetRect(&rect, 0, 0, 335, 400);
		FillRect(hdc, &rect, hbr[3]);
		
		// Черный квадрат - рамка для всей матрицы
		SetRect(&rect, 10, 50, 10 + 13 * 24 + 1, 50 + 13 * 24 + 1); 
		FillRect(hdc, &rect, hbr[4]);
			
		// Рисуем диапазон
		int index_comb = 0;		// Индекс комбинации в массиве
		for (int y = 0; y < 13; y++)
		{
			for (int x = 0; x < 13; x++)
			{
				// Квадрат (левая, верхняя, правая, нижняя)
				SetRect(&rect, 10 + x * 24 + 1, 50 + y * 24 + 1, 10 + 24 + x * 24, 50 + 24 + y * 24);
				FillRect(hdc, &rect, hbr[colorRect[index_comb]]);

				// Выводим название комбинации поверх прямоугольниука
				SelectObject(hdc, hFont);
				SetTextColor(hdc, color[4]);			// Цвет - черный
				TextOut(hdc, 10 + x * 24 + 6, 50 + y * 24 + 9, hand169[index_comb], strlen(hand169[index_comb]));
				index_comb++;
			}
		}

		// Выводим действие (решение бота)
		SelectObject(hdc, hFont1);
		SetTextColor(hdc, color[colorText1]);			// Цвет
		TextOut(hdc, 10, 10, text1, strlen(text1));		// Текст

		// Выводим позицию
		SelectObject(hdc, hFont);
		SetTextColor(hdc, color[4]);
		TextOut(hdc, 120, 10, text2, strlen(text2));
	}

	// Разблокируем мьютекс
	mutex.unlock();
}