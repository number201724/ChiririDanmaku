#pragma once


// CControlWnd dialog

class CMainFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMainFrame();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROL };
#endif

	virtual BOOL OnInitDialog( );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnClose( );
	CSliderCtrl m_Alpha;
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar *pScrollBar );
};
