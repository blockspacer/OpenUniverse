#pragma once

extern _ATL_FUNC_INFO NavigateComplete;
extern _ATL_FUNC_INFO Destroy;
extern _ATL_FUNC_INFO DocumentComplete;

class CGridEvents : public IDispEventSimpleImpl</*nID =*/ 1, CGridEvents, &__uuidof(_IGridEvents)>
{
public:
	CGridEvents(){};
	virtual ~CGridEvents(){};
	virtual void __stdcall  OnNavigateComplete(){}
	virtual void __stdcall  OnDestroy(){}
	virtual void __stdcall  OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl){}
	BEGIN_SINK_MAP(CGridEvents)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000001, OnNavigateComplete, &NavigateComplete)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000002, OnDestroy, &Destroy)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000003, OnDocumentComplete, &DocumentComplete)
	END_SINK_MAP()
};