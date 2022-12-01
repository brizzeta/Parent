#include "ReadFileDlg.h"

ReadFileDlg* ReadFileDlg::ptr = NULL;

ReadFileDlg::ReadFileDlg(void)
{
	ptr = this;
}

ReadFileDlg::~ReadFileDlg(void)
{
}

void ReadFileDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

DWORD WINAPI Read_Thread(LPVOID lp)
{
	ReadFileDlg* ptr = (ReadFileDlg*)lp;
	char buf[4096];

	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{CF6C3C04-FCF2-489E-82D8-8B96CBD466CD}"));

	DWORD dwAnswer = WaitForSingleObject(hMutex, INFINITE);
	if (dwAnswer == WAIT_OBJECT_0)
	{
		ifstream in(TEXT("text.txt"), ios::in);
		if (!in)
		{
			MessageBox(ptr->hDialog, TEXT("Ошибка открытия файла!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
			return 1;
		}

		ofstream out("copytext.txt", ios::trunc);

		while (!in.eof())
		{
			in.read(buf, 4096);
			int n = in.gcount();
			out.write(buf, n);
		}

		out.close();
		in.close();
		ReleaseMutex(hMutex);

		MessageBox(ptr->hDialog, TEXT("Чтение данных из файла text.txt завершено!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
	}
	return 0;
}

BOOL ReadFileDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	CreateThread(NULL, 0, Read_Thread, this, 0, NULL);
	return TRUE;
}

BOOL CALLBACK ReadFileDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
	}
	return FALSE;
}