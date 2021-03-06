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
* OUTLINED IN THE GPL LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
*
********************************************************************************/
// TangramAddin.cpp : Declaration of the COfficeAddin 

#include "../stdafx.h"
#include "../UniverseApp.h"
#include "../Galaxy.h"
#include "../Grid.h"
#include "../TangramHtmlTreeWnd.h"
#include "../CloudUtilities/DownLoad.h"
//#include "../VisualStudioPlus/VSAddin.h"
#include "fm20.h"
#include "OfficeAddin.h"
#include "excelplus\excel.h"
#include "wordplus\msword.h"
#include "outlookplus\msoutl.h"

#include <io.h>
#include <stdio.h>
using namespace VBIDE;
/*
Private Sub MDIForm_Load()
Set TangramCore = CreateObject("Hubble.hubble")
Dim tangram As Object
Set tangram = TangramCore.CreateGalaxyCluster(Me.hWnd)
Dim frameX As Object
Set frameX = tangram.CreateGalaxy(0, 0, "test")
frameX.Open "", "d:\AppDoc1.APPXml"
End Sub
*/
namespace OfficePlus
{
	// COfficeAddin
	COfficeAddin::COfficeAddin()
	{
		if (m_hCBTHook == NULL)
		{
			m_hCBTHook = SetWindowsHookEx(WH_CBT, CUniverse::CBTProc, NULL, GetCurrentThreadId());
		}
		m_hHostWnd = NULL;
		m_strLib = _T("");
		m_strUser = _T("");
		m_strRibbonXml = _T("");
		m_strRibbonXmlFile = _T("");
		m_bOfficeApp = true;
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = ::DefWindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = theApp.m_hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = NULL;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = L"Tangram Remote Helper Window";
		wcex.hIconSm = NULL;
		RegisterClassEx(&wcex);
		COfficeExtender* pExtender = new CComObject<COfficeExtender>;
		pExtender->m_pAddin = this;
		m_pExtender = pExtender;
	}

	COfficeAddin::~COfficeAddin()
	{
		m_strLib = _T("");
		m_bEnableProcessFormTabKey = false;
		ATLTRACE(_T("**********COfficeAddin::~COfficeAddin: %x*********************\n"), this);
	}

	STDMETHODIMP COfficeAddin::OnConnection(IDispatch *pApplication, AddInDesignerObjects::ext_ConnectMode ConnectMode, IDispatch *pAddInInst, SAFEARRAY ** /*custom*/)
	{
		CComQIPtr<COMAddIn> _pAddInInst(pAddInInst);
		if (_pAddInInst)
			_pAddInInst->put_Object((IHubble*)this);

		if (::IsWindow(m_hHostWnd) == false)
		{
			m_hHostWnd = ::CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW, _T("Hubble Grid Class"), m_strDesignerToolBarCaption, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 0, 0, NULL, NULL, theApp.m_hInstance, NULL);
			m_hChildHostWnd = ::CreateWindowEx(NULL, _T("Hubble Grid Class"), _T(""), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_hHostWnd, NULL, theApp.m_hInstance, NULL);
		}
		OnConnection(pApplication, ConnectMode);
		return S_OK;
	}

	STDMETHODIMP COfficeAddin::OnDisconnection(AddInDesignerObjects::ext_DisconnectMode RemoveMode, SAFEARRAY ** /*custom*/)
	{
		m_bOfficeAddinUnLoad = true;
		g_pHubble->m_pActiveAppProxy = nullptr;
		if (m_pCTPFactory)
			m_pCTPFactory.Detach();
		OnDisconnection(RemoveMode);

		g_pHubble->m_pDesignerGalaxyCluster = nullptr;
		if (::IsWindow(m_hHostWnd))
			::DestroyWindow(m_hHostWnd);
		if (::IsWindow(m_hHubbleWnd))
			::DestroyWindow(m_hHubbleWnd);
		return S_OK;
	}

	STDMETHODIMP COfficeAddin::OnAddInsUpdate(SAFEARRAY ** /*custom*/)
	{
		return OnUpdate();
	}

	STDMETHODIMP COfficeAddin::OnStartupComplete(SAFEARRAY ** /*custom*/)
	{
		StartupComplete();
		return S_OK;
	}

	STDMETHODIMP COfficeAddin::OnBeginShutdown(SAFEARRAY ** /*custom*/)
	{
		//if (m_hCBTHook)
		//{
		//	UnhookWindowsHookEx(m_hCBTHook);
		//	m_hCBTHook = nullptr;
		//}
		if (::IsWindow(m_hHostWnd))
		{
			::DestroyWindow(m_hHostWnd);
			m_hHostWnd = NULL;
		}
		return S_OK;
	}

	void COfficeAddin::OnCloseOfficeObj(CString strName, HWND hWnd)
	{
		auto it = m_mapOfficeObjects.find(hWnd);
		if (it != m_mapOfficeObjects.end())
		{
			it->second->OnObjDestory();
			if (it->second->m_pOfficeObj != nullptr)
				it->second->m_pOfficeObj->Release();
			delete it->second;
			m_mapOfficeObjects.erase(it);
			if (m_mapOfficeObjects.size() == 0)
			{

			}
		}
	}

	void COfficeAddin::_AddDocXml(Office::_CustomXMLParts* pCustomXMLParts, BSTR bstrXml, BSTR bstrKey)
	{
#ifdef TANGRAMCOMMERCIALDITION
		CString strXml = OLE2T(bstrXml);
		CString strKey = OLE2T(bstrKey);
		if (strXml == _T("") || strKey == _T(""))
		{
			return ;
		}
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(strXml) || m_Parse.LoadFile(strXml))
		{
			CComPtr<_CustomXMLSchemaCollection> pCustomXMLSchemaCollection;
			if (pCustomXMLParts)
			{
				long nCount = 0;
				pCustomXMLParts->get_Count(&nCount);
				CString _strXML = _T("");
				vector<CComPtr<Office::_CustomXMLPart>> vec;
				for (int i = nCount; i >= 1; i--)
				{
					CComPtr<Office::_CustomXMLPart> pCustomXMLPart;
					pCustomXMLParts->get_Item(CComVariant(i), &pCustomXMLPart);
					if (pCustomXMLPart)
					{
						if (pCustomXMLSchemaCollection == nullptr)
							pCustomXMLPart->get_SchemaCollection(&pCustomXMLSchemaCollection);
						CComBSTR bstrXML(L"");
						pCustomXMLPart->get_XML(&bstrXML);
						_strXML = OLE2T(bstrXML);
						CTangramXmlParse m_Parse2;
						m_Parse2.LoadXml(_strXML);
						if (m_Parse2.name().CompareNoCase(strKey) == 0)
						{
							vec.push_back(pCustomXMLPart);
						}
					}
				}
				if (vec.size())
				{
					for (auto it : vec)
					{
						it->Delete();
					}
				}
				CComPtr<Office::_CustomXMLPart> pPart;
				if(pCustomXMLParts->Add(CComBSTR(strXml), CComVariant(pCustomXMLSchemaCollection), &pPart)==S_OK)
				{
					return ;
				}
			}
		}
#endif
	}

	HRESULT COfficeAddin::OnConnection(IDispatch* pHostApp, int ConnectMode)
	{
		m_bOfficeAddinUnLoad = false;
		m_strRibbonXmlFile = m_strAppCommonDocPath + _T("OfficeRibbon\\") + m_strExeName + _T("\\ribbon.xml");
		if (!::PathFileExists(m_strRibbonXmlFile))
		{
			m_strRibbonXmlFile = m_strProgramFilePath + _T("\\tangram\\officeribbon\\") + m_strExeName + _T("\\ribbon.xml");
		}

		if (::PathFileExists(m_strRibbonXmlFile))
		{
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadFile(m_strRibbonXmlFile))
			{
				m_strRibbonXml = m_Parse.xml();
			}
		}
		//auto task = create_task([this,strPath]()
		//{
		//	if (PathIsDirectory(strPath))
		//	{
		//		map<CString, CString> m_mapRibbon;
		//		_wfinddata_t fd;
		//		fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
		//		CString _strPath = strPath + _T("*.ribbonxml");
		//		intptr_t pf = _wfindfirst(_strPath, &fd);
		//		if (pf != -1)
		//		{ 
		//			while (pf != -1)
		//			{
		//				if ((fd.attrib&FILE_ATTRIBUTE_DIRECTORY) == 0 /*&& (fd.attrib&FILE_ATTRIBUTE_HIDDEN) == 0*/)
		//				{
		//					CString str = strPath + fd.name;
		//					if (str != _T(".."))
		//					{
		//						CTangramXmlParse m_Parse;
		//						if (m_Parse.LoadFile(str))
		//						{
		//							ATLTRACE(_T("COfficeAddin::OnConnection:%s\n"), str);

		//						}
		//					}
		//				}
		//				int nVal = _wfindnext(pf, &fd);
		//				ATLTRACE(_T("_wfindnext:%d\n"), nVal);
		//				if (nVal == -1)
		//					break;
		//			}
		//		}
		//		_findclose(pf);
		//	}
		//});
		return S_OK;
	};

	CString COfficeAddin::_GetDocXmlByKey(Office::_CustomXMLParts* pCustomXMLParts, BSTR bstrKey)
	{
		CString strKey = OLE2T(bstrKey);
		if (strKey == _T("") || pCustomXMLParts == nullptr)
		{
			return _T("");
		}

		if (pCustomXMLParts)
		{
			long nCount = 0;
			pCustomXMLParts->get_Count(&nCount);
			CString strXML = _T("");
			for (int i = nCount; i >= 1; i--)
			{
				CComPtr<Office::_CustomXMLPart> pCustomXMLPart;
				pCustomXMLParts->get_Item(CComVariant(i), &pCustomXMLPart);
				if (pCustomXMLPart)
				{
					CComBSTR bstrXML(L"");
					pCustomXMLPart->get_XML(&bstrXML);
					strXML = OLE2T(bstrXML);
					CTangramXmlParse m_Parse;
					m_Parse.LoadXml(strXML);
					if (m_Parse.name().CompareNoCase(strKey) == 0)
					{
						return strXML;
					}
				}
			}
		}
		return _T("");
	}

	// ICustomTaskPaneConsumer Methods
	STDMETHODIMP COfficeAddin::CTPFactoryAvailable(ICTPFactory * CTPFactoryInst)
	{
		if(m_pCTPFactory==nullptr)
			return CTPFactoryInst->QueryInterface(Office::IID_ICTPFactory, (void**)&m_pCTPFactory);
		return S_OK;
	};


	STDMETHODIMP COfficeAddin::GetCustomUI(BSTR RibbonID, BSTR * RibbonXml)
	{
		return S_OK;
	}

	STDMETHODIMP COfficeAddin::HubbleCommand(IDispatch* RibbonControl)
	{
		return S_OK;
	}

	STDMETHODIMP COfficeAddin::HubbleOnLoad(IDispatch* RibbonControl)
	{
		m_spRibbonUI = RibbonControl;
		Tangram_OnLoad(RibbonControl);
		return S_OK;
	}

	HRESULT COfficeAddin::HrGetResource(CString strLib, int nId, LPCTSTR lpType, LPVOID* ppvResourceData, DWORD* pdwSizeInBytes)
	{
		HMODULE hModule = ::LoadLibraryEx(strLib, nullptr, LOAD_LIBRARY_AS_DATAFILE);
		if (!hModule)
			return E_UNEXPECTED;
		HRSRC hRsrc = FindResource(hModule, MAKEINTRESOURCE(nId), lpType);
		if (!hRsrc)
			return HRESULT_FROM_WIN32(GetLastError());
		HGLOBAL hGlobal = LoadResource(hModule, hRsrc);
		if (!hGlobal)
			return HRESULT_FROM_WIN32(GetLastError());
		*pdwSizeInBytes = SizeofResource(hModule, hRsrc);
		*ppvResourceData = LockResource(hGlobal);
		::FreeLibrary(hModule);
		return S_OK;
	}

	STDMETHODIMP COfficeAddin::HubbleGetImage(BSTR strValue, IPictureDisp ** ppDispImage)
	{
		CImage m_Image;
		TCHAR szPath[MAX_PATH] = { 0 };
		GetModuleFileName(GetModuleHandle(_T("universe.dll")), szPath, MAX_PATH);
		CString strSelfPath = szPath;

		int nIndex = strSelfPath.ReverseFind(L'\\');
		strSelfPath = strSelfPath.Left(nIndex + 1);

		if (CString(strValue).MakeLower().Find(L".dll") != -1)
		{
			int nIndexSpilt = CString(strValue).Find(L"@");
			CString strResName = CString(strValue).Left(nIndexSpilt);
			CString  strDLLName= CString(strValue).Mid(nIndexSpilt + 1);
			nIndexSpilt = strDLLName.Find(L"@");
			CString strType = strDLLName.Left(nIndexSpilt);
			strDLLName = strDLLName.Mid(nIndexSpilt + 1);
			CString strLib = strSelfPath + strDLLName;
			HMODULE hResHandle = LoadLibraryEx(strLib,nullptr, LOAD_LIBRARY_AS_DATAFILE);

			LoadImageFromResource(&m_Image, hResHandle, strResName, strType);
			::FreeLibrary(hResHandle);
		}
		else
		{
			CString strPath = strSelfPath + strValue;
			if (::PathFileExists(strPath) == false)
				return S_OK;
			m_Image.Load(strPath);
		}

		PICTDESC pictureDesc;
		::ZeroMemory(&pictureDesc, sizeof(pictureDesc));
		pictureDesc.cbSizeofstruct = sizeof(pictureDesc);
		pictureDesc.picType = PICTYPE_BITMAP;
		pictureDesc.bmp.hbitmap = m_Image.Detach();
		HRESULT hr = ::OleCreatePictureIndirect(&pictureDesc, IID_IPicture, true, reinterpret_cast<void **>(ppDispImage));
		if (FAILED(hr))
		{
			::MessageBox(0, _T("Error in creating picture from bitmap."), _T("Tangram - COutLookApp::OnGetImage()"), MB_OK);
			return S_OK;
		}
		return S_OK;
	}

	STDMETHODIMP COfficeAddin::HubbleGetItemCount(IDispatch* RibbonControl, long* nCount)
	{
		return Tangram_GetItemCount(RibbonControl, nCount);
	}

	STDMETHODIMP COfficeAddin::HubbleGetItemLabel(IDispatch* RibbonControl, long nIndex, BSTR* bstrLabel)
	{
		return Tangram_GetItemLabel(RibbonControl, nIndex, bstrLabel);
	}

	STDMETHODIMP COfficeAddin::HubbleGetItemID(IDispatch* RibbonControl, long nIndex, BSTR* bstrID)
	{
		return Tangram_GetItemID(RibbonControl, nIndex, bstrID);
	}

	STDMETHODIMP COfficeAddin::HubbleGetVisible(IDispatch* RibbonControl, VARIANT* varVisible)
	{
		return S_OK;
	}

	STDMETHODIMP COfficeAddin::get_RemoteHelperHWND(LONGLONG* pVal)
	{
		if (::IsWindow(m_hHostWnd))
			*pVal = (LONGLONG)m_hHostWnd;
		return S_OK;
	}

	void COfficeAddin::CreateCommonDesignerToolBar()
	{
		CString strName = this->m_strExeName;
		if (::IsWindow(m_hHostWnd) == false)
		{
			auto it = g_pHubble->m_mapValInfo.find(_T("designertoolcaption"));
			if (it != g_pHubble->m_mapValInfo.end())
				m_strDesignerToolBarCaption = OLE2T(it->second.bstrVal);
			m_hHostWnd = ::CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW, _T("Hubble Grid Class"), m_strDesignerToolBarCaption, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 400, 400, NULL, 0, theApp.m_hInstance, NULL);
			m_hChildHostWnd = ::CreateWindowEx(NULL, _T("Hubble Grid Class"), _T(""), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_hHostWnd, 0, theApp.m_hInstance, NULL);
		}
		if (m_hHostWnd&&m_pDesignerGalaxyCluster == nullptr)
		{
			HWND hwnd = ::GetActiveWindow();
			RECT rc;
			::GetWindowRect(hwnd, &rc);
			::SetWindowPos(m_hHostWnd, NULL, rc.left + 40, rc.top + 40, 300, 700, SWP_NOACTIVATE | SWP_NOREDRAW);

			if (m_pDesignerGalaxyCluster == nullptr)
			{
				auto it = g_pHubble->m_mapWindowPage.find(m_hHostWnd);
				if (it != g_pHubble->m_mapWindowPage.end())
					m_pDesignerGalaxyCluster = (CGalaxyCluster*)it->second;
				else
				{
					m_pDesignerGalaxyCluster = new CComObject<CGalaxyCluster>();
					m_pDesignerGalaxyCluster->m_hWnd = m_hHostWnd;
					g_pHubble->m_mapWindowPage[m_hHostWnd] = m_pDesignerGalaxyCluster;
				}

				CString strPath = m_strExeName + _T(".designer");
				CTangramXmlParse m_Parse;
				if (m_Parse.LoadFile(strPath))
				{
					m_strDesignerXml = m_Parse.xml();
					if (m_strDesignerXml != _T(""))
					{
						auto it = m_pDesignerGalaxyCluster->m_mapGalaxy.find(m_hChildHostWnd);
						if (it == m_pDesignerGalaxyCluster->m_mapGalaxy.end())
						{
							IGalaxy* pGalaxy = nullptr;
							HRESULT hr = m_pDesignerGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((__int64)m_hChildHostWnd), CComBSTR(L"DeignerTool"), &pGalaxy);
							if (pGalaxy)
							{
								IGrid* pGrid = nullptr;
								pGalaxy->Observe(CComBSTR(L"DeignerToolBox"), CComBSTR(m_strDesignerXml), &pGrid);
								m_pDesignerFrame = (CGalaxy*)pGalaxy;
								m_pDesignerFrame->m_bDesignerState = false;
							}
						}
					}
				}
			}
		}
		::ShowWindow(m_hHostWnd, SW_SHOW);
		::UpdateWindow(m_hHostWnd);
	}

	COfficeExtender::COfficeExtender(void)
	{
	}

	COfficeExtender::~COfficeExtender(void)
	{
	}

	STDMETHODIMP COfficeExtender::Close()
	{
		g_pHubble->m_pExtender = nullptr;
		delete this;
		return S_OK;
	}

	STDMETHODIMP COfficeExtender::get_ActiveWorkBenchWindow(BSTR bstrID, IWorkBenchWindow** pVal)
	{
		CString strID = OLE2T(bstrID);
		strID.Trim();
		if (strID != _T(""))
		{
			IHubble* pHubble = nullptr;
			m_pAddin->get_RemoteHubble(bstrID, &pHubble);
			if (pHubble)
			{
				IWorkBenchWindow* pRet = nullptr;
				IHubbleExtender* pExtender = nullptr;
				pHubble->get_Extender(&pExtender);
				if (pExtender)
				{
					CComQIPtr<IEclipseExtender> pEclipse(pExtender);
					if (pEclipse)
						pEclipse->get_ActiveWorkBenchWindow(bstrID, &pRet);
					if (pRet)
					{
						*pVal = pRet;
						(*pVal)->AddRef();
					}
				}
			}
		}

		return S_OK;
	}

	STDMETHODIMP COfficeExtender::InitVBAForm(IDispatch* pFormDisp, long nStyle, BSTR bstrXml, IGrid** ppGrid)
	{
		CComQIPtr<IOleWindow> pOleWnd(pFormDisp);
		if (pOleWnd)
		{
			HWND hWnd = 0;
			pOleWnd->GetWindow(&hWnd);
			if (hWnd)
			{
				DISPID dispID = 0x80010000;
				DISPPARAMS dispParams = { NULL, NULL, 0, 0 };
				VARIANT result = { 0 };
				EXCEPINFO excepInfo;
				memset(&excepInfo, 0, sizeof excepInfo);
				UINT nArgErr = (UINT)-1;
				HRESULT hr = pFormDisp->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispParams, &result, &excepInfo, &nArgErr);
				if (S_OK == hr && VT_BSTR == result.vt)
				{
					CComBSTR bstrName = result.bstrVal;
					CString strName = OLE2T(bstrName);
					::VariantClear(&result);

					CString strXml = OLE2T(bstrXml);
					if (strXml == _T(""))
					{
						strXml = m_pAddin->GetFormXml(strName);
					}
					else
					{
						CString _strXml = OLE2T(bstrXml);
						CTangramXmlParse m_Parse;
						if (m_Parse.LoadXml(_strXml) == false)
						{
							if (m_Parse.LoadFile(_strXml) == false)
								strXml = _T("");
						}
						strXml = m_Parse.xml();
					}
					if (strXml == _T(""))
						return S_OK;

					HWND hChild = ::GetWindow(hWnd, GW_CHILD);
					IGalaxy* pGalaxy = nullptr;
					CGalaxyCluster* pGalaxyCluster = nullptr;
					auto it = g_pHubble->m_mapWindowPage.find(hChild);
					if (it != g_pHubble->m_mapWindowPage.end())
						pGalaxyCluster = (CGalaxyCluster*)it->second;
					else
					{
						pGalaxyCluster = new CComObject<CGalaxyCluster>();
						pGalaxyCluster->m_hWnd = hChild;
						g_pHubble->m_mapWindowPage[hChild] = pGalaxyCluster;
					}

					pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)hChild), CComBSTR(L"UserForm"), &pGalaxy);

					if (pGalaxy)
					{
						IGrid* pGrid = nullptr;
						pGalaxy->Observe(bstrName, CComBSTR(strXml), &pGrid);

						CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
						if (nStyle&&pGrid)
						{
							m_pAddin->m_mapVBAForm[hWnd] = _pGalaxy;
							::SetWindowLong(hWnd, GWL_STYLE, ::GetWindowLong(hWnd, GWL_STYLE) | WS_SIZEBOX | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);//::SetWindowLong(hWnd, GWL_STYLE, ::GetWindowLong(hWnd, GWL_STYLE)|nStyle);
							if (nStyle == 10)
							{
								_pGalaxy->m_bDesignerState = true;
								CGrid* _pGrid = (CGrid*)pGrid;
								_pGrid->m_pGridShareData->m_pOfficeObj = pFormDisp;
								_pGrid->m_pGridShareData->m_pOfficeObj->AddRef();
							}
						}
						m_pAddin->OnVbaFormInit(hWnd, _pGalaxy);
						*ppGrid = pGrid;
					}
					return S_OK;
				}
			}
		}
		return S_OK;
	}

	STDMETHODIMP COfficeExtender::AddVBAFormsScript(IDispatch* OfficeObject, BSTR bstrKey, BSTR bstrXml)
	{
		CString strKey = OLE2T(bstrKey);
		if (strKey == _T(""))
		{
			return S_FALSE;
		}

		CTangramXmlParse m_Parse2;
		CString strXml2 = OLE2T(bstrXml);
		if (m_Parse2.LoadXml(strXml2) == false && m_Parse2.LoadFile(strXml2) == false)
		{
			return S_FALSE;
		}

		CComBSTR szMember(L"CustomXMLParts");
		DISPID dispid = -1;
		HRESULT hr = OfficeObject->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &dispid);
		if (hr == S_OK)
		{
			DISPPARAMS dispParams = { NULL, NULL, 0, 0 };
			VARIANT result = { 0 };
			EXCEPINFO excepInfo;
			memset(&excepInfo, 0, sizeof excepInfo);
			UINT nArgErr = (UINT)-1;
			HRESULT hr = OfficeObject->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispParams, &result, &excepInfo, &nArgErr);
			if (S_OK == hr && VT_DISPATCH == result.vt)
			{
				Lock();
				CComQIPtr<Office::_CustomXMLParts> pCustomXMLParts(result.pdispVal);
				if (pCustomXMLParts)
				{
					CComPtr<_CustomXMLSchemaCollection> pCustomXMLSchemaCollection;
					long nCount = 0;
					pCustomXMLParts->get_Count(&nCount);
					for (int i = 1; i <= nCount; i++)
					{
						CComPtr<_CustomXMLPart> pCustomXMLPart;
						pCustomXMLParts->get_Item(CComVariant(i), &pCustomXMLPart);
						if (pCustomXMLPart)
						{
							CComBSTR bstrXML(L"");
							pCustomXMLPart->get_XML(&bstrXML);
							CTangramXmlParse m_Parse;
							if (m_Parse.LoadXml(OLE2T(bstrXML))&&m_Parse.name().CompareNoCase(strKey) == 0)
							{
								if (pCustomXMLSchemaCollection == nullptr)
									pCustomXMLPart->get_SchemaCollection(&pCustomXMLSchemaCollection);
								pCustomXMLPart->Delete();
								pCustomXMLPart.Detach();
								int nCount2 = m_Parse2.GetCount();
								CTangramXmlParse* pFormsParse = m_Parse.GetChild(_T("userforms"));
								if (pFormsParse != nullptr)
								{
									for (int i = 0; i < nCount2; i++)
									{
										CTangramXmlParse* pChild = m_Parse2.GetChild(i);
										CTangramXmlParse* pChild2 = pFormsParse->GetChild(pChild->name());
										if (pChild2)
											pFormsParse->ReplaceNode(pChild2, pChild, _T(""));
										else
											pFormsParse->AddNode(pChild, _T(""));
									}
								}
								else
								{
									pFormsParse = m_Parse.AddNode(_T("userforms"));
									for (int i = 0; i < nCount2; i++)
									{
										CTangramXmlParse* pChild = m_Parse2.GetChild(i);
										CTangramXmlParse* pChild2 = pFormsParse->AddNode(pChild->name());
										if (pChild2)
											pFormsParse->ReplaceNode(pChild2, pChild, _T(""));
									}
								}
								pCustomXMLParts->Add(m_Parse.xml().AllocSysString(), CComVariant(pCustomXMLSchemaCollection), &pCustomXMLPart);
								break;
							}
						}
					}
				}
				Unlock();
			}
			::VariantClear(&result);
		}
		return S_OK;
	}

	STDMETHODIMP COfficeExtender::GetGalaxyFromVBAForm(IDispatch* pForm, IGalaxy** ppGalaxy)
	{
		CComQIPtr<IOleWindow> pOleWnd(pForm);
		if (pOleWnd)
		{
			HWND hWnd = NULL;
			pOleWnd->GetWindow(&hWnd);
			auto it = m_pAddin->m_mapVBAForm.find(hWnd);
			if (it != m_pAddin->m_mapVBAForm.end())
			{
				*ppGalaxy = it->second;
			}
		}

		return S_OK;
	}

	STDMETHODIMP COfficeExtender::GetActiveTopGrid(IDispatch* pForm, IGrid** WndGrid)
	{
		CComQIPtr<IOleWindow> pOleWnd(pForm);
		if (pOleWnd)
		{
			HWND hWnd = NULL;
			pOleWnd->GetWindow(&hWnd);
			auto it = m_pAddin->m_mapVBAForm.find(hWnd);
			if (it != m_pAddin->m_mapVBAForm.end())
			{
				*WndGrid = it->second->m_pWorkGrid;
			}
		}
		return S_OK;
	}

	STDMETHODIMP COfficeExtender::GetObjectFromWnd(LONG hWnd, IDispatch** ppObjFromWnd)
	{
		return S_OK;
	}

	COfficeObject::COfficeObject(void)
	{
		m_bMDIClient = false;
		m_hClient = NULL;
		m_hTaskPane = NULL;
		m_hTaskPaneWnd = NULL;
		m_hTaskPaneChildWnd = NULL;
		m_pGalaxyCluster = nullptr;
		m_pGalaxy = nullptr;
		m_pOfficeObj = nullptr;
	}

	COfficeObject::~COfficeObject(void)
	{
	}

	STDMETHODIMP COfficeObject::Show(VARIANT_BOOL bShow)
	{
		if (m_pGalaxyCluster == nullptr)
			return S_OK;
		if (bShow)
		{
			HWND h = ::GetParent(m_hForm);
			m_pGalaxy->ModifyHost((LONGLONG)h);
		}
		else
		{
			m_pGalaxy->ModifyHost((LONGLONG)m_hChildClient);
		}

		return S_OK;
	}

	STDMETHODIMP COfficeObject::Observe(BSTR bstrKey, BSTR bstrXml, IGrid** ppGrid)
	{
		CString strKey = OLE2T(bstrKey);
		strKey.Trim();
		if (strKey == _T(""))
			return S_FALSE;

		strKey = OLE2T(bstrXml);
		strKey.Trim();
		if (strKey == _T(""))
			return S_FALSE;

		if (m_pGalaxyCluster == nullptr/*&&m_bMDIClient == false*/)
		{
			m_hClient = ::CreateWindowEx(NULL, L"Tangram Remote Helper Window", _T("Tangram Office Plus Addin Helper Window"), WS_CHILD, 0, 0, 0, 0, (HWND)m_hForm, NULL, theApp.m_hInstance, NULL);
			m_hChildClient = ::CreateWindowEx(NULL, L"Tangram Remote Helper Window", _T("Tangram Excel Helper Window"), WS_CHILD, 0, 0, 0, 0, (HWND)m_hClient, NULL, AfxGetInstanceHandle(), NULL);

			auto it = g_pHubble->m_mapWindowPage.find(m_hClient);
			if (it != g_pHubble->m_mapWindowPage.end())
				m_pGalaxyCluster = (CGalaxyCluster*)it->second;
			else
			{
				m_pGalaxyCluster = new CComObject<CGalaxyCluster>();
				m_pGalaxyCluster->m_hWnd = m_hClient;
				g_pHubble->m_mapWindowPage[m_hClient] = m_pGalaxyCluster;
			}

			if (m_pGalaxyCluster == nullptr)
				return S_FALSE;
			if (m_pGalaxy == nullptr)
			{
				m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)m_hChildClient), CComBSTR(L"Default"), &m_pGalaxy);
				if (m_pGalaxy == nullptr)
				{
					delete m_pGalaxyCluster;
					m_pGalaxyCluster = nullptr;
					return S_FALSE;
				}
			}
		}

		return m_pGalaxy->Observe(bstrKey, bstrXml, ppGrid);
	}

	STDMETHODIMP COfficeObject::UnloadHubble()
	{
		if (m_pGalaxyCluster)
		{
			m_pGalaxy->ModifyHost((long)m_hChildClient);
			::DestroyWindow(m_hClient);
			m_hClient = ::CreateWindowEx(NULL, L"Tangram Remote Helper Window", _T("Tangram Office Plus Addin Helper Window"), WS_CHILD, 0, 0, 0, 0, (HWND)m_hForm, NULL, theApp.m_hInstance, NULL);
			m_hChildClient = ::CreateWindowEx(NULL, L"Tangram Remote Helper Window", _T("Tangram Excel Helper Window"), WS_CHILD, 0, 0, 0, 0, (HWND)m_hClient, NULL, AfxGetInstanceHandle(), NULL);
			m_pGalaxy = nullptr;
			m_pGalaxyCluster = nullptr;
		}
		return S_OK;
	}
}



