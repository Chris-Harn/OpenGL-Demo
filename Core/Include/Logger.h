#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>

enum PROTO {
	MAX_SIZE_OF_MESSAGE = 2048
};

class Logger {
public:
	static Logger *Instance() {
		if( s_pInstance == 0 ) {
			s_pInstance = new Logger();
			s_pInstance->Open( ".\\Logs" );
		}
		return s_pInstance;
	}

	void Open( const char *path );
	void LogError( const char *fmt, ... );
	void Printf_tstamp( const char *fmt, ... );
	void Printf_ntstamp( const char *fmt, ... );
	inline FILE *GetFileHandle( void ) { return m_pFhandle; }

private:
	Logger();
	~Logger() {}

	static Logger *s_pInstance;

	FILE *m_pFhandle;
};

typedef Logger TheLogger;

#endif