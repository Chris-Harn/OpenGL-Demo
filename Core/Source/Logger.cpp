#include "Logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <Windows.h>
#include <cstdlib>

Logger *Logger::s_pInstance = 0;

Logger::Logger() {
	m_pFhandle = nullptr;
}

void Logger::Open( const char *path ) {
	char filename[MAX_SIZE_OF_MESSAGE];

	SYSTEMTIME st;
	GetSystemTime( &st );
	sprintf_s( filename, "%s\\%04d%02d%02d_%02d%02d%02d_%03d.log",
		path, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );

	fopen_s( &m_pFhandle, filename, "w" );
}

void Logger::LogError( const char *fmt, ... ) {
	va_list argp;
	char outputline[MAX_SIZE_OF_MESSAGE];

	if( fmt == nullptr ) return; // Exit early as nothing passed in

	va_start( argp, fmt );
	sprintf_s( outputline, fmt, argp );
	va_end( argp );

	if( m_pFhandle != nullptr ) {
		SYSTEMTIME st;
		GetSystemTime( &st );
		fprintf( m_pFhandle, "%04d%02d%02d_%02d%02d%02d_%03d: ",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
		fprintf( m_pFhandle, outputline );
		fprintf( m_pFhandle, "\n" );
	}
	#ifdef _DEBUG
		printf( outputline );
		printf( "\n" );
	#endif
}

void Logger::Printf_tstamp( const char *fmt, ... ) {
	va_list argp;
	char outputline[MAX_SIZE_OF_MESSAGE];

	if( fmt == nullptr ) return; // Exit early as nothing passed in

	va_start( argp, fmt );
	sprintf_s( outputline, fmt, argp );
	va_end( argp );

	if( m_pFhandle != nullptr ) {
		SYSTEMTIME st;
		GetSystemTime( &st );
		fprintf( m_pFhandle, "%04d%02d%02d_%02d%02d%02d_%03d: ",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
		fprintf( m_pFhandle, outputline );
		fprintf( m_pFhandle, "\n" );
	}
	#ifdef _DEBUG
		printf( outputline );
		printf( "\n" );
	#endif
}

void Logger::Printf_ntstamp( const char *fmt, ... ) {
	va_list argp;
	char outputline[MAX_SIZE_OF_MESSAGE];

	if( fmt == nullptr ) return; // Exit early as nothing passed in

	va_start( argp, fmt );
	sprintf_s( outputline, fmt, argp );
	va_end( argp );

	if( m_pFhandle != nullptr ) {
		fprintf( m_pFhandle, outputline );
		fprintf( m_pFhandle, "\n" );
	}
	#ifdef _DEBUG
		printf( outputline );
		printf( "\n" );
	#endif
}

