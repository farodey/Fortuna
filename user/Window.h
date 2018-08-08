#pragma once

// Состояние рендера
bool cls;				// Очистка
int color[169];			// Диапазон (массив цветов для матрицы диапазона)
char text1[100] = "";	// Действие
char text2[100] = "";	// Позиция
char text3[100] = "";	// Рука
std::mutex mutex;

void Window(HANDLE hModule);
DWORD WINAPI WindowThread(LPVOID param);
LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitCharComb(char* char_comb[]);
void Paint(HDC hdc);
