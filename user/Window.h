#pragma once

// ��������� �������
bool cls;				// �������
int color[169];			// �������� (������ ������ ��� ������� ���������)
char text1[100] = "";	// ��������
char text2[100] = "";	// �������
char text3[100] = "";	// ����
std::mutex mutex;

void Window(HANDLE hModule);
DWORD WINAPI WindowThread(LPVOID param);
LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitCharComb(char* char_comb[]);
void Paint(HDC hdc);
