// ControlWnd.cpp : implementation file
//

#include "pch.h"
#include "resource.h"
#include "MainFrame.h"
#include "common.h"
#include "hud.h"

// CControlWnd dialog

IMPLEMENT_DYNAMIC(CMainFrame, CDialogEx)

CMainFrame::CMainFrame(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONTROL, pParent)
{

}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnInitDialog( )
{
	if( !CDialogEx::OnInitDialog( ) )
		return FALSE;

	
	m_Alpha.SetRange( 0, 255, FALSE );
	UpdateData( FALSE );
	return TRUE;
}
void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_SLIDER1, m_Alpha );
}


BEGIN_MESSAGE_MAP(CMainFrame, CDialogEx)
	ON_WM_CREATE( )
	ON_WM_CLOSE( )
	ON_WM_HSCROLL( )
END_MESSAGE_MAP()


// CControlWnd message handlers


int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CDialogEx::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	SetIcon( LoadIcon( AfxGetApp( )->m_hInstance, MAKEINTRESOURCE( IDI_ICON1 ) ), TRUE );

	return 0;
}


void CMainFrame::OnClose( )
{
	CDialogEx::OnClose( );
	PostQuitMessage( 0 );
}


void CMainFrame::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar *pScrollBar )
{
	if( (CWnd *)&m_Alpha == (CWnd *)pScrollBar )
	{
		gHUD.SetBackgroundAlpha( m_Alpha.GetPos( ) );
	}
	CDialogEx::OnHScroll( nSBCode, nPos, pScrollBar );
}
