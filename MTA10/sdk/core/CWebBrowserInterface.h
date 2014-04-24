/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        sdk/core/CWebBrowserInterface.h
*  PURPOSE:     Webbrowser interface class
*
*****************************************************************************/

#ifndef __CWEBBROWSERINTERFACE_H
#define __CWEBBROWSERINTERFACE_H

class CWebBrowserItem;
class CWebViewInterface;

class CWebBrowserInterface
{
public:
    virtual CWebViewInterface* CreateWebView(unsigned int uiWidth, unsigned int uiHeight, IDirect3DSurface9* pD3DSurface) = 0;
    
    virtual void GetScrollPosition(CWebBrowserItem* pWebBrowserItem, int& iScrollX, int& iScrollY) = 0;
    virtual void SetScrollPosition(CWebBrowserItem* pWebBrowserItem, int iScrollX, int iScrollY) = 0;

    virtual void InjectMouseMove(CWebBrowserItem* pWebBrowserItem, int iPosX, int iPosY) = 0;
    virtual void InjectMouseDown(CWebBrowserItem* pWebBrowserItem, int mouseButton) = 0;
    virtual void InjectMouseUp(CWebBrowserItem* pWebBrowserItem, int mouseButton) = 0;
    virtual void InjectKeyboardEvent(CWebBrowserItem* pWebBrowserItem, const SString& strKey, bool bKeyDown = true, bool bCharacter = false) = 0;

    virtual bool IsURLAllowed(const SString& strURL) = 0;
    virtual void ClearWhitelist() = 0;
    virtual void AddAllowedPage(const SString& strURL) = 0;
    virtual void RequestPages(const std::vector<SString>& pages) = 0;
    virtual void AllowPendingPages() = 0;
    virtual void DenyPendingPages() = 0;
};

#endif
