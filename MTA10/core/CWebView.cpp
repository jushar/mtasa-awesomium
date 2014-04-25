/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        core/CWebView.cpp
*  PURPOSE:     Web view class
*
*****************************************************************************/

#include "StdInc.h"
#include "CWebView.h"

CWebView::CWebView(IDirect3DSurface9* pD3DSurface)
{
    m_pD3DSurface = pD3DSurface;

    // Create a new offscreen webview
    CefBrowserSettings browserSettings;
    CefWindowInfo windowInfo;
    windowInfo.SetAsOffScreen(nullptr); // Enable offscreen rendering

    // Create the "proper" browser
    m_pBrowserHost = CefBrowserHost::CreateBrowserSync(windowInfo, this, "", browserSettings, NULL);
}

bool CWebView::LoadURL(const SString& strURL)
{
    // Todo: Might be better to move the url checking to CClientWebBrowser
    /*if (!IsURLAllowed(webURL.host()))
        return false; */

    m_pBrowserHost->GetMainFrame()->LoadURL(strURL);
    return true;
}

bool CWebView::IsLoading()
{
    return m_pBrowserHost->IsLoading();
}


////////////////////////////////////////////////////////////////////
//                                                                //
//      Implementation: CefClient::GetLoadHandler               //
// http://magpcss.org/ceforum/apidocs3/projects/(default)/CefClient.html#GetLoadHandler() //
//                                                                //
////////////////////////////////////////////////////////////////////
CefRefPtr<CefLoadHandler> CWebView::GetLoadHandler()
{
    // Well, we're a CefLoadHandler
    return this;
}

////////////////////////////////////////////////////////////////////
//                                                                //
//      Implementation: CefClient::GetRenderHandler               //
// http://magpcss.org/ceforum/apidocs3/projects/(default)/CefClient.html#GetRenderHandler() //
//                                                                //
////////////////////////////////////////////////////////////////////
CefRefPtr<CefRenderHandler> CWebView::GetRenderHandler()
{
    // Well, we're a CefRenderHandler
    return this;
}

////////////////////////////////////////////////////////////////////
//                                                                //
//      Implementation: CefLoadHandler::OnLoadStart               //
// http://magpcss.org/ceforum/apidocs3/projects/(default)/CefLoadHandler.html#OnLoadStart(CefRefPtr%3CCefBrowser%3E,CefRefPtr%3CCefFrame%3E) //
//                                                                //
////////////////////////////////////////////////////////////////////
void CWebView::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
    
}

////////////////////////////////////////////////////////////////////
//                                                                //
//      Implementation: CefLoadHandler::OnLoadError               //
// http://magpcss.org/ceforum/apidocs3/projects/(default)/CefLoadHandler.html#OnLoadError(CefRefPtr%3CCefBrowser%3E,CefRefPtr%3CCefFrame%3E,ErrorCode,constCefString&,constCefString&) //
//                                                                //
////////////////////////////////////////////////////////////////////
void CWebView::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{

}

////////////////////////////////////////////////////////////////////
//                                                                //
//      Implementation: CefLoadHandler::OnLoadError               //
// http://magpcss.org/ceforum/apidocs3/projects/(default)/CefLoadHandler.html#OnLoadError(CefRefPtr%3CCefBrowser%3E,CefRefPtr%3CCefFrame%3E,ErrorCode,constCefString&,constCefString&) //
//                                                                //
////////////////////////////////////////////////////////////////////
void CWebView::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
    // Todo: Add a Lua event
}

////////////////////////////////////////////////////////////////////
//                                                                //
//      Implementation: CefRenderHandler::GetViewRect             //
// http://magpcss.org/ceforum/apidocs3/projects/(default)/CefRenderHandler.html#GetViewRect(CefRefPtr%3CCefBrowser%3E,CefRect&) //
//                                                                //
////////////////////////////////////////////////////////////////////
bool CWebView::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
    D3DSURFACE_DESC SurfaceDesc;
    m_pD3DSurface->GetDesc(&SurfaceDesc);

    rect = CefRect(0, 0, SurfaceDesc.Width, SurfaceDesc.Height);
    return true;
}

////////////////////////////////////////////////////////////////////
//                                                                //
//      Implementation: CefRenderHandler::OnPaint                 //
// http://magpcss.org/ceforum/apidocs3/projects/(default)/CefRenderHandler.html#OnPaint(CefRefPtr%3CCefBrowser%3E,PaintElementType,constRectList&,constvoid*,int,int) //
//                                                                //
////////////////////////////////////////////////////////////////////
void CWebView::OnPaint(CefRefPtr<CefBrowser> browser, CefRenderHandler::PaintElementType paintType, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height)
{
    if (!m_pD3DSurface)
        return;

    D3DLOCKED_RECT LockedRect;
    D3DSURFACE_DESC SurfaceDesc;
    
    // First, lock the surface and request some information
    m_pD3DSurface->GetDesc(&SurfaceDesc);
    m_pD3DSurface->LockRect(&LockedRect, NULL, 0);

    // Paint buffer to surface (amount of pixels = width * height * 32bit color (ARGB))
    memcpy(LockedRect.pBits, buffer, width * height * 4);
    
    // Finally, unlock the surface
    m_pD3DSurface->UnlockRect();
}
