#pragma once

#include "color.h"
#include "font.h"
#include "sprite.h"

struct CUSTOMVERTEX
{
	XMFLOAT4 xyzrhw;
	D3DCOLOR color;
};

struct CUSTOMVERTEX_TEX
{
	XMFLOAT4 xyzrhw;
	D3DCOLOR color;
	int tu, tv;   // The texture coordinates
};

inline
void
BuildVertex(
	XMFLOAT4 xyzrhw,
	D3DCOLOR color,
	CUSTOMVERTEX *vertexList,
	int index
)
{
	vertexList[index].xyzrhw = xyzrhw;
	vertexList[index].color = color;
}



enum FontRenderFlag_t
{
	FONT_RENDER_LEFT = 0,
	FONT_RENDER_RIGHT = 1,
	FONT_RENDER_CENTER_V = 2,
	FONT_RENDER_CENTER_H = 4
};

class CDraw
{
public:
	CDraw( );
	~CDraw( );

	void					Init( void );
	void					Shutdown( void );

	void					BeginScene( void );
	void					EndScene( void );
	void					PreFrame( );
	void					PostFrame( );

	void					Reset( );


	HDXFont					CreateFont( CString pFontName, int nFontSize, DWORD dwFontFlags = FONT_CREATE_NONE );
	HDXSprite				CreateSprite( CString szFileName, int width, int height, Color transparentColorKey );

	void					DrawRect( int x, int y, int w, int h, Color color );
	void					DrawRectOut( int x, int y, int w, int h, Color color, Color out );
	void					DrawRectGradV( int x, int y, int w, int h, Color from, Color to );
	void					DrawRectGradH( int x, int y, int w, int h, Color from, Color to );
	void					DrawBorderBox( int x, int y, int w, int h, int t, Color color );
	void					DrawBorderBoxOut( int x, int y, int w, int h, int t, Color color, Color out );
	void					DrawLine( int x0, int y0, int x1, int y1, Color color );
	void					DrawCircle( int x, int y, int r, int s, Color color );
	void					DrawTriangle( int x, int y, int w, int h, Color color );
	void					DrawString( HDXFont hFont, int x, int y, DWORD dwAlign, Color color, LPCWSTR lpszFormat, ... );
	bool					GetTextSize( HDXFont hFont, int &w, int &h, LPCWSTR lpszFormat, ... );
	void					DrawSprite( HDXSprite hSprite, int x, int y, int scale );
	void					DrawGeometricTexture( HDXSprite hSprite, float x, float y, float radius, int geometric = 32 );

	
	CDXFont*				GetFont( HDXFont hFont );

private:
	std::vector<CDXFont *>		m_pFontList;
	std::vector<CDXSprite *>	m_pSpriteList;
public:
	D3DPRESENT_PARAMETERS		m_PersendParameters;
};

extern CDraw gDraw;