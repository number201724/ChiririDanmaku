#pragma once
#include <afxwinappex.h>
class CDanmakuApp : public CWinAppEx
{
public:
	CDanmakuApp( ) noexcept;
public:
	BOOL InitMainFrame( );
	virtual BOOL InitInstance( );
	virtual int ExitInstance( );
	virtual BOOL OnIdle( LONG lCount );
	virtual int Run( );
};

extern CDanmakuApp theApp;