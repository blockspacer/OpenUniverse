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

#include "../stdafx.h"

class CWinForm;
namespace Web
{
	class ATL_NO_VTABLE CWebPage :
		public CWebPageImpl,
		public CWindowImpl<CWebPage, CWindow>,
		public CComObjectRootEx<CComSingleThreadModel>,
		public IDispatchImpl<IWebPage, &_uuidof(IWebPage), &LIBID_Universe, 1, 0>
	{
	public:
		CWebPage();
		~CWebPage() override;

		BOOL								m_bDevToolWnd;
		bool								m_bWebContentVisible;

		HWND								m_hChildWnd;
		HWND								m_hExtendWnd;
		HWND								m_hWebHostWnd;

		CString								m_strURL;
		CString								m_strCurKey;
		CString								m_strCurXml;
		CString								m_strAppProxyID;

		CGalaxy*							m_pGalaxy;
		CHubbleDoc*							m_pDoc;
		CGrid*								m_pParentGrid;
		CGrid*								m_pBindGrid;
		CWinForm*							m_pBindWinForm;
		IUniverseAppProxy*					m_pAppProxy;
		CGalaxyCluster*						m_pGalaxyCluster;

		CWebPage*							m_pWebWnd;
		CWebPage*							m_pDevToolWnd;
		map<CString, CString>				m_mapFormsInfo;
		map<CString, CString>				m_mapAtlMFCsInfo;
		map<CString, CString>				m_mapStartupsInfo;
		map<CString, CString>				m_mapeclipsesInfo;
		map<CString, CString>				m_mapWorkBenchInfo;
		map<CString, CString>				m_mapUserControlsInfo;
		map<HWND, CWinForm*>				m_mapWinForm;
		map<HWND, CWinForm*>				m_mapSubWinForm;
		map<CString, CMDIChildFormInfo*>	m_mapChildFormsInfo;
		vector<HWND>						m_vSubForm;

		BEGIN_MSG_MAP(CWebPage)
			MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
			MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
			MESSAGE_HANDLER(WM_COSMOSMSG, OnHubbleMsg)
			MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
			MESSAGE_HANDLER(WM_CHROMEIPCMSG, OnChromeIPCMsgReceived)
			MESSAGE_HANDLER(WM_CHROMEWNDPARENTCHANGED, OnParentChanged)
		END_MSG_MAP()

		BEGIN_COM_MAP(CWebPage)
			COM_INTERFACE_ENTRY(IDispatch)
			COM_INTERFACE_ENTRY(IWebPage)
		END_COM_MAP()

		STDMETHOD(get_HostWnd)(LONGLONG* Val);
		STDMETHOD(put_HostWnd)(LONGLONG newVal);
		STDMETHOD(CreateForm)(BSTR bstrKey, LONGLONG hParent, IDispatch** pRetForm);

		void SendChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5);
		CChromeBrowserBase* GetChromeBrowserBase(HWND) override;

		void LoadDocument2Viewport(CString strName, CString strXML);

		CString FindToken(CString pszContent, CString pszDelimiter, int& nStart);

		void HandleChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5);
		void HandleAggregatedMessage(CString strParam1, CString strParam2);
		void HandleSingleMessage(CString strParam);
		void CustomizedDOMElement(CString strRuleName, CString strHTML);
		void CustomizedMainWindowElement(CString strHTML);
		void CustomizedIndWindowElement(CString strHTML);
		void CustomizedWebBrowserElement(CString strHTML);
		void CustomizedNodeDetailsElement(CString strHTML);
		void CustomizedObjectElement(CString strHTML);
		void CustomizedExtraElement(CString strHTML);
		void CustomizedDataElement(CString strHTML);
		void CustomizedDocElement(CString strHTML);
		void OnNTPLoaded();
		void OnWinFormCreated(HWND);
		void OnCloudMsgReceived(CSession*);

	protected:
		ULONG InternalAddRef() { return 1; }
		ULONG InternalRelease() { return 1; }

	private:
		CString m_strDocXml;
		IGrid* GetParentGrid() { return (IGrid*)m_pParentGrid; }
		IGalaxy* GetGalaxy() { return (IGalaxy*)m_pGalaxy; }
		void Show(CString strID);
		void OnFinalMessage(HWND hWnd) override;
		LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnHubbleMsg(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnParentChanged(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnChromeIPCMsgReceived(UINT, WPARAM, LPARAM, BOOL&);
	};
};