/********************************************************************************
*					Open Universe - version 1.0.1.10							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
* BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
* WRITTEN CONSENT OF TANGRAM TEAM.
*
* THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
* OUTLINED IN THE MIT LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
********************************************************************************/

#pragma once

class CGridHelper : public CWnd
{
	DECLARE_DYNCREATE(CGridHelper)
public:
	BOOL					m_bNoMove;
	BOOL					m_bBKWnd;
	BOOL					m_bEraseBkgnd;
	BOOL					m_bCreateExternal;
	HWND					m_hFormWnd;

	CString					m_strKey;
	CString					m_strXml;

	CGrid*					m_pGrid;
	CGrid*					m_pParentGrid;
	CTangramHtmlTreeWnd*	m_pXHtmlTree;

	map<CString, HWND>		m_mapDockCtrl;
	IOleInPlaceActiveObject* m_pOleInPlaceActiveObject;

	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CGridHelper();           // protected constructor used by dynamic creation
	virtual ~CGridHelper();
	void PostNcDestroy();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnActiveTangramObj(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnHubbleMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnHubbleData(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnTabChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetHubbleObj(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSplitterReposition(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
};

class CBKWnd : public CWindowImpl<CBKWnd, CWindow>
{
public:
	CBKWnd(void);
	HWND			m_hChild;
	CString			m_strURL;
	CGrid*			m_pGrid;
	CGalaxy*		m_pGalaxy;
	BEGIN_MSG_MAP(CBKWnd)
		MESSAGE_HANDLER(WM_MDICLIENTCREATED, OnMdiClientCreated)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanged)
	END_MSG_MAP()

private:
	LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMdiClientCreated(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnWindowPosChanged(UINT, WPARAM, LPARAM, BOOL&);
	void OnFinalMessage(HWND hWnd);
};
