#include "pch.h"
#include "common.h"
#include "sys.h"
#include "net.h"

CNet gNet;

CNet::CNet( void )
{

}

CNet::~CNet( void )
{

}

void CNet::Init( void )
{
	WSADATA WSAData;
	if( WSAStartup( MAKEWORD( 2, 2 ), &WSAData ) )
	{
		gSys.Error( TEXT( "WSAStartup failed." ) );
	}
}

void CNet::Shutdown( void )
{
	WSACleanup( );
}
