#pragma once

// ��������� �������
bool cls = true;					// ��������� ������� ������
int colorRect[169];					// ������ ������ ��������� � ������� ���������
char text1[100] = "";				// ��������
char text2[100] = "";				// �������
char text3[100] = "" ;				// ����
std::mutex mutex;					// �������
HWND hWnd;							// ����

// ����� ��� ����������


void Window(HANDLE hModule);
DWORD WINAPI WindowThread(LPVOID param);
LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitHand169(char* hand169[]);
void Paint(HDC hdc);
