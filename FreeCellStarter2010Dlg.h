
// FreeCellStarter2010Dlg.h : header file
//

#pragma once


// CFreeCellStarter2010Dlg dialog
class CFreeCellStarter2010Dlg : public CDialogEx
{
// Construction
public:
	CFreeCellStarter2010Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FREECELLSTARTER2010_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void ShuffleDeck();
	void CheckMoves(CPaintDC &dc);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void CFreeCellStarter2010Dlg::OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnFileNewgame();
	afx_msg void OnUndo();
	afx_msg void OnRedo();
	afx_msg void OnMenu4();
	afx_msg void OnMenu5();
	afx_msg void OnRetry();
};
