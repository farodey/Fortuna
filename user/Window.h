#pragma once

// Состояние рендера
bool cls = true;					// Индикатор очистки экрана
int colorRect[169];					// Массив цветов квадратов в матрице диапазона
char text1[100] = "";				// Действие
char text2[100] = "";				// Позиция
char text3[100] = "" ;				// Рука
std::mutex mutex;					// Мьютекс
HWND hWnd;							// Окно

// Место для переменных


void Window(HANDLE hModule);
DWORD WINAPI WindowThread(LPVOID param);
LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitHand169(char* hand169[]);
void Paint(HDC hdc);
