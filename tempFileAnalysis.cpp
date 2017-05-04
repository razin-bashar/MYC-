#include <iostream>
#include <Windows.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <io.h>
#include <atlstr.h> 
#include <fcntl.h>
#include "Logger.h"
using namespace std;

#pragma comment(lib, "winmm.lib")
#pragma warning(disable:4996)
#define BUFSIZE 1024



HANDLE hFile = INVALID_HANDLE_VALUE;
HANDLE hTempFile = INVALID_HANDLE_VALUE;

BOOL GetTemporaryFilePath(CString strExtension, CString& strFileName)
{
	TCHAR lpszTempPath[MAX_PATH] = { 0 };
	if (!GetTempPath(MAX_PATH, lpszTempPath))
		return FALSE;

	TCHAR lpszFilePath[MAX_PATH] = { 0 };
	do {
		if (!GetTempFileName(lpszTempPath, TEXT("TEMPAUD"), 0, lpszFilePath))
			return FALSE;

		strFileName = lpszFilePath;
		::DeleteFile(strFileName);
		strFileName.Replace(_T(".tmp"), strExtension);
	} while (_taccess(strFileName, 00) != -1);

	OutputDebugString(_T("GetTemporaryFilePath = '") + strFileName + _T("'\n"));
	return TRUE;
}
int tempfileAnalysis(HANDLE &FileHandle, char* &TempFileName) {


	FILE *fp;
	hFile = CreateFile("C:\\Users\\Acer\\Desktop\\record_374.wav",               // file name 
		GENERIC_READ,          // open for reading 
		0,                     // do not share 
		NULL,                  // default security 
		OPEN_EXISTING,         // existing file only 
		FILE_ATTRIBUTE_NORMAL, // normal file 
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("First CreateFile failed");
		return 1;
	}

	DWORD dwRetVal = 0;
	CString szTempFileName;
	TCHAR lpTempPathBuffer[MAX_PATH];

	BOOL iResult = GetTemporaryFilePath(".wav", szTempFileName);

	if (!iResult)printf("not Done");

	TempFileName=szTempFileName.GetBuffer();
	
	hTempFile = CreateFile((LPTSTR)TempFileName, // file name 
		GENERIC_READ | GENERIC_WRITE,        // open for write 
		FILE_SHARE_READ| FILE_SHARE_DELETE|FILE_SHARE_WRITE,                    // do not share 
		NULL,                 // default security 
		CREATE_ALWAYS,        // overwrite existing
		FILE_FLAG_DELETE_ON_CLOSE,// normal file 
		NULL);                // no template 


	FileHandle = hTempFile;
	if (hTempFile == INVALID_HANDLE_VALUE)
	{
		printf("Second CreateFile failed");
		if (!CloseHandle(hFile))
		{
			printf("CloseHandle(hFile) failed");
			return (7);
		}
		return (4);
	}


	char  chBuffer[BUFSIZE];
	DWORD dwBytesRead = 0;
	DWORD dwBytesWritten = 0;
	BOOL fSuccess = FALSE;
	int breakpointactivator = 0;

	do
	{
		if (ReadFile(hFile, chBuffer, BUFSIZE, &dwBytesRead, NULL))
		{
			fSuccess = WriteFile(hTempFile,
				chBuffer,
				dwBytesRead,
				&dwBytesWritten,
				NULL);
			if (!fSuccess)
			{
				printf("WriteFile failed");
				return (5);
			}
		}
		else
		{
			printf("ReadFile failed");
			return (6);
		}
		//  Continues until the whole file is processed.
	} while (dwBytesRead == BUFSIZE);

	if (!CloseHandle(hFile))
	{
		printf("CloseHandle(hFile) failed");
		return (7);
	}
	FlushFileBuffers(hTempFile);


}
void fileRead(char* TempFileName) {
	FILE *fp,*fp1;
	const int buffersize = 1024;
	byte buffer[buffersize] ;
	int fd = _tsopen(TempFileName, _O_RDWR | _O_TEMPORARY, SH_DENYNO);

	if (fd == -1) {

		printf("File open error %d\n", GetLastError());

	}
	else
	{
		fp = _tfdopen(fd, _T("rb"));
		fp1 = fopen("D:\\emm.wav","wb");
		if (fp == NULL)
			printf("File open error %d\n", GetLastError());
		else
		{
			printf("Got valid FILE* %p\n", fp);
			
			fseek(fp, 0L, SEEK_END);
			int length = ftell(fp);
			fseek(fp, 0L, SEEK_SET);

			int position = 0;
			while(true){
				memset(buffer, 0, buffersize);
				if (position < length) {
					
					fseek(fp, position, SEEK_SET);
					fread(buffer,1, buffersize, fp);
					fwrite(buffer, 1, buffersize, fp1);
					fflush(fp1);
					position += buffersize;
					continue ;
				}
				position -= buffersize;
				int left = length - position;
				fseek(fp, position, SEEK_SET);
				fread(buffer, 1, left, fp);
				fwrite(buffer, 1, left, fp1);
				fflush(fp1);
				break;

			}
			fclose(fp1);
			fclose(fp);
		}
	}
}

int Stop(HANDLE FileHandle) {
	if (!CloseHandle(FileHandle))
	{
		printf("CloseHandle(hTempFile) failed");
		return (8);
	}
}


int main()
{
	HANDLE FileHandle;
	char* TempFileName = NULL;
	tempfileAnalysis(FileHandle, TempFileName);
	fileRead(TempFileName);
	Stop(FileHandle);
	getchar();
}