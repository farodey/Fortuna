#define USER_DLL

#include "user.h"
#include "resource.h"
#include <conio.h>
#include <windows.h>
#include <bitset>
#include <intrin.h>
#include "OpenHoldemFunctions.h"

bool identified;
int color[169];			// Диапазон (массив цветов для матрицы диапазона)
char action[100] = "";		// Действие
char position[] = "";	// Позиция
char hand[] = "";		// Рука

// Объявления функций
void Preflop();
DWORD WINAPI GUIThread(LPVOID param);
void PaintDiapazon(HDC hdc, int x, int y, char* diapazon);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Paint(HDC hdc);

// Переменные для хранения выводимых данных
char paintRange[] = "22+, A2s+, K2s+, Q2s+, J2s+, T2s+, 92s+, 82s+, 72s+, 62s+, 52s+, 42s+, 32s, A2o+, K2o+, Q2o+, J2o+, T2o+, 92o+, 82o+, 72o+, 62o+, 52o+, 42o+, 32o";
char myHand[10] = "";
char myVarHand[10] = "";
bool handInRange = false;
char* actions[3] = { "Raise", "Call", "Fold" };

//******************************************************************************
//
// Place all your initalizations in the functions below
// DLLOnLoad() amd DLLOnUnLoad() get called by the DLL,
// the other functions get called by OpenHoldem
//
//******************************************************************************

void DLLOnLoad() {
}

void DLLOnUnLoad() {
}

void __stdcall DLLUpdateOnNewFormula() {
}

void __stdcall DLLUpdateOnConnection() {
}

void __stdcall DLLUpdateOnHandreset() {
}

void __stdcall DLLUpdateOnNewRound() {
}

void __stdcall DLLUpdateOnMyTurn() {
}

void __stdcall DLLUpdateOnHeartbeat() {
}

//******************************************************************************
//
// ProcessQuery()
// Handling the lookup of dll$symbols
//
//******************************************************************************

DLL_IMPLEMENTS double __stdcall ProcessQuery(const char* pquery) {

	if (pquery == NULL) {
		return 0;
	}

	if (strncmp(pquery, "dll$test", 9) == 0) {
		return GetSymbol("random");
	}

	if (strncmp(pquery, "dll$scrape", 11) == 0) {
		char* scraped_result;
		int result_lenght;
		scraped_result = ScrapeTableMapRegion("p0balance", result_lenght);
		if (scraped_result != nullptr) {
			// The TEXT() macro supports both ASCII and Unicode.
			// For the people who use Unicode but don't understand the "error".
			// http://www.maxinmontreal.com/forums/viewtopic.php?f=174&t=19999
			// http://stackoverflow.com/questions/15498070/what-does-t-stands-for-in-a-cstring
			MessageBox(0, scraped_result, TEXT("Scraped custom region"), 0);
			LocalFree(scraped_result);
		}
	}

	if (strncmp(pquery, "dll$preflop", 12) == 0) {
		Preflop();
	}
	return 0;
}

//******************************************************************************
//
// DLL entry point
//
//******************************************************************************

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	
	DWORD tPar = 0;
	HANDLE hThread;
	
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
#ifdef _DEBUG
		AllocConsole();
#endif _DEBUG
		InitializeOpenHoldemFunctionInterface();

		// Запускаем поток с графическим интерфейсом
		hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GUIThread, &hModule, NULL, &tPar);
		break;

	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
#ifdef _DEBUG
		FreeConsole();
#endif _DEBUG
		break;
	}
	return TRUE;
}

DWORD WINAPI GUIThread(LPVOID param)
{
	HINSTANCE hInstance = *(HINSTANCE *)param;
	
	// Зарегистрировать класс окна
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "FORTUNA";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassEx(&wcex);

	// Создать окно
	HWND hWnd = CreateWindow("FORTUNA", "Fortuna", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, 700, 600, NULL, NULL, hInstance, NULL);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hCompatibleDC;
	HANDLE hOldBitmap;
	BITMAP Bitmap;
	HANDLE mutex;

	switch (message)
	{
	case WM_CREATE:

		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);
		hCompatibleDC = CreateCompatibleDC(hdc);
		// mutex = CreateMutex();

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

void Paint(HDC hdc)
{	
	// Цвета
	COLORREF color1 = RGB(54, 31, 184);		// Темно-синий
	COLORREF color2 = RGB(128, 128, 192);	// Светло-синий
	COLORREF color3 = RGB(54, 196, 33);		// Зеленый
	COLORREF color4 = RGB(240, 10, 10);		// Красный
	COLORREF color5 = RGB(240, 240, 10);	// Оранжевый

	// Кисти
	HBRUSH hbr[5];
	hbr[0] = CreateSolidBrush(color1);
	hbr[1] = CreateSolidBrush(color2);
	hbr[2] = CreateSolidBrush(color3);
	hbr[3] = CreateSolidBrush(color4);
	hbr[4] = CreateSolidBrush(color5);

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

	RECT rect;
	//COLORREF oldColorFont = SetTextColor(hdc, colorFont);
	SetBkMode(hdc, TRANSPARENT); // Прозрачный фон шрифта

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

	if (identified)
	{
		// Диапазон
		int index_comb = 0;		// Индекс комбинации в массиве
		for (int y = 0; y < 13; y++)
		{
			for (int x = 0; x < 13; x++)
			{
				// Зарисовываем прямоугольник
				SetRect(&rect, 250 + x * 20 + 1, 10 + y * 20 + 1, 270 + x * 20, 30 + y * 20);
				FillRect(hdc, &rect, hbr[index_comb]);

				// Выводим название комбинации поверх прямоугольниука
				SelectObject(hdc, hFont);
				TextOut(hdc, 250 + x * 20 + 1, 10 + y * 20 + 1, char_comb[index_comb], lstrlen(char_comb[index_comb]));
				index_comb++;
			}
		}

		// Действие
		TextOut(hdc, 300, 300, action, lstrlen(action));
		
		// Позиция
		TextOut(hdc, 300, 350, position, lstrlen(position));

		// Рука
		TextOut(hdc, 300, 400, hand, lstrlen(hand));
	}
	else
	{
		// Зарисовываем прямоугольник
		SetRect(&rect, 1, 2, 3, 4);
		FillRect(hdc, &rect, hbr[2]);
	}
}

//
// Считывает диапазон комбинаций до запятой или до конца строки
// 2 - считали, есть еще (т.е. запятая)
// 1 - считали, но больше нету (конец строки, ноль)
// 0 - не считали, ошибка
//
int ReadSubrange(char* inputRange, char* outputSubrange)
{
	for (int x = 0; ; x++)
	{
		if (inputRange[x] == ',' || inputRange[x] == 0)
		{
			outputSubrange[x] = 0;
			return x;
		}
		else
		{
			outputSubrange[x] = inputRange[x];
		}
	}
}

// Возвращает ранг карты
int ValueCard(char card)
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

// Сдвиг вправо по кругу
unsigned int Rotr(unsigned int value, unsigned int size, unsigned int count)
{
	unsigned int temp = 0;
	for (int x = 0; x < count; x++)
	{
		temp = value & (1 << (size - 1));
		value >>= 1;
		value &= temp;
	}
	return value;
}

// Битовая маска коллеров от BB до игрока
int RightCalls()
{
	int call = GetSymbol("callbits1");
	int bb = GetSymbol("bigblindchair");
	int user = GetSymbol("userchair");
	int nchairs = GetSymbol("nchairs");

	if (user == bb || nchairs != 6) return 0;
	if (user < bb) user += nchairs;
	int rightChairs = user - bb - 1;
	int maska = pow(2, rightChairs) - 1;
	return (Rotr(call, nchairs, bb) & maska);
}

void Preflop()
{
	if (!GetSymbol("InBigBlind") && GetSymbol("Raises") == 0 && RightCalls() == 0)
	{
		if (GetSymbol("InMiddlePosition2"))
		{
			// Рисуем диапазон открытия рейзом из MP2
			if (GetSymbol("list_MP2_OR"))
			{

			}
			else
			{

			}
		}
		else if (GetSymbol("InMiddlePosition3"))
		{
			if (GetSymbol("list_MP3_OR"))
			{

			}
			else
			{

			}
		}
		else if (GetSymbol("InCutOff"))
		{
			if (GetSymbol("list_CO_OR"))
			{

			}
			else
			{

			}
		}
		else if (GetSymbol("InButton"))
		{
			if (GetSymbol("list_BU_OR"))
			{

			}
			else
			{

			}
		}
		else if (GetSymbol("InSmallBlind"))
		{
			if (GetSymbol("list_SB_OR"))
			{

			}
			else
			{

			}
		}
	}
}

/*
BOOL CheckHandSubrange(char* hand, char* subrange)
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


BOOL CheckHand(char* hand, char* range)
{
char subrange[10] = "";
char* pRange = range;

while (ReadSubrange(pRange, subrange) != 0)
{
if (CheckHandSubrange(hand, subrange) == TRUE)
{
// Рука входит в диапазон
return TRUE;
}
else
{
// Сдвигаем указатель для следующего чтения поддиапазона
pRange += ReadSubrange(pRange, subrange);
}
}
return FALSE;
}
*/


