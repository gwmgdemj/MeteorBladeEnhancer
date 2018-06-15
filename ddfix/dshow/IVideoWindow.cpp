#include "dshow.h"
#include <d3d9.h>
#include <vmr9.h>

HRESULT m_IVideoWindow::QueryInterface(THIS_ REFIID riid, LPVOID * ppvObj)
{
	return ProxyInterface->QueryInterface(riid, ppvObj);
}

ULONG m_IVideoWindow::AddRef(THIS)
{
	return ProxyInterface->AddRef();
}


ULONG m_IVideoWindow::Release(THIS)
{
	return ProxyInterface->Release();
}

/* [propput] */ HRESULT m_IVideoWindow::put_Caption(/* [in] */ __RPC__in BSTR strCaption)
{
	return S_OK;
	return ProxyInterface->put_Caption(strCaption);
}

/* [propget] */ HRESULT m_IVideoWindow::get_Caption(/* [retval][out] */ __RPC__deref_out_opt BSTR *strCaption)
{
	return ProxyInterface->get_Caption(strCaption);
}

/* [propput] */ HRESULT m_IVideoWindow::put_WindowStyle(/* [in] */ long WindowStyle)
{
	return S_OK;
	return ProxyInterface->put_WindowStyle(WindowStyle);
}

/* [propget] */ HRESULT m_IVideoWindow::get_WindowStyle(/* [retval][out] */ __RPC__out long *WindowStyle)
{
	return ProxyInterface->get_WindowStyle(WindowStyle);
}

/* [propput] */ HRESULT m_IVideoWindow::put_WindowStyleEx(/* [in] */ long WindowStyleEx)
{
	return ProxyInterface->put_WindowStyleEx(WindowStyleEx);
}

/* [propget] */ HRESULT m_IVideoWindow::get_WindowStyleEx(/* [retval][out] */ __RPC__out long *WindowStyleEx)
{
	return ProxyInterface->get_WindowStyleEx(WindowStyleEx);
}

/* [propput] */ HRESULT m_IVideoWindow::put_AutoShow(/* [in] */ long AutoShow)
{
	return ProxyInterface->put_AutoShow(AutoShow);
}

/* [propget] */ HRESULT m_IVideoWindow::get_AutoShow(/* [retval][out] */ __RPC__out long *AutoShow)
{
	return ProxyInterface->get_AutoShow(AutoShow);
}

/* [propput] */ HRESULT m_IVideoWindow::put_WindowState(/* [in] */ long WindowState)
{
	return ProxyInterface->put_WindowState(WindowState);
}

/* [propget] */ HRESULT m_IVideoWindow::get_WindowState(/* [retval][out] */ __RPC__out long *WindowState)
{
	return ProxyInterface->get_WindowState(WindowState);
}

/* [propput] */ HRESULT m_IVideoWindow::put_BackgroundPalette(/* [in] */ long BackgroundPalette)
{
	return ProxyInterface->put_BackgroundPalette(BackgroundPalette);
}

/* [propget] */ HRESULT m_IVideoWindow::get_BackgroundPalette(/* [retval][out] */ __RPC__out long *pBackgroundPalette)
{
	return ProxyInterface->get_BackgroundPalette(pBackgroundPalette);
}

/* [propput] */ HRESULT m_IVideoWindow::put_Visible(/* [in] */ long Visible)
{
	return S_OK;
	return ProxyInterface->put_Visible(Visible);
}

/* [propget] */ HRESULT m_IVideoWindow::get_Visible(/* [retval][out] */ __RPC__out long *pVisible)
{
	*pVisible = true;
	return S_OK;
	return ProxyInterface->get_Visible(pVisible);
}

/* [propput] */ HRESULT m_IVideoWindow::put_Left(/* [in] */ long Left)
{
	return ProxyInterface->put_Left(Left);
}

/* [propget] */ HRESULT m_IVideoWindow::get_Left(/* [retval][out] */ __RPC__out long *pLeft)
{
	return ProxyInterface->get_Left(pLeft);
}

/* [propput] */ HRESULT m_IVideoWindow::put_Width(/* [in] */ long Width)
{
	return ProxyInterface->put_Width(Width);
}

/* [propget] */ HRESULT m_IVideoWindow::get_Width(/* [retval][out] */ __RPC__out long *pWidth)
{
	return ProxyInterface->get_Width(pWidth);
}

/* [propput] */ HRESULT m_IVideoWindow::put_Top(/* [in] */ long Top)
{
	return ProxyInterface->put_Top(Top);
}

/* [propget] */ HRESULT m_IVideoWindow::get_Top(/* [retval][out] */ __RPC__out long *pTop)
{
	return ProxyInterface->get_Top(pTop);
}

/* [propput] */ HRESULT m_IVideoWindow::put_Height(/* [in] */ long Height)
{
	return ProxyInterface->put_Height(Height);
}

/* [propget] */ HRESULT m_IVideoWindow::get_Height(/* [retval][out] */ __RPC__out long *pHeight)
{
	return ProxyInterface->get_Height(pHeight);
}

/* [propput] */ HRESULT m_IVideoWindow::put_Owner(/* [in] */ OAHWND Owner)
{
	return S_OK;
	return ProxyInterface->put_Owner(Owner);
}

/* [propget] */ HRESULT m_IVideoWindow::get_Owner(/* [retval][out] */ __RPC__out OAHWND *Owner)
{
	return ProxyInterface->get_Owner(Owner);
}

/* [propput] */ HRESULT m_IVideoWindow::put_MessageDrain(/* [in] */ OAHWND Drain)
{
	return S_OK;
	return ProxyInterface->put_MessageDrain(Drain);
}

/* [propget] */ HRESULT m_IVideoWindow::get_MessageDrain(/* [retval][out] */ __RPC__out OAHWND *Drain)
{
	return ProxyInterface->get_MessageDrain(Drain);
}

/* [propget] */ HRESULT m_IVideoWindow::get_BorderColor(/* [retval][out] */ __RPC__out long *Color)
{
	return ProxyInterface->get_BorderColor(Color);
}

/* [propput] */ HRESULT m_IVideoWindow::put_BorderColor(/* [in] */ long Color)
{
	return ProxyInterface->put_BorderColor(Color);
}

/* [propget] */ HRESULT m_IVideoWindow::get_FullScreenMode(/* [retval][out] */ __RPC__out long *FullScreenMode)
{
	return ProxyInterface->get_FullScreenMode(FullScreenMode);
}

/* [propput] */ HRESULT m_IVideoWindow::put_FullScreenMode(/* [in] */ long FullScreenMode)
{
	return S_OK;
	return ProxyInterface->put_FullScreenMode(FullScreenMode);
}

HRESULT m_IVideoWindow::SetWindowForeground(/* [in] */ long Focus)
{
	return ProxyInterface->SetWindowForeground(Focus);
}

HRESULT m_IVideoWindow::NotifyOwnerMessage(/* [in] */ OAHWND hwnd, /* [in] */ long uMsg, /* [in] */ LONG_PTR wParam, /* [in] */ LONG_PTR lParam)
{
	return ProxyInterface->NotifyOwnerMessage(hwnd, uMsg, wParam, lParam);
}

HRESULT m_IVideoWindow::SetWindowPosition(/* [in] */ long Left, /* [in] */ long Top, /* [in] */ long Width, /* [in] */ long Height)
{
	HRESULT hr = S_OK;
	IGraphBuilder* graphBuilder = nullptr;
	IBaseFilter* vmr9 = nullptr;
	IVMRWindowlessControl9* vmr9Ctrl = nullptr;

	hr = QueryInterface(IID_IGraphBuilder, (LPVOID*)&graphBuilder);
	if (SUCCEEDED(hr))
	{
		hr = graphBuilder->FindFilterByName(L"Video Mixing Renderer 9", &vmr9);
		if (SUCCEEDED(hr))
		{
			hr = vmr9->QueryInterface(IID_IVMRWindowlessControl9, (LPVOID*)&vmr9Ctrl);
			if (SUCCEEDED(hr))
			{
				RECT rect;
				rect.left = Left;
				rect.top = Top;
				rect.right = Width + Left;
				rect.bottom = Height + Top;
				hr = vmr9Ctrl->SetVideoPosition(nullptr, &rect);
			}
		}

	}

	if (graphBuilder)
	{
		graphBuilder->Release();
	}

	if (vmr9)
	{
		vmr9->Release();
	}

	if (vmr9Ctrl)
	{
		vmr9Ctrl->Release();
	}

	return S_OK;
	return ProxyInterface->SetWindowPosition(Left, Top, Width, Height);
}

HRESULT m_IVideoWindow::GetWindowPosition(/* [out] */ __RPC__out long *pLeft, /* [out] */ __RPC__out long *pTop, /* [out] */ __RPC__out long *pWidth, /* [out] */ __RPC__out long *pHeight)
{
	return ProxyInterface->GetWindowPosition(pLeft,pTop, pWidth, pHeight);
}

HRESULT m_IVideoWindow::GetMinIdealImageSize(/* [out] */ __RPC__out long *pWidth, /* [out] */ __RPC__out long *pHeight)
{
	return ProxyInterface->GetMinIdealImageSize(pWidth, pHeight);
}

HRESULT m_IVideoWindow::GetMaxIdealImageSize(/* [out] */ __RPC__out long *pWidth, /* [out] */ __RPC__out long *pHeight)
{
	return ProxyInterface->GetMaxIdealImageSize(pWidth, pHeight);
}

HRESULT m_IVideoWindow::GetRestorePosition(/* [out] */ __RPC__out long *pLeft, /* [out] */ __RPC__out long *pTop, /* [out] */ __RPC__out long *pWidth, /* [out] */ __RPC__out long *pHeight)
{
	return ProxyInterface->GetRestorePosition(pLeft, pTop, pWidth, pHeight);
}

HRESULT m_IVideoWindow::HideCursor(/* [in] */ long HideCursor)
{
	return ProxyInterface->HideCursor(HideCursor);
}

HRESULT m_IVideoWindow::IsCursorHidden(/* [out] */ __RPC__out long *CursorHidden)
{
	return ProxyInterface->IsCursorHidden(CursorHidden);
}

HRESULT m_IVideoWindow::GetTypeInfoCount(__RPC__out UINT *pctinfo)
{
	return ProxyInterface->GetTypeInfoCount(pctinfo);
}

HRESULT m_IVideoWindow::GetTypeInfo(UINT iTInfo, LCID lcid, __RPC__deref_out_opt ITypeInfo **ppTInfo)
{
	return ProxyInterface->GetTypeInfo(iTInfo, lcid, ppTInfo);
}

HRESULT m_IVideoWindow::GetIDsOfNames(__RPC__in REFIID riid, __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames, __RPC__in_range(0, 16384) UINT cNames, LCID lcid, __RPC__out_ecount_full(cNames) DISPID *rgDispId)
{
	return ProxyInterface->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);
}

HRESULT m_IVideoWindow::Invoke(_In_ DISPID dispIdMember, _In_ REFIID riid, _In_ LCID lcid, _In_ WORD wFlags, _In_ DISPPARAMS *pDispParams, _Out_opt_ VARIANT *pVarResult, _Out_opt_ EXCEPINFO *pExcepInfo, _Out_opt_ UINT *puArgErr)
{
	return ProxyInterface->Invoke(dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

// HRESULT m_IVideoWindow::RemoteInvoke(DISPID dispIdMember, REFIID riid, LCID lcid, DWORD dwFlags, DISPPARAMS * pDispParams, VARIANT * pVarResult, EXCEPINFO * pExcepInfo, UINT * pArgErr, UINT cVarRef, UINT * rgVarRefIdx, VARIANTARG * rgVarRef)
// {
// 	return ProxyInterface->RemoteInvoke(dispIdMember, riid, dwFlags, pDispParams, pVarResult, pExcepInfo, pArgErr, cVarRef, rgVarRefIdx, rgVarRef);
// }
// 
