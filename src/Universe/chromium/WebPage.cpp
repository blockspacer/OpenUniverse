/********************************************************************************
*					Open Universe - version 1.0.1.10							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
********************************************************************************/

#include "../stdafx.h"
#include "../Hubble.h"
#include "../UniverseApp.h"
#include "../grid.h"
#include "../Wormhole.h"
#include "../Galaxy.h"
#include "../GridHelper.h"
#include "../GridWnd.h"
#include "../Markup.h" 
#include "WebPage.h"
#include "Browser.h"

namespace Web {
	CWebPage::CWebPage() {
		m_pWebWnd = nullptr;
		m_pDevToolWnd = nullptr;
		m_pAppProxy = nullptr;
		m_bDevToolWnd = false;
		m_bWebContentVisible = true;
		m_strCurKey = _T("");
		m_strCurXml = _T("");
		m_strDocXml = _T("");
		m_pParentGrid = nullptr;
		m_strAppProxyID = _T("");
		m_pGalaxyCluster = nullptr;
		m_pGalaxy = nullptr;
		m_hWebHostWnd = m_hExtendWnd = m_hChildWnd = NULL;
		m_pChromeRenderFrameHost = g_pHubble->m_pCreatingChromeRenderFrameHostBase;
		g_pHubble->m_pCreatingChromeRenderFrameHostBase = nullptr;
	}

	CWebPage::~CWebPage() {
	}

	LRESULT CWebPage::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		CBrowser* pParent = nullptr;
		auto it = g_pHubble->m_mapBrowserWnd.find(::GetParent(m_hWnd));
		if (it != g_pHubble->m_mapBrowserWnd.end())
		{
			pParent = (CBrowser*)it->second;
		}
		HWND hPWnd = ::GetParent(pParent->m_hWnd);
		if (hPWnd != NULL)
		{
			g_pHubble->m_pActiveHtmlWnd = this;
			g_pHubble->m_pGalaxy = nullptr;
			g_pHubble->m_bWinFormActived = false;
			g_pHubble->m_hActiveWnd = nullptr;
			LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
			::PostMessage(m_hWnd, WM_COSMOSMSG, 20190331, 1);
			m_pChromeRenderFrameHost->ShowWebPage(true);
			return lRes;
		}
		else
			g_pHubble->m_pActiveHtmlWnd = nullptr;
		return DefWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CWebPage::OnHubbleMsg(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL&) {
		bool bChild = ::GetWindowLongPtr(::GetParent(m_hWnd), GWL_STYLE) & WS_CHILD;
		switch (wParam)
		{
		case 20200429:
		{
			CSession* pSession = (CSession*)lParam;
			pSession->SendMessage();
		}
		break;
		case 20200310:
		{
			CGrid* pGrid = (CGrid*)lParam;
			if (pGrid && pGrid->m_pHubbleCloudSession == nullptr)
			{
				pGrid->m_pHubbleCloudSession = (CWormhole*)((CHubbleImpl*)g_pHubble)->CreateCloudSession(this);
				CWormhole* pSession = pGrid->m_pHubbleCloudSession;
				if (pSession)
				{
					pSession->InsertString(_T("msgID"), IPC_NODE_CREARED_ID);
					pSession->InsertLong(_T("autodelete"), 0);
					pSession->InsertLong(_T("gridtype"), pGrid->m_nViewType);
					pSession->InsertLong(_T("rows"), pGrid->m_nRows);
					pSession->InsertLong(_T("cols"), pGrid->m_nCols);
					pSession->InsertLong(_T("row"), pGrid->m_nRow);
					pSession->InsertLong(_T("col"), pGrid->m_nCol);
					pSession->InsertString(_T("objtype"), pGrid->m_strObjTypeID);
					pSession->InsertString(_T("name@page"), pGrid->m_strName);
					pSession->Insertint64(_T("gridobjhandle"), (__int64)pGrid->m_pHostWnd->m_hWnd);
					pSession->Insertint64(_T("gridobj"), (__int64)(IGrid*)pGrid);
					pSession->Insertint64(_T("Galaxyhandle"), (__int64)pGrid->m_pGridShareData->m_pGalaxy->m_hWnd);
					pSession->Insertint64(_T("rootgridhandle"), (__int64)pGrid->m_pRootObj->m_pHostWnd->m_hWnd);
					pSession->Insertint64(_T("domhandle"), (__int64)pGrid->m_pHubbleCloudSession);
					pSession->InsertString(_T("objID"), _T("wndnode"));
					switch (pGrid->m_nViewType)
					{
					case Grid:
					{
						CGridWnd* pWnd = (CGridWnd*)pGrid->m_pHostWnd;
						for (int i = 0; i < pGrid->m_nRows; i++)
						{
							for (int j = 0; j < pGrid->m_nCols; j++)
							{
								int nIndex = j + i * pGrid->m_nRows;
								CString strIndex = _T("");
								strIndex.Format(_T("%d"), nIndex);
								pSession->Insertint64(strIndex, (__int64)::GetDlgItem(pWnd->m_hWnd, pWnd->IdFromRowCol(i, j)));
							}
						}
					}
					break;
					case TabGrid:
					{
						for (auto it : pGrid->m_vChildNodes)
						{
							CString strIndex = _T("");
							strIndex.Format(_T("%d"), it->m_nCol);
							pSession->Insertint64(strIndex, (__int64)it->m_pHostWnd->m_hWnd);
						}
					}
					break;
					default:
						break;
					}
					if (pGrid->m_pParentObj)
					{
						pSession->Insertint64(_T("parentgridhandle"), (__int64)pGrid->m_pParentObj->m_pHostWnd->m_hWnd);
					}
					if (pGrid->m_pParentWinFormWnd)
					{
						pSession->Insertint64(_T("parentFormHandle"), (__int64)pGrid->m_pParentWinFormWnd->m_hWnd);
					}
					if (pGrid->m_pDisp)
					{
						pGrid->m_pHubbleCloudSession->Insertint64(_T("objectdisp"), (__int64)pGrid->m_pDisp);
						if (g_pHubble->m_pCLRProxy)
						{
							g_pHubble->m_pCLRProxy->ConnectGridToWebPage(pGrid, true);
						}
					}
					m_pChromeRenderFrameHost->SendHubbleMessage(pSession->m_pSession);
				}
			}
		}
		break;
		case 20200311:
		{
			if (m_pGalaxy)
			{
				if (m_pGalaxy->m_pWorkGrid->m_pHubbleCloudSession)
				{
					m_pGalaxy->m_pWorkGrid->m_pHubbleCloudSession->InsertString(_T("msgID"), _T("TANGRAMAPP_READY"));
					m_pGalaxy->m_pWorkGrid->m_pHubbleCloudSession->SendMessage();
				}
			}
		}
		break;
		case 20200131:
		{
			if (bChild && m_pChromeRenderFrameHost)
			{
				m_pChromeRenderFrameHost->ShowWebPage(true);
			}
		}
		break;
		case 20200221:
		{
			IPCMsg* pMsg = (IPCMsg*)lParam;
			if (pMsg && m_pChromeRenderFrameHost)
			{
				m_pChromeRenderFrameHost->SendHubbleMessage(pMsg);
				g_pHubble->m_pCurrentIPCMsg = nullptr;
			}
		}
		break;
		case 20200213:
		{
			HWND hWnd = (HWND)lParam;
			if (::IsWindow(hWnd))
			{
				CWinForm* pWnd = (CWinForm*)::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 20190214);
				if (pWnd)
				{
					m_mapSubWinForm[hWnd] = pWnd;
					pWnd->m_pParentHtmlWnd = this;
				}
			}
		}
		break;
		case 20201026:
		{
			HWND hWnd = (HWND)lParam;
			m_vSubForm.push_back(hWnd);
		}
		break;
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CWebPage::OnParentChanged(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL&) {
		if (g_pHubble->m_bChromeNeedClosed == false && lParam) {
			HWND hNewPWnd = (HWND)lParam;
			::GetClassName(hNewPWnd, g_pHubble->m_szBuffer, 256);
			CString strName = CString(g_pHubble->m_szBuffer);
			if (strName.Find(_T("Chrome_WidgetWin_"))==0)
			{
				if (m_hExtendWnd)
				{
					::SetParent(m_hExtendWnd, hNewPWnd);
					::ShowWindow(m_hExtendWnd, SW_HIDE);
				}
				HWND hNewPWnd2 = ::GetParent(m_hWnd);
				bool bNewParent = true;
				if (hNewPWnd != hNewPWnd2)
				{
					hNewPWnd = hNewPWnd2;
					bNewParent = true;
				}
				CBrowser* pBrowserWnd = nullptr;
				auto it = g_pHubble->m_mapBrowserWnd.find(hNewPWnd);
				if (it == g_pHubble->m_mapBrowserWnd.end())
				{
					if (::IsWindowVisible(hNewPWnd)) {
						pBrowserWnd = new CComObject<CBrowser>();
						pBrowserWnd->SubclassWindow(hNewPWnd);
						g_pHubble->m_mapBrowserWnd[hNewPWnd] = pBrowserWnd;
						pBrowserWnd->m_pBrowser = g_pHubble->m_pActiveBrowser;
						if (pBrowserWnd->m_pBrowser)
							pBrowserWnd->m_pBrowser->m_pProxy = pBrowserWnd;
						if (pBrowserWnd && m_hExtendWnd) {
							::SetParent(m_hExtendWnd, hNewPWnd);
							if (::IsWindowVisible(m_hWnd)) {
								pBrowserWnd->m_pVisibleWebWnd = this;
								if (bNewParent)
								{
									::PostMessageW(hNewPWnd, WM_BROWSERLAYOUT, 0, 4);
								}
							}
						}
					}
				}
			}
			else
			{
				if (m_hExtendWnd)
				{
					::SetParent(m_hExtendWnd, hNewPWnd);
					::ShowWindow(m_hExtendWnd, SW_SHOW);
				}
				HWND hNewPWnd2 = ::GetParent(m_hWnd);
				bool bNewParent = false;
				if (hNewPWnd != hNewPWnd2)
				{
					hNewPWnd = hNewPWnd2;
					bNewParent = true;
				}
				CBrowser* pChromeBrowserWnd = nullptr;
				auto it = g_pHubble->m_mapBrowserWnd.find(hNewPWnd);
				if (it != g_pHubble->m_mapBrowserWnd.end())
				{
					pChromeBrowserWnd = (CBrowser*)it->second;
					g_pHubble->m_pActiveBrowser = pChromeBrowserWnd->m_pBrowser;
					if (pChromeBrowserWnd && m_hExtendWnd) {
						if (::IsWindowVisible(m_hWnd)) {
							pChromeBrowserWnd->m_pVisibleWebWnd = this;
							if (bNewParent)
							{
								g_pHubble->m_pActiveBrowser->m_pProxy = pChromeBrowserWnd;
								pChromeBrowserWnd->BrowserLayout();
								::PostMessageW(hNewPWnd, WM_BROWSERLAYOUT, 0, 2);
							}
						}
					}
					if (pChromeBrowserWnd->m_pVisibleWebWnd && m_bDevToolWnd && pChromeBrowserWnd->m_pVisibleWebWnd->m_bDevToolWnd == false)
					{
						if (pChromeBrowserWnd->m_pVisibleWebWnd->m_pDevToolWnd == nullptr)
						{
							pChromeBrowserWnd->m_pVisibleWebWnd->m_pDevToolWnd = this;
							::ShowWindow(m_hWnd, SW_SHOW);
						}
					}
				}
				else
				{
					if (::IsWindowVisible(hNewPWnd)) {
						pChromeBrowserWnd = new CComObject<CBrowser>();
						pChromeBrowserWnd->SubclassWindow(hNewPWnd);
						g_pHubble->m_mapBrowserWnd[hNewPWnd] = pChromeBrowserWnd;
						pChromeBrowserWnd->m_pBrowser = g_pHubble->m_pActiveBrowser;
						pChromeBrowserWnd->m_pBrowser->m_pProxy = pChromeBrowserWnd;
						if (pChromeBrowserWnd && m_hExtendWnd) {
							::SetParent(m_hExtendWnd, hNewPWnd);
							if (::IsWindowVisible(m_hWnd)) {
								pChromeBrowserWnd->m_pVisibleWebWnd = this;
								if (bNewParent)
								{
									pChromeBrowserWnd->BrowserLayout();
									::PostMessageW(hNewPWnd, WM_BROWSERLAYOUT, 0, 2);
								}
							}
						}
					}
				}
			}
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CWebPage::OnDestroy(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL& /*bHandled*/) {
		if (m_hExtendWnd)
		{
			::DestroyWindow(m_hExtendWnd);
		}
		for (auto it : m_vSubForm)
		{
			::DestroyWindow(it);
		}
		if (g_pHubble->m_pCLRProxy)
			g_pHubble->m_pCLRProxy->OnWebPageCreated(m_hWnd, (CWebPageImpl*)this, (IWebPage*)this, 1);

		m_hExtendWnd = nullptr;

		if (m_bDevToolWnd) {
			if (m_pWebWnd) {
				m_pWebWnd->m_pDevToolWnd = nullptr;
			}
		}
		for (auto it : m_mapSubWinForm)
		{
			::DestroyWindow(it.first);
		}
		m_mapSubWinForm.erase(m_mapSubWinForm.begin(), m_mapSubWinForm.end());
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CWebPage::OnShowWindow(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL&) {
		if (g_pHubble->m_bChromeNeedClosed == false && ::IsWindow(m_hExtendWnd))
		{
			if (wParam) {
				CBrowser* pBrowserWnd = nullptr;
				HWND hPWnd = ::GetParent(m_hWnd);
				if (!m_bDevToolWnd)
				{
					auto it = g_pHubble->m_mapBrowserWnd.find(hPWnd);
					if (it != g_pHubble->m_mapBrowserWnd.end()) {
						pBrowserWnd = (CBrowser*)it->second;
						pBrowserWnd->m_pVisibleWebWnd = this;
					}
				}
				::ShowWindow(m_hExtendWnd, SW_SHOW);
				::SetParent(m_hExtendWnd, hPWnd);
				if (pBrowserWnd)
				{
					m_pChromeRenderFrameHost->ShowWebPage(true);
					pBrowserWnd->BrowserLayout();
					::PostMessage(hPWnd, WM_BROWSERLAYOUT, 0, 2);
				}
				//for (auto it : m_mapSubWinForm)
				//{
				//	::ShowWindow(it.first,SW_SHOW);
				//}
			}
			else
			{
				::SetParent(m_hExtendWnd, m_hWnd);
				//for (auto it : m_mapSubWinForm)
				//{
				//	::ShowWindow(it.first, SW_HIDE);
				//}
			}
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	void CWebPage::OnFinalMessage(HWND hWnd) {
		CBrowser* pPWnd = nullptr;
		auto it2 = g_pHubble->m_mapBrowserWnd.find(::GetParent(hWnd));
		if (it2 != g_pHubble->m_mapBrowserWnd.end())
		{
			pPWnd = (CBrowser*)it2->second;
			if (pPWnd->m_pVisibleWebWnd == this)
				pPWnd->m_pVisibleWebWnd = nullptr;
		}
		auto it = g_pHubble->m_mapHtmlWnd.find(hWnd);
		if (it != g_pHubble->m_mapHtmlWnd.end())
		{
			g_pHubble->m_mapHtmlWnd.erase(it);
		}
		if (g_pHubble->m_pActiveHtmlWnd == this)
			g_pHubble->m_pActiveHtmlWnd = nullptr;
		CWindowImpl::OnFinalMessage(hWnd);
		delete this;
	}

	void CWebPage::SendChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5)
	{
		if (m_pChromeRenderFrameHost != nullptr)
		{
			IPCMsg pIPCInfo;
			pIPCInfo.m_strId = strId;
			pIPCInfo.m_strParam1 = strParam1;
			pIPCInfo.m_strParam2 = strParam2;
			pIPCInfo.m_strParam3 = strParam3;
			pIPCInfo.m_strParam4 = strParam4;
			pIPCInfo.m_strParam5 = strParam5;
			m_pChromeRenderFrameHost->SendHubbleMessage(&pIPCInfo);
		}
		g_pHubble->m_pCurrentIPCMsg = nullptr;
	}

	LRESULT CWebPage::OnChromeIPCMsgReceived(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (lParam)
		{
			if (wParam && m_pChromeRenderFrameHost == nullptr)
			{
				m_pChromeRenderFrameHost = (CChromeRenderFrameHost*)wParam;
				m_pChromeRenderFrameHost->m_pProxy = this;
			}
			IPCMsg* pIPCInfo = (IPCMsg*)lParam;
			CString strId = pIPCInfo->m_strId;
			CString strParam1 = pIPCInfo->m_strParam1;
			CString strParam2 = pIPCInfo->m_strParam2;
			CString strParam3 = pIPCInfo->m_strParam3;
			CString strParam4 = pIPCInfo->m_strParam4;
			CString strParam5 = pIPCInfo->m_strParam5;
			if (pIPCInfo->m_strId.CompareNoCase(_T("TANGRAM_CTRL_VALUE_MESSAGE")) == 0)
			{
				HWND hCtrl = (HWND)pIPCInfo->m_nHandleFrom;
				if (g_pHubble->m_pCLRProxy)
				{
					IDispatch* pCtrl = g_pHubble->m_pCLRProxy->GetCtrlFromHandle(hCtrl);
					if (pCtrl)
						g_pHubble->m_pCLRProxy->SetCtrlValueByName(pCtrl, CComBSTR(""), true, CComBSTR(strParam4));
				}
			}
			else
			{
				HandleChromeIPCMessage(strId, strParam1, strParam2, strParam3, strParam4, strParam5);
			}
			g_pHubble->m_pCurrentIPCMsg = nullptr;
		}

		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	CChromeBrowserBase* CWebPage::GetChromeBrowserBase(HWND hHostWnd)
	{
		return nullptr;
	}

	void CWebPage::LoadDocument2Viewport(CString strName, CString strXML)
	{
		HWND hPPWnd = ::GetParent(::GetParent(m_hWnd));
		if (m_hExtendWnd == nullptr)
		{
			HWND hParent = NULL;
			if (::IsWindowVisible(m_hWnd))
				hParent = ::GetParent(m_hWnd);
			else
				hParent = m_hWnd;

			m_hExtendWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, hParent, NULL, theUniverse.m_hInstance, NULL);
			m_hChildWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, m_hExtendWnd, (HMENU)1, theUniverse.m_hInstance, NULL);

			::SetWindowLongPtr(m_hExtendWnd, GWLP_USERDATA, (LONG_PTR)m_hChildWnd);
			::SetWindowLongPtr(m_hChildWnd, GWLP_USERDATA, (LONG_PTR)this);
		}
		if (m_hExtendWnd)
		{
			if (m_pGalaxy == nullptr) {
				CGalaxyCluster* pGalaxyCluster = nullptr;
				auto it = g_pHubble->m_mapWindowPage.find(m_hExtendWnd);
				if (it != g_pHubble->m_mapWindowPage.end())
					pGalaxyCluster = (CGalaxyCluster*)it->second;
				else
				{
					pGalaxyCluster = new CComObject<CGalaxyCluster>();
					pGalaxyCluster->m_hWnd = m_hExtendWnd;
					g_pHubble->m_mapWindowPage[m_hExtendWnd] = pGalaxyCluster;
				}
				if (pGalaxyCluster) {
					IGalaxy* pGalaxy = nullptr;
					pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)m_hChildWnd), CComBSTR("default"), &pGalaxy);
					if (pGalaxy)
					{
						m_pGalaxy = (CGalaxy*)pGalaxy;
						m_pGalaxy->m_pWebPageWnd = this;
					}
				}
			}
			if (m_pGalaxy)
			{
				IGrid* pGrid = nullptr;
				m_pGalaxy->Observe(CComBSTR(strName), CComBSTR(strXML), &pGrid);
				if (pGrid)
				{
					m_strCurKey = strName;
					m_hWebHostWnd = NULL;
					if (m_pGalaxy->m_pBindingGrid)
					{
						m_hWebHostWnd = m_pGalaxy->m_pBindingGrid->m_pHostWnd->m_hWnd;
					}
				}
			}
		}
		CBrowser* pBrowserWnd = nullptr;
		auto it = g_pHubble->m_mapBrowserWnd.find(::GetParent(m_hWnd));
		if (it != g_pHubble->m_mapBrowserWnd.end())
		{
			pBrowserWnd = (CBrowser*)it->second;
			HWND hActive = NULL;
			hActive = pBrowserWnd->m_pBrowser->GetActiveWebContentWnd();
			if (hActive != m_hWnd)
				::ShowWindow(m_hWnd, SW_HIDE);
		}
		if (::IsWindowVisible(m_hWnd))
		{
			::SendMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 2);
			::PostMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 2);
		}
	}

	void CWebPage::HandleChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5)
	{
		if (strId.CompareNoCase(_T("RENDER_ELEMENT")) == 0)
		{
			//m_strCurKey = strParam1;
			//m_strCurXml = strParam2;
			//::PostMessage(m_hWnd, WM_COSMOSMSG, 20201013, 0);
			CustomizedDOMElement(strParam1, strParam2);
		}
		else if (strId.CompareNoCase(_T("AGGREGATED_MESSAGE")) == 0)
		{
			HandleAggregatedMessage(strParam1, strParam2);
			if (m_strDocXml != _T(""))
			{
				CustomizedDocElement(m_strDocXml);
			}
			if (g_pHubble->m_pHostHtmlWnd == nullptr && g_pHubble->m_strAppXml != _T(""))
			{
				g_pHubble->m_pHostHtmlWnd = this;
				g_pHubble->HubbleInitFromeWeb();
				if (g_pHubble->m_strMainWndXml != _T(""))
				{
					CustomizedMainWindowElement(g_pHubble->m_strMainWndXml);
				}
				auto t = create_task([this]()
					{
						SleepEx(200, true);
						try
						{
							::PostMessage(::GetParent(m_hWnd), WM_COSMOSMSG, 20200214, 0);
						}
						catch (...)
						{
							ATLASSERT(false);
							return 0;
						}
						return 1;
					});
			}
		}
		else if (strId.CompareNoCase(_T("NEW_TAB_PAGE_LOADED")) == 0)
		{
			OnNTPLoaded();
		}
		else if (strId.CompareNoCase(_T("TANGRAM_UI_MESSAGE")) == 0)
		{ 
			// Param1: Frame Name, Param2: HWND, Param3: XML, Param4: BindWebObj Name
			HWND hWndSource = (HWND)_wtoi(LPCTSTR(strParam2));
			if (::IsWindow(hWndSource))
			{
				auto it = m_mapWinForm.find(hWndSource);
				if (it != m_mapWinForm.end())
				{
					LoadDocument2Viewport(strParam1, strParam3);
					return;
				}
				LRESULT lRes = ::SendMessage(hWndSource, WM_HUBBLE_DATA, 0, 20190214);
				if (lRes)
				{
					LoadDocument2Viewport(strParam2, strParam3);
					return;
				}
			}
			auto it = m_mapBindWebObj.find(strParam4);
			if (it != m_mapBindWebObj.end())
			{
				BindWebObj* pObj = it->second;
				HWND hWnd = pObj->m_hWnd;
				switch (pObj->nType)
				{
				case 0:
				{
					IGalaxy* pGalaxy = nullptr;
					IGrid* pGrid = nullptr;
					m_pGalaxy->m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((__int64)hWnd), CComBSTR(pObj->m_strBindObjName), &pGalaxy);
					pGalaxy->Observe(CComBSTR(strParam1 + _T("_") + strParam4), CComBSTR(strParam3), &pGrid);
					if (pGrid)
					{
						CGrid* _pGrid = (CGrid*)pGrid;
						_pGrid->m_strLastIPCMsgID = strId;
						_pGrid->m_strLastIPCParam1 = strParam1;
						_pGrid->m_strLastIPCParam2 = strParam2;
						_pGrid->m_strLastIPCParam3 = strParam3;
						_pGrid->m_strLastIPCParam4 = strParam4;
						_pGrid->m_strLastIPCParam5 = strParam5;
					}
				}
				break;
				case 1:
				{
					CGrid* _pGrid = (CGrid*)pObj->m_pGrid;
					IGrid* pGrid = nullptr;
					_pGrid->Observe(CComBSTR(strParam1 + _T("_") + strParam4), CComBSTR(strParam3), &pGrid);
					if (pGrid)
					{
						CGrid* _pGrid = (CGrid*)pGrid;
						if (_pGrid->m_strLastIPCMsgID == _T("") || _pGrid->m_strLastIPCMsgID != strId)
						{
							_pGrid->m_strLastIPCMsgID = strId;
							_pGrid->m_strLastIPCParam1 = strParam1;
							_pGrid->m_strLastIPCParam2 = strParam2;
							_pGrid->m_strLastIPCParam3 = strParam3;
							_pGrid->m_strLastIPCParam4 = strParam4;
							_pGrid->m_strLastIPCParam5 = strParam5;
						}
						else if (_pGrid->m_strLastIPCMsgID == strId)
						{
							IPCMsg pIPCInfo;
							pIPCInfo.m_strId = strId;
							pIPCInfo.m_strParam1 = _pGrid->m_strLastIPCParam1;
							pIPCInfo.m_strParam2 = _pGrid->m_strLastIPCParam2;
							pIPCInfo.m_strParam3 = _pGrid->m_strLastIPCParam3;
							pIPCInfo.m_strParam4 = _pGrid->m_strLastIPCParam4;
							pIPCInfo.m_strParam5 = _pGrid->m_strLastIPCParam5;
							_pGrid->m_strLastIPCMsgID = _T("");
							m_pChromeRenderFrameHost->SendHubbleMessage(&pIPCInfo);
						}
						g_pHubble->m_pCurrentIPCMsg = nullptr;
					}
				}
				break;
				}
				return;
			}
			else
			{ 
				LoadDocument2Viewport(strParam1, strParam3);
			}
		}
		//else if (strId.CompareNoCase(_T("TO_TOPFRAME")) == 0)
		//{
		//	LoadDocument2Viewport(strParam1, strParam2);
		//	return;
		//}
		//else if (strId.CompareNoCase(_T("TO_PARENTNODE")) == 0)
		//{
		//	if (m_pParentGrid == nullptr)
		//	{
		//		HWND hWnd = ::GetParent(::GetParent(m_hWnd));
		//		if (::IsWindow(hWnd))
		//		{
		//			LRESULT lRes = ::SendMessage(hWnd, WM_HUBBLE_GETNODE, 0, 0);
		//			HWND _hWnd = (HWND)hWnd;
		//			if (lRes)
		//				m_pParentGrid = (CGrid*)lRes;
		//		}
		//		else
		//		{
		//			m_pParentGrid = m_pGalaxy->m_pBindingGrid->m_pParentObj;
		//			if (m_pParentGrid && m_pParentGrid->m_nViewType == TabGrid)
		//			{
		//				//IGrid* _pGrid = nullptr;
		//				//m_pGalaxy->m_pBindingGrid->Observe(CComBSTR(strParam2), CComBSTR(strParam1), &_pGrid);
		//				return;
		//			}
		//		}
		//	}
		//	if (m_pParentGrid)
		//	{
		//		IGrid* _pGrid = nullptr;
		//		m_pParentGrid->Observe(CComBSTR(strParam1), CComBSTR(strParam2), &_pGrid);
		//	}
		//	return;
		//}
		else
		{
			if (g_pHubble->m_pHubbleDelegate)
				g_pHubble->m_pHubbleDelegate->IPCMsg(m_hWnd, strId, strParam1, strParam2); // TODO: Missing parameters
		}
	}

	void CWebPage::HandleAggregatedMessage(CString strParam1, CString strParam2)
	{
		int nTokenPos = 0;
		CString strToken = strParam1.Tokenize(_T("$$$"), nTokenPos);
		while (!strToken.IsEmpty())
		{
			// Process a single message.
			HandleSingleMessage(strToken);
			strToken = strParam1.Tokenize(_T("$$$"), nTokenPos);
		}
	}

	CString CWebPage::FindToken(CString pszContent, CString pszDelimiter, int& nStart)
	{
		if (nStart == -1) {
			return _T("");
		}
		int nNextDelimiter = pszContent.Find(pszDelimiter, nStart);
		if (nNextDelimiter == -1)
		{
			CString pszToken = pszContent.Mid(nStart);
			nStart = -1;
			return pszToken;
		}
		else
		{
			int nCount = nNextDelimiter - nStart;
			CString pszToken = pszContent.Mid(nStart, nCount);
			nStart = nNextDelimiter + pszDelimiter.GetLength();
			if (nStart >= pszContent.GetLength()) {
				nStart = -1;
			}
			return pszToken;
		}
	}

	void CWebPage::HandleSingleMessage(CString strParam)
	{
		int nStart = 0;
		CString strId = FindToken(strParam, _T("%%%"), nStart);
		CString strParam1 = FindToken(strParam, _T("%%%"), nStart);
		CString strParam2 = FindToken(strParam, _T("%%%"), nStart);
		CString strParam3 = FindToken(strParam, _T("%%%"), nStart);
		CString strParam4 = FindToken(strParam, _T("%%%"), nStart);
		CString strParam5 = FindToken(strParam, _T("%%%"), nStart);
		HandleChromeIPCMessage(strId, strParam1, strParam2, strParam3, strParam4, strParam5);
	}

	void CWebPage::CustomizedDOMElement(CString strRuleName, CString strHTML)
	{
		if (strRuleName.CompareNoCase(_T("application")) == 0)
		{
			if (g_pHubble->m_strAppXml == _T(""))
			{
				g_pHubble->m_strAppXml = strHTML;
			}
		}
		else if (strRuleName.CompareNoCase(_T("mainWindow")) == 0)
		{
			if (g_pHubble->m_strMainWndXml == _T(""))
				g_pHubble->m_strMainWndXml = strHTML;
		}
		else if (strRuleName.CompareNoCase(_T("webBrowser")) == 0)
		{
			CustomizedWebBrowserElement(strHTML);
		}
		else if (strRuleName.CompareNoCase(_T("cloud-form")) == 0)
		{
			CustomizedIndWindowElement(strHTML);
		}
		else if (strRuleName.CompareNoCase(_T("gridTemplate")) == 0)
		{
			CustomizedNodeDetailsElement(strHTML);
		}
		else if (strRuleName.CompareNoCase(_T("object")) == 0)
		{
			CustomizedObjectElement(strHTML);
		}
		else if (strRuleName.CompareNoCase(_T("data")) == 0)
		{
			CustomizedDataElement(strHTML);
		}
		else if (strRuleName.CompareNoCase(_T("webViewPort")) == 0)
		{
			m_strDocXml = strHTML;
		}
		else if (strRuleName.CompareNoCase(_T("extra")) == 0)
		{
			CustomizedExtraElement(strHTML);
		}
		else
		{
			g_pHubble->m_pHubbleDelegate->CustomizedDOMElement(m_hWnd, strRuleName, strHTML);
		}
	}

	void CWebPage::CustomizedMainWindowElement(CString strHTML)
	{
		CTangramXmlParse xmlParse;
		if (xmlParse.LoadXml(strHTML))
		{
			if (g_pHubble->m_pCLRProxy == nullptr)
				g_pHubble->LoadCLR();
			if (g_pHubble->m_pCLRProxy)
			{
				CWebPageImpl* pChromeRenderFrameHostProxyBase = (CWebPageImpl*)this;
				xmlParse.put_attr(_T("renderframehostproxy"), (__int64)pChromeRenderFrameHostProxyBase);
				IDispatch* pDisp = g_pHubble->m_pCLRProxy->CreateCLRObj(xmlParse.xml());
			}
		}
	}

	void CWebPage::CustomizedIndWindowElement(CString strHTML)
	{
		CTangramXmlParse xmlParse;
		if (xmlParse.LoadXml(strHTML))
		{
			if (g_pHubble->m_pCLRProxy == nullptr)
				g_pHubble->LoadCLR();
			if (g_pHubble->m_pCLRProxy)
			{
				CWebPageImpl* pProxyBase = (CWebPageImpl*)this;
				xmlParse.put_attr(_T("renderframehostproxy"), (__int64)pProxyBase);
				IWebPage* pChromeWebPage = (IWebPage*)this;
				xmlParse.put_attr(_T("webpage"), (__int64)pChromeWebPage);
				xmlParse.put_attr(_T("webpagehandle"), (__int64)m_hWnd);
				IDispatch* pDisp = g_pHubble->m_pCLRProxy->CreateCLRObj(xmlParse.xml());
			}
		}
	}

	void CWebPage::CustomizedWebBrowserElement(CString strHTML)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(strHTML))
		{
			CString strUrls = _T("");
			int nCount = m_Parse.GetCount();
			if (nCount > 0)
			{
				for (int i = 0; i < nCount; i++)
				{
					CString strUrl = m_Parse.GetChild(i)->val();
					int nPos2 = strUrl.Find(_T(":"));
					if (nPos2 != -1)
					{
						CString strURLHeader = strUrl.Left(nPos2);
						if (strURLHeader.CompareNoCase(_T("host")) == 0)
						{
							strUrl = g_pHubble->m_strAppPath + strUrl.Mid(nPos2 + 1);
						}
					}
					strUrls = strUrls + strUrl + _T("|");
				}
			}
			else
			{
				strUrls = "chrome://newtab|";
			}

			g_pHubble->m_pBrowserFactory->CreateBrowser(NULL, strUrls);
		}
	}

	void CWebPage::CustomizedNodeDetailsElement(CString strHTML)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(strHTML))
		{
			CString strType = m_Parse.attr(_T("type"), _T(""));
			CString strUiKey = m_Parse.attr(_T("uikey"), _T("")).MakeLower();
			if (strUiKey != _T(""))
			{
				if (strType.CompareNoCase(_T("winform")) == 0)
				{
					m_mapFormsInfo[strUiKey] = m_Parse.xml();
				}
				else if (strType.CompareNoCase(_T("usercontrol")) == 0)
				{
					m_mapUserControlsInfo[strUiKey] = m_Parse.xml();
				}
			}
		}
	}

	void CWebPage::CustomizedObjectElement(CString strHTML)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(strHTML))
		{
			CString strType = m_Parse.attr(_T("type"), _T(""));
			if (strType.CompareNoCase(_T("winform")) == 0)
			{
				CString strStartup = _T("");
				CString strID = m_Parse.attr(_T("uikey"), _T("")).MakeLower();
				CString strName = m_Parse.name();
				if (strID != _T(""))
				{
					m_mapFormsInfo[strID] = m_Parse.xml();
				}
				if (m_Parse.attrBool(_T("showstartup")))
				{
					strStartup = strID;
				}
				if (strStartup != _T(""))
				{
					CString strID = strStartup.MakeLower();
					if (g_pHubble->m_pCLRProxy == nullptr)
						g_pHubble->LoadCLR();
					if (g_pHubble->m_pCLRProxy)
					{
						auto it = m_mapFormsInfo.find(strID);
						if (it != m_mapFormsInfo.end())
						{
							IDispatch* pDisp = g_pHubble->m_pCLRProxy->CreateCLRObj(it->second);
							HWND hwnd = g_pHubble->m_pCLRProxy->GetCtrlHandle(pDisp);
							if (hwnd)
							{
								CWinForm* pForm = (CWinForm*)::SendMessage(hwnd, WM_HUBBLE_DATA, 0, 20190214);
								if (pForm)
								{
									pForm->m_pOwnerHtmlWnd = this;
								}
							}
						}
					}
				}
			}
			else if (strType.CompareNoCase(_T("usercontrol")) == 0)
			{
				CString strID = m_Parse.attr(_T("uikey"), _T("")).MakeLower();
				if (strID != _T(""))
				{
					m_mapUserControlsInfo[strID] = m_Parse.xml();
				}
			}
			else if (strType.CompareNoCase(_T("browser")) == 0)
			{
				CString strUrls = _T("");
				int nCount = m_Parse.GetCount();
				for (int i = 0; i < nCount; i++)
				{
					CString strUrl = m_Parse.GetChild(i)->attr(_T("url"), _T(""));
					int nPos2 = strUrl.Find(_T(":"));
					if (nPos2 != -1)
					{
						CString strURLHeader = strUrl.Left(nPos2);
						if (strURLHeader.CompareNoCase(_T("host")) == 0)
						{
							strUrl = g_pHubble->m_strAppPath + strUrl.Mid(nPos2 + 1);
						}
					}
					strUrls = strUrls + strUrl + _T("|");
				}
				g_pHubble->m_pBrowserFactory->CreateBrowser(NULL, strUrls);
			}
		}
	}

	void CWebPage::CustomizedExtraElement(CString strHTML)
	{
		CMarkup rootXML;
		if (rootXML.SetDoc(strHTML) && rootXML.FindElem())
		{
			CString strTarget = rootXML.GetAttrib(_T("target"));
			if (strTarget.CompareNoCase(_T("ntp")) == 0)
			{
				g_pHubble->m_strNtpXml = strHTML;
			}
		}
	}

	void CWebPage::CustomizedDataElement(CString strHTML)
	{
		CMarkup rootXML;
		if (rootXML.SetDoc(strHTML) && rootXML.FindElem())
		{
			CString strKey = rootXML.GetAttrib(_T("key"));
			g_pHubble->m_mapValInfo[strKey] = CComVariant(strHTML);
		}
	}

	void CWebPage::CustomizedDocElement(CString strHTML)
	{
		CMarkup rootXML;
		if (rootXML.SetDoc(strHTML) && rootXML.FindElem())
		{
			CString strName = rootXML.GetAttrib(_T("name"));
			if (strName == _T(""))
			{
				strName = _T("__VIEWPORT_DEFAULT__");
			}
			LoadDocument2Viewport(strName, strHTML);
			::PostMessage(m_hWnd, WM_COSMOSMSG, 20200311, 0);
		}
	}

	void CWebPage::OnNTPLoaded()
	{
		if (g_pHubble->m_strNtpXml != _T(""))
		{
			LoadDocument2Viewport(_T("__NTP_DEFAULT__"), g_pHubble->m_strNtpXml);
		}
	}

	void CWebPage::OnWinFormCreated(HWND hwnd)
	{
		if (hwnd)
		{
			LRESULT l = ::SendMessage(hwnd, WM_HUBBLE_DATA, 0, 20190214);
			if (l == 0)
			{
				CWinForm* pWnd = new CWinForm();
				g_pHubble->m_hFormNodeWnd = NULL;
				g_pHubble->m_hFormNodeWnd = (HWND)hwnd;
				pWnd->SubclassWindow(hwnd);
				pWnd->m_pOwnerHtmlWnd = this;
				g_pHubble->m_mapFormWebPage[hwnd] = this;
				m_mapWinForm[hwnd] = pWnd;
				::PostMessage(g_pHubble->m_hFormNodeWnd, WM_WINFORMCREATED, 0, 0);
			}
		}
	}

	void CWebPage::OnCloudMsgReceived(CSession* pSession)
	{
		CString strMsgID = pSession->GetString(L"msgID");
		IGrid* pGrid = (IGrid*)pSession->Getint64(_T("gridobj"));
		if (pGrid)
		{
			if(((CGrid*)pGrid)->m_pHubbleCloudSession==nullptr) 
				((CGrid*)pGrid)->m_pHubbleCloudSession = (CWormhole*)pSession;
		}
		if (strMsgID == _T("CREATE_WINFORM"))
		{
			CString strFormXml = pSession->GetString(_T("formXml"));
			pSession->InsertString(_T("formXml"), _T(""));
			// formType: 0, normal form; 1 subform; 2: dlgform
			long formType = pSession->GetLong(_T("formType"));
			CTangramXmlParse xmlParse;
			if (xmlParse.LoadXml(strFormXml))
			{
				if (g_pHubble->m_pCLRProxy == nullptr)
					g_pHubble->LoadCLR();
				if (g_pHubble->m_pCLRProxy)
				{
					CWebPageImpl* pProxyBase = (CWebPageImpl*)this;
					xmlParse.put_attr(_T("renderframehostproxy"), (__int64)pProxyBase);
					xmlParse.put_attr(_T("ipcsession"), (__int64)pSession);
					pSession->Insertint64(_T("domhandle"), (__int64)pSession);
					pSession->InsertLong(_T("autodelete"), 0);
					IWebPage* pChromeWebPage = (IWebPage*)this;
					xmlParse.put_attr(_T("webpage"), (__int64)pChromeWebPage);
					xmlParse.put_attr(_T("webpagehandle"), (__int64)m_hWnd);
					if (formType == 2)
					{
						xmlParse.put_attr(_T("model"), 1);
					}
					pSession->m_pOwner = this;
					IDispatch* pDisp = g_pHubble->m_pCLRProxy->CreateCLRObj(xmlParse.xml());
				}
			}
		}
		else if(strMsgID == _T("OPEN_URL"))
		{ 
			CString strPath = g_pHubble->m_strAppPath;
			CString strUrl = pSession->GetString(_T("openurl"));
			long nPos = pSession->GetLong(_T("BrowserWndOpenDisposition"));
			int _nPos = strUrl.Find(_T("host:"));
			if (nPos==2&&_nPos != -1)
				strUrl.Replace(_T("host:"), strPath);
			else
			{
				strUrl.Replace(_T("host:"), strPath);
				strUrl += _T("|");
				strUrl.Replace(_T("||"), _T("|"));
			}

			if (m_pChromeRenderFrameHost)
			{
				if (nPos > 0x000a || nPos == 0)
				{
					g_pHubble->m_pBrowserFactory->CreateBrowser(0, strUrl);
				}
				else
				{
					IPCMsg msg;
					msg.m_strId = L"OPEN_URL";
					msg.m_strParam1 = strUrl;
					CString strDisposition = _T("");
					strDisposition.Format(_T("%d"), nPos);
					msg.m_strParam2 = strDisposition;
					m_pChromeRenderFrameHost->SendHubbleMessage(&msg);
				}
			}
		}
		else if (strMsgID == _T("OPEN_XML"))
		{
			CString strKey = pSession->GetString(_T("openkey"));
			CString strXml = pSession->GetString(_T("openxml"));
			//IGrid* pGrid = nullptr;
			HWND hWnd = (HWND)pSession->Getint64(_T("gridobjhandle"));
			CGrid* pParent = (CGrid*)::SendMessage(hWnd, WM_HUBBLE_GETNODE, 0, 0);
			if (pParent)
			{
				IGrid* _pGrid = nullptr;
				pParent->Observe(CComBSTR(strKey), CComBSTR(strXml), &_pGrid);
				if (_pGrid)
				{
					__int64 nHandle = 0;
					_pGrid->get_Handle(&nHandle);
					pSession->Insertint64(_T("openxmlreturnhandle"), nHandle);
					::PostMessage(m_hWnd, WM_COSMOSMSG, 20200429, (LPARAM)pSession);
				}
			}
		}
		else if (strMsgID == _T("OPEN_XML_SPLITTER"))
		{
			CString strKey = pSession->GetString(_T("openkey"));
			CString strXml = pSession->GetString(_T("openxml"));
			int nCol = pSession->GetLong(_T("opencol"));
			int nRow = pSession->GetLong(_T("openrow"));
			IGrid* pSplitterNode = nullptr;
			HWND hWnd = (HWND)pSession->Getint64(_T("gridobjhandle"));
			CGrid* pParent = (CGrid*)::SendMessage(hWnd, WM_HUBBLE_GETNODE, 0, 0);
			if (pParent)
			{
				IGrid* pGrid = nullptr;
				pParent->ObserveEx(nRow, nCol, CComBSTR(strKey), CComBSTR(strXml), &pGrid);
				if (pGrid)
				{
					__int64 nHandle = 0;
					pGrid->get_Handle(&nHandle);
					pSession->Insertint64(_T("openxmlreturnhandle"), nHandle);
					::PostMessage(m_hWnd, WM_COSMOSMSG, 20200429, (LPARAM)pSession);
				}
			}
		}
		else if (strMsgID == _T("OPEN_XML_CTRL"))
		{
			CString strName = pSession->GetString(_T("ctrlName"));
			CString strKey = pSession->GetString(_T("openkey"));
			CString strXml = pSession->GetString(_T("openxml"));
		}
	}

	STDMETHODIMP CWebPage::get_HostWnd(LONGLONG* Val)
	{
		*Val = (LONGLONG)m_hWebHostWnd;
		return S_OK;
	}

	STDMETHODIMP CWebPage::put_HostWnd(LONGLONG newVal)
	{
		HWND hWnd = (HWND)newVal;
		if (::IsWindow(hWnd))
			m_hWebHostWnd = hWnd;
		return S_OK;
	}

	STDMETHODIMP CWebPage::CreateForm(BSTR bstrKey, LONGLONG hParent, IDispatch** pRetForm)
	{
		if (g_pHubble->m_pCLRProxy == nullptr)
			g_pHubble->LoadCLR();
		if (hParent == 0)
			hParent = (__int64)m_hWnd;
		CString strKey = OLE2T(bstrKey);
		auto it = this->m_mapFormsInfo.find(strKey.MakeLower());
		if (it != m_mapFormsInfo.end())
		{
			*pRetForm = g_pHubble->m_pCLRProxy->CreateWinForm((HWND)hParent, CComBSTR(it->second));
		}
		return S_OK;
	}
}  // namespace Universe
