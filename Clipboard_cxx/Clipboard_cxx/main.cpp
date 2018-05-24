#define _CRT_SECURE_NO_WARNINGS

#define WINDOWS
#define FAIL_REPORT(var, func) \
if (!var)\
std::cout << "failed" << #func << std::endl;



#include <iostream>
#include <Windows.h>
//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR     lpCmdLine,int       nCmdShow)


void clipboard_save()
{
	BOOL av = IsClipboardFormatAvailable(CF_TEXT);
	if (av)
		std::cout << "available!" << std::endl;
	else
		std::cout << "not available!" << std::endl;

	std::string text = "my test string";

	HANDLE hGlobal = GlobalAlloc(GHND | GMEM_SHARE, text.length() + 1);
	BOOL result = FALSE;
	const char* str = text.c_str();
	char* pGlobal = (char*)GlobalLock(hGlobal);
	for (int i = 0; i < text.length(); i++)
		*pGlobal++ = *str++;
	GlobalUnlock(hGlobal);
	result = OpenClipboard(NULL);
	FAIL_REPORT(result, OpenClipboard);
	result = EmptyClipboard();
	FAIL_REPORT(result, EmptyClipboard);
	SetClipboardData(CF_TEXT, hGlobal);
	CloseClipboard();


	av = IsClipboardFormatAvailable(CF_TEXT);
	if (av)
		std::cout << "available!" << std::endl;


}
void clipboard_load()
{
	OpenClipboard(NULL);//�����̼� ���ÿ� Ŭ������ ���� �ʵ��� �ϰ� ���ε�.
	HANDLE hGlobal = GetClipboardData(CF_TEXT);
	if (hGlobal)
		std::cout << "it has CF_TEXT data! " << std::endl;//���⼭ ���� �ڵ��� �� ���α׷��� ���ϴ� �ڵ��� �ƴϰ� Ŭ�����忡 ���ϴ� �ڵ���. 
	char* pText = new char[GlobalSize(hGlobal)];
	char* pGlobal = (char*)GlobalLock(hGlobal);
	strcpy(pText, pGlobal);
	std::cout << pText << std::endl;
	GlobalUnlock(hGlobal);
	CloseClipboard();
	delete[] pText;

}


#ifndef WINDOWS



int main()
{
	while (true)
	{
		int cmd;
		std::cout << "select what you want" << std::endl;
		std::cout << "1. save_clipboard" << std::endl;
		std::cout << "2. load_clipboard" << std::endl;
		std::cin >> cmd;
		switch (cmd)
		{
		case 1:
			clipboard_save();
			break;
		case 2:
			clipboard_load();
			break;
		}
	}

	
	system("pause");

}
#endif