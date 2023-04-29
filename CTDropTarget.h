#pragma once
class CTDropTarget : public COleDropTarget
{
public:
    CHAR curdir[260];
    CHAR info[260]={0};
    CString url;
    CString titre;
    CTDropTarget::CTDropTarget() {
        HRESULT oleinit=OleInitialize(0);
        m_CF_URLA = RegisterClipboardFormat(_TEXT("UniformResourceLocator"));
        m_CF_URLW = RegisterClipboardFormat(_TEXT("UniformResourceLocatorW"));
    }
    virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
        DWORD dwKeyState, CPoint point)
    {
        STGMEDIUM rSM;
        BOOL fRet = pDataObject->GetData(m_CF_URLA, &rSM);
        if (fRet) {
            return(DROPEFFECT_LINK); // "Drop OK"
        }
        return(DROPEFFECT_NONE);
    };
    virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) {
        STGMEDIUM rSM;
        BOOL fRet = pDataObject->GetData(m_CF_URLA, &rSM);

        char* p = (char*)GlobalLock(rSM.hGlobal);
        CString sw(p);    
        GlobalUnlock(rSM.hGlobal);
        CEdit* pEd = (CEdit*)pWnd;
        pEd->SetSel(0, -1);
        pEd->ReplaceSel(sw);
        url = sw;
        titre = url;
        SYSTEMTIME st;
        GetLocalTime(&st);
        wsprintf(info, "%s\\%.2d%.2d%.2d.rul", curdir, st.wHour, st.wMinute, st.wSecond);
        WritePrivateProfileString("ID", "URL", url, info);
        WritePrivateProfileString("ID", "LABEL", titre, info);
        return(1); // success
    };
    UINT m_CF_URLA;
    UINT m_CF_URLW;
};


