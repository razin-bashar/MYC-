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

#define time 8

int sampleRate= 22100;


const int NUMPTS = sampleRate * time; // 3 seconds
									  // 'short int' is a 16-bit type; I request 16-bit samples below
									  // for 8-bit capture, you'd use 'unsigned char' or 'BYTE' 8-bit types

void PlayRecord(int i);
WAVEFORMATEX pFormat;

short int waveIn[20000];
WAVEHDR WaveInHdr;
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
		char* data = reinterpret_cast<char*>(dwInstance);
		if (!isRunning) {
			Logger("VoiceRecordNotification without enter");
			return;
		}
		LPWAVEHDR hdr;

		switch (uMsg)
		{
		case WIM_DATA:
		{
			//log("voiceRecordNotification with WIM_DATA");
			hdr = (LPWAVEHDR)dwParam1;
			if (hdr == NULL)return;

			SaveWavFile("",hdr);
			waveInUnprepareHeader(hWaveIn, hdr, sizeof(WAVEHDR));
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


void StartRecord(int i)
{
	
	pFormat.wFormatTag = WAVE_FORMAT_PCM; // simple, uncompressed format
	pFormat.nChannels = 1; // 1=mono, 2=stereo
	pFormat.nSamplesPerSec = sampleRate; // 8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz
	pFormat.nAvgBytesPerSec = sampleRate * 2; // = nSamplesPerSec × nBlockAlign
	pFormat.nBlockAlign = 2; // = (nChannels × wBitsPerSample) / 8
	pFormat.wBitsPerSample = 16; // 16 for high quality, 8 for telephone-grade
	pFormat.cbSize = 0;

	// Specify recording parameters

	result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat,
		0L, 0L, WAVE_FORMAT_DIRECT);

	result=waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, (DWORD_PTR)VoiceRecordNotification, (DWORD_PTR) WaveInHdr.lpData, CALLBACK_FUNCTION);


	// Set up and prepare header for input
	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = 20000;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;
	//waveInUnprepareHeader(hWaveIn[i % 2], &WaveInHdr[i % 2], sizeof(WAVEHDR));
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	// Insert a wave input buffer
	result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
	

	// Commence sampling input
	result = waveInStart(hWaveIn);

}



void fileinit() {
	//event isfull;
	isRunning = true;
	myAudioFile = new fstream();
	myAudioFile->open("D:\\recor.wav", std::fstream::out);
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

int main(){
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