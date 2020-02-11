#pragma once


class CSys
{
public:
	CSys( void );
	~CSys( void );

	void Init( void );
	void Shutdown( void );
	void Error( LPCTSTR lpszError );
};

extern CSys gSys;