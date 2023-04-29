#include "pch.h"
#include "framework.h"
#include <exdispid.h>
#include <windows.h>
#include <exdisp.h>

#define BEFORENAVIGATE2       WM_USER
#define DOWNLOADBEGIN         WM_USER+1
#define DOWNLOADCOMPLETE      WM_USER+2
#define NAVIGATECOMPLETE2     WM_USER+3
#define DOCUMENTCOMPLETE      WM_USER+4
#define COMMANDSTATECHANGE    WM_USER+5

class Evenem : public IDispatch
{
private:
	long ref=0;
	HWND fenetre;
	BSTR url=nullptr;
public:
	Evenem(HWND fenet);
	~Evenem();
	HRESULT __stdcall QueryInterface(REFIID iid, void** ppvObject);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall GetTypeInfoCount(unsigned int FAR* pctinfo);
	HRESULT __stdcall GetTypeInfo(unsigned int iTInfo, LCID  lcid, ITypeInfo FAR* FAR* ppTInfo);
	HRESULT __stdcall GetIDsOfNames(REFIID riid, OLECHAR FAR* FAR* rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId);
	HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR* pDispParams, VARIANT FAR* parResult, EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr);
};

IWebBrowser2* pIWeb{};
Evenem* pEvnm;
IConnectionPoint* pConnectionPoint;
DWORD dwCookie = 0;
HINSTANCE hDLL;
BSTR ConvertirChemin(char* Chemin);

Evenem::Evenem(HWND fenet) { 	fenetre = fenet; }
Evenem::~Evenem()	{	SysFreeString(url);	}
HRESULT __stdcall Evenem::QueryInterface(REFIID iid, void** ppvObject)
{
	if (iid == IID_IUnknown || iid == IID_IDispatch || iid == DIID_DWebBrowserEvents2)
	{
		*ppvObject = this;
		AddRef();
		return S_OK;
	}
	else return E_NOINTERFACE;
}
ULONG __stdcall Evenem::AddRef(){	return InterlockedIncrement(&ref);}
ULONG __stdcall Evenem::Release()
{
	int tmp = InterlockedDecrement(&ref);
	if (tmp == 0) delete this;
	return tmp;
}
HRESULT __stdcall Evenem::GetTypeInfoCount(unsigned int FAR* pctinfo) { return E_NOTIMPL; }
HRESULT __stdcall Evenem::GetTypeInfo(unsigned int iTInfo, LCID  lcid, ITypeInfo FAR* FAR* ppTInfo) { return E_NOTIMPL; }
HRESULT __stdcall Evenem::GetIDsOfNames(REFIID riid, OLECHAR FAR* FAR* rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId) { return E_NOTIMPL; }
HRESULT __stdcall Evenem::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR* pDispParams, VARIANT FAR* parResult, EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr)
{
	IUnknown* pIUnk;
	VARIANT* vurl;
	if (!pDispParams) return E_INVALIDARG;
	switch (dispIdMember)
	{
	case DISPID_BEFORENAVIGATE2:
		pIUnk = pDispParams->rgvarg[6].pdispVal;
		SendMessage(fenetre, BEFORENAVIGATE2, (WPARAM)pIUnk, 0);
		break;
	case DISPID_DOWNLOADBEGIN:
		SendMessage(fenetre, DOWNLOADBEGIN, 0, 0);
		break;
	case DISPID_DOWNLOADCOMPLETE:
		SendMessage(fenetre, DOWNLOADCOMPLETE, 0, 0);
		break;
	case DISPID_NAVIGATECOMPLETE2:
		pIUnk = pDispParams->rgvarg[1].pdispVal;
		vurl = pDispParams->rgvarg[0].pvarVal;
		url = vurl->bstrVal;
		SendMessage(fenetre, NAVIGATECOMPLETE2, (WPARAM)pIUnk, (LPARAM)url);
		break;

	case DISPID_DOCUMENTCOMPLETE:
		pIUnk = pDispParams->rgvarg[1].pdispVal;
		SendMessage(fenetre, DOCUMENTCOMPLETE, (WPARAM)pIUnk, 0);
		break;

	case DISPID_COMMANDSTATECHANGE:
		long command;
		command = pDispParams->rgvarg[1].lVal;
		VARIANT_BOOL etat;
		etat = pDispParams->rgvarg[0].boolVal;
		SendMessage(fenetre, COMMANDSTATECHANGE, (WPARAM)command, (LPARAM)etat);
		break;

	case DISPID_NEWWINDOW2:
		pDispParams->rgvarg[0].pvarVal->vt = VT_BOOL;
		pDispParams->rgvarg[0].pvarVal->boolVal = VARIANT_TRUE;
		break;

	default:
		break;
	}
	return S_OK;
}

bool InitExplorateur(HWND hDlg) {
	hDLL = LoadLibrary("atl.dll");
	if (hDLL == 0x00)return false;
	typedef HRESULT(WINAPI* PAttachControl)(IUnknown*, HWND, IUnknown**);
	PAttachControl AtlAxAttachControl = (PAttachControl)GetProcAddress(hDLL, "AtlAxAttachControl");
	RECT rect{};
	GetClientRect(hDlg, &rect);
	rect.bottom -= 0x1C;
	HRESULT init=CoInitialize(0);
	if (!CoCreateInstance(CLSID_WebBrowser, 0, CLSCTX_ALL, IID_IWebBrowser2, (void**)&pIWeb))return false;
	AtlAxAttachControl(pIWeb,hDlg, NULL);
	IConnectionPointContainer* pCPContainer;
	pIWeb->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPContainer);
	if (!pIWeb)return false;
	pCPContainer->FindConnectionPoint(DIID_DWebBrowserEvents2, &pConnectionPoint);
	if (!pCPContainer)return false;
	pEvnm = new Evenem(hDlg);
	if (!pEvnm)return false;
	pConnectionPoint->Advise(pEvnm, &dwCookie);
	if (pCPContainer) 
	{
		pCPContainer->Release();
	}
	else
	{
		return false;
	}
	pIWeb->GoHome();
	BSTR buf[0xff];
	pIWeb->get_LocationName(buf);
	SetDlgItemText(hDlg, 5000, (LPCSTR)buf);
	//>Navigate(url, 0, 0, 0, 0);	
	return true;
}
void NavigerExplorateur(HWND hDlg, char* Chemin)
{
	WCHAR url[256];
	MultiByteToWideChar(CP_ACP, 0, Chemin, -1, url, 256);
	pIWeb->Navigate(url, 0, 0, 0, 0);
}
void LibererExplorateur()
{
	pIWeb->Release();
	delete pEvnm;
	if (pConnectionPoint == 0x00) return;
	pConnectionPoint->Unadvise(dwCookie);
	pConnectionPoint->Release();
	CoUninitialize();
	FreeLibrary(hDLL);
}
BSTR ConvertirChemin(char* Chemin)
{
	WCHAR url[256];
	MultiByteToWideChar(CP_ACP, 0,Chemin , -1, url, 256);
	return url;
}
