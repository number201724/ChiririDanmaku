#include "pch.h"
#include "sys.h"

CSys gSys;

CSys::CSys( void )
{

}

CSys::~CSys( void )
{

}

void CSys::Init( void )
{

}

void CSys::Shutdown( void )
{

}

void CSys::Error( LPCTSTR lpszError )
{
	MessageBox( NULL, lpszError, APPLICATION_TITLE, MB_ICONERROR );
	TerminateProcess( GetCurrentProcess( ), 0 );
}