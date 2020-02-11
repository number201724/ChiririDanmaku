#include "pch.h"
#include "common.h"
#include "sys.h"
#include "render.h"
#include "overlay.h"

IDirect3D9Ex *g_pDirect3DEx;
IDirect3DDevice9Ex *g_pd3dDevice;

CDraw gDraw;


#pragma warning(disable:4244)
CDraw::CDraw( )
{

}

CDraw::~CDraw( )
{
	if( !m_pFontList.empty( ) )
	{
		for( auto &pFont : m_pFontList )
		{
			delete pFont;
			pFont = nullptr;
		}
	}

	if( !m_pSpriteList.empty( ) )
	{
		for( auto &pSprite : m_pSpriteList )
		{
			delete pSprite;
			pSprite = nullptr;
		}
	}
}

void CDraw::Init( void )
{
	HWND hwnd;
	RECT rct;

	hwnd = gOverlay.m_hWnd;
	gOverlay.GetWindowRect( &rct );

	m_PersendParameters.EnableAutoDepthStencil = TRUE;
	m_PersendParameters.AutoDepthStencilFormat = D3DFMT_D16;
	m_PersendParameters.Windowed = TRUE;
	m_PersendParameters.BackBufferCount = 1;
	m_PersendParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_PersendParameters.BackBufferWidth = rct.right - rct.left;
	m_PersendParameters.BackBufferHeight = rct.bottom - rct.top;
	m_PersendParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_PersendParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_PersendParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	m_PersendParameters.hDeviceWindow = hwnd;

	if( FAILED( Direct3DCreate9Ex( D3D_SDK_VERSION, &g_pDirect3DEx ) ) )
	{
		gSys.Error( TEXT( "Direct3DCreate9Ex failed." ) );
	}

	if( FAILED( g_pDirect3DEx->CreateDeviceEx( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_PersendParameters, NULL, &g_pd3dDevice ) ) )
	{
		gSys.Error( TEXT( "IDirect3D9Ex::CreateDeviceEx failed." ) );
	}
}

void CDraw::Shutdown( void )
{

}

void CDraw::BeginScene( void )
{
	g_pd3dDevice->Clear( NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.0f, NULL );
	g_pd3dDevice->BeginScene( );
	g_pd3dDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
}


void CDraw::EndScene( void )
{
	g_pd3dDevice->EndScene( );
	g_pd3dDevice->PresentEx( NULL, NULL, NULL, NULL, 0 );
}


void CDraw::PreFrame( )
{
	if( !m_pFontList.empty( ) )
	{
		for( auto &pFont : m_pFontList )
		{
			if( pFont )
			{
				pFont->PreFrame( );
			}
		}
	}

	if( !m_pSpriteList.empty( ) )
	{
		for( auto &pSprite : m_pSpriteList )
		{
			if( pSprite )
			{
				pSprite->PreFrame( );
			}
		}
	}

	g_pd3dDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
}

void CDraw::PostFrame( )
{
	if( !m_pFontList.empty( ) )
	{
		for( auto &pFont : m_pFontList )
		{
			if( pFont )
			{
				pFont->PostFrame( );
			}
		}
	}

	if( !m_pSpriteList.empty( ) )
	{
		for( auto &pSprite : m_pSpriteList )
		{
			if( pSprite )
			{
				pSprite->PostFrame( );
			}
		}
	}
}

void CDraw::Reset( )
{
	RECT rct;
	gOverlay.GetWindowRect( &rct );

	m_PersendParameters.BackBufferWidth = rct.right - rct.left;
	m_PersendParameters.BackBufferHeight = rct.bottom - rct.top;

	g_pd3dDevice->Reset( &m_PersendParameters );

	if( !m_pFontList.empty( ) )
	{
		for( auto &pFont : m_pFontList )
		{
			if( pFont )
			{
				pFont->OnReset( );
			}
		}
	}

	if( !m_pSpriteList.empty( ) )
	{
		for( auto &pSprite : m_pSpriteList )
		{
			if( pSprite )
			{
				pSprite->OnReset( );
			}
		}
	}
}

HDXFont CDraw::CreateFont( CString pFontName, int nFontSize, DWORD dwFontFlags )
{
	CDXFont *pFont = new CDXFont( pFontName, nFontSize, dwFontFlags );

	m_pFontList.push_back( pFont );

	return (m_pFontList.size( ) - 1);
}

HDXSprite CDraw::CreateSprite( CString szFileName, int width, int height, Color transparentColorKey )
{
	CDXSprite *pSprite = new CDXSprite( szFileName, width, height, transparentColorKey );

	m_pSpriteList.push_back( pSprite );

	return (m_pSpriteList.size( ) - 1);
}

void CDraw::DrawRect( int x, int y, int w, int h, Color color )
{
	CUSTOMVERTEX vertexList[4];

	BuildVertex( XMFLOAT4( x, y + h, 0, 1 ), color.Code( ), vertexList, 0 );
	BuildVertex( XMFLOAT4( x, y, 0, 1 ), color.Code( ), vertexList, 1 );
	BuildVertex( XMFLOAT4( x + w, y + h, 0, 1 ), color.Code( ), vertexList, 2 );
	BuildVertex( XMFLOAT4( x + w, y, 0, 1 ), color.Code( ), vertexList, 3 );

	g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertexList, sizeof( CUSTOMVERTEX ) );
}

void CDraw::DrawRectOut( int x, int y, int w, int h, Color color, Color out )
{
	DrawRect( x, y, w, h, color );
	DrawBorderBox( x, y, w, h, 1, out );
}

void CDraw::DrawRectGradV( int x, int y, int w, int h, Color from, Color to )
{
	int r = (int)(to.R( ) - from.R( )) / h;
	int g = (int)(to.G( ) - from.G( )) / h;
	int b = (int)(to.B( ) - from.B( )) / h;
	int a = (int)(to.A( ) - from.A( )) / h;

	for( int i = 0; i < h; i++ )
	{
		int R = from.R( ) + r * i;
		int G = from.G( ) + g * i;
		int B = from.B( ) + b * i;
		int A = from.A( ) + a * i;

		DrawRect( x, y + i, w, 1, Color( R, G, B, A ) );
	}
}

void CDraw::DrawRectGradH( int x, int y, int w, int h, Color from, Color to )
{
	int r = (int)(to.R( ) - from.R( )) / w;
	int g = (int)(to.G( ) - from.G( )) / w;
	int b = (int)(to.B( ) - from.B( )) / w;
	int a = (int)(to.A( ) - from.A( )) / w;

	for( int i = 0; i < h; i++ )
	{
		int R = from.R( ) + r * i;
		int G = from.G( ) + g * i;
		int B = from.B( ) + b * i;
		int A = from.A( ) + a * i;

		DrawRect( x + i, y, 1, h, Color( R, G, B, A ) );
	}
}

void CDraw::DrawBorderBox( int x, int y, int w, int h, int t, Color color )
{
	DrawRect( x, y, w, t, color );
	DrawRect( x, y, t, h, color );
	DrawRect( x + w, y, t, h, color );
	DrawRect( x, y + h, w + t, t, color );
}

void CDraw::DrawBorderBoxOut( int x, int y, int w, int h, int t, Color color, Color out )
{
	DrawBorderBox( x, y, w, h, t, color );
	DrawBorderBox( x - t, y - t, w + t * 2, h + t * 2, 1, out );
	DrawBorderBox( x + t, y + t, w - t * 2, h - t * 2, 1, out );
}

void CDraw::DrawLine( int x0, int y0, int x1, int y1, Color color )
{
	CUSTOMVERTEX vertexList[2];

	BuildVertex( XMFLOAT4( x0, y0, 0, 0 ), color.Code( ), vertexList, 0 );
	BuildVertex( XMFLOAT4( x1, y1, 0, 0 ), color.Code( ), vertexList, 1 );

	g_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 2, vertexList, sizeof( CUSTOMVERTEX ) );
}

void CDraw::DrawCircle( int x, int y, int r, int s, Color color )
{
	int Step = M_PI * 2.0 / s;
	for( int a = 0; a < (M_PI * 2.0); a += Step )
	{
		int x1 = r * cos( a ) + x;
		int y1 = r * sin( a ) + y;
		int x2 = r * cos( a + Step ) + x;
		int y2 = r * sin( a + Step ) + y;

		DrawLine( x1, y1, x2, y2, color );
	}
}

void CDraw::DrawTriangle( int x, int y, int w, int h, Color color )
{
	CUSTOMVERTEX vertexList[3];

	BuildVertex( XMFLOAT4( x + (w / 2), y, 0, 1 ), color.Code( ), vertexList, 0 );
	BuildVertex( XMFLOAT4( x, y + h, 0, 1 ), color.Code( ), vertexList, 1 );
	BuildVertex( XMFLOAT4( x + w, y + h, 0, 1 ), color.Code( ), vertexList, 2 );

	g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 1, vertexList, sizeof( CUSTOMVERTEX ) );
}

void CDraw::DrawString( HDXFont hFont, int x, int y, DWORD dwAlign, Color color, LPCTSTR lpszFormat, ... )
{
	static WCHAR buffer[4096];
	CDXFont *pFont = m_pFontList[hFont];

	if( !pFont )
		return;

	va_list args;
	va_start( args, lpszFormat );
	vswprintf_s( buffer, lpszFormat, args );
	va_end( args );

	int w = 0, h = 0;

	pFont->GetTextSize( buffer, w, h );

	if( dwAlign & FONT_RENDER_RIGHT )
		x -= w;
	else if( dwAlign & FONT_RENDER_CENTER_H )
		x -= w / 2;

	if( dwAlign & FONT_RENDER_CENTER_V )
		y -= h / 2;

	pFont->DrawString( x, y, color, buffer );
}

bool CDraw::GetTextSize( HDXFont hFont, int &w, int &h, LPCWSTR lpszFormat, ... )
{
	static WCHAR buffer[4096];
	CDXFont *pFont = m_pFontList[hFont];

	if( !pFont )
		return false;

	va_list args;
	va_start( args, lpszFormat );
	vswprintf_s( buffer, lpszFormat, args );
	va_end( args );

	pFont->GetTextSize( buffer, w, h );
	return true;
}

void CDraw::DrawSprite( HDXSprite hSprite, int x, int y, int scale )
{
	CDXSprite *pSprite = m_pSpriteList[hSprite];

	if( !pSprite )
		return;

	pSprite->DrawSprite( x, y, scale );
}

void CDraw::DrawGeometricTexture( HDXSprite hSprite, float x, float y, float radius, int geometric )
{
	CDXSprite *pSprite = m_pSpriteList[hSprite];

	if( !pSprite )
		return;

	pSprite->DrawGeometricTexture( x, y, radius, geometric );
}


CDXFont *CDraw::GetFont( HDXFont hFont )
{
	return m_pFontList[hFont];
}