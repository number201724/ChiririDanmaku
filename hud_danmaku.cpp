#include "pch.h"
#include "common.h"
#include "sys.h"
#include "render.h"
#include "protocol.h"
#include "hud.h"


CHudDanmakuText::CHudDanmakuText( CStringW szNickName, CStringW szText, int Flags, int Guard,int UL )
{
	m_szNickName = szNickName;
	m_szText = szText;
	m_nFlags = m_nFlags;
	m_iGuard = Guard;
	m_fHasMedal = FALSE;
	m_nUL = UL;
}

CHudDanmakuText::~CHudDanmakuText( void )
{

}

void CHudDanmakuText::SetMedal( int nLevel, LPWSTR szMedalName )
{
	m_nMedalLevel = nLevel;
	m_szMedalName = szMedalName;
}

void CHudDanmakuText::SetTitleName( CStringA szTitleName )
{
	m_szTitleName = szTitleName;
}