#pragma once

// Round
#define PREFLOP	1
#define FLOP	2
#define TERN	3
#define RIVER	4

// � ������ ������ ����� �� ������� ������ ���������� 0 (NULL).

class Scraper
{
public:
	Scraper();
	~Scraper();

	// �������� ������ (����� ������ �������� ������)
	int Screen();

	// ����������� ������ �� ���������
	int DeleteScreen();

	// ������� ��������� �������
	int Fold();

	// ����� ��������
	int Round();

	// ����� �������
	unsigned __int64 Play();

	// �����
	int Card(int nCard, char* chCard);

	// ����� ����������
	int Player();

	// ������ ����� � ��������
	double Pot();

	// ������ ���� � ��������
	double Call();

	// ���������� ����� ����� ��� �������������
	int SetTable(int table);

private:

	IplImage*			screen;
	char				chCall[10];
	unsigned __int64	numberPlay;
	int					numActiv;
	char				chCard[5];
	int					iCard;
	int					table;

	// ������ � ������� �� �������� ��������
	IplImage* Xroi(IplImage* img);				// ��������� ������� �����
	IplImage* Wroi(IplImage* img);				// ��������� ������� ������
	IplImage* Yroi(IplImage* img);				// ��������� ������� ������
	IplImage* Hroi(IplImage* img);				// ��������� ������� �����
	unsigned __int64 HashBin(IplImage* img);	// ��������� ��� �������� 8 � 8
	bitset<130> BitHashBin(IplImage* img);		// ��������� ��� �������� � 130 ��������
};

