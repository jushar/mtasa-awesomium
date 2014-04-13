/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        core/CAwesomium.cpp
*  PURPOSE:     Awesomium class
*
*****************************************************************************/

#include "StdInc.h"
#include "CAwesomium.h"
#include "CWebsiteRequests.h"

CAwesomium::CAwesomium()
{
    m_pWebCore = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
    m_pRequestsGUI = NULL;
}

CAwesomium::~CAwesomium()
{
    m_pWebCore->Shutdown();

    if (m_pRequestsGUI)
        delete m_pRequestsGUI;
}

Awesomium::WebView* CAwesomium::CreateWebView(unsigned int uiWidth, unsigned int uiHeight)
{
    // Create a new offscreen webview
    Awesomium::WebView* pWebView = m_pWebCore->CreateWebView(uiWidth, uiHeight, 0, Awesomium::WebViewType::kWebViewType_Offscreen);
    return pWebView;
}

void CAwesomium::Update()
{
    m_pWebCore->Update();
}

bool CAwesomium::IsLoading(CWebBrowserItem* pWebBrowserItem)
{
    return pWebBrowserItem->m_pWebView->IsLoading();
}

void CAwesomium::GetPageTitle(CWebBrowserItem* pWebBrowserItem, SString& outPageTitle)
{
    WebStringToSString(pWebBrowserItem->m_pWebView->title(), outPageTitle);
}

void CAwesomium::GetPageURL(CWebBrowserItem* pWebBrowserItem, SString& outURL)
{
    WebStringToSString(pWebBrowserItem->m_pWebView->url().spec(), outURL);
}

bool CAwesomium::LoadURL(CWebBrowserItem* pWebBrowserItem, const SString& strURL)
{
    Awesomium::WebURL webURL(Awesomium::WSLit(strURL.c_str()));

    // Stop here if requested URL is not on the whitelist
    if (!IsURLAllowed(webURL.host()))
        return false;

    pWebBrowserItem->m_pWebView->LoadURL(webURL);
    return true;
}

void CAwesomium::InjectMouseMove(CWebBrowserItem* pWebBrowserItem, int iPosX, int iPosY)
{
    pWebBrowserItem->m_pWebView->InjectMouseMove(iPosX, iPosY);
}

void CAwesomium::InjectMouseDown(CWebBrowserItem* pWebBrowserItem, int mouseButton)
{
    pWebBrowserItem->m_pWebView->InjectMouseDown(static_cast<Awesomium::MouseButton>(mouseButton));
}

void CAwesomium::InjectMouseUp(CWebBrowserItem* pWebBrowserItem, int mouseButton)
{
    pWebBrowserItem->m_pWebView->InjectMouseUp(static_cast<Awesomium::MouseButton>(mouseButton));
}

void CAwesomium::InjectKeyboardEvent(CWebBrowserItem* pWebBrowserItem, const SString& strKey, bool bKeyDown, bool bCharacter)
{
    // Hack fix
    SString key = strKey;
    if (key == " ")
        key = "space";

    Awesomium::WebKeyboardEvent keyboardEvent;
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
    
    pWebBrowserItem->m_pWebView->InjectKeyboardEvent(keyboardEvent);

    // A weird behaviour expects a heap c-string???
    //char* buffer = new char[20];
    //Awesomium::GetKeyIdentifierFromVirtualKeyCode(pBindableKey->ulCode, &buffer);
    //strcpy_s(keyboardEvent.key_identifier, 20, buffer);
    //delete[] buffer;
}

bool CAwesomium::IsURLAllowed(const WebString& strURL)
{
    // Todo: Implement wildcards
    for (std::vector<WebString>::iterator iter = m_Whitelist.begin(); iter != m_Whitelist.end(); ++iter)
    {
        if (*iter == strURL)
            return true;
    }
    return false;
}

void CAwesomium::ClearWhitelist()
{
    m_Whitelist.clear();
    m_PendingRequests.clear();
}

void CAwesomium::AddAllowedPage(const SString& strURL)
{
    m_Whitelist.push_back(Awesomium::WSLit(strURL.c_str()));
}

void CAwesomium::RequestPages(const std::vector<SString>& pages)
{
    // Add to pending pages queue
    bool bNewItem = false;
    for (std::vector<SString>::const_iterator iter = pages.begin(); iter != pages.end(); ++iter)
    {
        if (IsURLAllowed(Awesomium::WSLit(iter->c_str())))
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

void CAwesomium::AllowPendingPages()
{
    for (std::vector<SString>::iterator iter = m_PendingRequests.begin(); iter != m_PendingRequests.end(); ++iter)
    {
        m_Whitelist.push_back(Awesomium::WSLit(iter->c_str()));
    }
    m_PendingRequests.clear();

    // Trigger an event now
    CModManager::GetSingleton().GetCurrentMod()->WebsiteRequestResultHandler(true);
}

void CAwesomium::DenyPendingPages()
{
    m_PendingRequests.clear();
    
    // Trigger an event now
    CModManager::GetSingleton().GetCurrentMod()->WebsiteRequestResultHandler(false);
}

void CAwesomium::WebStringToSString(const WebString& webString, SString& strString)
{
    std::stringstream sstream;
    sstream << webString;
    sstream >> strString;
}

////////////////////////////////////////////////////////////////////
//                                                                //
//          CAwesomiumResourceInterceptor section                 //
//  This methods get called when the player requests resources    //
// http://www.awesomium.com/docs/1_7_2/cpp_api/class_awesomium_1_1_resource_interceptor.html //
//                                                                //
////////////////////////////////////////////////////////////////////
bool CAwesomiumResourceInterceptor::OnFilterNavigation(int origin_process_id, int origin_routing_id, const Awesomium::WebString& method, const Awesomium::WebURL& url, bool is_main_frame)
{
    if (!g_pCore->GetAwesomium()->IsURLAllowed(url.host()))
        // Block the action
        return true;

    // Don't do anything
    return false;
}

Awesomium::ResourceResponse* CAwesomiumResourceInterceptor::OnRequest(Awesomium::ResourceRequest* pRequest)
{
    if (!g_pCore->GetAwesomium()->IsURLAllowed(pRequest->url().host()))
        // Block the action
        pRequest->Cancel();

    // We don't want to modify anything
    return NULL;
}
