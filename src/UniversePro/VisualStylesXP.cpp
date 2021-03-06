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


#include "stdafx.h"
#include "visualstylesxp.h"
#include <Shlwapi.h>

CVisualStylesXP g_xpStyle;

CVisualStylesXP::CVisualStylesXP(void)
{
	m_hThemeDll = LoadLibrary(_T("UxTheme.dll"));
}

CVisualStylesXP::~CVisualStylesXP(void)
{
	if (m_hThemeDll!=NULL)
		FreeLibrary(m_hThemeDll);
	m_hThemeDll = NULL;
}

void* CVisualStylesXP::GetProc(LPCSTR szProc, void* pfnFail)
{
	void* pRet = pfnFail;
	if (m_hThemeDll != NULL)
		pRet = GetProcAddress(m_hThemeDll, szProc);
	return pRet;
}

HTHEME CVisualStylesXP::OpenThemeData(HWND hwnd, LPCWSTR pszClassList)
{
	static PFNOPENTHEMEDATA pfnOpenThemeData = nullptr;
	if( !pfnOpenThemeData )
		pfnOpenThemeData = (PFNOPENTHEMEDATA)GetProc("OpenThemeData", (void*)OpenThemeDataFail);
	return (*pfnOpenThemeData)(hwnd, pszClassList);
}

HRESULT CVisualStylesXP::CloseThemeData(HTHEME hTheme)
{
	static PFNCLOSETHEMEDATA pfnCloseThemeData = nullptr;
	if( !pfnCloseThemeData )
		pfnCloseThemeData = (PFNCLOSETHEMEDATA)GetProc("CloseThemeData", (void*)CloseThemeDataFail);
	return (*pfnCloseThemeData)(hTheme);
}

HRESULT CVisualStylesXP::DrawThemeBackground(HTHEME hTheme, HDC hdc, 
											 int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect)
{
	static PFNDRAWTHEMEBACKGROUND pfnDrawThemeBackground = nullptr;
	if( !pfnDrawThemeBackground )
		pfnDrawThemeBackground = (PFNDRAWTHEMEBACKGROUND)GetProc("DrawThemeBackground", (void*)DrawThemeBackgroundFail);
	return (*pfnDrawThemeBackground)(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}


HRESULT CVisualStylesXP::DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, 
									   int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, 
									   DWORD dwTextFlags2, const RECT *pRect)
{
	static PFNDRAWTHEMETEXT pfn = nullptr;
	if( !pfn )
		pfn = (PFNDRAWTHEMETEXT)GetProc("DrawThemeText", (void*)DrawThemeTextFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect);
}
HRESULT CVisualStylesXP::GetThemeBackgroundContentRect(HTHEME hTheme,  HDC hdc, 
													   int iPartId, int iStateId,  const RECT *pBoundingRect, 
													   RECT *pContentRect)
{
	static PFNGETTHEMEBACKGROUNDCONTENTRECT pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEBACKGROUNDCONTENTRECT)GetProc("GetThemeBackgroundContentRect", (void*)GetThemeBackgroundContentRectFail);
	return (*pfn)(hTheme,  hdc, iPartId, iStateId,  pBoundingRect, pContentRect);
}
HRESULT CVisualStylesXP::GetThemeBackgroundExtent(HTHEME hTheme,  HDC hdc,
												  int iPartId, int iStateId, const RECT *pContentRect, 
												  RECT *pExtentRect)
{
	static PFNGETTHEMEBACKGROUNDEXTENT pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEBACKGROUNDEXTENT)GetProc("GetThemeBackgroundExtent", (void*)GetThemeBackgroundExtentFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId, pContentRect, pExtentRect);
}
HRESULT CVisualStylesXP::GetThemePartSize(HTHEME hTheme, HDC hdc, 
										  int iPartId, int iStateId, RECT * pRect, enum THEMESIZE eSize, SIZE *psz)
{
	static PFNGETTHEMEPARTSIZE pfnGetThemePartSize = nullptr;
	if( !pfnGetThemePartSize )
		pfnGetThemePartSize = (PFNGETTHEMEPARTSIZE)GetProc("GetThemePartSize", (void*)GetThemePartSizeFail);
	return (*pfnGetThemePartSize)(hTheme, hdc, iPartId, iStateId, pRect, eSize, psz);
}

HRESULT CVisualStylesXP::GetThemeTextExtent(HTHEME hTheme, HDC hdc, 
											int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, 
											DWORD dwTextFlags,  const RECT *pBoundingRect, 
											RECT *pExtentRect)
{
	static PFNGETTHEMETEXTEXTENT pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMETEXTEXTENT)GetProc("GetThemeTextExtent", (void*)GetThemeTextExtentFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags,  pBoundingRect, pExtentRect);
}

HRESULT CVisualStylesXP::GetThemeTextMetrics(HTHEME hTheme,  HDC hdc, 
											 int iPartId, int iStateId,  TEXTMETRIC* ptm)
{
	static PFNGETTHEMETEXTMETRICS pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMETEXTMETRICS)GetProc("GetThemeTextMetrics", (void*)GetThemeTextMetricsFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId,  ptm);
}

HRESULT CVisualStylesXP::GetThemeBackgroundRegion(HTHEME hTheme,  HDC hdc,  
												  int iPartId, int iStateId, const RECT *pRect,  HRGN *pRegion)
{
	static PFNGETTHEMEBACKGROUNDREGION pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEBACKGROUNDREGION)GetProc("GetThemeBackgroundRegion", (void*)GetThemeBackgroundRegionFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId, pRect, pRegion);
}

HRESULT CVisualStylesXP::HitTestThemeBackground(HTHEME hTheme,  HDC hdc, int iPartId, 
												int iStateId, DWORD dwOptions, const RECT *pRect,  HRGN hrgn, 
												POINT ptTest,  WORD *pwHitTestCode)
{
	static PFNHITTESTTHEMEBACKGROUND pfn = nullptr;
	if( !pfn )
		pfn = (PFNHITTESTTHEMEBACKGROUND)GetProc("HitTestThemeBackground", (void*)HitTestThemeBackgroundFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId, dwOptions, pRect, hrgn, ptTest, pwHitTestCode);
}

HRESULT CVisualStylesXP::DrawThemeEdge(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, 
									   const RECT *pDestRect, UINT uEdge, UINT uFlags,   RECT *pContentRect)
{
	static PFNDRAWTHEMEEDGE pfn = nullptr;
	if( !pfn )
		pfn = (PFNDRAWTHEMEEDGE)GetProc("DrawThemeEdge", (void*)DrawThemeEdgeFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId, pDestRect, uEdge, uFlags, pContentRect);
}

HRESULT CVisualStylesXP::DrawThemeIcon(HTHEME hTheme, HDC hdc, int iPartId, 
									   int iStateId, const RECT *pRect, HIMAGELIST himl, int iImageIndex)
{
	static PFNDRAWTHEMEICON pfn = nullptr;
	if( !pfn )
		pfn = (PFNDRAWTHEMEICON)GetProc("DrawThemeIcon", (void*)DrawThemeIconFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId, pRect, himl, iImageIndex);
}

BOOL CVisualStylesXP::IsThemePartDefined(HTHEME hTheme, int iPartId, 
										 int iStateId)
{
	static PFNISTHEMEPARTDEFINED pfn = nullptr;
	if( !pfn )
		pfn = (PFNISTHEMEPARTDEFINED)GetProc("IsThemePartDefined", (void*)IsThemePartDefinedFail);
	return (*pfn)(hTheme, iPartId, iStateId);
}

BOOL CVisualStylesXP::IsThemeBackgroundPartiallyTransparent(HTHEME hTheme, 
															int iPartId, int iStateId)
{
	static PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT pfn = nullptr;
	if( !pfn )
		pfn = (PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT)GetProc("IsThemeBackgroundPartiallyTransparent", (void*)IsThemeBackgroundPartiallyTransparentFail);
	return (*pfn)(hTheme, iPartId, iStateId);
}

HRESULT CVisualStylesXP::GetThemeColor(HTHEME hTheme, int iPartId, 
									   int iStateId, int iPropId,  COLORREF *pColor)
{
	static PFNGETTHEMECOLOR pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMECOLOR)GetProc("GetThemeColor", (void*)GetThemeColorFail);
	return (*pfn)(hTheme, iPartId, iStateId, iPropId, pColor);
}

HRESULT CVisualStylesXP::GetThemeMetric(HTHEME hTheme,  HDC hdc, int iPartId, 
										int iStateId, int iPropId,  int *piVal)
{
	static PFNGETTHEMEMETRIC pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEMETRIC)GetProc("GetThemeMetric", (void*)GetThemeMetricFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId, iPropId, piVal);
}

HRESULT CVisualStylesXP::GetThemeString(HTHEME hTheme, 
										int iPartId, 
										int iStateId, 
										int iPropId,  
										LPWSTR pszBuff, 
										int cchMaxBuffChars)
{
	static PFNGETTHEMESTRING pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMESTRING)GetProc("GetThemeString", (void*)GetThemeStringFail);
	return (*pfn)(hTheme, iPartId, iStateId, iPropId, pszBuff, cchMaxBuffChars);
}

HRESULT CVisualStylesXP::GetThemeBool(HTHEME hTheme, int iPartId, 
									  int iStateId, int iPropId,  BOOL *pfVal)
{
	static PFNGETTHEMEBOOL pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEBOOL)GetProc("GetThemeBool", (void*)GetThemeBoolFail);
	return (*pfn)(hTheme, iPartId, iStateId, iPropId, pfVal);
}

HRESULT CVisualStylesXP::GetThemeInt(HTHEME hTheme, int iPartId, 
									 int iStateId, int iPropId,  int *piVal)
{
	static PFNGETTHEMEINT pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEINT)GetProc("GetThemeInt", (void*)GetThemeIntFail);
	return (*pfn)(hTheme, iPartId, iStateId, iPropId, piVal);
}

HRESULT CVisualStylesXP::GetThemeEnumValue(HTHEME hTheme, int iPartId, 
										   int iStateId, int iPropId,  int *piVal)
{
	static PFNGETTHEMEENUMVALUE pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEENUMVALUE)GetProc("GetThemeEnumValue", (void*)GetThemeEnumValueFail);
	return (*pfn)(hTheme, iPartId, iStateId, iPropId, piVal);
}

HRESULT CVisualStylesXP::GetThemePosition(HTHEME hTheme, int iPartId, 
										  int iStateId, int iPropId,  POINT *pPoint)
{
	static PFNGETTHEMEPOSITION pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEPOSITION)GetProc("GetThemePosition", (void*)GetThemePositionFail);
	return (*pfn)(hTheme, iPartId, iStateId, iPropId, pPoint);
}

HRESULT CVisualStylesXP::GetThemeFont(HTHEME hTheme,  HDC hdc, int iPartId, 
									  int iStateId, int iPropId,  LOGFONT *pFont)
{
	static PFNGETTHEMEFONT pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEFONT)GetProc("GetThemeFont", (void*)GetThemeFontFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId, iPropId, pFont);
}

HRESULT CVisualStylesXP::GetThemeRect(HTHEME hTheme, int iPartId, 
									  int iStateId, int iPropId,  RECT *pRect)
{
	static PFNGETTHEMERECT pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMERECT)GetProc("GetThemeRect", (void*)GetThemeRectFail);
	return (*pfn)(hTheme, iPartId, iStateId, iPropId, pRect);
}

HRESULT CVisualStylesXP::GetThemeMargins(HTHEME hTheme,  HDC hdc, int iPartId, 
										 int iStateId, int iPropId,  RECT *prc,  MARGINS *pMargins)
{
	static PFNGETTHEMEMARGINS pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEMARGINS)GetProc("GetThemeMargins", (void*)GetThemeMarginsFail);
	return (*pfn)(hTheme, hdc, iPartId, iStateId, iPropId, prc, pMargins);
}

HRESULT CVisualStylesXP::GetThemeIntList(HTHEME hTheme, int iPartId, 
										 int iStateId, int iPropId,  INTLIST *pIntList)
{
	static PFNGETTHEMEINTLIST pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEINTLIST)GetProc("GetThemeIntList", (void*)GetThemeIntListFail);
	return (*pfn)(hTheme, iPartId, iStateId, iPropId, pIntList);
}

HRESULT CVisualStylesXP::GetThemePropertyOrigin(HTHEME hTheme, int iPartId, 
												int iStateId, int iPropId,  enum PROPERTYORIGIN *pOrigin)
{
	static PFNGETTHEMEPROPERTYORIGIN pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEPROPERTYORIGIN)GetProc("GetThemePropertyOrigin", (void*)GetThemePropertyOriginFail);
	return (*pfn)(hTheme, iPartId, iStateId, iPropId, pOrigin);
}

HRESULT CVisualStylesXP::SetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName, 
										LPCWSTR pszSubIdList)
{
	static PFNSETWINDOWTHEME pfn = nullptr;
	if( !pfn )
		pfn = (PFNSETWINDOWTHEME)GetProc("SetWindowTheme", (void*)SetWindowThemeFail);
	return (*pfn)(hwnd, pszSubAppName, pszSubIdList);
}

HRESULT CVisualStylesXP::GetThemeFilename(HTHEME hTheme, int iPartId, 
										  int iStateId, int iPropId,  LPWSTR pszThemeFileName, int cchMaxBuffChars)
{
	static PFNGETTHEMEFILENAME pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEFILENAME)GetProc("GetThemeFilename", (void*)GetThemeFilenameFail);
	return (*pfn)(hTheme, iPartId, iStateId, iPropId,  pszThemeFileName, cchMaxBuffChars);
}

COLORREF CVisualStylesXP::GetThemeSysColor(HTHEME hTheme, int iColorId)
{
	static PFNGETTHEMESYSCOLOR pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMESYSCOLOR)GetProc("GetThemeSysColor", (void*)GetThemeSysColorFail);
	return (*pfn)(hTheme, iColorId);
}

HBRUSH CVisualStylesXP::GetThemeSysColorBrush(HTHEME hTheme, int iColorId)
{
	static PFNGETTHEMESYSCOLORBRUSH pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMESYSCOLORBRUSH)GetProc("GetThemeSysColorBrush", (void*)GetThemeSysColorBrushFail);
	return (*pfn)(hTheme, iColorId);
}

BOOL CVisualStylesXP::GetThemeSysBool(HTHEME hTheme, int iBoolId)
{
	static PFNGETTHEMESYSBOOL pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMESYSBOOL)GetProc("GetThemeSysBool", (void*)GetThemeSysBoolFail);
	return (*pfn)(hTheme, iBoolId);
}

int CVisualStylesXP::GetThemeSysSize(HTHEME hTheme, int iSizeId)
{
	static PFNGETTHEMESYSSIZE pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMESYSSIZE)GetProc("GetThemeSysSize", (void*)GetThemeSysSizeFail);
	return (*pfn)(hTheme, iSizeId);
}

HRESULT CVisualStylesXP::GetThemeSysFont(HTHEME hTheme, int iFontId,  LOGFONT *plf)
{
	static PFNGETTHEMESYSFONT pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMESYSFONT)GetProc("GetThemeSysFont", (void*)GetThemeSysFontFail);
	return (*pfn)(hTheme, iFontId, plf);
}

HRESULT CVisualStylesXP::GetThemeSysString(HTHEME hTheme, int iStringId, 
										   LPWSTR pszStringBuff, int cchMaxStringChars)
{
	static PFNGETTHEMESYSSTRING pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMESYSSTRING)GetProc("GetThemeSysString", (void*)GetThemeSysStringFail);
	return (*pfn)(hTheme, iStringId, pszStringBuff, cchMaxStringChars);
}

HRESULT CVisualStylesXP::GetThemeSysInt(HTHEME hTheme, int iIntId, int *piValue)
{
	static PFNGETTHEMESYSINT pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMESYSINT)GetProc("GetThemeSysInt", (void*)GetThemeSysIntFail);
	return (*pfn)(hTheme, iIntId, piValue);
}

BOOL CVisualStylesXP::IsThemeActive()
{
	static PFNISTHEMEACTIVE pfn = nullptr;
	if( !pfn )
		pfn = (PFNISTHEMEACTIVE)GetProc("IsThemeActive", (void*)IsThemeActiveFail);
	return (*pfn)();
}

BOOL CVisualStylesXP::IsAppThemed()
{
	static PFNISAPPTHEMED pfnIsAppThemed = nullptr;
	if( !pfnIsAppThemed )
		pfnIsAppThemed = (PFNISAPPTHEMED)GetProc("IsAppThemed", (void*)IsAppThemedFail);
	return (*pfnIsAppThemed)();
}

HTHEME CVisualStylesXP::GetWindowTheme(HWND hwnd)
{
	static PFNGETWINDOWTHEME pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETWINDOWTHEME)GetProc("GetWindowTheme", (void*)GetWindowThemeFail);
	return (*pfn)(hwnd);
}

HRESULT CVisualStylesXP::EnableThemeDialogTexture(HWND hwnd, DWORD dwFlags)
{
	static PFNENABLETHEMEDIALOGTEXTURE pfn = nullptr;
	if( !pfn )
		pfn = (PFNENABLETHEMEDIALOGTEXTURE)GetProc("EnableThemeDialogTexture", (void*)EnableThemeDialogTextureFail);
	return (*pfn)(hwnd, dwFlags);
}

BOOL CVisualStylesXP::IsThemeDialogTextureEnabled(HWND hwnd)
{
	static PFNISTHEMEDIALOGTEXTUREENABLED pfn = nullptr;
	if( !pfn )
		pfn = (PFNISTHEMEDIALOGTEXTUREENABLED)GetProc("IsThemeDialogTextureEnabled", (void*)IsThemeDialogTextureEnabledFail);
	return (*pfn)(hwnd);
}

DWORD CVisualStylesXP::GetThemeAppProperties()
{
	static PFNGETTHEMEAPPPROPERTIES pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEAPPPROPERTIES)GetProc("GetThemeAppProperties", (void*)GetThemeAppPropertiesFail);
	return (*pfn)();
}

void CVisualStylesXP::SetThemeAppProperties(DWORD dwFlags)
{
	static PFNSETTHEMEAPPPROPERTIES pfn = nullptr;
	if( !pfn )
		pfn = (PFNSETTHEMEAPPPROPERTIES)GetProc("SetThemeAppProperties", (void*)SetThemeAppPropertiesFail);
	(*pfn)(dwFlags);
}

HRESULT CVisualStylesXP::GetCurrentThemeName(
	LPWSTR pszThemeFileName, int cchMaxNameChars, 
	LPWSTR pszColorBuff, int cchMaxColorChars,
	LPWSTR pszSizeBuff, int cchMaxSizeChars)
{
	static PFNGETCURRENTTHEMENAME pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETCURRENTTHEMENAME)GetProc("GetCurrentThemeName", (void*)GetCurrentThemeNameFail);
	return (*pfn)(pszThemeFileName, cchMaxNameChars, pszColorBuff, cchMaxColorChars, pszSizeBuff, cchMaxSizeChars);
}

HRESULT CVisualStylesXP::GetThemeDocumentationProperty(LPCWSTR pszThemeName,
													   LPCWSTR pszPropertyName,  LPWSTR pszValueBuff, int cchMaxValChars)
{
	static PFNGETTHEMEDOCUMENTATIONPROPERTY pfn = nullptr;
	if( !pfn )
		pfn = (PFNGETTHEMEDOCUMENTATIONPROPERTY)GetProc("GetThemeDocumentationProperty", (void*)GetThemeDocumentationPropertyFail);
	return (*pfn)(pszThemeName, pszPropertyName, pszValueBuff, cchMaxValChars);
}


HRESULT CVisualStylesXP::DrawThemeParentBackground(HWND hwnd, HDC hdc,  RECT* prc)
{
	static PFNDRAWTHEMEPARENTBACKGROUND pfn = nullptr;
	if( !pfn )
		pfn = (PFNDRAWTHEMEPARENTBACKGROUND)GetProc("DrawThemeParentBackground", (void*)DrawThemeParentBackgroundFail);
	return (*pfn)(hwnd, hdc, prc);
}

HRESULT CVisualStylesXP::EnableTheming(BOOL fEnable)
{
	static PFNENABLETHEMING pfn = nullptr;
	if( !pfn )
		pfn = (PFNENABLETHEMING)GetProc("EnableTheming", (void*)EnableThemingFail);
	return (*pfn)(fEnable);
}


/////////////////////////////////////////////////////////////////////////////
// Added by Mathias Tunared 2002-07-27
//
BOOL CVisualStylesXP::UseVisualStyles()
{
	static BOOL bUse = -1;
	if( bUse != -1 )
		return bUse;

	HINSTANCE hinstDll;
	DLLVERSIONINFO dvi;
	ZeroMemory(&dvi, sizeof(dvi));

	hinstDll = LoadLibrary(_T("comctl32.dll"));
	if(hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC) GetProcAddress(hinstDll, "DllGetVersion");
		if(pDllGetVersion)
		{
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			(*pDllGetVersion)(&dvi);
		}
		FreeLibrary(hinstDll);
	}

	if( dvi.dwMajorVersion == 6 && dvi.dwMinorVersion == 0 )
	{
		//+++
		WCHAR szName[MAX_PATH];
		WCHAR szColor[MAX_PATH];

		if (!GetCurrentThemeName(szName, MAX_PATH-1, szColor, MAX_PATH-1, NULL, 0))
		{
			//TRACE(_T("theme=<%ls>  color=<%ls>\n"), szName, szColor);
			if ((_wcsicmp(szName, L"normalcolor") == 0) ||
				(_wcsicmp(szName, L"homestead") == 0) ||
				(_wcsicmp(szName, L"metallic") == 0))
			{
				bUse = true;
				return IsAppThemed() && m_hThemeDll; // No need to test HTHEME, but...
			}
		}
	}

	bUse = false;
	return false;
}

