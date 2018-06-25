#pragma once

// Round
#define PREFLOP	1
#define FLOP	2
#define TERN	3
#define RIVER	4

// В случае ошибки любой из методов класса возвращает 0 (NULL).

class Scraper
{
public:
	Scraper();
	~Scraper();

	// Скриншот экрана (новый снимок заменяет старый)
	int Screen();

	// Освобождает память от скриншота
	int DeleteScreen();

	// Очередь принимать решение
	int Fold();

	// Раунд торговли
	int Round();

	// Номер раздачи
	unsigned __int64 Play();

	// Карта
	int Card(int nCard, char* chCard);

	// Число оппонентов
	int Player();

	// Размер банка в долларах
	double Pot();

	// Размер кола в долларах
	double Call();

	// Установить номер стола для распознавания
	int SetTable(int table);

private:

	IplImage*			screen;
	char				chCall[10];
	unsigned __int64	numberPlay;
	int					numActiv;
	char				chCard[5];
	int					iCard;
	int					table;

	// Работа с текстом на бинарной картинке
	IplImage* Xroi(IplImage* img);				// Отсечение области слева
	IplImage* Wroi(IplImage* img);				// Отсечение области справа
	IplImage* Yroi(IplImage* img);				// Отсечение области сверху
	IplImage* Hroi(IplImage* img);				// Отсечение области снизу
	unsigned __int64 HashBin(IplImage* img);	// Вычисляет хеш картинки 8 х 8
	bitset<130> BitHashBin(IplImage* img);		// Вычисляет хеш картинки в 130 пикселей
};

