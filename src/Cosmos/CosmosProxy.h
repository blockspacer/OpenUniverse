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

// CosmosProxy.h : Declaration of the CCosmosProxy

#pragma once

#include "resource.h"       // main symbols
#include "TangramBase.h"       
#include "Browser.h"

using namespace Cosmos;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::Collections;

class CMenuHelperWnd :
	public CWindowImpl<CMenuHelperWnd, CWindow>
{
public:
	CMenuHelperWnd(void)
	{
	};

	~CMenuHelperWnd(void) {};
	HWND m_hOwner = nullptr;
	gcroot<ToolStripDropDownMenu^> m_pToolStripDropDownMenu = nullptr;
	BEGIN_MSG_MAP(CHelperWnd)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_CHAR, OnSysKeyDown)
	END_MSG_MAP()
	void OnFinalMessage(HWND hWnd);
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSysKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class FormInfo
{
public:
	FormInfo();
	~FormInfo();
	map<int, gcroot<ToolStripMenuItem^>> m_mapShortcutItem;
};

class CCosmosProxy : public IHubbleCLRImpl
{
public: 
	CCosmosProxy();
	virtual~CCosmosProxy();

	bool												m_bInitApp = false;
	HWND												m_hCreatingCLRWnd = nullptr;
	gcroot<String^>										m_strExtendableTypes;
	map<CString, gcroot<Assembly^>>						m_mapAssembly;
	map<HWND, gcroot<MenuStrip^>>						m_mapFormMenuStrip;
	map<HWND, FormInfo*>								m_mapFormInfo;
	map<HWND, gcroot<MenuStrip^>>						m_mapFormMenuStrip2;
	map<HWND, gcroot<WebPage^>>							m_mapChromeWebPage;
	map<HWND, GalaxyInfo*>								m_mapGalaxyInfo;
	map<HWND, CMenuHelperWnd*>							m_mapMenuHelperWnd;
	map<HWND, CMenuHelperWnd*>							m_mapVisibleMenuHelperWnd;
	map<IBrowser*, gcroot<Browser^>>					m_mapWebBrowser;
	map<HWND, CString>									m_mapEventBindInfo;
	map<CSession*, gcroot<Wormhole^>>					m_mapSession2Wormhole;

	CMenuHelperWnd*										m_pWorkingMenuHelperWnd = nullptr;
	gcroot<Form^>										m_pCurrentPForm;
	Object^ _getObject(Object^ key);
	bool _insertObject(Object^ key, Object^ val);
	bool _removeObject(Object^ key);

	template<typename T1, typename T2>
	T2^ _createObject(T1* punk)
	{
		T2^ pRetObj = nullptr;

		if (punk != NULL)
		{
			LONGLONG l = (LONGLONG)punk;
			pRetObj = (T2^)_getObject(l);

			if (pRetObj == nullptr)
			{
				pRetObj = gcnew T2(punk);
				_insertObject(l, pRetObj);
			}
		}
		return pRetObj;
	}

	IDispatch* CreateCLRObj(CString bstrObjID);
	void HubbleAction(BSTR bstrXml, void*);
	void _GetMenuInfo(FormInfo*, ToolStripMenuItem^);
private:
	map<HWND, gcroot<Form^>>				m_mapForm;
	map<HWND, CString>						m_mapUIData;
	gcroot<Hashtable^>						m_htObjects;
	gcroot<EventHandler^>					m_pOnLoad;
	gcroot<EventHandler^>					m_pOnCtrlVisible;

	virtual void OnCLRHostExit();
	CWPFObj* CreateWPFControl(IGrid* pGrid, HWND hPWnd, UINT nID);
	//HRESULT ActiveCLRMethod(BSTR bstrObjID, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData);
	//HRESULT ActiveCLRMethod(IDispatch* pCLRObj, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData);
	HRESULT ProcessCtrlMsg(HWND hCtrl, bool bShiftKey);
	BOOL ProcessFormMsg(HWND hFormWnd, LPMSG lpMsg, int nMouseButton);
	IDispatch* CreateObject(BSTR bstrObjID, HWND hParent, IGrid* pHostNode);
	int IsWinForm(HWND hWnd);
	IDispatch* GetCLRControl(IDispatch* CtrlDisp, BSTR bstrNames);
	BSTR GetCtrlName(IDispatch* pCtrl);
	HWND GetMDIClientHandle(IDispatch* pMDICtrl) { return NULL; };
	IDispatch* GetCtrlByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild);
	HWND GetCtrlHandle(IDispatch* pCtrl);
	BSTR GetCtrlType(IDispatch* pCtrl);
	IDispatch* GetCtrlFromHandle(HWND hWnd);
	HWND IsGalaxy(IDispatch* ctrl);
	void ReleaseHubbleObj(IDispatch*);
	BSTR GetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild);
	void SetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild, BSTR strVal);
	HRESULT NavigateURL(IGrid* pGrid, CString strURL, IDispatch* dispObjforScript);
	Control^ GetCanSelect(Control^ ctrl, bool direct);

	HWND GetHwnd(HWND parent, int x, int y, int width, int height);
	void OnDestroyChromeBrowser(IBrowser*);

	void* Extend(CString strKey, CString strData, CString strFeatures);
	bool IsSupportDesigner();
	HICON GetAppIcon(int nIndex);
	void OnWinFormActivate(HWND, int nState);
	void OnWebPageCreated(HWND, CWebPageImpl*, IWebPage*, int nState);
	void HideMenuStripPopup();
	bool PreWindowPosChanging(HWND hWnd, WINDOWPOS* lpwndpos, int nType);
	void ConnectGridToWebPage(IGrid*, bool);
	void OnCloudMsgReceived(CSession*);

	void WindowCreated(LPCTSTR strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd);
	void WindowDestroy(HWND hWnd);
	//void SelectGrid(IGrid* );
	void InitControl(Form^ pForm, Control^ pCtrl, bool bSave, CTangramXmlParse* pParse);
	void InitGrid(IGrid* pGrid, Control^ pCtrl, bool bSave, CTangramXmlParse* pParse);
	void SetObjectProperty(IDispatch* pObj, BSTR bstrPropertyName, BSTR bstrPropertyValue) {};
	IDispatch* CreateWinForm(HWND hParent, BSTR strXML);

	void CtrlInit(int nType, Control^ treeview, IGalaxyCluster* pGalaxyCluster);
	System::Void LoadNode(TreeView^ pTreeView, TreeNode^ pGrid, IGalaxyCluster* pGalaxyCluster, CTangramXmlParse* pParse);
	static void OnLoad(Object ^sender, EventArgs ^e);
	static void OnApplicationExit(Object ^sender, EventArgs ^e);
	static void OnVisibleChanged(Object ^sender, EventArgs ^e);
	static void OnItemSelectionChanged(Object ^sender, ListViewItemSelectionChangedEventArgs ^e);
	static void OnClick(Object ^sender, EventArgs ^e);
	static void OnTextChanged(System::Object^ sender, System::EventArgs^ e);
};

