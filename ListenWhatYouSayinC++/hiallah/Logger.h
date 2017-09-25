#pragma once

#ifndef _logger_
#define _logger_
#define Logger(msg, ...) logger(__FILE__, __LINE__, __FUNCTION__, msg, __VA_ARGS__)		//__func__	or __FUNCTION__


void logger(const char *fname, int lineno, const char *funcname, char* fmt, ...);

#endif

