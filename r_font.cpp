#include "pch.h"
#include "common.h"
#include "render.h"
#include "font.h"

extern IDirect3DDevice9Ex *g_pd3dDevice;

CDXFont::CDXFont( LPCWSTR pFontName, int nFontSize, DWORD dwFontFlags )
{
	m_pFont = nullptr;
	m_pSprite = nullptr;
	m_pFontName = pFontName;
	m_nFontSize = nFontSize;
	m_dwFontFlags = dwFontFlags;
}

CDXFont::~CDXFont( )
{
	if( m_pFont )
	{
		m_pFont->Release( );
		m_pFont = nullptr;
	}

	if( m_pSprite )
	{
		m_pSprite->Release( );
		m_pSprite = nullptr;
	}
}

void CDXFont::PreFrame( )
{
	if( m_dwFontFlags & FONT_CREATE_SPRITE )
	{
		if( !m_pSprite )
		{
			D3DXCreateSprite( g_pd3dDevice, &m_pSprite );
		}

		if( m_pSprite )
		{
			m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
		}
	}

	if( !m_pFont )
	{
		UINT Weight = (m_dwFontFlags & FONT_CREATE_BOLD)			? FW_HEAVY : FW_NORMAL;
		BOOL Italic = (m_dwFontFlags & FONT_CREATE_ITALIC)		? TRUE : FALSE;
		UINT Aliasing = (m_dwFontFlags & FONT_CREATE_ANTIALIASED)	? CLEARTYPE_NATURAL_QUALITY : NONANTIALIASED_QUALITY;

		D3DXCreateFontW( g_pd3dDevice,
						 m_nFontSize,
						 NULL,
						 Weight,
						 NULL,
						 Italic,
						 DEFAULT_CHARSET,
						 OUT_DEFAULT_PRECIS,
						 Aliasing,
						 DEFAULT_PITCH | FF_DONTCARE,
						 m_pFontName,
						 &m_pFont );
	}
}

void CDXFont::PostFrame( )
{
	if( m_dwFontFlags & FONT_CREATE_SPRITE )
	{
		if( m_pSprite )
		{
			m_pSprite->End( );
		}
	}
}

void CDXFont::OnReset( )
{
	if( m_pFont )
	{
		m_pFont->OnLostDevice( );
		m_pFont->OnResetDevice( );
	}

	if( m_dwFontFlags & FONT_CREATE_SPRITE )
	{
		if( m_pSprite )
		{
			m_pSprite->OnLostDevice( );
			m_pSprite->OnResetDevice( );
		}
	}
}

void CDXFont::DrawString( int x, int y, Color color, LPCWSTR str )
{
	if( !m_pFont )
		return;

	if( !str )
		return;

	if( m_dwFontFlags & FONT_CREATE_OUTLINE )
	{
		RECT Out[4] = {
			{x + 1, y, 0, 0},
		{x - 1, y, 0, 0},
		{x, y + 1, 0, 0},
		{x, y - 1, 0, 0}
		};

		for( auto &Rect : Out )
		{
			m_pFont->DrawTextW( m_pSprite, str, -1, &Rect, DT_NOCLIP, Color::Black( ).Code( ) );
		}
	}

	RECT Rect = {x, y, 0, 0};

	m_pFont->DrawTextW( m_pSprite, str, -1, &Rect, DT_NOCLIP, color.Code( ) );
}

void CDXFont::GetTextSize( LPCWSTR text, int &width, int &height )
{
	if( !m_pFont )
	{
		return;
	}

	if( !text )
	{
		return;
	}

	RECT Rect = {0, 0, 0, 0};

	m_pFont->DrawTextW( m_pSprite, text, -1, &Rect, DT_CALCRECT, Color::None( ).Code( ) );

	width = Rect.right - Rect.left;
	height = Rect.bottom - Rect.top;
}

int CDXFont::GetFontSize( ) const
{
	return m_nFontSize;
}