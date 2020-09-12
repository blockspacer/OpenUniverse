/********************************************************************************
 *					Tangram Library - version 10.0.0
 **
 *********************************************************************************
 * Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.
 **
 *
 * THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
 * BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
 * WRITTEN CONSENT OF TANGRAM TEAM.
 *
 * THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
 * OUTLINED IN THE TANGRAM LICENSE AGREEMENT.TANGRAM TEAM
 * GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
 * THIS SOFTWARE ON A SINGLE COMPUTER.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com
 * https://www.tangramteam.com
 *
 ********************************************************************************/
#pragma once

#include <map>
#include <atlstr.h>
#include <string>
#include "jniforchrome.h"
#include "universe.h"
#include <ShlObj.h>

#define TANGRAM_CONST_OPENFILE			19920612
#define TANGRAM_CONST_NEWDOC			19631222
#define TANGRAM_CONST_PANE_FIRST		20022017
#define TANGRAM_CHROME_APP_INIT			20190501

#define IPC_CLR_CONTROL_CREARED							20200220
#define IPC_CLR_CONTROL_CREARED_ID						_T("Tangram_CLR_Control_Created")
#define IPC_NODE_CREARED								20200221
#define IPC_NODE_CREARED_ID								_T("Tangram_WndNode_Created")
#define IPC_NODE_ONMOUSEACTIVATE						20200222
#define IPC_NODE_ONMOUSEACTIVATE_ID						_T("Tangram_WndNode_OnMouseActivate")
#define IPC_MDIWINFORM_ACTIVEMDICHILD					20200224
#define IPC_MDIWINFORM_ACTIVEMDICHILD_ID				_T("MdiWinForm_ActiveMdiChild")

#define APP_WIN32				500
#define APP_BROWSER				1000
#define APP_ECLIPSE				2000
#define APP_BROWSER_ECLIPSE		3000
#define APP_BROWSERAPP			4000

#define APP_WILLCLOSE			2019
#define APP_OTHER				0
#define APP_SDI					1965
#define APP_MDI					1992
#define APP_MDT					1963

#define WM_TANGRAM_WEBNODEDOCCOMPLETE	(WM_USER + 0x00004001)
#define WM_OPENDOCUMENT					(WM_USER + 0x00004002)
#define WM_SPLITTERREPOSITION			(WM_USER + 0x00004003)
#define WM_ECLIPSEWORKBENCHCREATED		(WM_USER + 0x00004004)
#define WM_TABCHANGE					(WM_USER + 0x00004005)
#define WM_COSMOSMSG					(WM_USER + 0x00004006)
#define WM_NAVIXTML						(WM_USER + 0x00004007)
#define WM_OFFICEOBJECTCREATED			(WM_USER + 0x00004008)
#define WM_MDICHILDMIN					(WM_USER + 0x00004009)
#define WM_TANGRAMAPPINIT				(WM_USER + 0x0000400a)
#define WM_TANGRAMAPPQUIT				(WM_USER + 0x0000400b)
#define WM_TANGRAMDATA					(WM_USER + 0x0000400c)
#define WM_DOWNLOAD_MSG					(WM_USER + 0x0000400d)
#define WM_TANGRAMNEWOUTLOOKOBJ			(WM_USER + 0x0000400e)
#define WM_TANGRAMACTIVEINSPECTORPAGE	(WM_USER + 0x0000400f)
#define WM_USER_TANGRAMTASK				(WM_USER + 0x00004010)
#define WM_SETWNDFOCUSE					(WM_USER + 0x00004011)
#define WM_UPLOADFILE					(WM_USER + 0x00004012)
#define WM_TANGRAMDESIGNMSG				(WM_USER + 0x00004013)
#define WM_INSERTTREENODE				(WM_USER + 0x00004014)
#define WM_REFRESHDATA					(WM_USER + 0x00004015)
#define WM_GETSELECTEDNODEINFO			(WM_USER + 0x00004016)
#define WM_TANGRAMDESIGNERCMD			(WM_USER + 0x00004017)
#define WM_TANGRAMGETTREEINFO			(WM_USER + 0x00004018)
#define WM_TANGRAMGETNODE				(WM_USER + 0x00004019)
#define WM_TANGRAMUPDATENODE			(WM_USER + 0x0000401a)
#define WM_TANGRAMSAVE					(WM_USER + 0x0000401b)
#define WM_MDICLIENTCREATED				(WM_USER + 0x0000401c)
#define WM_TGM_SETACTIVEPAGE			(WM_USER + 0x0000401d)
#define WM_TGM_SET_CAPTION				(WM_USER + 0x0000401e)	
#define WM_GETNODEINFO					(WM_USER + 0x0000401f)
#define WM_CREATETABPAGE				(WM_USER + 0x00004020)
#define WM_ACTIVETABPAGE				(WM_USER + 0x00004021)
#define WM_MODIFYTABPAGE				(WM_USER + 0x00004022)
#define WM_ADDTABPAGE					(WM_USER + 0x00004023)
#define WM_TANGRAMITEMLOAD				(WM_USER + 0x00003024)
#define WM_INITOUTLOOK					(WM_USER + 0x00004026)
#define WM_CONTROLBARCREATED			(WM_USER + 0x00004027)
#define WM_QUERYAPPPROXY				(WM_USER + 0x00004028)
#define WM_TANGRAMACTIVEPAGE			(WM_USER + 0x00004029)
#define WM_TANGRAMSETAPPTITLE			(WM_USER + 0x0000402a)
#define WM_LYNCIMWNDCREATED				(WM_USER + 0x0000402b)
#define WM_STOPTRACKING					(WM_USER + 0x0000402c)
#define WM_TANGRAMINIT					(WM_USER + 0x0000402d)
#define WM_VSSHOWPROPERTYGRID			(WM_USER + 0x0000402e)
#define WM_REMOVERESTKEY				(WM_USER + 0x0000402f)
#define WM_TANGRAMGETXML				(WM_USER + 0x00004030)
#define WM_CHROMEWEBCLIENTCREATED		(WM_USER + 0x00004031)
#define WM_CHROMERENDERERFRAMEHOSTINIT	(WM_USER + 0x00004032)
#define WM_CHROMEOPENWINDOWMSG			(WM_USER + 0x00004033)
#define WM_BROWSERLAYOUT                (WM_USER + 0x00004034)
#define WM_CHROMEMSG	                (WM_USER + 0x00004035)
#define WM_CHROMEWNDPARENTCHANGED       (WM_USER + 0x00004035)
#define WM_DEVICESCALEFACTORCHANGED     (WM_USER + 0x00004036)
#define WM_CHROMEDEVTOOLMSG	            (WM_USER + 0x00004037)
#define WM_BACKGROUNDWEBPROXY_MSG       (WM_USER + 0x00004039)
#define WM_CHROMEWNDNODEMSG             (WM_USER + 0x00004040)
#define WM_DOTNETCONTROLCREATED         (WM_USER + 0x00004041)
#define WM_DOCUMENTONLOADCOMPLETED      (WM_USER + 0x00004043)
#define WM_DOCUMENTFAILLOADWITHERROR    (WM_USER + 0x00004044)
#define WM_CHROMEHELPWND                (WM_USER + 0x00004045)
#define WM_CHROMEOMNIBOXPOPUPVISIBLE    (WM_USER + 0x00004046)
#define WM_HOSTNODEFORSPLITTERCREATED   (WM_USER + 0x00004047)
#define WM_CHROMEOPENURLMSG				(WM_USER + 0x00004048)
#define WM_LBUTTONDOWNEX			    (WM_USER + 0x00004049)
#define WM_WINFORMCREATED				(WM_USER + 0x0000404a)
#define WM_CHROMEIPCMSG					(WM_USER + 0x00004052)
#define WM_CHROMEAPPINIT				(WM_USER + 0x00004053)
#define WM_CREATETANGRAMVIEW			(WM_USER + 0x00004054)

#define TANGRAM_OBJECT_ENTRY_AUTO(clsid, class) \
    __declspec(selectany) ATL::_ATL_OBJMAP_CACHE __objCache__##class = { NULL, 0 }; \
	const ATL::_ATL_OBJMAP_ENTRY_EX __objMap_##class = {&clsid, class::UpdateRegistry, class::_ClassFactoryCreatorClass::CreateInstance, class::CreateInstance, &__objCache__##class, class::GetObjectDescription, class::GetCategoryMap, class::ObjectMain }; \
	extern "C" __declspec(allocate("ATL$__m")) __declspec(selectany) const ATL::_ATL_OBJMAP_ENTRY_EX* const __pobjMap_##class = &__objMap_##class; \
	OBJECT_ENTRY_PRAGMA(class)

#define OBJECT_ENTRY_AUTO_EX(clsid, class) \
    __declspec(selectany) ATL::_ATL_OBJMAP_CACHE __objCache__##class = { NULL, 0 }; \
	const ATL::_ATL_OBJMAP_ENTRY_EX __objMap_##class = {&clsid, class::UpdateRegistry, class::_ClassFactoryCreatorClass::CreateInstance, class::CreateInstance, &__objCache__##class, class::GetObjectDescription, class::GetCategoryMap, class::ObjectMain }; \
	extern "C" __declspec(allocate("ATL$__m")) __declspec(selectany) const ATL::_ATL_OBJMAP_ENTRY_EX* const __pobjMap_##class = &__objMap_##class; \
	OBJECT_ENTRY_PRAGMA(class)

#define TGM_NAME				_T("name")
#define TGM_CAPTION				_T("caption")
#define TGM_NODE_TYPE			_T("startype")
#define TGM_CNN_ID				_T("objid")
#define TGM_HEIGHT				_T("height")
#define TGM_WIDTH				_T("width")
#define TGM_STYLE				_T("style")
#define TGM_ACTIVE_PAGE			_T("activepage")
#define TGM_TAG					_T("tag")
#define TGM_CLUSTER				_T("cluster")
#define TGM_NODE				_T("star")

#define TGM_ROWS				_T("rows")
#define TGM_COLS				_T("cols")


#define TGM_SPLITTER			_T("splitter")
#define TGM_TABBED				_T("tab")

#define TGM_SETTING_HEAD		_T("#$^&TANGRAM")
#define TGM_SETTING_FOMRAT		_T("#$^&TANGRAM[%ld][%ld]")

#define TGM_S_EXCEL_INPUT		1

namespace CommonUniverse {
	using namespace std;
	using namespace ATL;

	class CChromeTab;
	class CHubbleImpl;
	class IHubbleWindow;
	class CChromeTabProxy;
	class CSession;
	class IHubbleDelegate;
	class CChromeBrowserBase;
	class CBrowserImpl;
	class CChromeRenderProcess;
	class CChromeWebContentBase;
	class CHubbleMainDllLoader;
	class OmniboxViewViewsProxy;
	class CHubbleBrowserFactory;
	class CChromeRenderProcessProxy;
	class CChromeWebContentProxyBase;
	class CChromeRenderFrameHost;
	class CChromeChildProcessHostImpl;
	class CWebPageImpl;
	class CChromeChildProcessHostImplProxy;

	class IHubbleCLRImpl;
	class IHubbleAppProxy;

	extern CHubbleImpl* g_pHubbleImpl;

	typedef struct IPCMsg {
		CString m_strId = _T("");
		CString m_strParam1 = _T("");
		CString m_strParam2 = _T("");
		CString m_strParam3 = _T("");
		CString m_strParam4 = _T("");
		CString m_strParam5 = _T("");
		long m_nHandleFrom = 0;
		long m_nHandleTo = 0;
	} IPCMsg;

	typedef struct QuasarInfo
	{
		HWND			m_hCtrlHandle;
		IDispatch*		m_pDisp;
		IDispatch*		m_pParentDisp;
		CString			m_strCtrlName;
		CString			m_strQuasarName;
		CString			m_strNodeXml;
		CString			m_strParentCtrlName;
	}QuasarInfo;

	typedef struct DocTemplateInfo
	{
		bool bDll;
		int nResID;
		CString strProxyName;
		CString strProxyID;
		CString strFileTypeID;
		CString strExt;
		CString strfilterName;
	} DocTemplateInfo;

	typedef struct BindWebObj
	{
		int nType = -1;
		HWND m_hWnd = NULL;
		CString m_strObjName = _T("");
		CString m_strBindObjName = _T("");
		CString m_strObjType = _T("");
		CString m_strBindData = _T("");
		IStar* m_pNode = nullptr;
		IDispatch* m_pObjDisp = nullptr;
		LPARAM lParam = 0;
	} BindWebObj;

	typedef struct HubbleDocTemplateInfo
	{
		BOOL			m_bCOMObj;
		int				m_nImageIndex;
		HWND			m_hWnd;
		CString			m_strLib;
		CString			m_strExt;
		CString			m_strFilter;
		CString			m_strProxyID;
		CString			m_strDocTemplateKey;
		CString			m_strTemplatePath;
		void*			m_pDocTemplate;
	}HubbleDocTemplateInfo;

	typedef struct TangramProjectInfo
	{
		BOOL			m_bTangramSupport;
		int				m_nPrjType;
		int				m_nImageIndex;
		int				m_nIndex;
		CString			m_strPrjFullPath;
		CString			m_strExt;
		CString			m_strFilter;
		IDispatch*		m_pPrjDisp;
	}TangramProjectInfo;

	typedef struct TangramDocInfo
	{
		CString		m_strTangramID;
		CString		m_strAppProxyID;
		CString		m_strAppName;
		CString		m_strMainFrameID;
		CString		m_strDocID;
		CString		m_strHubbleData;
	}TangramDocInfo;

	typedef struct CtrlInfo
	{
		HWND					m_hWnd;
		CString					m_strName;
		IGalaxyCluster*			m_pGalaxyCluster;
		IStar*					m_pNode;
		IDispatch*				m_pCtrlDisp;
	}CtrlInfo;

	typedef struct IPCSession
	{
	public:
		std::map<std::wstring, std::wstring> m_mapString;
		std::map<std::wstring, long> m_mapLong;
		std::map<std::wstring, __int64> m_mapint64;
		std::map<std::wstring, float> m_mapFloat;
	}IPCSession;

	class CSession
	{
	public:
		CSession() {}
		virtual ~CSession() {}
		
		CWebPageImpl* m_pOwner;

		virtual void InsertString(CString key, CString value) = 0;
		virtual void InsertLong(CString key, long value) = 0;
		virtual void Insertint64(CString key, __int64 value) = 0;
		virtual void InsertFloat(CString key, float value) = 0;
		virtual CString GetString(CString key) = 0;
		virtual long GetLong(CString key) = 0;
		virtual __int64 Getint64(CString key) = 0;
		virtual float GetFloat(CString key) = 0;
		virtual void SendMessage() = 0;
		virtual void addEventListener(CString ListenerName) = 0;
		virtual void removeEventListener(CString ListenerName) = 0;
	};

	class CMDIChildFormInfo
	{
	public:
		CMDIChildFormInfo() {}
		~CMDIChildFormInfo() {}
		map<CString, CString>	m_mapFormsInfo;
	};

	typedef CHubbleImpl* (__stdcall* GetHubbleImplFunction)(IHubble** ppHubble);

	class CWPFObj
	{
	public:
		CWPFObj()
		{
			m_pDisp = nullptr;
			m_hwndWPF = nullptr;
		}
		~CWPFObj() {}
		HWND m_hwndWPF;
		IDispatch* m_pDisp;
		map<CString, IDispatch*> m_mapWPFObj;
		virtual BOOL IsVisible() { return false; }
		virtual void InvalidateVisual() {}
		virtual void ShowVisual(BOOL bShow) {}
		virtual void Focusable(BOOL bFocus) {}
	};

	class IHubbleWindowProvider
	{
	public:
		IHubbleWindowProvider() {}
		virtual ~IHubbleWindowProvider() {}

		CString					m_strProviderID = _T("");
		CString					m_strContainer = _T("");
		IHubble*				m_pHubble = nullptr;
		IStar*					m_pCreatingNode = nullptr;
		map<CString, CString>	m_mapInnerObjStyle;
		map<CString, void*>		m_mapInnerObjInfo;

		virtual bool HubbleInit(CString strID) = 0;
		virtual CString GetNames() = 0;
		virtual CString GetTags(CString strName) = 0;
		virtual HWND Create(HWND hParentWnd, IStar* pNode) = 0;
	};

	class IHubbleWindow
	{
	public:
		IHubbleWindow() {}
		virtual ~IHubbleWindow() {}

		virtual void Save() = 0;
	};

	class CStarProxy
	{
	public:
		CStarProxy() { }
		virtual ~CStarProxy() {}

		bool	m_bAutoDelete;

		virtual void OnOpenComplete() {}
		virtual void OnDestroy() {}
		virtual void OnNodeAddInCreated(IDispatch* pAddIndisp, CString bstrAddInID, CString bstrAddInXml) {}
		virtual void OnNodeAddInsCreated() {}
		virtual void OnNodeDocumentComplete(IDispatch* ExtenderDisp, CString bstrURL) {}
		virtual void OnControlNotify(IStar* sender, LONG NotifyCode, LONG CtrlID, HWND CtrlHandle, CString CtrlClassName) {}
		virtual void OnTabChange(LONG ActivePage, LONG OldPage) {}
		virtual void OnHubbleDocEvent(IHubbleEventObj* pEventObj) {}
	};

	class CGalaxyClusterProxy
	{
	public:
		CGalaxyClusterProxy() { }
		virtual ~CGalaxyClusterProxy() {}

		bool	m_bAutoDelete;

		virtual void OnGalaxyClusterLoaded(IDispatch* sender, CString url) {}
		virtual void OnNodeCreated(IStar* pNodeCreated) {}
		virtual void OnAddInCreated(IStar* pRootNode, IDispatch* pAddIn, CString bstrID, CString bstrAddInXml) {}
		virtual void OnBeforeOpenXml(CString bstrXml, HWND hWnd) {}
		virtual void OnOpenXmlComplete(CString bstrXml, HWND hWnd, IStar* pRetRootNode) {}
		virtual void OnDestroy() {}
		virtual void OnNodeMouseActivate(IStar* pActiveNode) {}
		virtual void OnClrControlCreated(IStar* Node, IDispatch* Ctrl, CString CtrlName, HWND CtrlHandle) {}
		virtual void OnTabChange(IStar* sender, LONG ActivePage, LONG OldPage) {}
		virtual void OnEvent(IDispatch* sender, IDispatch* EventArg) {}
		virtual void OnControlNotify(IStar* sender, LONG NotifyCode, LONG CtrlID, HWND CtrlHandle, CString CtrlClassName) {}
		virtual void OnHubbleEvent(IHubbleEventObj* NotifyObj) {}
	};

	class CQuasarProxy
	{
	public:
		CQuasarProxy() { }
		virtual ~CQuasarProxy() {}

		bool	m_bAutoDelete;
		virtual void OnExtend(IStar* pRetNode, CString bstrKey, CString bstrXml) {}
	};

	class CHubbleDocProxy
	{
	public:
		CHubbleDocProxy() {}
		virtual ~CHubbleDocProxy()
		{
			m_bDocLoaded = false;
			m_bCanDestroyFrame = true;
			m_strHubbleData = _T("");
			m_pDoc = nullptr;
		}

		BOOL		m_bDocLoaded;
		BOOL		m_bCanDestroyFrame;
		CString		m_strTangramID;
		CString		m_strAppProxyID;
		CString		m_strAppName;
		CString		m_strMainFrameID;
		CString		m_strDocID;
		CString		m_strHubbleData;

		IHubbleDoc* m_pDoc;
		virtual void SaveDoc() {}
		virtual void HubbleDocEvent(IHubbleEventObj* pEventObj) {}
	};

	class IHubbleAppProxy
	{
	public:
		IHubbleAppProxy() {}
		virtual ~IHubbleAppProxy() {}

		BOOL								m_bAutoDelete;
		HWND								m_hMainWnd;
		HWND								m_hCreatingView;
		LPCTSTR								m_strProxyName;
		LPCTSTR								m_strProxyID;
		LPCTSTR								m_strCreatingFrameTitle;
		LPCTSTR								m_strClosingFrameID;
		void* m_pvoid;
		CHubbleDocProxy*					m_pCurDocProxy;
		CHubbleImpl*						m_pHubbleImpl;

		BOOL								m_bCreatingNewFrame;
		int									m_nFrameIndex;
		HWND								m_hClosingFrame;
		CString								m_strAppKey;
		map<CString, void*>					m_mapMainFrame;
		map<LONGLONG, IHubbleDoc*>			m_mapHubbleDoc;
		map<void*, LONG>					m_mapHubbleDocTemplateID;

		virtual BOOL InitHubble(void* pVoid) { return false; }
		virtual BOOL HubbleSaveAllModified() { return false; }

		virtual void OnActiveMainFrame(HWND) {}
		virtual int OnDestroyMainFrame(CString strID, int nMainFrameCount, int nWndType) { return 0; }
		virtual LRESULT OnForegroundIdleProc() { return 0; }
		virtual BOOL UniversePreTranslateMessage(MSG* pMsg) { return false; }
		virtual void OnHubbleClose() {}
		virtual void OnOpenComplete(HWND hWnd, CString bstrUrl, IStar* pRootNode) {}
		virtual void OnHubbleEvent(IHubbleEventObj* NotifyObj) {}
		virtual void RegistWndClassToHubble() {}
		virtual void OnActiveDocument(IHubbleDoc* ActiveDoc, IStar* pNodeInDoc, IStar* pNodeInCtrlBar, HWND hCtrlBar) {}
		virtual HWND CreateNewFrame(CString strFrameKey) { return NULL; }
		virtual HWND GetActivePopupMenu(HWND) { return NULL; }
		virtual HRESULT CreateHubbleCtrl(void* pv, REFIID riid, LPVOID* ppv) { return S_OK; }
		virtual IHubbleDoc* CreateNewDocument(LPCTSTR lpszFrameID, LPCTSTR lpszAppTitle, void* pDocTemplate, BOOL bNewFrame) { return NULL; }
		virtual IHubbleDoc* OpenDocument(void* pDocTemplate, CString strFile, BOOL bNewFrame) { return NULL; }
		virtual CStarProxy* OnHubbleNodeInit(IStar* pNewNode) { return nullptr; }
		virtual CQuasarProxy* OnQuasarCreated(IQuasar* pNewQuasar) { return nullptr; }
		virtual CGalaxyClusterProxy* OnGalaxyClusterCreated(IGalaxyCluster* pNewQuasar) { return nullptr; }
		virtual void MouseMoveProxy(HWND hWnd) {}
		virtual void AddDoc(LONGLONG llDocID, IHubbleDoc* pDoc) {}
		virtual void RemoveDoc(LONGLONG llDocID) {}
		virtual IHubbleDoc* GetDoc(LONGLONG llDocID) { return nullptr; }
		virtual IHubbleDoc* NewDoc() { return nullptr; }
		virtual HWND InitHubbleApp() { return NULL; }
	};

	class CHubblePackageProxy
	{
	public:
		CHubblePackageProxy()
		{
			m_hTangramToolWnd = nullptr;
			m_hVSGridView = nullptr;
			m_pQuasar = nullptr;
			m_pProxy = nullptr;
			m_pToolBoxFrame = nullptr;
			m_pClassViewFrame = nullptr;
			m_pPropertyFrame = nullptr;

			m_strOrgs = _T("");
			m_strRepo = _T("");
			m_strBranch = _T("");
			m_strToolBoxXML = _T("");
			m_strClassViewXML = _T("");
			m_strPropertiesXML = _T("");
			m_strTangramToolWndXML = _T("");
			m_strCurrentXtmlFilePath = _T("");
		}

		HWND								m_hTangramToolWnd;
		HWND								m_hVSGridView;
		HWND								m_hPropertyWnd;
		HWND								m_hPropertyPWnd;

		CString								m_strOrgs;
		CString								m_strRepo;
		CString								m_strBranch;
		CString								m_strToolBoxXML;
		CString								m_strClassViewXML;
		CString								m_strPropertiesXML;
		CString								m_strTangramToolWndXML;
		CString								m_strCurrentXtmlFilePath;

		IQuasar*							m_pQuasar;
		CHubbleImpl*						m_pProxy;
		map<HWND, IQuasar*>					m_mapWinFormQuasar;

		IQuasar*							m_pToolBoxFrame;
		IQuasar*							m_pClassViewFrame;
		IQuasar*							m_pPropertyFrame;

		virtual IDispatch* CreateToolWnd(CString strXml, bool bShow) { return nullptr; }
		virtual void* HubbleAction(CString strKey, CString strXml) { return nullptr; }
	};

	class IHubbleCLRImpl
	{
	public:
		IHubbleCLRImpl()
		{
		}

		CString							m_strCurrentWinFormTemplate;

		virtual HRESULT ActiveCLRMethod(BSTR bstrObjID, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData) = 0;
		virtual HRESULT ActiveCLRMethod(IDispatch* obj, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData) = 0;
		virtual IDispatch* CreateCLRObj(CString bstrObjID) = 0;
		virtual HRESULT ProcessCtrlMsg(HWND hCtrl, bool bShiftKey) = 0;
		virtual BOOL ProcessFormMsg(HWND hFormWnd, LPMSG lpMsg, int nMouseButton) = 0;
		virtual IDispatch* CreateObject(BSTR bstrObjID, HWND hParent, IStar* pHostNode) = 0;
		virtual int IsWinForm(HWND hWnd) = 0;
		virtual int IsSpecifiedType(IUnknown* pUnknown, BSTR bstrName) = 0;
		virtual IDispatch* GetCLRControl(IDispatch* CtrlDisp, BSTR bstrNames) = 0;
		virtual BSTR GetCtrlName(IDispatch* pCtrl) = 0;
		virtual IDispatch* GetCtrlFromHandle(HWND hWnd) = 0;
		virtual HWND GetMDIClientHandle(IDispatch* pMDICtrl) = 0;
		virtual IDispatch* GetCtrlByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild) = 0;
		virtual HWND GetCtrlHandle(IDispatch* pCtrl) = 0;
		virtual BSTR GetCtrlType(IDispatch* pCtrl) = 0;
		virtual HWND IsCtrlCanNavigate(IDispatch* ctrl) = 0;
		virtual void HubbleAction(BSTR bstrXml, void*) = 0;
		virtual BSTR GetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild) = 0;
		virtual void SetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild, BSTR strVal) = 0;
		virtual void SelectNode(IStar*) {}
		virtual void SelectObj(IDispatch*) {}
		virtual void ReleaseHubbleObj(IDispatch*) {}
		virtual void AttachVSPropertyWnd(HWND) {}
		virtual void WindowCreated(LPCTSTR strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd) {}
		virtual void WindowDestroy(HWND hWnd) {}
		virtual CWPFObj* CreateWPFControl(IStar* pNode, HWND hPWnd, UINT nID) { return nullptr; }
		virtual HRESULT NavigateURL(IStar* pNode, CString strURL, IDispatch* dispObjforScript) { return S_FALSE; }
		virtual void OnCLRHostExit() {}
		virtual void OnDestroyChromeBrowser(IBrowser*) {}
		virtual void ExportAllFormsInfo() {}
		virtual void SetObjectProperty(IDispatch* pObj, BSTR bstrPropertyName, BSTR bstrPropertyValue) {}
		virtual void* Extend(CString strKey, CString strData, CString strFeatures) { return nullptr; }
		virtual bool IsSupportDesigner() { return false; }
		virtual HICON GetAppIcon(int nIndex) = 0;
		virtual void OnWinFormActivate(HWND, int nState) = 0;
		virtual IDispatch* CreateWinForm(HWND hParent, BSTR strXML) = 0;
		virtual void OnWebPageCreated(HWND, CWebPageImpl*, IWebPage* pChromeWebContent, int nState) = 0;
		virtual void HideMenuStripPopup() = 0;
		virtual bool PreWindowPosChanging(HWND hWnd, WINDOWPOS* lpwndpos, int nType) = 0;
		virtual void OnCloudMsgReceived(CSession*) = 0;
		virtual void ConnectNodeToWebPage(IStar*, bool) = 0;
	};

	class CHubbleImpl {
	public:
		CHubbleImpl() {
			m_bClose = false;
			m_bInit = false;
			m_hParent = NULL;
			m_hSmallIcon = nullptr;
			m_hLargeIcon = nullptr;
			m_bExportComponent = false;
			m_hHostWnd = nullptr;
			m_hFormNodeWnd = nullptr;
			m_hMainWnd = nullptr;
			m_hTangramWnd = nullptr;
			m_pCLRProxy = nullptr;
			m_hChildHostWnd = nullptr;
			m_pActiveAppProxy = nullptr;
			m_pCosmosAppProxy = nullptr;
			m_pUniverseAppProxy = nullptr;
			m_pCurMDIChildFormInfo = nullptr;
			m_strNtpXml = _T("");
			m_pHubblePackageProxy = nullptr;
			m_strAppCurrentFormTemplatePath = _T("");
		}

		virtual ~CHubbleImpl() {}

		bool									m_bClose;
		bool									m_bInit;
		bool									m_bChromeNeedClosed;
		bool									m_bCreatingDevTool;
		bool									m_bOMNIBOXPOPUPVISIBLE;
		bool									m_b64bitSystem = false;
		bool									m_bX86App = false;
		bool									m_bOfficeApp = false;
		bool									m_bEclipse = false;
		bool									m_bLoadEclipseDelay = false;
		bool									m_bHubbleInit = false;
		bool									m_bAdmin;
		bool									m_bCanClose;
		bool									m_bWinFormActived;
		bool									m_bEnableProcessFormTabKey;
		bool									m_bDeleteGalaxyCluster;
		bool									m_bNewFile;
		bool									m_bFirstDocCreated;
		bool									m_bUsingDefaultAppDocTemplate;
		bool									m_bCreatingForm;
		bool									m_bCLRObjTemplateInit;
		bool									m_bJniInit;
		bool									m_bExportComponent;
		bool									m_bIsSupportCrashReporting = false;
		bool									m_bIsAppInit = false;
		bool									m_bIsEclipseInit = false;
		bool									m_bIsChromeRunning = false;

		DWORD									m_dwThreadID;
		DWORD									m_nAppType;
		HWND									m_hTemplateWnd;
		HWND									m_hTemplateChildWnd;
		HWND									m_hActiveWnd;
		HWND									m_hEclipseHideWnd;
		HWND									m_hMainWnd;
		HWND									m_hHostWnd;
		HWND									m_hChildHostWnd;
		HWND									m_hTangramWnd;
		HWND									m_hFormNodeWnd;
		HWND									m_hParent;
		HWND									m_hHostBrowserWnd;
		HWND									m_hVSToolBoxWnd;
		HICON									m_hLargeIcon;
		HICON									m_hSmallIcon;

		CString									m_strAppKey;
		CString									m_strAppName;
		CString									m_strExeName;
		CString									m_strAppPath;
		CString									m_strTempPath;
		CString 								m_strConfigDataFile;
		CString									m_strAppDataPath;
		CString									m_strCurrentAppID;
		CString									m_strProgramFilePath;
		CString									m_strAppCommonDocPath;
		CString									m_strAppCommonDocPath2;
		CString									m_strAppCommonFormsPath;
		CString									m_strAppFormsPath;
		CString									m_strAppFormsInfoPath;
		CString									m_strAppWPFObjsInfoPath;
		CString									m_strAppControlsInfoPath;
		CString									m_strAppFormsTemplatePath;
		CString									m_strAppCurrentFormTemplatePath;
		CString									m_strNodeSelectedText;
		CString									m_strDesignerTip1;
		CString									m_strDesignerTip2;
		CString									m_strDesignerXml;
		CString									m_strDesignerToolBarCaption;
		CString									m_strStartView;
		CString									m_strNewDocXml;
		CString									m_strStartXml;
		CString									m_strNtpXml;

		CString 								m_strConfigFile;
		CString									m_strDocFilters;
		CString									m_strDesignerInfo;
		CString									m_strTemplatePath;
		CString									m_strCurrentFrameID;
		CString									m_strDocTemplateStrs;
		CString									m_strDefaultTemplate;
		CString									m_strDefaultTemplate2;
		CString									m_strCurrentDocTemplateXml;
		CString									m_strCurrentKey;
		CString									m_strWorkBenchStrs;
		CString									m_strStartupURL = _T("");
		CString									m_strStartupCLRObj;
		CString									m_strCurrentEclipsePagePath;
		CString									m_strDefaultXml;
		CString									m_strLibs;

		CStringA								m_strBridgeJavaClass;
		CString									m_strStartJarPath;

		IPCMsg*									m_pCurrentIPCMsg;
		IHubbleCLRImpl*							m_pCLRProxy;
		IHubbleAppProxy*						m_pActiveAppProxy;
		IHubbleAppProxy*						m_pUniverseAppProxy;
		IHubbleAppProxy*						m_pCosmosAppProxy;
		CMDIChildFormInfo*						m_pCurMDIChildFormInfo;
		CHubblePackageProxy*					m_pHubblePackageProxy;
		IDispatch*								m_pMainFormDisp = nullptr;
		IDispatch*								m_pAppDisp = nullptr;
		IStar*									m_pHostViewDesignerNode = nullptr;
		IHubbleExtender*						m_pExtender = nullptr;
		IHubbleDelegate*						m_pHubbleDelegate = nullptr;
		CChromeBrowserBase*						m_pActiveBrowser = nullptr;
		CHubbleBrowserFactory*					m_pBrowserFactory = nullptr;
		IHubbleWindow*							m_pCreatingWindow = nullptr;
		OmniboxViewViewsProxy*					m_pCreatingOmniboxViewViews = nullptr;
		CChromeRenderFrameHost*					m_pCreatingChromeRenderFrameHostBase = nullptr;
		CWebPageImpl*							m_pMainChromeRenderFrameHostProxy = nullptr;

		map<CString, IDispatch*>				m_mapObjDic;
		map<HWND, IGalaxyCluster*>				m_mapFramePage;
		map<HWND, IGalaxyCluster*>				m_mapWindowPage;
		map<CString, IDispatch*>				m_mapAppDispDic;
		map<CString, CComVariant>				m_mapValInfo;
		map<CString, void*>						m_mapTemplateInfo;
		map<CString, IHubble*>					m_mapRemoteHubble;
		map<IStar*, CString>					m_mapControlScript;
		map<CString, void*>						m_mapExcludedObjects;
		map<CString, IHubbleAppProxy*>			m_mapHubbleAppProxy;
		map<CString, IHubbleWindowProvider*>	m_mapWindowProvider;
		map<int, HubbleDocTemplateInfo*>		m_mapHubbleDocTemplateInfo;
		map<CString, HubbleDocTemplateInfo*>	m_mapHubbleDocTemplateInfo2;
		map<CString, HubbleDocTemplateInfo*>	m_mapHubbleFormsTemplateInfo;
		map<int, HubbleDocTemplateInfo*>		m_mapHubbleFormsTemplateInfo2;
		map<HWND, CWebPageImpl*>				m_mapHtmlWnd;
		map<HWND, IWebPage*>					m_mapFormWebPage;
		map<HWND, IBrowser*>					m_mapBrowserWnd;
		map<HWND, IWorkBenchWindow*>			m_mapWorkBenchWnd;
		map<void*, IUnknown*>					m_mapObjects;
		map<IDispatch*, CString>				m_mapObjEventDic;
		map<CString, CString>					m_mapJavaNativeInfo;
		map<CString, CString>					m_mapCreatingWorkBenchInfo;
		map<HWND, HWND>							m_mapVSWebPage;

		virtual void BrowserAppStart() = 0;
		virtual IQuasar* ConnectGalaxyCluster(HWND, CString, IGalaxyCluster* pGalaxyCluster, QuasarInfo*) { return nullptr; }
		virtual void OnSubBrowserWndCreated(HWND hParent, HWND hBrowser) = 0;
		virtual void OnRenderProcessCreated(CChromeRenderProcess* pProcess) = 0;
		virtual void OnDocumentOnLoadCompleted(CChromeRenderFrameHost*,	HWND hHtmlWnd, void*) = 0;
		virtual void ChromeTabCreated(CChromeTab* pTab) = 0;
		virtual CString GetProcessPath(const char* _ver, CString process_type) = 0;
		virtual CString GetSchemeBaseName() = 0;
		virtual HICON GetAppIcon(int nIndex) = 0;
		virtual CChromeBrowserBase* GetChromeBrowserBase(HWND) = 0;
		virtual IBrowser* GetHostBrowser(HWND hNodeWnd) = 0;
		virtual void AttachNode(void* pNodeEvents) {}
		virtual void HubbleInit() {}
		virtual IHubbleDoc* ConnectHubbleDoc(IHubbleAppProxy* AppProxy, LONGLONG docID, HWND hView, HWND hFrame, LPCTSTR strDocType) { return nullptr; }
		virtual CString GetNewLayoutNodeName(BSTR strCnnID, IStar* pDesignNode) { return _T(""); }
		virtual IGalaxyCluster* Observe(HWND, CString strName, CString strKey) { return nullptr; }
		virtual IStar* ObserveCtrl(__int64 handle, CString name, CString NodeTag) { return nullptr; }
		virtual bool IsMDIClientQuasarNode(IStar*) { return false; }
		virtual void DotNetControlCreated(MSG* lpMsg) {}
		virtual void ExportComponentInfo() {}
		virtual void ConnectDocTemplate(LPCTSTR strType, LPCTSTR strExt, void* pTemplate) {}
		virtual void InserttoDataMap(int nType, CString strKey, void* pData) {}
		virtual IWebPage* GetWebPageFromForm(HWND) { return nullptr; }
		virtual long GetIPCMsgIndex(CString strMsgID) { return 0; }
		virtual CSession* CreateCloudSession(CWebPageImpl*) { return nullptr; }
		virtual CSession* GetCloudSession(IStar*) { return nullptr; }
		virtual void ReleaseCLR() {}
		virtual void SetMainWnd(HWND hMain) {}
		virtual void HubbleNotify(CString strPara1, CString strPara2, WPARAM, LPARAM) {}
	};

	class IHubbleDelegate {
	public:
		IHubbleDelegate() {
			m_bBrowserWndCreated = false;
			m_pJVM = nullptr;
			m_pJVMenv = nullptr;
			systemClass = nullptr;
			exitMethod = nullptr;
			loadMethod = nullptr;
		}

		virtual ~IHubbleDelegate() {}

		BOOL				m_bBrowserWndCreated;
		JavaVM*				m_pJVM;
		JNIEnv*				m_pJVMenv;
		jclass				systemClass;
		jmethodID			exitMethod;
		jmethodID			loadMethod;

		virtual void ProcessMsg(MSG* msg) = 0;
		virtual void ForegroundIdleProc() = 0;
		virtual bool DoIdleWork() = 0;
		virtual BOOL OnAppIdle(BOOL& bIdle, LONG lCount) = 0;
		virtual BOOL IsAppIdleMessage() = 0;
		virtual bool OnUniversePreTranslateMessage(MSG* pMsg) = 0;
		virtual CString GetNTPXml() = 0;
		virtual CString GetNTPData() = 0;
		virtual HWND GetMainWnd() = 0;
		virtual bool BrowserAppStart() = 0;
		virtual HWND QueryCanClose(HWND hWnd) = 0;
		virtual bool GetClientAreaBounds(HWND hWnd, RECT& rc) = 0;
		virtual bool HookAppDocTemplateInfo() = 0;
		virtual void BeginBrowserSession() = 0;
		virtual void EndBrowserSession() = 0;
		virtual bool EclipseAppInit() = 0;
		virtual void IPCMsg(HWND hWnd, CString strType, CString strParam1, CString strParam2) = 0;
		virtual void RenderHTMLElement(HWND hWnd, CString strRuleName, CString strHTML) = 0;
		virtual void HubbleNotify(CString strPara1, CString strPara2, WPARAM, LPARAM) {}
	};

	class CHubbleMainDllLoader {
	public:
		CHubbleMainDllLoader() {}
		virtual ~CHubbleMainDllLoader() {}
		virtual void LaunchEx(CHubbleImpl*) {}
	};

	class CHubbleBrowserFactory {
	public:
		CHubbleBrowserFactory() {}
		virtual ~CHubbleBrowserFactory() {}

		virtual HWND CreateBrowser(HWND hParent, CString strXml) = 0;
	};

	class CChromeChildProcessHostImpl {
	public:
		CChromeChildProcessHostImpl() { m_pChildProcessHostProxy = nullptr; }
		virtual ~CChromeChildProcessHostImpl() {}

		CChromeChildProcessHostImplProxy* m_pChildProcessHostProxy;
	};

	class CChromeChildProcessHostImplProxy {
	public:
		CChromeChildProcessHostImplProxy() {}

		virtual ~CChromeChildProcessHostImplProxy() {}

		virtual void OnRenderProcessCreated() = 0;
	};

	class CChromeRenderProcess {
	public:
		CChromeRenderProcess() { m_pRenderProcessProxy = nullptr; }
		virtual ~CChromeRenderProcess() {}

		CChromeRenderProcessProxy* m_pRenderProcessProxy;
	};

	class CChromeRenderProcessProxy {
	public:
		CChromeRenderProcessProxy() {}

		virtual ~CChromeRenderProcessProxy() {}

		virtual CString ProcessPath() = 0;
		virtual void OnRenderProcessCreated() = 0;
	};

	class CChromeTab {
	public:
		CChromeTab() {
			m_pTabProxy = nullptr;
			HMODULE hModule = ::GetModuleHandle(L"universe.dll");
			if (hModule != nullptr) {
				GetHubbleImplFunction GetHubbleImplFunction =
					(CommonUniverse::GetHubbleImplFunction)GetProcAddress(
						hModule, "GetHubbleImpl");
				if (GetHubbleImplFunction != NULL) {
					IHubble* pHubble = nullptr;
					CHubbleImpl* _pImpl = GetHubbleImplFunction(&pHubble);
					if (_pImpl) {
						_pImpl->ChromeTabCreated(this);
					}
				}
			}
		}
		virtual ~CChromeTab() {
		}

		CChromeTabProxy* m_pTabProxy;
	};

	class CChromeTabProxy {
	public:
		CChromeTabProxy() {
			m_pTab = nullptr;
		}
		virtual ~CChromeTabProxy() {
		}
		CChromeTab* m_pTab;
	};

	class OmniboxViewViewsProxy {
	public:
		OmniboxViewViewsProxy() {
			HMODULE hModule = ::GetModuleHandle(L"universe.dll");
			if (hModule != nullptr) {
				GetHubbleImplFunction GetHubbleImplFunction =
					(CommonUniverse::GetHubbleImplFunction)GetProcAddress(
						hModule, "GetHubbleImpl");
				if (GetHubbleImplFunction != NULL) {
					IHubble* pHubble = nullptr;
					CHubbleImpl* _pImpl = GetHubbleImplFunction(&pHubble);
					if (_pImpl) {
						_pImpl->m_pCreatingOmniboxViewViews = this;
					}
				}
			}
		}
		virtual ~OmniboxViewViewsProxy() {}

		virtual void Blur() = 0;
		virtual bool IsFocused() = 0;
	};

	class CChromeBrowserBase {
	public:
		CChromeBrowserBase() {
			HMODULE hModule = ::GetModuleHandle(L"universe.dll");
			if (hModule != nullptr) {
				GetHubbleImplFunction GetHubbleImplFunction =
					(CommonUniverse::GetHubbleImplFunction)GetProcAddress(
						hModule, "GetHubbleImpl");
				if (GetHubbleImplFunction != NULL) {
					IHubble* pTangram = nullptr;
					CHubbleImpl* _pImpl = GetHubbleImplFunction(&pTangram);
					if (_pImpl) {
						m_pProxy = nullptr;
						_pImpl->m_pActiveBrowser = this;
					}
				}
			}
		}
		virtual ~CChromeBrowserBase() {}

		CBrowserImpl* m_pProxy;

		virtual int GetType() = 0;
		virtual void* GetBrowser() = 0;
		virtual void LayoutBrowser() = 0;
		virtual void OpenURL(std::wstring strURL,
			BrowserWndOpenDisposition nPos,
			void* pVoid) = 0;
		virtual HWND GetActiveWebContentWnd() = 0;
		virtual bool IsActiveWebContentWnd(HWND hWebWnd) = 0;
	};

	class CBrowserImpl {
	public:
		CBrowserImpl() {}
		virtual ~CBrowserImpl() {}

		CChromeBrowserBase*		m_pBrowser;
		OmniboxViewViewsProxy*	m_pOmniboxViewViews;

		virtual void UpdateContentRect(HWND hWebContent, RECT& rc, int nTopFix) = 0;
		virtual void ActiveChromeTab(HWND hActive, HWND hOldWnd) = 0;
	};

	class CChromeWebContentBase {
	public:
		CChromeWebContentBase() { m_pProxy = nullptr; }

		virtual ~CChromeWebContentBase() {}

		CChromeWebContentProxyBase* m_pProxy;

		virtual CChromeRenderFrameHost* GetMainRenderFrameHost() = 0;
	};

	class CChromeWebContentProxyBase {
	public:
		CChromeWebContentProxyBase() { m_pWebContent = nullptr; }

		virtual ~CChromeWebContentProxyBase() {}

		CChromeWebContentBase* m_pWebContent;
	};

	class CChromeRenderFrameHost {
	public:
		CChromeRenderFrameHost() {
			m_pProxy = nullptr;
		}

		virtual ~CChromeRenderFrameHost() {
			for (auto it : m_mapHubbleSession)
			{
				delete it.second;
			}
			m_mapHubbleSession.erase(m_mapHubbleSession.begin(), m_mapHubbleSession.end());
		}

		CWebPageImpl* m_pProxy;
		map<CString, IPCSession*> m_mapHubbleSession;
		virtual void ShowWebPage(bool bShow) = 0;
		virtual void SendHubbleMessage(IPCMsg*) = 0;
		virtual void SendHubbleMessage(IPCSession* var) = 0;
		virtual IPCSession* GetIPCSession() = 0;
		virtual void InsertString(IPCSession* , CString key, CString value) = 0;
		virtual void InsertLong(IPCSession* , CString key, long value) = 0;
		virtual void Insertint64(IPCSession* , CString key, __int64 value) = 0;
		virtual void InsertFloat(IPCSession* , CString key, float value) = 0;
		virtual CString GetString(IPCSession*, CString key) = 0;
		virtual long GetLong(IPCSession*, CString key) = 0;
		virtual __int64 Getint64(IPCSession*, CString key) = 0;
		virtual float GetFloat(IPCSession*, CString key) = 0;
	};

	class CWebPageImpl {
	public:
		CWebPageImpl() { 
			m_pRemoteHubble = nullptr;
			m_pChromeRenderFrameHost = nullptr; 
		}

		virtual ~CWebPageImpl() 
		{
			for (auto it = m_mapBindWebObj.begin(); it != m_mapBindWebObj.end(); it++)
			{
				delete it->second;
			}
			m_mapBindWebObj.erase(m_mapBindWebObj.begin(), m_mapBindWebObj.end());
		}

		IHubble* m_pRemoteHubble = nullptr;
		map<CString, BindWebObj*>	m_mapBindWebObj;
		CChromeRenderFrameHost*		m_pChromeRenderFrameHost;
		virtual void SendChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5) = 0;
		virtual CChromeBrowserBase* GetChromeBrowserBase(HWND) = 0;
		virtual void OnWinFormCreated(HWND) = 0;
		virtual IStar* GetParentNode() = 0;
		virtual IQuasar* GetQuasar() = 0;
		virtual void OnCloudMsgReceived(CSession*) = 0;
	};

	class CChromeRendererFrameBase {
	public:
		CChromeRendererFrameBase() {}

		virtual ~CChromeRendererFrameBase() {}
	};

}  // namespace CommonUniverse
