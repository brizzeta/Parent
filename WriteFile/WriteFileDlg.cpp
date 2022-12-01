#include "WriteFileDlg.h"

WriteFileDlg* WriteFileDlg::ptr = NULL;

WriteFileDlg::WriteFileDlg(void)
{
	ptr = this;
}

WriteFileDlg::~WriteFileDlg(void)
{
}

void WriteFileDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

DWORD WINAPI Write_Thread(LPVOID lp)
{
	WriteFileDlg* ptr = (WriteFileDlg*)lp;
	char buf[4096];
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{CF6C3C04-FCF2-489E-82D8-8B96CBD466CD}"));
	DWORD dwAnswer = WaitForSingleObject(hMutex, INFINITE);

	if (dwAnswer == WAIT_OBJECT_0)
	{
		ofstream out("text.txt", ios::trunc);

		if (!out.is_open())
		{
			MessageBox(ptr->hDialog, TEXT("Ошибка открытия text.txt!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
			return 1;
		}

		char a[100];
		for (int i = 0; i < 100; i++)
		{
			a[i] = rand() % 20;
			out << a[i] << ' ';
		}

		out.close();
		ReleaseMutex(hMutex);
		MessageBox(ptr->hDialog, TEXT("Запись данных в text.txt завершена!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
	}
	return 0;
}

BOOL WriteFileDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	CreateThread(NULL, 0, Write_Thread, this, 0, NULL);
	return TRUE;
}


BOOL CALLBACK WriteFileDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
	}
	return FALSE;
}