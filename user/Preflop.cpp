#include "OpenHoldemFunctions.h"
#include "Preflop.h"

// ���� �����
void Rank(int rank, char* cRank)
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
void Hand(char* hand)
{
	Rank(GetSymbol("$$pr0"), hand);
	Suit(GetSymbol("$$ps0"), hand + 1);
	Rank(GetSymbol("$$pr1"), hand + 2);
	Suit(GetSymbol("$$ps1"), hand + 3);
}

void Hand169(char* hand, char* hand169)
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

// ��������� ���� �� ��������
void Preflop()
{
	// ���� ����
	char hand[10] = "";
	char hand169[10] = "";
	Hand(hand);
	Hand169(hand, hand169);
}

/*

bool ChekHand(char* hand169, char* range)
{

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

	char* arr_hand169[169];
	InitHand169(arr_hand169);

	// ����� �� �������� ����������� �� �����������
	if (!GetSymbol("InBigBlind") && GetSymbol("Raises") == 0 && RightCalls() == 0)
	{
		for (int i = 0; i < 169; i++)
		{
			// ���� ���� � �������� ��� ������ � �������� �������� ������ � ���� �������
			if ((GetSymbol("InMiddlePosition2") && ChekHand(arr_hand169[i], or_mp2)) ||
				(GetSymbol("InMiddlePosition3") && ChekHand(arr_hand169[i], or_mp3)) ||
				(GetSymbol("InCutOff")			&& ChekHand(arr_hand169[i], or_co))  ||
				(GetSymbol("InButton")			&& ChekHand(arr_hand169[i], or_bu))  ||
				(GetSymbol("InSmallBlind ")		&& ChekHand(arr_hand169[i], or_sb)))
			{
				colorRect[i] = 2; // ������� �������
				if (strcmp(arr_hand169[i], hand169))
				{
					// ������� �����
					colorFrame[i] = 2;
				}
			}
			else
			{
				colorRect[i] = 1;	// ������ �������
				if (strcmp(arr_hand169[i], hand169))
				{
					// ����� �����
					colorFrame[i] = 1;
				}
			}
		}
	}

	*/

	// ����������� �������
	// mutex.unlock();		

/*

// ���������� ��� �������� ��������� ������
char paintRange[] = "22+, A2s+, K2s+, Q2s+, J2s+, T2s+, 92s+, 82s+, 72s+, 62s+, 52s+, 42s+, 32s, A2o+, K2o+, Q2o+, J2o+, T2o+, 92o+, 82o+, 72o+, 62o+, 52o+, 42o+, 32o";
char myHand[10] = "";
char myVarHand[10] = "";
bool handInRange = false;
char* actions[3] = { "Raise", "Call", "Fold" };

//
// ��������� �������� ���������� �� ������� ��� �� ����� ������
// 2 - �������, ���� ��� (�.�. �������)
// 1 - �������, �� ������ ���� (����� ������, ����)
// 0 - �� �������, ������
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

// ���������� ���� �����
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
// ���� ����������� ����
if (podDiap[0] == podDiap[1])
{
// ���� � ������������ ���� ����
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
// ����������� ����������

}
else if (ch_comb[2] == 'o')
{
// ������������ ����������

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
// ���� ������ � ��������
return TRUE;
}
else
{
// �������� ��������� ��� ���������� ������ ������������
pRange += ReadSubrange(pRange, subrange);
}
}
return FALSE;
}
*/