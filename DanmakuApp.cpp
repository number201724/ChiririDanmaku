#include "pch.h"
#include "DanmakuApp.h"
#include "resource.h"

#include "common.h"
#include "sys.h"
#include "net.h"
#include "http.h"
#include "overlay.h"
#include "render.h"
#include "hud.h"
#include "font.h"


#include "resource.h"
#include "MainFrame.h"

CDanmakuApp theApp;
CMainFrame *g_pMainFrame;

CDanmakuApp::CDanmakuApp( ) noexcept
{
	SetAppID( _T( "{70D8C0BA-55C8-4719-912A-30FC259D04CC}" ) );
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

BOOL CDanmakuApp::InitMainFrame( )
{
	g_pMainFrame = new CMainFrame( );

	if( !g_pMainFrame )
	{
		delete g_pMainFrame;
		return FALSE;
	}

	if( !g_pMainFrame->Create( IDD_CONTROL ) )
	{
		delete g_pMainFrame;
		return FALSE;
	}

	m_pMainWnd = g_pMainFrame;

	return TRUE;
}

BOOL CDanmakuApp::InitInstance( )
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &InitCtrls );

	CWinAppEx::InitInstance( );
	// Initialize OLE libraries
	if( !AfxOleInit( ) )
	{
		AfxMessageBox( IDP_OLE_INIT_FAILED );
		return FALSE;
	}

	AfxEnableControlContainer( );
	EnableTaskbarInteraction( );
	SetRegistryKey( _T( "ChiririDanmaku" ) );
	LoadStdProfileSettings( 4 );
	InitContextMenuManager( );
	InitKeyboardManager( );
	InitTooltipManager( );

	gSys.Init( );
	gNet.Init( );
	gHttp.Init( );
	gNet.Init( );
	gHttp.Init( );

	if( !InitMainFrame( ) )
		return FALSE;

	gOverlay.Init( );
	gDraw.Init( );
	gHUD.Init( );

	g_pMainFrame->ShowWindow( m_nCmdShow );
	g_pMainFrame->UpdateWindow( );

	gDanmakuNet.Init( 21304638, 11588230 );

	gOverlay.ShowWindow( SW_SHOW );
	gOverlay.UpdateWindow( );

	//CWebJsonRequest *pJsonRequest = new CWebJsonRequest( );
	//pJsonRequest->SetUrl( "https://api.live.bilibili.com/rc/v1/Title/webTitles" );
	//gHttp.Run( pJsonRequest );

	return TRUE;
}

int CDanmakuApp::ExitInstance( )
{
	gDanmakuNet.Shutdown( );
	gHUD.Shutdown( );
	gDraw.Shutdown( );
	gOverlay.Shutdown( );
	gHttp.Shutdown( );
	gNet.Shutdown( );
	gSys.Shutdown( );

	return 0;
}

BOOL CDanmakuApp::OnIdle( LONG lCount )
{
	gDanmakuNet.Frame( );
	gHttp.Frame( );
	gHUD.Frame( );
	gOverlay.Frame( );

	return FALSE;
}

int CDanmakuApp::Run( )
{
	ASSERT_VALID( this );
	_AFX_THREAD_STATE *pState = AfxGetThreadState( );


	LONG lIdleCount = 0;
	for( ;;)
	{
		while( ::PeekMessage( &(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE ) )
		{
			lIdleCount = 0;
			if( !PumpMessage( ) )
				return ExitInstance( );
		}

		lIdleCount++;

		OnIdle( lIdleCount );
		// Sleep( 1 );
	}
	return 0;
}