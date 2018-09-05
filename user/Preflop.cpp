#include <Windows.h>
#include <mutex>
#include "OpenHoldemFunctions.h"
#include "Preflop.h"

// ���� �����
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

// ����� �����
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

// ���� ����
void GetHand(char* hand)
{
	RankIntToChar(GetSymbol("$$pr0"), hand);
	Suit(GetSymbol("$$ps0"), hand + 1);
	RankIntToChar(GetSymbol("$$pr1"), hand + 2);
	Suit(GetSymbol("$$ps1"), hand + 3);
}

// ����������� hand � hand169
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

// ����� ������ �� �����
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

// ������� ����� �������� �� BB �� ������
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
//  ��������� �������� ���������� �� ������� ��� �� ����� ������
//  ��������� ������ � ������ ����������
//  return ����� ��������� ��������
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
			// ���� ������ � ��������
			return true;
		}
		else
		{
			// �������� ��������� ��� ���������� ������ ������������
			pRange = pRange + ReadSubrange(pRange, subrange) + 2;
		}
	}
	return false;
}

bool CheckHand169Subrange(char* hand169, char* subrange)
{
	// ����
	if (hand169[0] == hand169[1])
	{
		// ���� � ������������ ���� ����
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
	
	// ����������� ����������
	else if (hand169[2] == 's')
	{
		// ���� � ������������ ���� ����������� ����������
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

	// ������������ ����������
	else if (hand169[2] == 'o')
	{
		// ���� � ������������ ���� ������������ ����������
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

// �������
void Preflop()
{
	// ������ �������� � ����� �������
	static int calsHandNumber = 0;
	int handNumber = atoi(GetHandnumber());
	if (handNumber != calsHandNumber &&
		((!GetSymbol("InBigBlind") && !GetSymbol("InSmallBlind") && GetSymbol("currentbet") == 0) ||
		(GetSymbol("InBigBlind")   && GetSymbol("currentbet") == GetSymbol("bblind ")) ||
		(GetSymbol("InSmallBlind") && GetSymbol("currentbet") == GetSymbol("sblind "))))
	{
		
		// ������ �������� �� ��������
		FirstAction();
		calsHandNumber = handNumber;
	}
}

// ���� ������ �������� � ����� �������
void FirstAction()
{
	// ������� ����������, ������������� ������� ����� ������� 
	extern int colorRect[169];
	extern bool cls;					// �������
	extern int colorRect[169];			// ������ ������ ��������� � ������� ���������
	extern char text1[100];				// ��������
	extern int  colorText1;				// ���� ��������
	extern char text2[100];				// �������
	extern char text3[100];				// ����
	extern std::mutex mutex;			// �������
	extern HWND hWnd;					// ����
	
	// ����
	char hand[10] = "";
	char hand169[10] = "";
	char* a_hand169[169];
	
	// �������������� ����������
	GetHand(hand);
	HandToHand169(hand, hand169);
	InitHand169(a_hand169);

	// ��������� �������
	mutex.lock();

	//
	//  ����� ����������� �� ����������� � ���� (Open Raise)
	//
	if (GetSymbol("Calls") == 0 && GetSymbol("Raises") == 0)
	{
		cls = false;
		for (int i = 0; i < 169; i++)
		{
			// ���� ���� � �������� ��� ������ � �������� �������� ������ � ���� �������
			if ((GetSymbol("InMiddlePosition2") && CheckHand169Range(a_hand169[i], OR_MP2)) ||
				(GetSymbol("InMiddlePosition3") && CheckHand169Range(a_hand169[i], OR_MP3)) ||
				(GetSymbol("InCutOff") && CheckHand169Range(a_hand169[i], OR_CO)) ||
				(GetSymbol("InButton") && CheckHand169Range(a_hand169[i], OR_BU)) ||
				(GetSymbol("InSmallBlind ") && CheckHand169Range(a_hand169[i], OR_SB)))
			{
				// Raise
				if (!strcmp(a_hand169[i], hand169))
				{
					// ������� ������� ����
					colorRect[i] = 5;
					colorText1 = 0;
					strncpy(text1, "Raise", strlen("Raise"));
				}
				else colorRect[i] = 0;
			}
			else
			{
				// Fold
				if (!strcmp(a_hand169[i], hand169))
				{
					// ������� ������� ����
					colorRect[i] = 7;
					colorText1 = 2;
					strncpy(text1, "Fold", strlen("Fold"));
				}
				else colorRect[i] = 2;
			}
		}
	}

	//
	// ����� ���� ��� ���� ����
	//
	else if (GetSymbol("Calls") == 0 && GetSymbol("Raises") == 1)
	{
		cls = false;
		for (int i = 0; i < 169; i++)
		{
			// 3-BET
			if ((GetSymbol("InMiddlePosition3") && GetSymbol("betpositionrais") == 3 && CheckHand169Range(a_hand169[i], TB_MP3vsMP2)) ||
				(GetSymbol("InCutOff")          && GetSymbol("betpositionrais") == 3 && CheckHand169Range(a_hand169[i], TB_COvsMP2))  ||
				(GetSymbol("InButton")          && GetSymbol("betpositionrais") == 3 && CheckHand169Range(a_hand169[i], TB_BUvsMP2))  ||
				(GetSymbol("InSmallBlind")      && GetSymbol("betpositionrais") == 3 && CheckHand169Range(a_hand169[i], TB_SBvsMP2))  ||
				(GetSymbol("InBigBlind")        && GetSymbol("betpositionrais") == 3 && CheckHand169Range(a_hand169[i], TB_BBvsMP2))  ||

				(GetSymbol("InCutOff")			&& GetSymbol("betpositionrais") == 4 && CheckHand169Range(a_hand169[i], TB_COvsMP3))  ||
				(GetSymbol("InButton")			&& GetSymbol("betpositionrais") == 4 && CheckHand169Range(a_hand169[i], TB_BUvsMP3))  ||
				(GetSymbol("InSmallBlind")		&& GetSymbol("betpositionrais") == 4 && CheckHand169Range(a_hand169[i], TB_SBvsMP3))  ||
				(GetSymbol("InBigBlind")		&& GetSymbol("betpositionrais") == 4 && CheckHand169Range(a_hand169[i], TB_BBvsMP3))  ||

				(GetSymbol("InButton")			&& GetSymbol("betpositionrais") == 5 && CheckHand169Range(a_hand169[i], TB_BUvsCO))    ||
				(GetSymbol("InSmallBlind")		&& GetSymbol("betpositionrais") == 5 && CheckHand169Range(a_hand169[i], TB_SBvsCO))    ||
				(GetSymbol("InBigBlind")		&& GetSymbol("betpositionrais") == 5 && CheckHand169Range(a_hand169[i], TB_BBvsCO))    ||

				(GetSymbol("InSmallBlind")		&& GetSymbol("betpositionrais") == 6 && CheckHand169Range(a_hand169[i], TB_SBvsBU))    ||
				(GetSymbol("InBigBlind")		&& GetSymbol("betpositionrais") == 6 && CheckHand169Range(a_hand169[i], TB_BBvsBU))    ||

				(GetSymbol("InBigBlind")		&& GetSymbol("betpositionrais") == 1 && CheckHand169Range(a_hand169[i], TB_BBvsSB)))
			{
				if (!strcmp(a_hand169[i], hand169))
				{
					// ������� ������� ����
					colorRect[i] = 5;
					colorText1 = 0;
					strncpy(text1, "Raise", strlen("Raise"));
				}
				else colorRect[i] = 0;
			}
			
			// Cold - Call
			else if ((GetSymbol("InMiddlePosition3")	&& GetSymbol("betpositionrais") == 3 && CheckHand169Range(a_hand169[i], CC_MP3vsMP2)) ||
					(GetSymbol("InCutOff")				&& GetSymbol("betpositionrais") == 3 && CheckHand169Range(a_hand169[i], CC_COvsMP2))  ||
					(GetSymbol("InButton")				&& GetSymbol("betpositionrais") == 3 && CheckHand169Range(a_hand169[i], CC_BUvsMP2))  ||
					(GetSymbol("InSmallBlind")			&& GetSymbol("betpositionrais") == 3 && CheckHand169Range(a_hand169[i], CC_SBvsMP2))  ||
					(GetSymbol("InBigBlind")			&& GetSymbol("betpositionrais") == 3 && CheckHand169Range(a_hand169[i], CC_BBvsMP2))  ||

					(GetSymbol("InCutOff")				&& GetSymbol("betpositionrais") == 4 && CheckHand169Range(a_hand169[i], CC_COvsMP3)) ||
					(GetSymbol("InButton")				&& GetSymbol("betpositionrais") == 4 && CheckHand169Range(a_hand169[i], CC_BUvsMP3)) ||
					(GetSymbol("InSmallBlind")			&& GetSymbol("betpositionrais") == 4 && CheckHand169Range(a_hand169[i], CC_SBvsMP3)) ||
					(GetSymbol("InBigBlind")			&& GetSymbol("betpositionrais") == 4 && CheckHand169Range(a_hand169[i], CC_BBvsMP3)) ||

					(GetSymbol("InButton")				&& GetSymbol("betpositionrais") == 5 && CheckHand169Range(a_hand169[i], CC_BUvsCO)) ||
					(GetSymbol("InSmallBlind")			&& GetSymbol("betpositionrais") == 5 && CheckHand169Range(a_hand169[i], CC_SBvsCO)) ||
					(GetSymbol("InBigBlind")			&& GetSymbol("betpositionrais") == 5 && CheckHand169Range(a_hand169[i], CC_BBvsCO)) ||

					(GetSymbol("InSmallBlind")			&& GetSymbol("betpositionrais") == 6 && CheckHand169Range(a_hand169[i], CC_SBvsBU)) ||
					(GetSymbol("InBigBlind")			&& GetSymbol("betpositionrais") == 6 && CheckHand169Range(a_hand169[i], CC_BBvsBU)) ||

					(GetSymbol("InBigBlind")			&& GetSymbol("betpositionrais") == 1 && CheckHand169Range(a_hand169[i], CC_BBvsSB)))
			{
				if (!strcmp(a_hand169[i], hand169))
				{
					// ������� ������� ����
					colorRect[i] = 6;
					colorText1 = 1;
					strncpy(text1, "Call", strlen("Call"));
				}
				else colorRect[i] = 1;
			}

			// Fold
			else
			{
				if (!strcmp(a_hand169[i], hand169))
				{
					// ������� ������� ����
					colorRect[i] = 7;
					colorText1 = 2;
					strncpy(text1, "Fold", strlen("Fold"));
				}
				else colorRect[i] = 2;
			}
		}
	}

	// ����������� ��������, ������� �����
	else
	{
		cls = true;
	}
	
	// �������������� ����
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);

	// ����������� �������
	mutex.unlock();
}
