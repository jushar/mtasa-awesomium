/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        core/CAwesomium.h
*  PURPOSE:     Awesomium class
*
*****************************************************************************/

#ifndef __CAWESOMIUM_H
#define __CAWESOMIUM_H

#include <core/CAwesomiumInterface.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/WebView.h>
#include <Awesomium/STLHelpers.h>
class CWebBrowserItem;
class CWebsiteRequests;

using Awesomium::WebString;

class CAwesomium : public CAwesomiumInterface
{
public:
    CAwesomium();
    ~CAwesomium();

    Awesomium::WebView* CreateWebView(unsigned int uiWidth, unsigned int uiHeight);
    void Update();
    
    bool IsLoading(CWebBrowserItem* pWebBrowserItem);
    void GetPageTitle(CWebBrowserItem* pWebBrowserItem, SString& outPageTitle);
    void GetPageURL(CWebBrowserItem* pWebBrowserItem, SString& ourURL);

    void InjectMouseMove(CWebBrowserItem* pWebBrowserItem, int iPosX, int iPosY);
    void InjectMouseDown(CWebBrowserItem* pWebBrowserItem, int mouseButton);
    void InjectMouseUp(CWebBrowserItem* pWebBrowserItem, int mouseButton);
    void InjectKeyboardEvent(CWebBrowserItem* pWebBrowserItem, const SString& strKey, bool bKeyDown = true, bool bCharacter = false);

    bool LoadURL(CWebBrowserItem* pWebBrowserItem, const SString& strURL);
    bool IsURLAllowed(const WebString& strURL);
    void ClearWhitelist();
    void AddAllowedPage(const SString& strURL);
    void RequestPages(const std::vector<SString>& pages);
    void AllowPendingPages();
    void DenyPendingPages();

private:
    Awesomium::WebCore* m_pWebCore;
    CWebsiteRequests* m_pRequestsGUI;

    std::vector<Awesomium::WebString> m_Whitelist;
    std::vector<SString> m_PendingRequests;

    void WebStringToSString(const WebString& webString, SString& strString);
};

class CAwesomiumResourceInterceptor : public Awesomium::ResourceInterceptor
{
    virtual bool OnFilterNavigation(int origin_process_id, int origin_routing_id, const Awesomium::WebString& method, const Awesomium::WebURL& url, bool is_main_frame);
    virtual Awesomium::ResourceResponse* OnRequest(Awesomium::ResourceRequest* pRequest);
};

#endif
