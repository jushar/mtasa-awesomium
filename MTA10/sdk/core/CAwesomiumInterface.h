/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        sdk/core/CAwesomiumInterface.h
*  PURPOSE:     Awesomium interface class
*
*****************************************************************************/

#ifndef __CAWESOMIUMINTERFACE_H
#define __CAWESOMIUMINTERFACE_H

namespace Awesomium
{
    class WebView;
    class WebString;
}
class CWebBrowserItem;

class CAwesomiumInterface
{
public:
    virtual Awesomium::WebView* CreateWebView(unsigned int uiWidth, unsigned int uiHeight) = 0;
    virtual void Update() = 0;
    
    virtual bool IsLoading(CWebBrowserItem* pWebBrowserItem) = 0;
    virtual void GetPageTitle(CWebBrowserItem* pWebBrowserItem, SString& outPageTitle) = 0;
    virtual void GetPageURL(CWebBrowserItem* pWebBrowserItem, SString& outURL) = 0;

    virtual void InjectMouseMove(CWebBrowserItem* pWebBrowserItem, int iPosX, int iPosY) = 0;
    virtual void InjectMouseDown(CWebBrowserItem* pWebBrowserItem, int mouseButton) = 0;
    virtual void InjectMouseUp(CWebBrowserItem* pWebBrowserItem, int mouseButton) = 0;
    virtual void InjectKeyboardEvent(CWebBrowserItem* pWebBrowserItem, const SString& strKey, bool bKeyDown = true, bool bCharacter = false) = 0;

    virtual bool LoadURL(CWebBrowserItem* pWebBrowserItem, const SString& strURL) = 0;
    virtual bool IsURLAllowed(const Awesomium::WebString& strURL) = 0;
    virtual void ClearWhitelist() = 0;
    virtual void AddAllowedPage(const SString& strURL) = 0;
    virtual void RequestPages(const std::vector<SString>& pages) = 0;
    virtual void AllowPendingPages() = 0;
    virtual void DenyPendingPages() = 0;
};

#endif
