#pragma once

#define FLAG_ADMIN (1<<0)
#define FLAG_VIP (1<<1)
#define FLAG_SVIP (1<<2)

class CHudDanmakuText
{
public:
	CHudDanmakuText( CStringW szNickName, CStringW szText, int Flags, int Guard, int UL );
	~CHudDanmakuText( );

	void SetMedal( int nLevel, LPWSTR szMedalName );
	void SetTitleName( CStringA szTitleName );

public:
	CStringW m_szNickName;
	CStringW m_szText;
	int m_nFlags;
	int m_iGuard;
	BOOL m_fHasMedal;
	CStringW m_szMedalName;
	int m_nMedalLevel;
	int m_nUL;

	CStringA m_szTitleName;
};

class CHud
{
public:
	CHud( );
	~CHud( );


	void Init( void );
	void Shutdown( void );

	void Redraw( void );
	void Reset( void );
	void Frame( void );

	void InitTitles( void );

	void RenderBackground( void );
	void SetBackgroundAlpha( int Alpha );

	void RenderDanmaku( void );
	void AddDanmaku( danmaku_data_t *pDanmakuData );


private:
	int m_nBackgroundAlpha;
	CList< CHudDanmakuText *> m_DanmakuTextList;
	int m_nRenderDanmakuCount;

	HDXFont m_hDanmakuFont;
};


extern CHud gHUD;
