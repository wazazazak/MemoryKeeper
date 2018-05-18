#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <tlhelp32.h>
#include "atlstr.h"
#include <string>

#define MEMSIZE 1024 * 10
CString name = "paintSample.exe";
int* points;
bool IsExistProcess()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processEntry32;

	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		exit(EXIT_FAILURE);
	}

	processEntry32.dwSize = sizeof(PROCESSENTRY32);

	if( !Process32First(hProcessSnap, &processEntry32) )
	{
		CloseHandle(hProcessSnap);
		exit(EXIT_FAILURE); 
	}

	while(Process32Next(hProcessSnap, &processEntry32))
	{
		// �������� ���μ��� ��
		if(name.Compare(processEntry32.szExeFile) == 0)
		{
			// �������� ���μ����� �ִٸ�
			return true;
		}
	}
	// �������� ���μ��� �߿� ã�� ���μ����� ���ٸ�
	return false;


}

void storeDB()
{
	FILE *fp;
	fopen_s(&fp, "../../database", "w");
	fwrite(points, sizeof(int), MEMSIZE / sizeof(int), fp);
	fclose(fp);
}

int main()
{
	HANDLE handle;
	handle = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, _T("TestSharedMemory"));
	void* startMem;
	startMem = (void *)::MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, MEMSIZE);
	if(!startMem) return 1;
	points = (int*)startMem;
	
// 	for(unsigned i(0); i < points[0]; ++i)
// 	{
// 		std::cout << "(" << points[1 + i * 4 + 0] << ",";
// 		std::cout << points[1 + i * 4 + 1] << ") (";
// 		std::cout << points[1 + i * 4 + 2] << ",";
// 		std::cout << points[1 + i * 4 + 3] << ")\n";
// 	}
	std::cout << "process�� �������Դϴ�." << std::endl;
	while(IsExistProcess())
	{
		Sleep(1000);
	}
	storeDB();
	std::cout << "process�� ����Ǿ� �����͸� �����Ͽ����ϴ�." << std::endl;
}

