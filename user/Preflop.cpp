#include <Windows.h>
#include <mutex>
#include "Window.h"
#include "OpenHoldemFunctions.h"
#include "Preflop.h"

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
	mutex.lock();
	if (!GetSymbol("InBigBlind") && GetSymbol("Raises") == 0 && RightCalls() == 0)
	{
		if (GetSymbol("InMiddlePosition2"))
		{
			// position = ;
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
	mutex.unlock();
}


/*

bool identified;
int color[169];			// Диапазон (массив цветов для матрицы диапазона)
char action[100] = "";	// Действие
char position[] = "";	// Позиция
char hand[10] = "";		// Рука

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