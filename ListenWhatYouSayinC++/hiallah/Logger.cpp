
#pragma once
#include <stdio.h>
//#include <string.h>
#include "Logger.h"

#include <windows.h>
//#include <fstream>
#include <wchar.h>
#include <string>
#pragma warning(disable:4996)
//
//static System::String^ pStringFromChar(const char* val)
//{
//	std::string s_str = std::string((char*)val);
//	std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
//	const wchar_t* w_char = wid_str.c_str();
//	System::String^ p_string = gcnew System::String(w_char);
//	return p_string;
//}

void logger(const char *fname, int lineno, const char *funcname, char* fmt, ...)
{
	//return;

//#ifdef _DEBUG
	char outstr[1000];
	memset(outstr, 0, sizeof(outstr));
	char *head = outstr;

	va_list argp;
	va_start(argp, fmt);

	const char *p = fmt;
	int i;
	char *s;
	char fmtbuf[100];
	for(; *p != '\0'; p++)
	{
		if(*p != '%')
		{
			*head++ = *p;
			continue;
		}

		switch(*++p)
		{
			case 'c':
				i = va_arg(argp, int);
				*head++ = i;
				break;

			case 'd':
				i = va_arg(argp, int);
				s = _itoa(i, fmtbuf, 10);
				while(*s != '\0')
					*head++ = *s++;
				break;

			case 's':
			{
				s = va_arg(argp, char *);
				int size = strlen(s);
				if (size > (sizeof(outstr)-100))
					return;
				while (*s != '\0')
					*head++ = *s++;
				break;
			}
			case 'x':
				i = va_arg(argp, int);
				s = _itoa(i, fmtbuf, 16);
				while(*s != '\0')
					*head++ = *s++;
				break;
				
			case '%':
				*head++ = *p;
				break;
		}
	}
	va_end(argp);

	//get filename from filepath
	i = strlen(fname);
	while(i-->0)
	{
		if(fname[i] == '\\')
			break;
	}
	fname= fname+i+1;

	//System::Diagnostics::Debug::WriteLine("file: {0}\t func : {1}\t line : {2}\n--->log: {3}\n", pStringFromChar(fname), pStringFromChar(funcname), lineno, pStringFromChar(outstr));
	char tt1[2048];
	sprintf(tt1,"\n file: %s\t func: %s\t line: %d\n--->log: %s\n", fname, funcname, lineno, outstr);
	wchar_t wtext1[2048];
	mbstowcs(wtext1, tt1, strlen(tt1)+1);//Plus null
	LPWSTR ptr1 = wtext1;
	OutputDebugStringW(ptr1);
	
}