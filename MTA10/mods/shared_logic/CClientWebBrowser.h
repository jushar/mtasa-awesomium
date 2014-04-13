/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/CClientWebBrowser.h
*  PURPOSE:     Web browser (Awesomium) class
*
*****************************************************************************/

#ifndef __CCLIENTWEBBROWSER_H
#define __CCLIENTWEBBROWSER_H

class CClientWebBrowser : public CClientTexture
{
    DECLARE_CLASS(CClientWebBrowser, CClientTexture)
public:
    CClientWebBrowser(CClientManager* pManager, ElementID ID, CWebBrowserItem* pWebBrowserItem);
    ~CClientWebBrowser();

    inline CWebBrowserItem*      GetWebBrowserItem(void)                       { return (CWebBrowserItem*)m_pRenderItem; }

    bool IsLoading();
    bool LoadURL(const SString& strURL);
    void GetTitle(SString& outPageTitle);
    void GetURL(SString& outURL);

    void InjectMouseMove(int iPosX, int iPosY);
    void InjectMouseDown(int mouseButton);
    void InjectMouseUp(int mouseButton);
    void InjectKeyboardEvent(const SString& strKey, bool bKeyDown = true, bool bCharacter = false);
};

#endif
