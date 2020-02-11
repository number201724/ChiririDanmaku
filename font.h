#pragma once

#include "color.h"

enum FontCreateFlag_t
{
	FONT_CREATE_NONE = 0,
	FONT_CREATE_BOLD = 1,
	FONT_CREATE_ITALIC = 2,
	FONT_CREATE_OUTLINE = 4,
	FONT_CREATE_SPRITE = 8,
	FONT_CREATE_ANTIALIASED = 16
};

class CDXFont
{
public:
	CDXFont( LPCWSTR pFontName,
			 int nFontSize,
			 DWORD dwFontFlags = FONT_CREATE_NONE | FONT_CREATE_ANTIALIASED
	);
	~CDXFont( );


	void				PreFrame( );
	void				PostFrame( );

	void				OnReset( );

	void				DrawString( int x, int y, Color color, LPCWSTR str );
	void				GetTextSize( LPCWSTR str, int &width, int &height );
	int					GetFontSize( ) const;

private:
	ID3DXFont			*m_pFont;
	ID3DXSprite			*m_pSprite;

	CStringW			m_pFontName;
	int					m_nFontSize;
	DWORD				m_dwFontFlags;
};