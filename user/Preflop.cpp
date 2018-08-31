#include <Windows.h>
#include <mutex>
#include "OpenHoldemFunctions.h"
#include "Preflop.h"

// Ранг карты
void RankIntToChar(int rank, char* cRank)
{
	switch (rank)
	{
	case 14:
		*cRank = 'A';
		break;
	case 13:
		*cRank = 'K';
		break;
	case 12:
		*cRank = 'Q';
		break;
	case 11:
		*cRank = 'J';
		break;
	case 10:
		*cRank = 'T';
		break;
	case 9:
		*cRank = '9';
		break;
	case 8:
		*cRank = '8';
		break;
	case 7:
		*cRank = '7';
		break;
	case 6:
		*cRank = '6';
		break;
	case 5:
		*cRank = '5';
		break;
	case 4:
		*cRank = '4';
		break;
	case 3:
		*cRank = '3';
		break;
	case 2:
		*cRank = '2';
		break;
	}
}

int RankCharToInt(char* cRank)
{
	int x = 0;
	switch (*cRank)
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

// Масть карты
void Suit(int suit, char* cSuit)
{
	switch (suit)
	{
	case 0:
		*cSuit = 'h';
		break;

	case 1:
		*cSuit = 'd';
		break;

	case 2:
		*cSuit = 'c';
		break;

	case 3:
		*cSuit = 's';
		break;
	}
}

// Наша рука
void GetHand(char* hand)
{
	RankIntToChar(GetSymbol("$$pr0"), hand);
	Suit(GetSymbol("$$ps0"), hand + 1);
	RankIntToChar(GetSymbol("$$pr1"), hand + 2);
	Suit(GetSymbol("$$ps1"), hand + 3);
}

// Конвертация hand в hand169
void HandToHand169(char* hand, char* hand169)
{
	if (hand[0] == hand[2])
	{
		hand169[0] = hand[0];
		hand169[1] = hand[0];
	}
	else if (hand[1] == hand[3])
	{
		hand169[0] = hand[0];
		hand169[1] = hand[2];
		hand169[2] = 's';
	}
	else if (hand[1] != hand[3])
	{
		hand169[0] = hand[0];
		hand169[1] = hand[2];
		hand169[2] = 'o';
	}

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

//
//  Считывает диапазон комбинаций до запятой или до конца строки
//  Учитывает пробел в начале комбинации
//  return число считанных символов
//
int ReadSubrange(char* inputRange, char* outputSubrange)
{
	int x = 0;
	if (inputRange[0] == ' ')
	{
		inputRange++;
		x = 1;
	}

	for (;; x++)
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

bool CheckHand169Range(char* hand169, char* range)
{
	char subrange[10] = "";
	char* pRange = range;

	while (ReadSubrange(pRange, subrange) != 0)
	{
		if (CheckHand169Subrange(hand169, subrange))
		{
			// Рука входит в диапазон
			return true;
		}
		else
		{
			// Сдвигаем указатель для следующего чтения поддиапазона
			pRange = pRange + ReadSubrange(pRange, subrange) + 2;
		}
	}
	return false;
}

bool CheckHand169Subrange(char* hand169, char* subrange)
{
	// Пара
	if (hand169[0] == hand169[1])
	{
		// Если в поддиапазоне тоже пара
		if (subrange[0] == subrange[1])
		{
			
			if (subrange[2] == '+')
			{
				if (RankCharToInt(subrange) <= RankCharToInt(hand169))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (subrange[2] == ',')
			{
				if (RankCharToInt(subrange) == RankCharToInt(hand169))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (subrange[2] == '-')
			{
				if (RankCharToInt(subrange) <= RankCharToInt(hand169) && RankCharToInt(subrange + 3) >= RankCharToInt(hand169))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}
	
	// Одномастная комбинация
	else if (hand169[2] == 's')
	{
		// Если в поддиапазоне тоже одномастная комбинация
		if (subrange[2] == 's')
		{

			if (subrange[3] == '+')
			{
				if (RankCharToInt(subrange) == RankCharToInt(hand169) && RankCharToInt(subrange + 1) <= RankCharToInt(hand169 + 1))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (subrange[3] == ',')
			{
				if (RankCharToInt(subrange) == RankCharToInt(hand169) && RankCharToInt(subrange + 1) == RankCharToInt(hand169 + 1))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (subrange[3] == '-')
			{
				if (RankCharToInt(subrange) == RankCharToInt(hand169) && RankCharToInt(subrange + 1) <= RankCharToInt(hand169 + 1))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}

	// Разномастная комбинация
	else if (hand169[2] == 'o')
	{
		// Если в поддиапазоне тоже разномастная комбинация
		if (subrange[2] == 'o')
		{

			if (subrange[3] == '+')
			{
				if (RankCharToInt(subrange) == RankCharToInt(hand169) && RankCharToInt(subrange + 1) <= RankCharToInt(hand169 + 1))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (subrange[3] == ',')
			{
				if (RankCharToInt(subrange) == RankCharToInt(hand169) && RankCharToInt(subrange + 1) == RankCharToInt(hand169 + 1))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (subrange[3] == '-')
			{
				if (RankCharToInt(subrange) == RankCharToInt(hand169) && RankCharToInt(subrange + 1) <= RankCharToInt(hand169 + 1))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}
	return false;
}

// Ситуация на префлопе
void Preflop()
{
	// Индикатор уже посчитанной ситуации на префлопе
	static int calsHandNumber = 0;
	static double calsPot = 0;
	
	// Индикатор новой необработанной ситуации на префлопе
	int handNumber = atoi(GetHandnumber());
	double pot = GetSymbol("pot");

	if (handNumber != calsHandNumber || pot != calsPot)
	{
		// Уникальная ситуация на префлопе
		UniqPreflop();

		// Обновляем индикатор расчитанной ситуации
		calsHandNumber = handNumber;
		calsPot = pot;
	}
}

// Уникальная ситуация на префлопе
void UniqPreflop()
{
	// Внешние переменные, синхронизация доступа через мьютекс 
	extern int colorRect[169];
	extern bool cls;					// Очистка
	extern int colorRect[169];			// Массив цветов квадратов в матрице диапазона
	extern char text1[100];				// Действие
	extern char text2[100];				// Позиция
	extern char text3[100];				// Рука
	extern std::mutex mutex;			// Мьютекс
	extern HWND hWnd;					// Окно
	
	// Рука
	char hand[10] = "";
	char hand169[10] = "";
	char* a_hand169[169];
	
	// Инициализируем переменные
	GetHand(hand);
	HandToHand169(hand, hand169);
	InitHand169(a_hand169);

	// Блокируем мьютекс
	mutex.lock();

	// Никто на префлопе добровольно не вкладывался
	if (!GetSymbol("InBigBlind") && GetSymbol("Raises") == 0 && RightCalls() == 0)
	{
		cls = false;
		for (int i = 0; i < 169; i++)
		{
			// Если рука в счетчике рук входит в диапазон открытия рейзом с этой позиции
			if ((GetSymbol("InMiddlePosition2") && CheckHand169Range(a_hand169[i], OR_MP2))	||
				(GetSymbol("InMiddlePosition3") && CheckHand169Range(a_hand169[i], OR_MP3))	||
				(GetSymbol("InCutOff")			&& CheckHand169Range(a_hand169[i], OR_CO))	||
				(GetSymbol("InButton")			&& CheckHand169Range(a_hand169[i], OR_BU))	||
				(GetSymbol("InSmallBlind ")		&& CheckHand169Range(a_hand169[i], OR_SB)))
			{
				if (!strcmp(a_hand169[i], hand169)) colorRect[i] = 5;	// Ярко-зеленый квадрат
				else colorRect[i] = 0;									// Зеленый квадрат
			}
			else
			{
				if (!strcmp(a_hand169[i], hand169)) colorRect[i] = 7;	// Ярко-красный квадрат
				else colorRect[i] = 2;									// Красный квадрат
		
			}
		}
	}
	else
	{
		cls = true;
	}

	// Перерисовываем окно
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
	
	// Освобождаем мьютекс
	mutex.unlock();	
}

/*

// Переменные для хранения выводимых данных
char paintRange[] = "22+, A2s+, K2s+, Q2s+, J2s+, T2s+, 92s+, 82s+, 72s+, 62s+, 52s+, 42s+, 32s, A2o+, K2o+, Q2o+, J2o+, T2o+, 92o+, 82o+, 72o+, 62o+, 52o+, 42o+, 32o";
char myHand[10] = "";
char myVarHand[10] = "";
bool handInRange = false;
char* actions[3] = { "Raise", "Call", "Fold" };


//









*/