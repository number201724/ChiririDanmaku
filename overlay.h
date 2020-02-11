#pragma once

class COverlay : public CWnd
{
public:
	COverlay( void );
	~COverlay( void );

	void Init( void );
	void Shutdown( void );

	void Render( void );
	void Frame( void );
protected:
	DECLARE_MESSAGE_MAP( )
public:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnClose( );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnPaint( );
	afx_msg LRESULT OnNcHitTest( CPoint point );

private:
	bool m_fChangeSize;
};

extern COverlay gOverlay;