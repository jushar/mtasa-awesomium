/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        core/CWebBrowser.cpp
*  PURPOSE:     Webbrowser class
*
*****************************************************************************/

#include "StdInc.h"
#include "CWebBrowser.h"
#include "CWebView.h"
#include "CWebsiteRequests.h"

// Set to 0 to disable sandbox support.
#define CEF_ENABLE_SANDBOX 0 // Todo: Make use of the sandbox
#if CEF_ENABLE_SANDBOX
    #include <cef/include/cef_sandbox_win.h>
    #pragma comment(lib, "cef_sandbox.lib")
#endif

CWebBrowser::CWebBrowser()
{
    m_pRequestsGUI = NULL;

    Initialise();
}

CWebBrowser::~CWebBrowser()
{
    // Shutdown CEF
    CefShutdown();

    if (m_pRequestsGUI)
        delete m_pRequestsGUI;
}

bool CWebBrowser::Initialise()
{
    // Start rendering process
    CefMainArgs mainArgs;
    int exitCode = CefExecuteProcess(mainArgs, NULL, NULL);

    void* sandboxInfo = NULL;
#if CEF_ENABLE_SANDBOX
    CefScopedSandboxInfo scopedSandbox;
    sandboxInfo = scopedSandbox.sandbox_info();
#endif

    // Initialize CEF
    if (exitCode >= 0)
        return false;

    CefSettings settings;
#if !CEF_ENABLE_SANDBOX
     settings.no_sandbox = true;
#endif

     // Set as single-processed (The way CEF implements Chromium's multi-process architecture is not suitable for our GTA DLL injection system)
     settings.single_process = true;
     //settings.multi_threaded_message_loop = true;
     
     return CefInitialize(mainArgs, settings, NULL, sandboxInfo);
}

CWebViewInterface* CWebBrowser::CreateWebView(unsigned int uiWidth, unsigned int uiHeight, IDirect3DSurface9* pD3DSurface)
{
    // Create our webview implementation
    CWebView* pWebView = new CWebView(pD3DSurface);
    return pWebView;
}

void CWebBrowser::Update()
{
    CefDoMessageLoopWork();
}

void CWebBrowser::GetScrollPosition(CWebBrowserItem* pWebBrowserItem, int& iScrollX, int& iScrollY)
{

}

void CWebBrowser::SetScrollPosition(CWebBrowserItem* pWebBrowserItem, int iScrollX, int iScrollY)
{
    
}

void CWebBrowser::InjectMouseMove(CWebBrowserItem* pWebBrowserItem, int iPosX, int iPosY)
{
    //pWebBrowserItem->m_pWebView->InjectMouseMove(iPosX, iPosY);
}

void CWebBrowser::InjectMouseDown(CWebBrowserItem* pWebBrowserItem, int mouseButton)
{
    //pWebBrowserItem->m_pWebView->InjectMouseDown(static_cast<Awesomium::MouseButton>(mouseButton));
}

void CWebBrowser::InjectMouseUp(CWebBrowserItem* pWebBrowserItem, int mouseButton)
{
    //pWebBrowserItem->m_pWebView->InjectMouseUp(static_cast<Awesomium::MouseButton>(mouseButton));
}

void CWebBrowser::InjectKeyboardEvent(CWebBrowserItem* pWebBrowserItem, const SString& strKey, bool bKeyDown, bool bCharacter)
{
    // Hack fix
    SString key = strKey;
    if (key == " ")
        key = "space";

    /*Awesomium::WebKeyboardEvent keyboardEvent;
    keyboardEvent.is_system_key = false;
    keyboardEvent.modifiers = Awesomium::WebKeyboardEvent::kModIsAutorepeat;

    if (!bCharacter)
    {
        // Get BindableKey structure (to be able to process MTA's key names correctly)
        const SBindableKey* pBindableKey = g_pCore->GetKeyBinds()->GetBindableFromKey(key.c_str());
        if (!pBindableKey)
            return;

        keyboardEvent.virtual_key_code = pBindableKey->ulCode;
        //keyboardEvent.native_key_code = pBindableKey->ulCode;
        keyboardEvent.type = bKeyDown ? Awesomium::WebKeyboardEvent::kTypeKeyDown : Awesomium::WebKeyboardEvent::kTypeKeyUp;
    }
    else
    {
        std::wstring strCharacter (MbUTF8ToUTF16(strKey));
        if (strCharacter.length() > 4)
            return;
        memcpy(keyboardEvent.text, strCharacter.c_str(), strCharacter.length() * 2);
        keyboardEvent.type = Awesomium::WebKeyboardEvent::kTypeChar;
    }
    
    pWebBrowserItem->m_pWebView->InjectKeyboardEvent(keyboardEvent);*/
}

bool CWebBrowser::IsURLAllowed(const SString& strURL)
{
    // Todo: Implement wildcards
    for (std::vector<SString>::iterator iter = m_Whitelist.begin(); iter != m_Whitelist.end(); ++iter)
    {
        if (*iter == strURL)
            return true;
    }
    return false;
}

void CWebBrowser::ClearWhitelist()
{
    m_Whitelist.clear();
    m_PendingRequests.clear();
}

void CWebBrowser::AddAllowedPage(const SString& strURL)
{
    m_Whitelist.push_back(strURL);
}

void CWebBrowser::RequestPages(const std::vector<SString>& pages)
{
    // Add to pending pages queue
    bool bNewItem = false;
    for (std::vector<SString>::const_iterator iter = pages.begin(); iter != pages.end(); ++iter)
    {
        if (IsURLAllowed(*iter))
            continue;

        m_PendingRequests.push_back(*iter);
        bNewItem = true;
    }

    if (bNewItem)
    {
        // Open CEGUI dialog
        if (!m_pRequestsGUI)
            m_pRequestsGUI = new CWebsiteRequests;

        // Set pending requests memo content and show the window
        m_pRequestsGUI->SetPendingRequests(m_PendingRequests);
        m_pRequestsGUI->Show();
    }
}

void CWebBrowser::AllowPendingPages()
{
    for (std::vector<SString>::iterator iter = m_PendingRequests.begin(); iter != m_PendingRequests.end(); ++iter)
    {
        m_Whitelist.push_back(*iter);
    }
    m_PendingRequests.clear();

    // Trigger an event now
    CModManager::GetSingleton().GetCurrentMod()->WebsiteRequestResultHandler(true);
}

void CWebBrowser::DenyPendingPages()
{
    m_PendingRequests.clear();
    
    // Trigger an event now
    CModManager::GetSingleton().GetCurrentMod()->WebsiteRequestResultHandler(false);
}

////////////////////////////////////////////////////////////////////
//                                                                //
//          CWebBrowserResourceInterceptor section                 //
//  This methods get called when the player requests resources    //
// http://www.awesomium.com/docs/1_7_2/cpp_api/class_awesomium_1_1_resource_interceptor.html //
//                                                                //
////////////////////////////////////////////////////////////////////
/*bool CWebBrowserResourceInterceptor::OnFilterNavigation(int origin_process_id, int origin_routing_id, const Awesomium::WebString& method, const Awesomium::WebURL& url, bool is_main_frame)
{
    if (!g_pCore->GetWebBrowser()->IsURLAllowed(url.host()))
        // Block the action
        return true;

    // Don't do anything
    return false;
}

Awesomium::ResourceResponse* CWebBrowserResourceInterceptor::OnRequest(Awesomium::ResourceRequest* pRequest)
{
    if (!g_pCore->GetWebBrowser()->IsURLAllowed(pRequest->url().host()))
        // Block the action
        pRequest->Cancel();

    // We don't want to modify anything
    return NULL;
}*/

