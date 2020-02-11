#include "pch.h"
#include "common.h"
#include "sys.h"
#include "overlay.h"
#include "render.h"
#include "hud.h"

COverlay gOverlay;

BEGIN_MESSAGE_MAP( COverlay, CWnd )
	ON_WM_CREATE( )
	ON_WM_CLOSE( )
	ON_WM_SIZE( )
	ON_WM_PAINT( )
	ON_WM_NCHITTEST( )
END_MESSAGE_MAP( )

COverlay::COverlay( void )
{
	m_fChangeSize = false;
}

COverlay::~COverlay( void )
{

}

void COverlay::Init( void )
{
	CString strWndClass = AfxRegisterWndClass( CS_VREDRAW | CS_HREDRAW );

	BOOL bRet = CreateEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_COMPOSITED | WS_EX_LAYERED,
						  strWndClass,
						  strWndClass,
						  WS_POPUP,
						  100,
						  100,
						  400,
						  750,
						  NULL,
						  0,
						  NULL
	);
	if( !bRet )
	{
		gSys.Error( TEXT( "CreateWindowEx failed." ) );
	}

	SetLayeredWindowAttributes( 0, 255, LWA_ALPHA );

	MARGINS margin = {-1, -1, -1, -1};
	DwmExtendFrameIntoClientArea( m_hWnd, &margin );
}

void COverlay::Shutdown( void )
{

}

void COverlay::Render( void )
{
	gDraw.BeginScene( );
	gDraw.PreFrame( );

	gHUD.Redraw( );


	gDraw.PostFrame( );
	gDraw.EndScene( );
}

void COverlay::Frame( void )
{
	if( m_fChangeSize )
	{
		gDraw.Reset( );
		m_fChangeSize = false;
	}

	Render( );
}

int COverlay::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CWnd::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	ShowWindow( SW_SHOWNORMAL );
	return 0;
}


void COverlay::OnClose( )
{

}

void COverlay::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
	m_fChangeSize = true;
}

void COverlay::OnPaint( )
{
	Render( );
}

LRESULT COverlay::OnNcHitTest( CPoint point )
{
	RECT rct;
	CPoint pt = point;

	ScreenToClient( &pt );
	GetClientRect( &rct );

	if( pt.x < rct.left + 5 && pt.y < rct.top + 5 )
	{
		return HTTOPLEFT;
	}
	else if( pt.x > rct.right - 5 && pt.y < rct.top + 5 )
	{
		return HTTOPRIGHT;
	}
	else if( pt.x<rct.left + 5 && pt.y>rct.bottom - 5 )
	{
		return HTBOTTOMLEFT;
	}
	else if( pt.x > rct.right - 5 && pt.y > rct.bottom - 5 )
	{
		return HTBOTTOMRIGHT;
	}
	else if( pt.x < rct.left + 5 )
	{
		return HTLEFT;
	}
	else if( pt.x > rct.right - 5 )
	{
		return HTRIGHT;
	}
	else if( pt.y < rct.top + 5 )
	{
		return HTTOP;
	}
	if( pt.y > rct.bottom - 5 )
	{
		return HTBOTTOM;
	}
	
	return HTCAPTION;
}