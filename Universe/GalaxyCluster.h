/********************************************************************************
*					Open Universe - version 0.9.999								*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* https://www.tangram.dev
********************************************************************************/

// GalaxyCluster.h : CGalaxyCluster implementation file

#pragma once

#include <assert.h>
#include <wininet.h>

// CGalaxyCluster
class ATL_NO_VTABLE CGalaxyCluster : 
	public CComObjectRootBase,
	public IConnectionPointContainerImpl <CGalaxyCluster>,
	public IConnectionPointImpl<CGalaxyCluster, &__uuidof(_IHubbleObjEvents)>,
	public IDispatchImpl<IGalaxyCluster, &IID_IGalaxyCluster, &LIBID_Universe, 1, 0>
{
	friend CHubble;
public:
	CGalaxyCluster();
	virtual ~CGalaxyCluster();

	BOOL										m_bNewVersion;
	HWND										m_hWnd;

	IHubbleAppProxy*							m_pUniverseAppProxy;
	map<CString, HWND>							m_mapWnd;
	map<HWND, CQuasar*>							m_mapQuasar;
	map<CString, CGrid*>						m_mapGrid;

	BEGIN_COM_MAP(CGalaxyCluster)
		COM_INTERFACE_ENTRY(IGalaxyCluster)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CGalaxyCluster)
		CONNECTION_POINT_ENTRY(__uuidof(_IHubbleObjEvents))
	END_CONNECTION_POINT_MAP()

	void Lock(){}
	void Unlock(){}
	void BeforeDestory();

	void OnNodeDocComplete(WPARAM);

protected:
	ULONG InternalAddRef(){ return 1; }
	ULONG InternalRelease(){ return 1; }

public:
	STDMETHOD(get_Count)(long* pCount);
	STDMETHOD(get_Quasar)(VARIANT vIndex, IQuasar ** ppQuasar);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	STDMETHOD(get_Handle)(LONGLONG* pVal);
	STDMETHOD(get_Extender)(BSTR bstrExtenderName, IDispatch** pVal);
	STDMETHOD(put_Extender)(BSTR bstrExtenderName, IDispatch* newVal);
	STDMETHOD(get_QuasarName)(LONGLONG hHwnd, BSTR* pVal);
	STDMETHOD(get_Grid)(BSTR bstrNodeName, IGrid** pVal);
	STDMETHOD(get_GridNames)(BSTR* pVal);
	STDMETHOD(get_XObject)(BSTR bstrName, IDispatch** pVal);
	STDMETHOD(get_Parent)(IGalaxyCluster** pVal);
	STDMETHOD(get_Width)(long* pVal);
	STDMETHOD(put_Width)(long newVal);
	STDMETHOD(get_Height)(long* pVal);
	STDMETHOD(put_Height)(long newVal);
	STDMETHOD(get_xtml)(BSTR strKey, BSTR* pVal);
	STDMETHOD(put_xtml)(BSTR strKey, BSTR newVal);
	STDMETHOD(get_GalaxyClusterXML)(BSTR* pVal);
	STDMETHOD(put_ConfigName)(BSTR newVal);
	STDMETHOD(get_NewVersion)(VARIANT_BOOL* bNewVersion);
	STDMETHOD(put_NewVersion)(VARIANT_BOOL bNewVersion);

	STDMETHOD(CreateQuasar)(VARIANT ParentObj, VARIANT HostWnd, BSTR bstrFrameName, IQuasar** pRetFrame);
	STDMETHOD(GetGrid)(BSTR bstrFrameName, BSTR bstrNodeName, IGrid** pRetNode);
	STDMETHOD(GetQuasarFromCtrl)(IDispatch* ctrl, IQuasar** ppQuasar);
	STDMETHOD(GetCtrlInGrid)(BSTR NodeName, BSTR CtrlName, IDispatch** ppCtrl);
	STDMETHOD(Observe)(IDispatch* Parent, BSTR CtrlName, BSTR FrameName, BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid);
	STDMETHOD(ObserveCtrl)(VARIANT MdiForm, BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid);
	STDMETHOD(ConnectHubbleCtrl)(IHubbleCtrl* eventSource);
	STDMETHOD(CreateQuasarWithDefaultNode)(ULONGLONG hFrameWnd, BSTR bstrFrameName, BSTR bstrDefaultNodeKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfig, IGrid** ppGrid);
	STDMETHOD(ObserveQuasars)(BSTR bstrFrames, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfigFile);
	STDMETHOD(get_CurrentDesignQuasarType)(QuasarType* pVal);
	STDMETHOD(get_CurrentDesignNode)(IGrid** pVal);

private:
	bool										m_bIsBlank;
	bool										m_bIsDestory;
	bool										m_bDocComplete;
};

