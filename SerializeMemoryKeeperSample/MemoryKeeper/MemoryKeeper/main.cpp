#include <Windows.h>
#include <tchar.h>
#include "db.h"
#include <iostream>
#include <tlhelp32.h>
#include "atlstr.h"
#include <string>

#define MEMSIZE (10000 * 4 + 1) * 4
CString name = "paintSample.exe";
int* db;
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
		// 실행중인 프로세스 비교
		if(name.Compare(processEntry32.szExeFile) == 0)
		{
			// 실행중인 프로세스가 있다면
			return true;
		}
	}
	// 실행중인 프로세스 중에 찾는 프로세스가 없다면
	return false;


}

void storeDB()
{
	FILE *fp;
	fopen_s(&fp, "../../database", "w");
	fwrite(db, 1, MEMSIZE, fp);
	fclose(fp);
}

int main()
{
	HANDLE handle;
	handle = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, _T("TestSharedMemory"));
	void* startMem;
	startMem = (void *)::MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, MEMSIZE);
	if(!startMem) return 1;
	db = (int*)startMem;

	std::cout << "process가 실행중입니다." << std::endl;
	while(IsExistProcess())
	{
		Sleep(1000);
	}
	storeDB();
	std::cout << "process가 종료되어 데이터를 저장하였습니다." << std::endl;
}

