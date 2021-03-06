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

#include "StdAfx.h"
#include "dllmain.h"
#include "GridCLREvent.h"
#include "Cosmos.h"

using namespace System::Threading;
using namespace System::Diagnostics;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

namespace Cosmos
{
    Grid::Grid(IGrid* pGrid)
    {
        m_hWnd = NULL;
        m_pGridEvent = new CCosmosGridEvent();
        m_pGridEvent->m_pGrid = pGrid;
        m_pGridEvent->m_pGridCLREvent = new CGridCLREvent();
        m_pGridCLREvent = m_pGridEvent->m_pGridCLREvent;
        m_pGridEvent->m_pGridCLREvent->m_pGrid = this;
        HRESULT hr = m_pGridEvent->DispEventAdvise(pGrid);
        if (theApp.m_pHubbleImpl)
            theApp.m_pHubbleImpl->AttachGrid(m_pGridEvent);
        m_pGrid = pGrid;
        LONGLONG nValue = (LONGLONG)pGrid;
        theAppProxy._insertObject(nValue, this);
    }

    Grid::~Grid()
    {
        delete m_pGridCLREvent;
        m_pGrid = NULL;
    }

    Galaxy^ Grid::Galaxy::get()
    {
        CComPtr<IGalaxy> pGalaxy;
        m_pGrid->get_Galaxy(&pGalaxy);

        return theAppProxy._createObject<IGalaxy, Cosmos::Galaxy>(pGalaxy);
    }

    String^ Grid::Caption::get()
    {
        if (m_pGrid)
        {
            CComBSTR bstrCap("");
            m_pGrid->get_Caption(&bstrCap);
            String^ strCap = BSTR2STRING(bstrCap);
            return strCap;
        }
        return "";
    }

    String^ Grid::URL::get()
    {
        if (m_pGrid)
        {
            ::GridType nType;
            m_pGrid->get_GridType(&nType);
            CComBSTR bstrCap("");
            switch (nType)
            {
            case BlankView:
            {
                m_pGrid->get_URL(&bstrCap);
                return BSTR2STRING(bstrCap);
            }
            break;
            case ActiveX:
            {
            }
            break;
            default:
                break;
            }
        }
        return "";
    }

    void Grid::URL::set(String^ newVal)
    {
        if (m_pGrid)
        {
            ::GridType nType;
            m_pGrid->get_GridType(&nType);
            switch (nType)
            {
            case BlankView:
                if (nType == BlankView)
                {
                    m_pGrid->put_URL(STRING2BSTR(newVal));

                }
                break;
            case ActiveX:
            {
            }
            break;
            default:
                break;
            }
        }
    }

    void Grid::Init()
    {
        LONGLONG h = 0;
        if (m_pGrid)
        {
            m_pGrid->get_Handle(&h);
            ::SendMessage((HWND)h, WM_COSMOSMSG, 1, 0);
        }
    }

    void Grid::Fire_OnTabChange(Grid^ ActivePage, Grid^ OldGrid)
    {
        OnTabChange(ActivePage, OldGrid);
    }

    Hubble::Hubble(IHubble* pHubble)
    {
    }

    Hubble::~Hubble(void)
    {
    }

    Hubble^ Hubble::GetHubble()
    {
        if (theApp.m_pHubble == nullptr)
        {
            HMODULE hModule = ::GetModuleHandle(L"universe.dll");
            if (hModule == nullptr) {
                TCHAR m_szBuffer[MAX_PATH];
                if (SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, m_szBuffer) ==
                    S_OK) {
                    ATL::CString m_strProgramFilePath = ATL::CString(m_szBuffer);
                    m_strProgramFilePath += _T("\\Hubble\\universe.dll");
                    if (::PathFileExists(m_strProgramFilePath)) {
                        hModule = ::LoadLibrary(m_strProgramFilePath);
                    }
                }
            }
            if (hModule) {
                typedef CHubbleImpl* (__stdcall* GetHubbleImpl)(IHubble**);
                GetHubbleImpl _pHubbleImplFunction;
                _pHubbleImplFunction = (GetHubbleImpl)GetProcAddress(hModule, "GetHubbleImpl");
                if (_pHubbleImplFunction != NULL) {
                    theApp.m_pHubbleImpl = _pHubbleImplFunction(&theApp.m_pHubble);
                    if (theApp.m_pHubbleImpl->m_nAppType == 0)
                        theApp.m_pHubbleImpl->m_nAppType = APP_BROWSERAPP;
                    theApp.m_pHubbleImpl->m_pHubbleDelegate = (IHubbleDelegate*)&theApp;
                    theApp.m_pHubbleImpl->m_pUniverseAppProxy = (IUniverseAppProxy*)&theApp;
                    theApp.m_pHubbleImpl->m_pCLRProxy = &theAppProxy;
                }
            }
        }
        if (m_pHubble == nullptr)
            m_pHubble = gcnew Hubble();
        return m_pHubble;
    }

    Hubble^ Hubble::InitHubbleApp(bool bSupportCrashReporting, CosmosAppType AppType)
    {
        if (m_pHubble == nullptr)
            m_pHubble = gcnew Hubble();
        if (theApp.m_bBrowserModeInit)
            return m_pHubble;
        theApp.m_pHubbleImpl->m_nAppType = (DWORD)AppType;
        //m_nAppType = AppType;
        switch (AppType)
        {
        case CosmosAppType::APPBROWSER:
        case CosmosAppType::APPBROWSER_ECLIPSE:
            theApp.InitHubbleApp(bSupportCrashReporting);
            break;
        case CosmosAppType::APPECLIPSE:
            break;
        case CosmosAppType::APPBROWSERAPP:
            break;
        }
        return m_pHubble;
    }

    Grid^ Hubble::CreatingGrid::get()
    {
        Object^ pRetObject = nullptr;
        if (theApp.m_pHubble)
        {
            IGrid* pGrid = nullptr;
            theApp.m_pHubble->get_CreatingGrid(&pGrid);
            if (pGrid)
                return theAppProxy._createObject<IGrid, Grid>(pGrid);
        }
        return nullptr;
    }

    bool Hubble::Fire_OnAppInit()
    {
        if (IsAppInit == false)
        {
            IsAppInit = true;
            return OnAppInit();
        }
        return false;
    }

    void Hubble::Fire_OnClose()
    {
        OnClose();
    }

    int Hubble::HubbleInit(String^ strInit)
    {
        CString strInfo = strInit;
        if (strInfo != _T(""))
        {
            __int64 nPointer = _wtoi64(strInfo);
            if (nPointer)
            {
                theApp.m_pHubbleImpl = (CHubbleImpl*)nPointer;
                theApp.m_pHubbleImpl->m_pCLRProxy = &theAppProxy;
                theApp.m_pHubbleImpl->m_pCosmosAppProxy = static_cast<IUniverseAppProxy*>(&theApp);
                //20180817
                if (theApp.m_pHubble == nullptr)
                {
                    HMODULE hModule = ::GetModuleHandle(_T("universe.dll"));
                    if (hModule) {
                        typedef IHubble* (__stdcall* GetHubble)();
                        GetHubble _pHubbleFunction;
                        _pHubbleFunction = (GetHubble)GetProcAddress(hModule, "GetHubble");
                        if (_pHubbleFunction != NULL) {
                            theApp.m_pHubble = _pHubbleFunction();
                        }
                    }
                }
            }
        }
        return 0;
    };

    Grid^ Hubble::GetGridFromHandle(IntPtr handle)
    {
        IGrid* pWndGrid = nullptr;
        HRESULT hr = theApp.m_pHubble->GetGridFromHandle((LONGLONG)handle.ToPointer(), &pWndGrid);
        if (hr != S_OK || pWndGrid == nullptr)
        {
            return nullptr;
        }
        return theAppProxy._createObject<IGrid, Grid>(pWndGrid);
    }

    Grid^ Hubble::GetGridFromControl(Control^ ctrl)
    {
        if (ctrl == nullptr)
        {
            return nullptr;
        }
        IGrid* pWndGrid = nullptr;
        HRESULT hr = theApp.m_pHubble->GetGridFromHandle((LONGLONG)ctrl->Handle.ToPointer(), &pWndGrid);
        if (hr != S_OK || pWndGrid == nullptr)
        {
            return nullptr;
        }
        return theAppProxy._createObject<IGrid, Grid>(pWndGrid);
    }

    Grid^ Hubble::Observe(Control^ ctrl, String^ key, String^ strGridXml)
    {
        if (ctrl != nullptr)
        {
            if (ctrl->Dock != DockStyle::Fill && ctrl->Dock != DockStyle::None)
                return nullptr;
            IGalaxy* pGalaxy = nullptr;
            theApp.m_pHubble->GetGalaxy((__int64)ctrl->Handle.ToPointer(), &pGalaxy);
            if (pGalaxy)
            {
                IGrid* pGrid = nullptr;
                BSTR bstrKey = STRING2BSTR(key);
                BSTR bstrXml = STRING2BSTR(strGridXml);
                pGalaxy->Observe(bstrKey, bstrXml, &pGrid);
                ::SysFreeString(bstrKey);
                ::SysFreeString(bstrXml);
                if (pGrid)
                    return theAppProxy._createObject<IGrid, Grid>(pGrid);
                return nullptr;
            }
            HWND hTopWnd = nullptr;
            Control^ topCtrl = ctrl->TopLevelControl;
            if (topCtrl == nullptr)
            {
                hTopWnd = ::GetParent((HWND)ctrl->Handle.ToPointer());
            }
            else
            {
                if (topCtrl->GetType()->IsSubclassOf(Form::typeid))
                {
                    Form^ pForm = (Form^)topCtrl;
                    Form^ pForm2 = nullptr;
                    if (pForm != nullptr && pForm->IsMdiContainer)
                    {
                        bool bMdiClientChild = false;
                        int nCount = pForm->Controls->Count;
                        String^ strName = L"";
                        for (int i = nCount - 1; i >= 0; i--)
                        {
                            Control^ pCtrl = pForm->Controls[i];
                            strName = pCtrl->GetType()->Name->ToLower();
                            if (strName == L"mdiclient")
                            {
                                if(::IsChild((HWND)pCtrl->Handle.ToPointer(),(HWND)ctrl->Handle.ToPointer()))
                                { 
                                    bMdiClientChild = true;
                                }
                                break;
                            }
                        }
                        if(bMdiClientChild)
                        {
                            Control^ parent = ctrl->Parent;
                            while (!parent->GetType()->IsSubclassOf(Form::typeid))
                            {
                                parent = parent->Parent;
                            }
                            if (parent != nullptr)
                                pForm2 = (Form^)parent;
                        }
                        else
                        {
                            pForm2 = pForm;
                        }
                    }
                    else
                        pForm2 = pForm;
                    if (pForm2 != nullptr)
                    {
                        IGalaxyCluster* pGalaxyCluster = nullptr;
                        theApp.m_pHubble->CreateGalaxyCluster(pForm2->Handle.ToInt64(), &pGalaxyCluster);
                        if (pGalaxyCluster != nullptr)
                        {
                            IGalaxy* pGalaxy = nullptr;
                            BSTR bstrName = STRING2BSTR(ctrl->Name);
                            Grid^ thisGrid = nullptr;
                            pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)ctrl->Handle.ToInt64()), bstrName, &pGalaxy);
                            if (pGalaxy)
                            {
                                IGrid* pGrid = nullptr;
                                BSTR bstrKey = STRING2BSTR(key);
                                BSTR bstrXml = STRING2BSTR(strGridXml);
                                pGalaxy->Observe(bstrKey, bstrXml, &pGrid);
                                if (pGrid)
                                {
                                    thisGrid = theAppProxy._createObject<IGrid, Grid>(pGrid);
                                }
                                ::SysFreeString(bstrKey);
                                ::SysFreeString(bstrXml);
                            }
                            ::SysFreeString(bstrName);
                            return thisGrid;
                        }
                    }
                }
                else
                {
                    hTopWnd = ::GetParent((HWND)topCtrl->Handle.ToPointer());
                }
            }
            if (hTopWnd != nullptr)
            {
                HWND _hTopWnd = hTopWnd;
                Grid^ pGrid = GetGridFromHandle((IntPtr)hTopWnd);
                while (pGrid == nullptr)
                {
                    hTopWnd = ::GetParent(hTopWnd);
                    pGrid = GetGridFromHandle((IntPtr)hTopWnd);
                }

                Grid^ _pRetGrid = nullptr;
                IGalaxyCluster* pGalaxyCluster = nullptr;
                if (pGrid != nullptr)
                {
                    pGrid->m_pGrid->get_GalaxyCluster(&pGalaxyCluster);
                    if (pGalaxyCluster)
                    {
                        String^ strName = ctrl->Name;
                        BSTR bstrName = STRING2BSTR(strName);
                        HWND hWnd = (HWND)ctrl->Handle.ToPointer();
                        pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)hWnd), bstrName, &pGalaxy);
                        if (pGalaxy)
                        {
                            IGrid* pGrid = nullptr;
                            BSTR bstrKey = STRING2BSTR(key);
                            BSTR bstrXml = STRING2BSTR(strGridXml);
                            pGalaxy->Observe(bstrKey, bstrXml, &pGrid);
                            ::SysFreeString(bstrKey);
                            ::SysFreeString(bstrXml);
                            if(pGrid)
                                _pRetGrid = theAppProxy._createObject<IGrid, Grid>(pGrid);
                        }
                        ::SysFreeString(bstrName);
                        ::InvalidateRect(hWnd, nullptr, true);
                    }
                }
                else
                {
                    theApp.m_pHubble->CreateGalaxyCluster((__int64)_hTopWnd, &pGalaxyCluster);
                    if (pGalaxyCluster != nullptr)
                    {
                        BSTR bstrName = STRING2BSTR(ctrl->Name);
                        pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)ctrl->Handle.ToInt64()), bstrName, &pGalaxy);
                        if (pGalaxy)
                        {
                            IGrid* pGrid = nullptr;
                            BSTR bstrKey = STRING2BSTR(key);
                            BSTR bstrXml = STRING2BSTR(strGridXml);
                            pGalaxy->Observe(bstrKey, bstrXml, &pGrid);
                            ::SysFreeString(bstrKey);
                            ::SysFreeString(bstrXml);
                            if (pGrid)
                                _pRetGrid = theAppProxy._createObject<IGrid, Grid>(pGrid);
                        }
                        ::SysFreeString(bstrName);
                    }
                }
                return _pRetGrid;
            }
        }
        return nullptr;
    }

    Type^ Hubble::GetType(String^ strObjID)
    {
        if (String::IsNullOrEmpty(strObjID) == true)
            return nullptr;

        String^ m_strID = strObjID->ToLower()->Replace(L" ", L"");
        String^ strLib = nullptr;
        Type^ pType = nullptr;
        if (m_strID != L"")
        {
            Monitor::Enter(m_pHubbleCLRTypeDic);
            String^ strID = nullptr;
            if (m_pHubbleCLRTypeDic->TryGetValue(m_strID, pType) == false)
            {
                BSTR bstrID = STRING2BSTR(m_strID);
                CString _strID = OLE2T(bstrID);
                ::SysFreeString(bstrID);
                int nIndex = _strID.Find(_T(","));
                if (nIndex != -1)
                {
                    CString s1 = _strID.Mid(nIndex + 1);
                    if (s1 == _T("host"))
                    {
                        CString s = _strID.Left(nIndex);
                        _strID = _strID.Left(nIndex + 1) + theApp.m_pHubbleImpl->m_strExeName;
                        _strID.MakeLower();
                    }
                }
                _strID = theApp.GetLibPathFromAssemblyQualifiedName(_strID);
                if (_strID != _T(""))
                {
                    int nPos = _strID.Find(_T("|"));
                    CString strObjName = _strID.Left(nPos);
                    _strID = _strID.Mid(nPos + 1);
                    nPos = _strID.Find(_T("|"));
                    strID = BSTR2STRING(_strID);
                    CString strLibName = _strID.Left(nPos);
                    strLib = BSTR2STRING(_strID.Mid(nPos + 1));
                    Assembly^ m_pDotNetAssembly = nullptr;
                    bool bSystemObj = false;
                    int nIndex = m_strID->IndexOf(L"system.windows.forms");;
                    if (nIndex != -1)
                    {
                        bSystemObj = true;
                        Control^ pObj = gcnew Control();
                        m_pDotNetAssembly = pObj->GetType()->Assembly;
                    }
                    else
                    {
                        try
                        {
                            if (strLibName.CompareNoCase(_T("Cosmos")) == 0)
                                m_pDotNetAssembly = GetHubble()->GetType()->Assembly;
                            else
                            {
                                auto it = theAppProxy.m_mapAssembly.find(strLib);
                                if (it != theAppProxy.m_mapAssembly.end())
                                    m_pDotNetAssembly = it->second;
                                else
                                {
                                    m_pDotNetAssembly = Assembly::LoadFile(strLib);
                                    theAppProxy.m_mapAssembly[strLib] = m_pDotNetAssembly;
                                }
                            }
                        }
                        catch (ArgumentNullException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject: " + e->Message);
                        }
                        catch (ArgumentException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject: " + e->Message);
                        }
                        catch (FileNotFoundException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject: " + e->Message);
                        }
                        catch (FileLoadException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject: " + e->Message);
                        }
                        catch (BadImageFormatException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject: " + e->Message);
                        }
                    }
                    if (m_pDotNetAssembly != nullptr)
                    {
                        try
                        {
                            pType = m_pDotNetAssembly->GetType(BSTR2STRING(strObjName), true, true);
                        }
                        catch (TypeLoadException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (ArgumentNullException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (ArgumentException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (FileNotFoundException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (FileLoadException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (BadImageFormatException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        finally
                        {
                            if (pType != nullptr)
                            {
                                m_pHubbleCLRTypeDic->Add(m_strID, pType);
                            }
                        }
                    }
                }
            }
            Monitor::Exit(m_pHubbleCLRTypeDic);
        }
        return pType;
    }

    Form^ Hubble::CreateForm(IWin32Window^ parent, String^ strObjID)
    {
        Object^ m_pObj = nullptr;
        int nIndex = strObjID->IndexOf(L"<");
        if (nIndex == 0)
        {
            CTangramXmlParse m_Parse;
            CString strXml = strObjID;
            if (m_Parse.LoadXml(strXml) || m_Parse.LoadFile(strXml))
            {
                CString strType = m_Parse.attr(_T("type"), _T(""));
                if (strType.CompareNoCase(_T("winform")) == 0)
                {
                    CString strID = m_Parse.attr(_T("objid"), _T(""));
                    CString strName = m_Parse.name();
                    if (strID != _T(""))
                    {
                        Object^ pObj = CreateObject(BSTR2STRING(strID));
                        if (pObj != nullptr)
                        {
                            if (pObj->GetType()->IsSubclassOf(Form::typeid))
                            {
                                Form^ thisForm = (Form^)pObj;
                                thisForm->Show(parent);
                            }
                            return (Form^)pObj;
                        }
                    }
                }
            }
            return nullptr;
        }
        return nullptr;
    }

    Object^ Hubble::CreateObject(String^ strObjID)
    {
        Object^ m_pObj = nullptr;
        int nIndex = strObjID->IndexOf(L"<");
        if (nIndex == 0)
        {
            CTangramXmlParse m_Parse;
            CString strXml = strObjID;
            if (m_Parse.LoadXml(strXml) || m_Parse.LoadFile(strXml))
            {
                CString strType = m_Parse.attr(_T("type"), _T(""));
                if (strType.CompareNoCase(_T("winform")) == 0)
                {
                    CString strID = m_Parse.attr(_T("objid"), _T(""));
                    CString strName = m_Parse.name();
                    if (strID != _T(""))
                    {
                        Object^ pObj = CreateObject(BSTR2STRING(strID));
                        if (pObj != nullptr)
                        {
                            if (pObj->GetType()->IsSubclassOf(Form::typeid))
                            {
                                CString strCaption = m_Parse.attr(_T("caption"), _T(""));
                                Form^ thisForm = (Form^)pObj;
                                if (m_Parse.attrBool(_T("mainwindow")))
                                    theApp.m_pHubbleImpl->m_hMainWnd = (HWND)thisForm->Handle.ToPointer();
                                if (strCaption != _T(""))
                                    thisForm->Text = BSTR2STRING(strCaption);
                                thisForm->Show();
                            }
                            return pObj;
                        }
                    }
                }
            }
            return nullptr;
        }
        Type^ pType = GetType(strObjID);
        if (pType)
        {
            try
            {
                m_pObj = (Object^)Activator::CreateInstance(pType);
            }
            catch (TypeLoadException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (ArgumentNullException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (ArgumentException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (NotSupportedException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (TargetInvocationException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (MethodAccessException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (InvalidComObjectException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (MissingMethodException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (COMException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
        }

        return m_pObj;
    }

    Grid^ Grid::Observe(String^ layerName, String^ layerXML)
    {
        if (m_pGrid)
        {
            BSTR blayerName = STRING2BSTR(layerName);
            BSTR blayerXML = STRING2BSTR(layerXML);
            IGrid* pGrid = nullptr;
            m_pGrid->Observe(blayerName, blayerXML, &pGrid);
            ::SysFreeString(blayerName);
            ::SysFreeString(blayerXML);
            if (pGrid)
            {
                return theAppProxy._createObject<IGrid, Grid>(pGrid);
            }
        }
        return nullptr;
    }

    Grid^ Grid::ObserveChild(int rowNum, int colNum, String^ layerName, String^ layerXML)
    {
        if (m_pGrid)
        {
            BSTR blayerName = STRING2BSTR(layerName);
            BSTR blayerXML = STRING2BSTR(layerXML);
            IGrid* pGrid = nullptr;
            m_pGrid->ObserveEx(rowNum, colNum, blayerName, blayerXML, &pGrid);
            ::SysFreeString(blayerName);
            ::SysFreeString(blayerXML);
            if (pGrid)
            {
                return theAppProxy._createObject<IGrid, Grid>(pGrid);
            }
        }
        return nullptr;
    }

    Grid^ Galaxy::Observe(String^ layerName, String^ layerXML)
    {
        Cosmos::Grid^ pRetNode = nullptr;
        BSTR blayerName = STRING2BSTR(layerName);
        BSTR blayerXML = STRING2BSTR(layerXML);
        CComPtr<IGrid> pGrid;
        m_pGalaxy->Observe(blayerName, blayerXML, &pGrid);
        if (pGrid)
        {
            pRetNode = theAppProxy._createObject<IGrid, Cosmos::Grid>(pGrid);
            Cosmos::Grid^ pRetNode2 = nullptr;
            if (!TryGetValue(layerName, pRetNode2))
            {
                Add(layerName, pRetNode);
            }
        }
        ::SysFreeString(blayerName);
        ::SysFreeString(blayerXML);
        return pRetNode;
    }
}
