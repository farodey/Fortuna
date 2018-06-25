#include "stdafx.h"
#include "Scraper.h"

// Конструктор
Scraper::Scraper()
{
	table = 1;
}

// Деструктор
Scraper::~Scraper()
{

}

// Скриншот в формате OPENCV
int Scraper::Screen()
{
	// Отладка
	// screen = cvLoadImage("2.png");
	// return 1;
	
	// Создаем контекст устройства и совместимый контекст
	HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
	HDC hdcCompatible = CreateCompatibleDC(hdcScreen);

	// Определяем разрешение устройства
	int iXRes = GetDeviceCaps(hdcScreen, HORZRES);
	int iYRes = GetDeviceCaps(hdcScreen, VERTRES);

	// Создаем точечный рисуник, совместимый с контекстом устройства
	HBITMAP hScreen = CreateCompatibleBitmap(hdcScreen, iXRes, iYRes);
	if (hScreen == 0)
	{
		// Ошибка
		// Освобождаем ресурсы
		DeleteDC(hdcCompatible);
		DeleteDC(hdcScreen);
		return 0;
	}

	// Подготавливаем рисунок в формате OPENCV
	IplImage* image = cvCreateImage(cvSize(iXRes, iYRes), IPL_DEPTH_8U, 3);
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = iXRes;
	bitmapInfo.bmiHeader.biHeight = iYRes;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 24;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	// Выбираем картинку в совместимый контекст
	if (!SelectObject(hdcCompatible, hScreen))
	{
		// Ошибка
		// Освобождаем ресурсы
		cvReleaseImage(&image);
		DeleteObject(hScreen);
		DeleteDC(hdcCompatible);
		DeleteDC(hdcScreen);
		return 0;
	}

	// Копируем изображение
	if (!BitBlt(hdcCompatible, 0, 0, GetDeviceCaps(hdcCompatible, HORZRES), GetDeviceCaps(hdcCompatible, VERTRES), hdcScreen, 0, 0, SRCCOPY))
	{
		// Ошибка
		// Освобождаем ресурсы
		cvReleaseImage(&image);
		DeleteObject(hScreen);
		DeleteDC(hdcCompatible);
		DeleteDC(hdcScreen);
		return 0;
	}

	// Копируем изображение в буфер как DIB (аппаратно-независимый растр)
	if (!GetDIBits(hdcCompatible, hScreen, 0, iYRes, image->imageData, &bitmapInfo, DIB_RGB_COLORS))
	{
		// Ошибка
		// Освобождаем ресурсы
		cvReleaseImage(&image);
		DeleteObject(hScreen);
		DeleteDC(hdcCompatible);
		DeleteDC(hdcScreen);
		return 0;
	}

	// Переворачиваем
	cvMirror(image);

	// освобождаем ресурсы
	DeleteObject(hScreen);
	DeleteDC(hdcCompatible);
	DeleteDC(hdcScreen);

	screen = image;
	return 1;
}

// Освобождает паять от скриншота
int Scraper::DeleteScreen()
{
	cvReleaseImage(&screen);
	return 1;
}

int Scraper::Fold()
{
	// Устанавливаем координаты проверяемой точки в зависимости от стола
	int x = 778;
	int y = 564;

	if (table == 2) x += 800;

	// Проверяем цвет
	uchar* pPix = (uchar*)(screen->imageData + (screen->widthStep) * y + x * 3);
	if (pPix[0] == 5 && pPix[1] == 1 && pPix[2] == 30)
	{
		// Да, есть кнопка
		return 1;
	}
	else
	{
		// Нету кнопки
		return 0;
	}
}

int Scraper::Round()
{
	int y = 212;
	int xRiver = 534;
	int xTern = 480;
	int xFlop = 318;
	
	if (table == 2)
	{
		xRiver += 800;
		xTern += 800;
		xFlop += 800;
	}

	// Ривер
	uchar* pPix = (uchar*)(screen->imageData + screen->widthStep * y + xRiver * 3);
	if (pPix[0] == 255 && pPix[1] == 255 && pPix[2] == 255)
	{
		return RIVER;
	}

	// Терн
	pPix = (uchar*)(screen->imageData + screen->widthStep * y + xTern * 3);
	if (pPix[0] == 255 && pPix[1] == 255 && pPix[2] == 255)
	{
		return TERN;
	}

	// Флоп, префлоп
	pPix = (uchar*)(screen->imageData + screen->widthStep * y + xFlop * 3);
	if (pPix[0] == 255 && pPix[1] == 255 && pPix[2] == 255)
	{
		return FLOP;
	}
	else if (pPix[0] == 160 && pPix[1] == 77 && pPix[2] == 13)
	{
		return PREFLOP;
	}
	else
	{
		// Ошибка
		return 0;
	}
}

// Определяет номер раздачи
unsigned __int64 Scraper::Play()
{
	// Устанавливаем ROI
	int xNumberPlay = 64;
	int yNumberPlay = 57;
	int wNumberPlay = 75;
	int hNumberPlay = 10;

	// Второй стол
	if (table == 2) xNumberPlay += 800;

	CvRect rectNumberPlay = cvRect(xNumberPlay, yNumberPlay, wNumberPlay, hNumberPlay);
	cvSetImageROI(screen, rectNumberPlay);

	// Переводим выделенную область в градации серого
	IplImage* image_ser = cvCreateImage(cvSize(wNumberPlay, hNumberPlay), IPL_DEPTH_8U, 1);
	cvCvtColor(screen, image_ser, CV_BGR2GRAY);

	// Построим бинарное изображение (91 - пороговая величина)
	IplImage* image_bin = cvCreateImage(cvSize(wNumberPlay, hNumberPlay), IPL_DEPTH_8U, 1);
	cvThreshold(image_ser, image_bin, 91, 255, CV_THRESH_BINARY);

	unsigned __int64 rNumberPlay = 0;
	unsigned __int64 hash_mas_comb[] = { 0x000000746318c62e, 0x0000000000924974, 0xf88888422e, 0x000000746106422e, 0x000000423e952988, 0x000000746107845e, 0x000000746317862e, 0x000000108442211f, 0x746317462e, 0x7461e8c62e };

	while (TRUE)
	{
		// Ищем X и отсекаем область слева
		IplImage* x_bin = Xroi(image_bin);
		if (x_bin == 0)
		{
			// Ошибка - выходим из цикла
			break;
		}
		cvSaveImage("x_bin.bmp", x_bin);
		// Ищем ширину символа и отсекаем область справа
		IplImage* w_bin = Wroi(x_bin);
		if (w_bin == 0)
		{
			// Ошибка - выходим из цикла
			cvReleaseImage(&x_bin);
			break;
		}

		// Ищем Y и отсекаем область сверху
		IplImage* y_bin = Yroi(w_bin);

		// Ищем высоту символа и отсекаем область снизу
		IplImage* h_bin = Hroi(y_bin);

		// Вычисляем хеш символа и ищем его в массиве хешей
		unsigned __int64 hash_img = HashBin(h_bin);
		int i_hash;
		for (i_hash = 0; i_hash < 10; i_hash++)
		{
			if (hash_img == hash_mas_comb[i_hash])
			{
				// Хеш найден, идем дальше
				break;
			}
		}

		if (i_hash != 10)
		{
			// Добавть к возвращаемому значению
			rNumberPlay = rNumberPlay * 10 + i_hash;

			// Заменить стартовую картинку символа
			cvReleaseImage(&image_bin);
			image_bin = cvCreateImage(cvSize(x_bin->width - h_bin->width, x_bin->height), 8, 1);
			cvSetImageROI(x_bin, cvRect(h_bin->width, 0, x_bin->width - h_bin->width, x_bin->height));
			cvCopy(x_bin, image_bin);

			// Освободить ресурсы
			cvReleaseImage(&x_bin);
			cvReleaseImage(&w_bin);
			cvReleaseImage(&y_bin);
			cvReleaseImage(&h_bin);

			// Ищем новый символ
			continue;
		}
		else
		{
			// Ошибка, освободить ресурсы, сбросить ROI
			cvReleaseImage(&x_bin);
			cvReleaseImage(&w_bin);
			cvReleaseImage(&y_bin);
			cvReleaseImage(&h_bin);
			cvReleaseImage(&image_bin);
			cvReleaseImage(&image_ser);
			cvResetImageROI(screen);
			return 0;
		}
	}

	// Освобождаем ресурсы, сбрасываем ROI
	cvReleaseImage(&image_bin);
	cvReleaseImage(&image_ser);
	cvResetImageROI(screen);

	return rNumberPlay;
}

// Определяет карту
int Scraper::Card(int nCard, char* chCard)
{
	int xMast;
	int yMast;
	int xCard;
	int yCard;
	int wCard = 11;
	int hCard = 15;

	if (nCard == 1)
	{
		xMast = 372;
		yMast = 405;
		xCard = 367;
		yCard = 382;
	}
	else if (nCard == 2)
	{
		xMast = 387;
		yMast = 409;
		xCard = 382;
		yCard = 386;
	}
	else if (nCard == 3)
	{
		xMast = 280;
		yMast = 236;
		xCard = 275;
		yCard = 213;
	}
	else if (nCard == 4)
	{
		xMast = 334;
		yMast = 236;
		xCard = 329;
		yCard = 213;
	}
	else if (nCard == 5)
	{
		xMast = 388;
		yMast = 236;
		xCard = 383;
		yCard = 213;
	}
	else if (nCard == 6)
	{
		xMast = 442;
		yMast = 236;
		xCard = 437;
		yCard = 213;
	}
	else if (nCard == 7)
	{
		xMast = 496;
		yMast = 236;
		xCard = 491;
		yCard = 213;
	}
	else
	{
		// Ошибка
		return FALSE;
	}

	// Второй стол
	if (table == 2)
	{
		xCard += 800;
		xMast += 800;
	}

	// Устанавливаем ROI
	CvRect rectCard = cvRect(xCard, yCard, wCard, hCard);
	cvSetImageROI(screen, rectCard);
	cvSaveImage("raw.bmp", screen);

	// Уменьшаем картинку
	IplImage* img_res = cvCreateImage(cvSize(8, 8), 8, 3);
	cvResize(screen, img_res);

	// Переводим выделенную область в градации серого
	IplImage* image_ser = cvCreateImage(cvSize(8, 8), IPL_DEPTH_8U, 1);
	cvCvtColor(img_res, image_ser, CV_BGR2GRAY);

	// Построим бинарное изображение (254 - пороговая величина)
	IplImage* image_bin = cvCreateImage(cvSize(8, 8), IPL_DEPTH_8U, 1);
	cvThreshold(image_ser, image_bin, 254, 255, CV_THRESH_BINARY);
	cvSaveImage("image_bin.bmp", image_bin);

	// Вычисляем хеш карты и определяем карту
	int retCard = 0;
	char pCard[5] = "";
	unsigned __int64 hash_img = HashBin(image_bin);
	if (hash_img == 0x0000c1870f1800c3 || hash_img == 0x0000c1871f1800c3)
	{
		// Двойка
		pCard[0] = 50;
		retCard = 13;
	}
	else if (hash_img == 0xc3001807878981e7)
	{
		// Тройка
		pCard[0] = 51;
		retCard = 12;
	}
	else if (hash_img == 0x07070000c0c1c7c7)
	{
		// Четверка
		pCard[0] = 52;
		retCard = 11;
	}
	else if (hash_img == 0xc300180080c08081 || hash_img == 0xc300181880c08181)
	{
		// Пятерка
		pCard[0] = 53;
		retCard = 10;
	}
	else if (hash_img == 0xc300180080c080c3 || hash_img == 0xc300181880c080c3)
	{
		// Шестерка
		pCard[0] = 54;
		retCard = 9;
	}
	else if (hash_img == 0xe7e7c7878f080000 || hash_img == 0xe7e7c7c78f080000)
	{
		// Семерка
		pCard[0] = 55;
		retCard = 8;
	}
	else if (hash_img == 0xc3001800818181e7)
	{
		// Восьмерка
		pCard[0] = 56;
		retCard = 7;
	}
	else if (hash_img == 0xc3010301081800c3 || hash_img == 0xc3010301181800c3)
	{
		// Девятка
		pCard[0] = 57;
		retCard = 6;
	}
	else if (hash_img == 0x8000000000000080 || hash_img == 0xe1e0e0e0e0e0e0e1)
	{
		// Десятка
		pCard[0] = 84;
		retCard = 5;
	}
	else if (hash_img == 0xe380889f9f9f0707)
	{
		// Валет
		pCard[0] = 74;
		retCard = 4;
	}
	else if (hash_img == 0x03000010181800c3)
	{
		// Дама
		pCard[0] = 81;
		retCard = 3;
	}
	else if (hash_img == 0x000089c1c1810000)
	{
		// Король
		pCard[0] = 75;
		retCard = 2;
	}
	else if (hash_img == 0x0000008181c3c3e7)
	{
		// Туз
		pCard[0] = 65;
		retCard = 1;
	}
	else
	{
		// Ошибка, освобождаем ресурсы, выходим
		cvReleaseImage(&img_res);
		cvReleaseImage(&image_ser);
		cvReleaseImage(&image_bin);
		return 0;
	}

	// Определяем масть по цвету одной точки
	uchar* pPix = (uchar*)(screen->imageData + screen->widthStep * yMast + xMast * 3);
	if (pPix[0] == 32 && pPix[1] == 124 && pPix[2] == 13)
	{
		// Зеленые крести
		pCard[1] = 99;
		retCard = retCard + 39;
	}
	else if (pPix[0] == 0 && pPix[1] == 0 && pPix[2] == 0)
	{
		// Черные пики
		pCard[1] = 115;
		retCard = retCard + 26;
	}
	else if (pPix[0] == 17 && pPix[1] == 17 && pPix[2] == 206)
	{
		// Красные черви
		pCard[1] = 104;
		retCard = retCard + 13;
	}
	else if (pPix[0] == 163 && pPix[1] == 25 && pPix[2] == 13)
	{
		// Синии буби
		pCard[1] = 100;
	}
	else
	{
		// Ошибка, освобождаем ресурсы, выходим
		cvReleaseImage(&img_res);
		cvReleaseImage(&image_ser);
		cvReleaseImage(&image_bin);
		return 0;
	}

	cvReleaseImage(&img_res);
	cvReleaseImage(&image_ser);
	cvReleaseImage(&image_bin);

	memcpy(chCard, pCard, 2);
	return retCard;
}

double Scraper::Pot()
{
	// Устанавливаем ROI
	int xPot = 409;
	int yPot = 75;
	int wPot = 33;
	int hPot = 12;

	// Второй стол
	if (table == 2) xPot += 800;

	CvRect rectPot = cvRect(xPot, yPot, wPot, hPot);
	cvSetImageROI(screen, rectPot);

	// Переводим выделенную область в градации серого
	IplImage* image_ser = cvCreateImage(cvSize(wPot, hPot), IPL_DEPTH_8U, 1);
	cvCvtColor(screen, image_ser, CV_BGR2GRAY);

	// Построим бинарное изображение (140 - пороговая величина)
	IplImage* image_bin = cvCreateImage(cvSize(wPot, hPot), IPL_DEPTH_8U, 1);
	cvThreshold(image_ser, image_bin, 140, 255, CV_THRESH_BINARY);

	unsigned __int64 hash_mas_comb[11] = { 0x37c78f1e3c78df1c, 0x0fcc30c30c30c3cc, 0x0fff18c630c30fde, 0xb0e0c1f1e60c1f9e, 0x307ffd9b26cf1c30, 0x03d8e1870f0417df, 0x77c78fbbf060ff3c, 0x061830c106183ffe, 0x67c7cdf1e6d8df1c, 0xb0c1b3fe3c78df9c, 0x000000000000000f };
	int nSimv = 0; // Номер считываемого символа
	char rPot[10] = "";

	while (TRUE)
	{
		// Ищем X и отсекаем область слева
		IplImage* x_bin = Xroi(image_bin);
		if (x_bin == 0)
		{
			// Ошибка - выходим из цикла
			break;
		}

		// Ищем ширину символа и отсекаем область справа
		IplImage* w_bin = Wroi(x_bin);
		if (w_bin == 0)
		{
			// Ошибка - выходим из цикла
			cvReleaseImage(&x_bin);
			break;
		}

		// Ищем Y и отсекаем область сверху
		IplImage* y_bin = Yroi(w_bin);

		// Ищем высоту символа и отсекаем область снизу
		IplImage* h_bin = Hroi(y_bin);

		// Вычисляем хеш символа и ищем его в массиве хешей
		unsigned __int64 hash_img = HashBin(h_bin);
		int i_hash;
		for (i_hash = 0; i_hash < 11; i_hash++)
		{
			if (hash_img == hash_mas_comb[i_hash])
			{
				// Хеш найден, идем дальше
				break;
			}
		}

		// Если десятичная точка
		if (i_hash == 10)
		{
			// Добавить точку в строку
			rPot[nSimv] = 46;
			nSimv++;

			// Заменить стартовую картинку символа
			cvReleaseImage(&image_bin);
			image_bin = cvCreateImage(cvSize(x_bin->width - h_bin->width, x_bin->height), 8, 1);
			cvSetImageROI(x_bin, cvRect(h_bin->width, 0, x_bin->width - h_bin->width, x_bin->height));
			cvCopy(x_bin, image_bin);

			// Освободить ресурсы
			cvReleaseImage(&x_bin);
			cvReleaseImage(&w_bin);
			cvReleaseImage(&y_bin);
			cvReleaseImage(&h_bin);

			// Ищем новый символ
			continue;
		}
		else if (i_hash != 11) // Иначе если хеш найден
		{
			// Добавить в строку
			rPot[nSimv] = i_hash + 48;
			nSimv++;

			// Заменить стартовую картинку символа
			cvReleaseImage(&image_bin);
			image_bin = cvCreateImage(cvSize(x_bin->width - h_bin->width, x_bin->height), 8, 1);
			cvSetImageROI(x_bin, cvRect(h_bin->width, 0, x_bin->width - h_bin->width, x_bin->height));
			cvCopy(x_bin, image_bin);

			// Освободить ресурсы
			cvReleaseImage(&x_bin);
			cvReleaseImage(&w_bin);
			cvReleaseImage(&y_bin);
			cvReleaseImage(&h_bin);

			// Ищем новый символ
			continue;
		}
		else
		{
			// Ошибка, освободить ресурсы, сбросить ROI
			cvReleaseImage(&x_bin);
			cvReleaseImage(&w_bin);
			cvReleaseImage(&y_bin);
			cvReleaseImage(&h_bin);
			cvReleaseImage(&image_bin);
			cvReleaseImage(&image_ser);
			cvResetImageROI(screen);

			return 0;
		}
	}

	// Освобождаем ресурсы, сбросить ROI
	cvReleaseImage(&image_bin);
	cvReleaseImage(&image_ser);
	cvResetImageROI(screen);

	return atof(rPot);
}


int Scraper::Player()
{
	int x1 = 210;
	int x2 = 160;
	int x3 = 183;
	int x4 = 265;
	int x5 = 524;
	int x6 = 603;
	int x7 = 628;
	int x8 = 569;

	if (table == 2)
	{
		x1 += 800;
		x2 += 800;
		x3 += 800;
		x4 += 800;
		x5 += 800;
		x6 += 800;
		x7 += 800;
		x8 += 800;
	}

	int retPlayer = 0;

	uchar* pPix = (uchar*)(screen->imageData + screen->widthStep * 332 + x1 * 3);
	if (pPix[0] == 210 && pPix[1] == 210 && pPix[2] == 222)
	{
		retPlayer++;
	}

	pPix = (uchar*)(screen->imageData + screen->widthStep * 264 + x2 * 3);
	if (pPix[0] == 210 && pPix[1] == 210 && pPix[2] == 222)
	{
		retPlayer++;
	}

	pPix = (uchar*)(screen->imageData + screen->widthStep * 185 + x3 * 3);
	if (pPix[0] == 210 && pPix[1] == 210 && pPix[2] == 222)
	{
		retPlayer++;
	}

	pPix = (uchar*)(screen->imageData + screen->widthStep * 145 + x4 * 3);
	if (pPix[0] == 210 && pPix[1] == 210 && pPix[2] == 222)
	{
		retPlayer++;
	}

	pPix = (uchar*)(screen->imageData + screen->widthStep * 145 + x5 * 3);
	if (pPix[0] == 210 && pPix[1] == 210 && pPix[2] == 222)
	{
		retPlayer++;
	}

	pPix = (uchar*)(screen->imageData + screen->widthStep * 182 + x6 * 3);
	if (pPix[0] == 210 && pPix[1] == 210 && pPix[2] == 222)
	{
		retPlayer++;
	}

	pPix = (uchar*)(screen->imageData + screen->widthStep * 272 + x7 * 3);
	if (pPix[0] == 210 && pPix[1] == 210 && pPix[2] == 222)
	{
		retPlayer++;
	}

	pPix = (uchar*)(screen->imageData + screen->widthStep * 333 + x8 * 3);
	if (pPix[0] == 210 && pPix[1] == 210 && pPix[2] == 222)
	{
		retPlayer++;
	}

	return retPlayer;
}

// Возвращает размер кола в долларах
double Scraper::Call()
{
	// Иксы точек кола
	int x1 = 643;
	int x2 = 578;
	int x3 = 578;
	int x4 = 778;
	int x5 = 713;
	int x6 = 713;

	// Прямоугольник одной кнопки кола
	int xCall = 552;
	int yCall = 542;
	int wCall = 65;
	int hCall = 18;
 
	// Прямоугольник альтернативной кнопки кола
	int xCall2 = 687;
	int yCall2 = 542;
	int wCall2 = 65;
	int hCall2 = 18;

	// Второй стол
	if (table == 2)
	{
		x1 += 800;
		x2 += 800;
		x3 += 800;
		x4 += 800;
		x5 += 800;
		x6 += 800;

		xCall += 800;
		xCall2 += 800;
	}

	// Точки для проверки кнопки кола
	uchar* pPix1 = (uchar*)(screen->imageData + screen->widthStep * 515 + x1 * 3);
	uchar* pPix2 = (uchar*)(screen->imageData + screen->widthStep * 522 + x2 * 3);
	uchar* pPix3 = (uchar*)(screen->imageData + screen->widthStep * 533 + x3 * 3);

	// Альтернативная кнопка кола, которая находится на обычном месте кнопки рейза
	uchar* pPix4 = (uchar*)(screen->imageData + screen->widthStep * 515 + x4 * 3);
	uchar* pPix5 = (uchar*)(screen->imageData + screen->widthStep * 522 + x5 * 3);
	uchar* pPix6 = (uchar*)(screen->imageData + screen->widthStep * 533 + x6 * 3);

	// Проверяем наличие кнопки колла и устанавливаем прямоугольник
	if (pPix1[0] == 57 && pPix1[1] == 57 && pPix1[2] == 86 &&
		pPix2[0] == 255 && pPix2[1] == 255 && pPix2[2] == 255 &&
		pPix3[0] == 255 && pPix3[1] == 255 && pPix3[2] == 255)
	{
		//
		//
	}
	else if (pPix4[0] == 57 && pPix4[1] == 57 && pPix4[2] == 86 &&
		pPix5[0] == 255 && pPix5[1] == 255 && pPix5[2] == 255 &&
		pPix6[0] == 255 && pPix6[1] == 255 && pPix6[2] == 255)
	{
		xCall = xCall2;
		yCall = yCall2;
		wCall = wCall2;
		hCall = hCall2;
	}
	else
	{
		return 0;
	}

	// Инициализируем предварительный массив хешей
	unsigned __int64 call_mas_comb[12][3][3];

	// Сначала нулями
	for (int x = 0; x < 12; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int z = 0; z < 3; z++)
			{
				call_mas_comb[x][y][z] = 0;
			}
		}
	}

	// 0
	call_mas_comb[0][0][0] = 0x782318641b06c0b0;
	call_mas_comb[0][1][0] = 0x3c0f02c1986610f8;

	// 1
	call_mas_comb[1][0][0] = 0x0000000007f3060c;
	call_mas_comb[1][1][0] = 0x183060c183060c1e;

	// 2
	call_mas_comb[2][0][0] = 0x000000ffff03060c;
	call_mas_comb[2][1][0] = 0x183060e0c0c0e07f;

	// 3
	call_mas_comb[3][0][0] = 0x0000003f61c0c0c0;
	call_mas_comb[3][1][0] = 0xc0301c2040c0e07f;

	// 4
	call_mas_comb[4][0][0] = 0x8020080203ff20c8;
	call_mas_comb[4][1][0] = 0x22108824090380c0;

	// 5
	call_mas_comb[5][0][0] = 0x0000000001f61830;
	call_mas_comb[5][1][0] = 0x60c0c078102040ff;

	// 6
	call_mas_comb[6][0][0] = 0x7863186c1b06c1d8;
	call_mas_comb[6][1][0] = 0x73c80201806031f8;

	// 7
	call_mas_comb[7][0][0] = 0x000000030606040c;
	call_mas_comb[7][1][0] = 0x081810204040c0ff;

	// 8
	call_mas_comb[8][0][0] = 0x0003c63e0f068361;
	call_mas_comb[8][1][0] = 0x1d0784e61209847c;

	// 9
	call_mas_comb[9][0][0] = 0x0001f303018180ce;
	call_mas_comb[9][1][0] = 0x79b8783c1b0d867e;

	// .
	call_mas_comb[10][0][0] = 0;
	call_mas_comb[10][1][0] = 0x000000000000000f;

	// $
	call_mas_comb[11][0][0] = 0x101873c0c0c07030;
	call_mas_comb[11][1][0] = 0x1c161212167c1010;

	// Альтернативные хеши
	// 1
	call_mas_comb[1][0][1] = 0x0000000007f3060c;
	call_mas_comb[1][1][1] = 0x183060c183060c1c;

	// 3
	call_mas_comb[3][0][1] = 0x0000003f61c0c0c0;
	call_mas_comb[3][1][1] = 0xc0301c6040c0e07f;

	// 4
	call_mas_comb[4][0][1] = 0x8020080203ff20c8;
	call_mas_comb[4][1][1] = 0x22108826090380c0;

	// Инициализируем массив готовых хешей, до трех хешей на символ
	bitset<130> call_hash[12][3];
	for (int num_hash = 0; num_hash < 12; num_hash++)
	{
		for (int i = 0; i < 3; i++)
		{
			bitset<130> bit_temp(call_mas_comb[num_hash][0][i]);
			call_hash[num_hash][i] |= bit_temp;
			call_hash[num_hash][i] <<= 64;

			bitset<130> bit_temp1(call_mas_comb[num_hash][1][i]);
			call_hash[num_hash][i] |= bit_temp1;
		}
	}

	// Устанавливаем ROI
	CvRect rectPot = cvRect(xCall, yCall, wCall, hCall);
	cvSetImageROI(screen, rectPot);

	// Переводим выделенную область в градации серого
	IplImage* image_ser = cvCreateImage(cvSize(wCall, hCall), IPL_DEPTH_8U, 1);
	cvCvtColor(screen, image_ser, CV_BGR2GRAY);

	// Построим бинарное изображение (240 - пороговая величина)
	IplImage* image_bin = cvCreateImage(cvSize(wCall, hCall), IPL_DEPTH_8U, 1);
	cvThreshold(image_ser, image_bin, 240, 255, CV_THRESH_BINARY);

	char rCall[100] = "";
	int nSimv = 0;

	while (TRUE)
	{
		// Ищем X и отсекаем область слева
		IplImage* x_bin = Xroi(image_bin);
		if (x_bin == 0)
		{
			// Ошибка - выходим из цикла
			break;
		}

		// Ищем ширину символа и отсекаем область справа
		IplImage* w_bin = Wroi(x_bin);
		if (w_bin == 0)
		{
			// Ошибка - выходим из цикла
			cvReleaseImage(&x_bin);
			break;
		}

		// Ищем Y и отсекаем область сверху
		IplImage* y_bin = Yroi(w_bin);

		// Ищем высоту символа и отсекаем область снизу
		IplImage* h_bin = Hroi(y_bin);

		bitset<130> call_h = BitHashBin(h_bin);
		int i;
		for (i = 0; i < 12; i++)
		{
			if (call_hash[i][0] == call_h)
			{
				break;
			}
			if (call_hash[i][1] == call_h)
			{
				break;
			}
			if (call_hash[i][2] == call_h)
			{
				break;
			}
		}

		// Если десятичная точка
		if (i == 10)
		{
			// Добавить точку в строку
			rCall[nSimv] = 46;
			nSimv++;

			// Заменить стартовую картинку символа
			cvReleaseImage(&image_bin);
			image_bin = cvCreateImage(cvSize(x_bin->width - h_bin->width, x_bin->height), 8, 1);
			cvSetImageROI(x_bin, cvRect(h_bin->width, 0, x_bin->width - h_bin->width, x_bin->height));
			cvCopy(x_bin, image_bin);

			// Освободить ресурсы
			cvReleaseImage(&x_bin);
			cvReleaseImage(&w_bin);
			cvReleaseImage(&y_bin);
			cvReleaseImage(&h_bin);

			// Ищем новый символ
			continue;
		}
		else if (i == 11) // Если знак долара
		{
			// Заменить стартовую картинку символа
			cvReleaseImage(&image_bin);
			image_bin = cvCreateImage(cvSize(x_bin->width - h_bin->width, x_bin->height), 8, 1);
			cvSetImageROI(x_bin, cvRect(h_bin->width, 0, x_bin->width - h_bin->width, x_bin->height));
			cvCopy(x_bin, image_bin);

			// Освободить ресурсы
			cvReleaseImage(&x_bin);
			cvReleaseImage(&w_bin);
			cvReleaseImage(&y_bin);
			cvReleaseImage(&h_bin);

			// Ищем новый символ
			continue;
		}
		else if (i < 10) // Если цифра
		{
			// Добавить в строку
			rCall[nSimv] = i + 48;
			nSimv++;

			// Заменить стартовую картинку символа
			cvReleaseImage(&image_bin);
			image_bin = cvCreateImage(cvSize(x_bin->width - h_bin->width, x_bin->height), 8, 1);
			cvSetImageROI(x_bin, cvRect(h_bin->width, 0, x_bin->width - h_bin->width, x_bin->height));
			cvCopy(x_bin, image_bin);

			// Освободить ресурсы
			cvReleaseImage(&x_bin);
			cvReleaseImage(&w_bin);
			cvReleaseImage(&y_bin);
			cvReleaseImage(&h_bin);

			// Ищем новый символ
			continue;
		}
		else
		{
			// Ошибка, освободить ресурсы, сбросить ROI
			cvReleaseImage(&x_bin);
			cvReleaseImage(&w_bin);
			cvReleaseImage(&y_bin);
			cvReleaseImage(&h_bin);
			cvReleaseImage(&image_bin);
			cvReleaseImage(&image_ser);
			cvResetImageROI(screen);

			return 0;
		}
	}

	// Освобождаем ресурсы, сбросить ROI
	cvReleaseImage(&image_bin);
	cvReleaseImage(&image_ser);
	cvResetImageROI(screen);

	strcpy_s(chCall, rCall);
	return atof(rCall);
}

int Scraper::SetTable(int iTable)
{
	if (iTable == 1)
	{
		table = 1;
		return 1;
	}
	else if (iTable == 2)
	{
		table = 2;
		return 1;
	}
	else
	{
		return 0;
	}
}

// Отсечение области слева до начала первого символа
IplImage* Scraper::Xroi(IplImage* img)
{
	for (int x = 0; x < img->width; x++)
	{
		for (int y = 0; y < img->height; y++)
		{
			uchar* ptr = (uchar*)(img->imageData + y * img->widthStep + x);
			if (*ptr == 0xff)
			{
				IplImage*  xroi = cvCreateImage(cvSize(img->width - x, img->height), 8, 1);
				cvSetImageROI(img, cvRect(x, 0, img->width - x, img->height));
				cvCopy(img, xroi);
				return xroi;
			}
		}
	}

	// Ошибка, символ не найден
	return 0;
}

// Отсечение области справа
IplImage* Scraper::Wroi(IplImage* img)
{
	for (int x = 0; x < 12; x++)
	{
		int bel = 0;
		for (int y = 0; y < img->height; y++)
		{
			uchar* ptr = (uchar*)(img->imageData + y * img->widthStep + x);
			if (*ptr == 0xff)
			{
				bel = 1;
				break;
			}
		}
		if (bel == 0)
		{
			IplImage* wroi = cvCreateImage(cvSize(x, img->height), 8, 1);
			cvSetImageROI(img, cvRect(0, 0, x, img->height));
			cvCopy(img, wroi);
			return wroi;
		}
	}

	// Ошибка
	return 0;
}

// Отсечение области сверху
IplImage* Scraper::Yroi(IplImage* img)
{
	uchar* ptr = 0;
	for (int y = 0; y < img->height; y++)
	{
		for (int x = 0; x < img->width; x++)
		{
			ptr = (uchar*)(img->imageData + y * img->widthStep + x);
			if (*ptr == 0xff)
			{
				IplImage* yroi = cvCreateImage(cvSize(img->width, img->height - y), 8, 1);
				cvSetImageROI(img, cvRect(0, y, img->width, img->height - y));
				cvCopy(img, yroi);
				return yroi;
			}
		}
	}

	// Ошибка
	return 0;
}

// Отсечение пустой области снизу
IplImage* Scraper::Hroi(IplImage* img)
{
	uchar* ptr = 0;
	int y;
	for (y = 0; y < img->height; y++)
	{
		int bel = 0;
		for (int x = 0; x < img->width; x++)
		{
			ptr = (uchar*)(img->imageData + y * img->widthStep + x);
			if (*ptr == 0xff)
			{
				bel = 1;
				break;
			}
		}
		if (bel == 0)
		{
			break;
		}
	}

	IplImage* hroi = cvCreateImage(cvSize(img->width, y), 8, 1);
	cvSetImageROI(img, cvRect(0, 0, img->width, y));
	cvCopy(img, hroi);
	return hroi;
}

// Возращет хеш бинарной картинки
unsigned __int64 Scraper::HashBin(IplImage* img)
{
	unsigned __int64 hash = 0;
	int i = 0;

	for (int y = 0; y < img->height; y++)
	{
		for (int x = 0; x < img->width; x++)
		{
			if (i == 64)
			{
				return hash;
			}

			uchar* ptr = (uchar*)(img->imageData + img->widthStep * y + x);
			if (*ptr == 0xff)
			{
				hash |= 1i64 << i;
			}
			i++;
		}
	}

	return hash;
}

bitset<130> Scraper::BitHashBin(IplImage* img)
{
	bitset<130> hash;
	int i = 0;

	for (int y = 0; y < img->height; y++)
	{
		for (int x = 0; x < img->width; x++)
		{
			uchar* ptr = (uchar*)(img->imageData + img->widthStep * y + x);
			if (*ptr == 0xff)
			{
				hash[i] = 1;
			}
			i++;
		}
	}

	return hash;
}
