#pragma once

extern _ATL_FUNC_INFO NavigateComplete;
extern _ATL_FUNC_INFO Destroy;
extern _ATL_FUNC_INFO DocumentComplete;

class CWndGridEvents : public IDispEventSimpleImpl</*nID =*/ 1, CWndGridEvents, &__uuidof(_IGridEvents)>
{
public:
	CWndGridEvents(){};
	virtual ~CWndGridEvents(){};
	virtual void __stdcall  OnNavigateComplete(){}
	virtual void __stdcall  OnDestroy(){}
	virtual void __stdcall  OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl){}
	BEGIN_SINK_MAP(CWndGridEvents)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000001, OnNavigateComplete, &NavigateComplete)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000002, OnDestroy, &Destroy)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000003, OnDocumentComplete, &DocumentComplete)
	END_SINK_MAP()
};