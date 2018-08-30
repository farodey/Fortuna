#pragma once

// Состояние рендера
bool cls = true;					// Индикатор очистки экрана
int colorRect[169];					// Массив цветов квадратов в матрице диапазона
bool frame[169];					// Массив индикаторов наличия рамки у каждой клетки
int colorFrame[169];				// Массив цветов рамок
char text1[100] = "";				// Действие
char text2[100] = "";				// Позиция
char text3[100] = "" ;				// Рука
std::mutex mutex;					// Мьютекс
HWND hWnd;							// Окно

void Window(HANDLE hModule);
DWORD WINAPI WindowThread(LPVOID param);
LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitHand169(char* hand169[]);
void Paint(HDC hdc);
