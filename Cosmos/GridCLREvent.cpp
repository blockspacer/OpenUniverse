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

#include "stdafx.h"
#include "GridCLREvent.h"

using namespace System::Runtime::InteropServices;

CGridCLREvent::CGridCLREvent()
{
}


CGridCLREvent::~CGridCLREvent()
{
	//delete m_pGrid;
}

void CGridCLREvent::OnObserverComplete(IGrid* pGrid)
{
	m_pGrid->Fire_ObserveComplete(m_pGrid);
}

void CGridCLREvent::OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl)
{
	Object^ pObj = reinterpret_cast<Object^>(Marshal::GetObjectForIUnknown((System::IntPtr)(pDocdisp)));
	m_pGrid->Fire_OnDocumentComplete(m_pGrid, pObj, BSTR2STRING(bstrUrl));
}

void CGridCLREvent::OnDestroy()
{
	if (m_pGrid->m_pHostObj)
	{
		Cosmos::Grid^ pGrid = nullptr;
		if (Cosmos::Hubble::m_pFrameworkElementDic->TryGetValue(m_pGrid->m_pHostObj, pGrid))
		{
			Cosmos::Hubble::m_pFrameworkElementDic->Remove(m_pGrid->m_pHostObj);
		}
	}
	m_pGrid->Fire_OnDestroy(m_pGrid);
}

void CGridCLREvent::OnGridAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml)
{
}

void CGridCLREvent::OnGridAddInsCreated()
{
}

void CGridCLREvent::OnTabChange(int nActivePage, int nOldPage)
{
	Cosmos::Grid^ pActiveGrid = m_pGrid->GetGrid(0, nActivePage);
	Cosmos::Grid^ pOldGrid = m_pGrid->GetGrid(0, nOldPage);
	m_pGrid->Fire_OnTabChange(pActiveGrid, pOldGrid);
}

void CGridCLREvent::OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
{
	m_pGrid->Fire_OnIPCMessageReceived(BSTR2STRING(bstrFrom), BSTR2STRING(bstrTo), BSTR2STRING(bstrMsgId), BSTR2STRING(bstrPayload), BSTR2STRING(bstrExtra));
}
