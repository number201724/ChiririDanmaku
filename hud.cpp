#include "pch.h"
#include "common.h"
#include "sys.h"
#include "overlay.h"
#include "render.h"
#include "hud.h"
#include "font.h"
#include "sprite.h"
#include "http.h"
#include "web.h"


bool hud_drawframe = false;
bool d_resize = false;

wchar_t draw_text[1024] = L"number201724";

CHud gHUD;

CHud::CHud( void )
{
	m_nRenderDanmakuCount = 20;
}

CHud::~CHud( void )
{

}

void CHud::InitTitles( void )
{
	CHudTitleLoadRequest *pLoadTitleRequest = new CHudTitleLoadRequest( );
	pLoadTitleRequest->SetUrl( "https://api.live.bilibili.com/rc/v1/Title/webTitles" );
	gHttp.Run( pLoadTitleRequest );
}

void CHud::Init( void )
{
	m_nBackgroundAlpha = 50;

	InitTitles( );
	m_hDanmakuFont = gDraw.CreateFont( TEXT( "System" ), -22,/*FONT_CREATE_BOLD |*/ FONT_CREATE_OUTLINE | FONT_CREATE_SPRITE );
}

void CHud::Shutdown( void )
{

}

INT g_nCount = 0;

void CHud::Redraw( void )
{
	RenderBackground( );
	RenderDanmaku( );

	gDraw.DrawString( m_hDanmakuFont, 10, 600, 0, Color::Red( ), L"%d", g_nCount );
}

void CHud::Reset( void )
{

}

void CHud::Frame( void )
{

}

void CHud::RenderBackground( void )
{
	gDraw.DrawRect( 0,
					0,
					gDraw.m_PersendParameters.BackBufferWidth,
					gDraw.m_PersendParameters.BackBufferHeight,
					Color( D3DCOLOR_RGBA( 0, 0, 0, m_nBackgroundAlpha ) )
	);
}

void CHud::SetBackgroundAlpha( int Alpha )
{
	m_nBackgroundAlpha = Alpha;
}

void CHud::RenderDanmaku( void )
{
	int nStartPosH = 0;
	if( m_DanmakuTextList.IsEmpty( ) )
		return;

	CHudDanmakuText *pDanmakuText = NULL;

	POSITION Pos = m_DanmakuTextList.GetHeadPosition( );

	while( true )
	{
		int nWidth, nHeight;
		pDanmakuText = m_DanmakuTextList.GetNext( Pos );

		gDraw.GetTextSize( m_hDanmakuFont, nWidth, nHeight, TEXT( "[UL:%d]%s: %s" ),
						   pDanmakuText->m_nUL,
						   pDanmakuText->m_szNickName,
						   pDanmakuText->m_szText );

		gDraw.DrawString( m_hDanmakuFont, 0, nStartPosH, 0, Color::Green( ), TEXT( "[UL:%d]%s: %s" ),
						  pDanmakuText->m_nUL,
						  pDanmakuText->m_szNickName,
						  pDanmakuText->m_szText );

		nStartPosH += nHeight;

		if( Pos == NULL )
			break;
	}
}
void CHud::AddDanmaku( danmaku_data_t *pDanmakuData )
{
	int Flags = 0;

	if( pDanmakuData->dminfo.type != 0 )
		return;

	if( pDanmakuData->userinfo.is_admin )
		Flags |= FLAG_ADMIN;

	if( pDanmakuData->userinfo.is_vip )
		Flags |= FLAG_VIP;

	if( pDanmakuData->userinfo.is_svip )
		Flags |= FLAG_SVIP;

	CHudDanmakuText *pDanmakuText = new CHudDanmakuText(
		pDanmakuData->userinfo.nickname,
		pDanmakuData->text,
		Flags,
		pDanmakuData->guard,
		pDanmakuData->rank.level
	);

	// TRACE( L"%s\n", pDanmakuData->text );

	if( pDanmakuData->has_medal )
	{
		pDanmakuText->SetMedal( pDanmakuData->medal.level, pDanmakuData->medal.medal_name );
	}

	if( pDanmakuData->has_title )
	{
		pDanmakuText->SetTitleName( pDanmakuData->title.newname );
	}

	if( m_DanmakuTextList.GetSize( ) + 1 > m_nRenderDanmakuCount )
	{
		CHudDanmakuText *pTailDanmaku = m_DanmakuTextList.RemoveHead( );
		delete pTailDanmaku;
	}

	m_DanmakuTextList.AddTail( pDanmakuText );
}