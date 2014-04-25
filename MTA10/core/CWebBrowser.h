/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        core/CWebBrowser.h
*  PURPOSE:     Webbrowser class
*
*****************************************************************************/

#ifndef __CWEBBROWSER_H
#define __CWEBBROWSER_H

#undef GetNextSibling // Hack that fixes name conflicts with windows.h
#include <core/CWebBrowserInterface.h>
#include <cef/include/cef_app.h>
#include <cef/include/cef_browser.h>
#include <cef/include/cef_client.h>
#include <cef/include/cef_render_handler.h>
#include <cef/include/cef_command_line.h>
#include <cef/include/cef_task.h>
#define GetNextSibling(hwnd) GetWindow(hwnd, GW_HWNDNEXT) // Re-define the conflicting macro

class CWebBrowserItem;
class CWebsiteRequests;

class CWebBrowser : public CWebBrowserInterface /*, public CefApp*/
{
public:
    CWebBrowser();
    ~CWebBrowser();
    bool Initialise();

    CWebViewInterface* CreateWebView(unsigned int uiWidth, unsigned int uiHeight, IDirect3DSurface9* pD3DSurface);
    
    void GetScrollPosition(CWebBrowserItem* pWebBrowserItem, int& iScrollX, int& iScrollY);
    void SetScrollPosition(CWebBrowserItem* pWebBrowserItem, int iScrollX, int iScrollY);

    void InjectMouseMove(CWebBrowserItem* pWebBrowserItem, int iPosX, int iPosY);
    void InjectMouseDown(CWebBrowserItem* pWebBrowserItem, int mouseButton);
    void InjectMouseUp(CWebBrowserItem* pWebBrowserItem, int mouseButton);
    void InjectKeyboardEvent(CWebBrowserItem* pWebBrowserItem, const SString& strKey, bool bKeyDown = true, bool bCharacter = false);

    bool IsURLAllowed(const SString& strURL);
    void ClearWhitelist();
    void AddAllowedPage(const SString& strURL);
    void RequestPages(const std::vector<SString>& pages);
    void AllowPendingPages();
    void DenyPendingPages();

private:
    CWebsiteRequests* m_pRequestsGUI;

    std::vector<SString> m_Whitelist;
    std::vector<SString> m_PendingRequests;

    //IMPLEMENT_REFCOUNTING(CWebBrowser);
};


/*class CWebBrowserResourceInterceptor : public Awesomium::ResourceInterceptor
{
    virtual bool OnFilterNavigation(int origin_process_id, int origin_routing_id, const Awesomium::WebString& method, const Awesomium::WebURL& url, bool is_main_frame);
    virtual Awesomium::ResourceResponse* OnRequest(Awesomium::ResourceRequest* pRequest);
};*/


#endif
