/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/CClientWebBrowser.cpp
*  PURPOSE:     Web browser (Awesomium) class
*
*****************************************************************************/

#include "StdInc.h"
#include "CClientWebBrowser.h"

CClientWebBrowser::CClientWebBrowser(CClientManager* pManager, ElementID ID, CWebBrowserItem* pWebBrowserItem) : ClassInit(this), CClientTexture(pManager, ID, pWebBrowserItem)
{
}

CClientWebBrowser::~CClientWebBrowser()
{
}

bool CClientWebBrowser::IsLoading()
{
    return g_pCore->GetAwesomium()->IsLoading(GetWebBrowserItem());
}

bool CClientWebBrowser::LoadURL(const SString& strURL)
{
    return g_pCore->GetAwesomium()->LoadURL(GetWebBrowserItem(), strURL);
}

void CClientWebBrowser::GetTitle(SString& outPageTitle)
{
    g_pCore->GetAwesomium()->GetPageTitle(GetWebBrowserItem(), outPageTitle);
}

void CClientWebBrowser::GetURL(SString& outURL)
{
    g_pCore->GetAwesomium()->GetPageURL(GetWebBrowserItem(), outURL);
}

void CClientWebBrowser::InjectMouseMove(int iPosX, int iPosY)
{
    g_pCore->GetAwesomium()->InjectMouseMove(GetWebBrowserItem(), iPosX, iPosY);
}

void CClientWebBrowser::InjectMouseDown(int mouseButton)
{
    g_pCore->GetAwesomium()->InjectMouseDown(GetWebBrowserItem(), mouseButton);
}

void CClientWebBrowser::InjectMouseUp(int mouseButton)
{
    g_pCore->GetAwesomium()->InjectMouseUp(GetWebBrowserItem(), mouseButton);
}

void CClientWebBrowser::InjectKeyboardEvent(const SString& strKey, bool bKeyDown, bool bCharacter)
{
    g_pCore->GetAwesomium()->InjectKeyboardEvent(GetWebBrowserItem(), strKey, bKeyDown, bCharacter);
}
