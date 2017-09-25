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
#define BUFSIZE 800

#define time 8

int sampleRate= 22100;


const int NUMPTS = sampleRate * time; // 3 seconds
									  // 'short int' is a 16-bit type; I request 16-bit samples below
									  // for 8-bit capture, you'd use 'unsigned char' or 'BYTE' 8-bit types

void PlayRecord(PWAVEHDR hdr);
void StartRecord(int i);
WAVEFORMATEX pFormat;

char waveIn[100][BUFSIZE];
WAVEHDR WaveInHdr[100];

HWAVEIN hWaveIn;
bool iscon;
bool recordoff;

HWAVEOUT hWaveOut;
MMRESULT result;
fstream *myAudioFile;
bool isRunning;
void SaveWavFile(char *FileName, PWAVEHDR WaveHeader)
{
	Logger("new session recording");
	int subchunk2size = WaveHeader->dwBytesRecorded*1;//numChannels
	
	char* prechunksize_char = new char[4];
	char* presubchunk2size = new char[4]; 
	int current_position = myAudioFile->tellp();


	Logger("current position of file = %d ",current_position);
	
	myAudioFile->seekg(4, ios::beg);
	myAudioFile->read(prechunksize_char,4);

	int* prechunksize_int = (int*)prechunksize_char;
	int chunksize = subchunk2size + (*prechunksize_int);

	Logger("previous chunksize = %d ", (*prechunksize_int));
	Logger("current chunksize = %d ", chunksize);

	myAudioFile->seekp(4, ios::beg);
	myAudioFile->write((char*)&chunksize,4);
	myAudioFile->flush();

	if (current_position > 44) {

		myAudioFile->seekg(40, ios::beg);
		myAudioFile->read(presubchunk2size, 4);

		int* presubchunk2size_int = (int*)presubchunk2size;
		int newAudiolengh = subchunk2size + (*presubchunk2size_int);

		Logger("previous subchunk2size = %d ", (*presubchunk2size_int));
		Logger("current subchunk2size = %d ", newAudiolengh);

		myAudioFile->seekp(40, ios::beg);
		myAudioFile->write((char*)&newAudiolengh, 4);
		myAudioFile->flush();

		myAudioFile->seekp(current_position);
		myAudioFile->write(WaveHeader->lpData, WaveHeader->dwBytesRecorded);
	
		Logger(" data = %s ", WaveHeader->lpData);

		/*for (int i = 0; i < WaveHeader->dwBytesRecorded; i++) {
			printf("\n %d \n", WaveHeader->lpData[i]);
		}*/
		myAudioFile->flush();
		return;
	}

	
	myAudioFile->seekp(current_position, ios::beg);
	myAudioFile->write("data", 4);                   // subchunk2ID
	myAudioFile->write((char*)&subchunk2size, 4);           // subchunk2size (NumSamples * NumChannels * BitsPerSample/8)
	
	Logger("previous subchunk2size = 0 ");
	Logger("current subchunk2size = %d ", subchunk2size);

	myAudioFile->write(WaveHeader->lpData, WaveHeader->dwBytesRecorded); // data
	myAudioFile->flush();
	Logger(" data = %s ", WaveHeader->lpData);
	
}
void CALLBACK VoiceRecordNotification(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	
	try {
		//char* data = reinterpret_cast<char*>(dwInstance);
		/*if (!isRunning) {
			Logger("VoiceRecordNotification without enter");
			return;
		}*/
		LPWAVEHDR hdr;

		switch (uMsg)
		{
		case WIM_DATA:
		{
			//				log("voiceRecordNotification with WIM_DATA");



			hdr = (LPWAVEHDR)dwParam1;
			if (hdr == NULL)return;
			//printf("\n%d\n", hdr->dwUser);
		//	SaveWavFile("",hdr);

			PlayRecord(hdr);
		//	waveInUnprepareHeader(hWaveIn, hdr, sizeof(WAVEHDR));
//			waveInStart(hWaveIn);
			/*waveInUnprepareHeader(hWaveIn, hdr, sizeof(WAVEHDR));
			memset(&waveIn, 0, sizeof(waveIn));

			WaveInHdr.lpData = (LPSTR)waveIn;
			WaveInHdr.dwBufferLength = 20000;
			WaveInHdr.dwBytesRecorded = 0;
			WaveInHdr.dwUser = 0L;
			WaveInHdr.dwFlags = 0L;
			WaveInHdr.dwLoops = 0L;
			StartRecord(0);*/

		//	waveOutUnprepareHeader(hWaveOut, hdr, sizeof(WAVEHDR));

			iscon = false;
			

		}
		break;
		case WIM_CLOSE:
			Logger("voiceRecordNotification with WIM_CLOSE");
			break;
		case WIM_OPEN:
			Logger("voiceRecordNotification with WIM_OPEN");
			break;
		}
	}
	catch (int e) {
		Logger("..........................................");
		return;
	}
	//Logger("VoiceRecordNotification exit");
}

void CALLBACK VoicePlayNotification(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{

	try {
		//char* data = reinterpret_cast<char*>(dwInstance);
		/*if (!isRunning) {
		Logger("VoiceRecordNotification without enter");
		return;
		}*/
		

		switch (uMsg)
		{
		case WOM_DONE:
		{
			LPWAVEHDR hdr = (LPWAVEHDR)dwParam1;
			waveInUnprepareHeader(hWaveIn, hdr, sizeof(WAVEHDR));
			waveInPrepareHeader(hWaveIn, hdr, sizeof(WAVEHDR));
			waveInAddBuffer(hWaveIn, hdr, sizeof(WAVEHDR));
		}
		break;
		case WOM_CLOSE:
			Logger("voicePlayNotification with WIM_CLOSE");
			break;
		case WOM_OPEN:
			Logger("voicePlayNotification with WIM_OPEN");
			break;
		}
	}
	catch (int e) {
		Logger("..........................................");
		return;
	}
	//Logger("VoiceRecordNotification exit");
}
void StartRecord(int i)
{
	


	

	for (int i = 0; i < 100; i++) {

		//waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
		waveInPrepareHeader(hWaveIn, &WaveInHdr[i], sizeof(WAVEHDR));

		// Insert a wave input buffer
		result = waveInAddBuffer(hWaveIn, &WaveInHdr[i], sizeof(WAVEHDR));
	}

	// Commence sampling input
	result = waveInStart(hWaveIn);

}

void PlayRecord(PWAVEHDR hdr)
{

	
	
//	cout << "playing..." << endl;
	waveOutWrite(hWaveOut, &WaveInHdr[hdr->dwUser-1], sizeof(WaveInHdr[hdr->dwUser - 1])); // Playing the data

}
void StopRec() {
	isRunning = false;
}
void fileinit() {
	//event isfull;
	isRunning = true;
	myAudioFile = new fstream();
	//myAudioFile->open("D:\\recor.wav", std::fstream::out);
	myAudioFile->open("D:\\recor.wav", std::fstream::trunc);
	myAudioFile->open("D:\\recor.wav", fstream::in | fstream::out| fstream::binary);
	int chunksize = 36, pcmsize, NumSamples, subchunk1size;
	int audioFormat = WAVE_FORMAT_PCM;
	int numChannels = 1;
	int bitsPerSample = 16;
	NumSamples = ((long)(NUMPTS / sampleRate) * 1000);
	pcmsize = sizeof(PCMWAVEFORMAT);
	subchunk1size = 16;
	int byteRate = sampleRate*numChannels*bitsPerSample / 8;
	int blockAlign = numChannels*bitsPerSample / 8;
	// write the wav file per the wav file format
	
	
	myAudioFile->seekp(0, ios::beg);
	myAudioFile->write("RIFF", 4);                   // chunk id
	myAudioFile->write((char*)&chunksize, 4);               // chunk size (36 + SubChunk2Size))
	myAudioFile->write("WAVE", 4);                   // format
	
	
	myAudioFile->write("fmt ", 4);                   // subchunk1ID
	myAudioFile->write((char*)&subchunk1size, 4);           // subchunk1size (16 for PCM)
	myAudioFile->write((char*)&audioFormat, 2);         // AudioFormat (1 for PCM)
	myAudioFile->write((char*)&numChannels, 2);         // NumChannels
	myAudioFile->write((char*)&sampleRate, 4);          // sample rate
	myAudioFile->write((char*)&byteRate, 4);            // byte rate (SampleRate * NumChannels * BitsPerSample/8)
	myAudioFile->write((char*)&blockAlign, 2);          // block align (NumChannels * BitsPerSample/8)
	myAudioFile->write((char*)&bitsPerSample, 2);           // bits per sample
	myAudioFile->flush();
	//myAudioFile->close();
}

DWORD WINAPI Recorder(LPVOID lpvoid) {
	fileinit();

	for (int i = 0;; i++) {
		iscon = true;
		StartRecord(i);
		//std::printf("step %d", i);
		while (iscon);

		if (!recordoff) break;
	}
	StopRec();
	myAudioFile->close();
	return 0;
}


DWORD WINAPI Recordercon(LPVOID lpvoid) {
	int what;
	while (true) {
		printf("Enter a key to exit");
		scanf("%d", &what);
		if (what == 0) {
			recordoff = false;
		}
	}
	printf("kije hocche");
	return 0;
}



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
void inittemp() {
	HANDLE FileHandle;
	char* TempFileName = NULL;
	tempfileAnalysis(FileHandle, TempFileName);
	fileRead(TempFileName);
	Stop(FileHandle);
}

int main()
{
	//inittemp();



	pFormat.wFormatTag = WAVE_FORMAT_PCM; // simple, uncompressed format
	pFormat.nChannels = 1; // 1=mono, 2=stereo
	pFormat.nSamplesPerSec = sampleRate; // 8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz
	pFormat.nAvgBytesPerSec = sampleRate * 2; // = nSamplesPerSec × nBlockAlign
	pFormat.nBlockAlign = 2; // = (nChannels × wBitsPerSample) / 8
	pFormat.wBitsPerSample = 16; // 16 for high quality, 8 for telephone-grade
	pFormat.cbSize = 0;

	// Specify recording parameters

	result = waveInOpen(&hWaveIn, 123, &pFormat,
		0L, 0L, WAVE_FORMAT_DIRECT);

	result = waveInOpen(&hWaveIn, 123, &pFormat, (DWORD_PTR)VoiceRecordNotification, (DWORD_PTR)WaveInHdr[0].lpData, CALLBACK_FUNCTION);

	for (int i = 0; i < 100; i++) {
		// Set up and prepare header for input
		WaveInHdr[i].lpData = (LPSTR)waveIn[i];
		WaveInHdr[i].dwBufferLength = BUFSIZE;
		WaveInHdr[i].dwBytesRecorded = 0;
		WaveInHdr[i].dwUser = i+1;
		WaveInHdr[i].dwFlags = 0L;
		WaveInHdr[i].dwLoops = 0L;


	}

	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &pFormat, (DWORD_PTR)VoicePlayNotification, (DWORD_PTR)WaveInHdr[0].lpData, CALLBACK_FUNCTION);


	fileinit();


	StartRecord(0);


	StopRec();
	myAudioFile->close();
	getchar();
	return 0;
}