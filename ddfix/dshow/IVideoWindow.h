#pragma once

class m_IVideoWindow : public IVideoWindow, public AddressLookupTableObject
{
private:
	IVideoWindow *ProxyInterface;

public:
	m_IVideoWindow(IVideoWindow *aOriginal, void *temp) : ProxyInterface(aOriginal)
	{
		ProxyAddressLookupTable.SaveAddress(this, ProxyInterface);
	}
	~m_IVideoWindow()
	{
		ProxyAddressLookupTable.DeleteAddress(this);
	}

	IVideoWindow *GetProxyInterface() { return ProxyInterface; }

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDispatch methods ***/
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(
		UINT * pctinfo
	);

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(
		UINT iTInfo,
		LCID lcid,
		ITypeInfo ** ppTInfo
	);

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
		REFIID riid,
		LPOLESTR * rgszNames,
		UINT cNames,
		LCID lcid,
		DISPID * rgDispId
	);

	virtual HRESULT STDMETHODCALLTYPE Invoke(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS * pDispParams,
		VARIANT * pVarResult,
		EXCEPINFO * pExcepInfo,
		UINT * puArgErr
	);
	/*
	virtual HRESULT STDMETHODCALLTYPE RemoteInvoke(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		DWORD dwFlags,
		DISPPARAMS * pDispParams,
		VARIANT * pVarResult,
		EXCEPINFO * pExcepInfo,
		UINT * pArgErr,
		UINT cVarRef,
		UINT * rgVarRefIdx,
		VARIANTARG * rgVarRef
	);
	*/
	/*** IVideoWindow methods ***/
	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Caption(
		/* [in] */ __RPC__in BSTR strCaption);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Caption(
		/* [retval][out] */ __RPC__deref_out_opt BSTR *strCaption);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_WindowStyle(
		/* [in] */ long WindowStyle);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_WindowStyle(
		/* [retval][out] */ __RPC__out long *WindowStyle);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_WindowStyleEx(
		/* [in] */ long WindowStyleEx);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_WindowStyleEx(
		/* [retval][out] */ __RPC__out long *WindowStyleEx);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_AutoShow(
		/* [in] */ long AutoShow);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AutoShow(
		/* [retval][out] */ __RPC__out long *AutoShow);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_WindowState(
		/* [in] */ long WindowState);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_WindowState(
		/* [retval][out] */ __RPC__out long *WindowState);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_BackgroundPalette(
		/* [in] */ long BackgroundPalette);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BackgroundPalette(
		/* [retval][out] */ __RPC__out long *pBackgroundPalette);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Visible(
		/* [in] */ long Visible);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Visible(
		/* [retval][out] */ __RPC__out long *pVisible);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Left(
		/* [in] */ long Left);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Left(
		/* [retval][out] */ __RPC__out long *pLeft);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Width(
		/* [in] */ long Width);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Width(
		/* [retval][out] */ __RPC__out long *pWidth);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Top(
		/* [in] */ long Top);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Top(
		/* [retval][out] */ __RPC__out long *pTop);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Height(
		/* [in] */ long Height);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Height(
		/* [retval][out] */ __RPC__out long *pHeight);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Owner(
		/* [in] */ OAHWND Owner);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Owner(
		/* [retval][out] */ __RPC__out OAHWND *Owner);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_MessageDrain(
		/* [in] */ OAHWND Drain);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MessageDrain(
		/* [retval][out] */ __RPC__out OAHWND *Drain);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BorderColor(
		/* [retval][out] */ __RPC__out long *Color);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_BorderColor(
		/* [in] */ long Color);

	virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FullScreenMode(
		/* [retval][out] */ __RPC__out long *FullScreenMode);

	virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_FullScreenMode(
		/* [in] */ long FullScreenMode);

	virtual HRESULT STDMETHODCALLTYPE SetWindowForeground(
		/* [in] */ long Focus);

	virtual HRESULT STDMETHODCALLTYPE NotifyOwnerMessage(
		/* [in] */ OAHWND hwnd,
		/* [in] */ long uMsg,
		/* [in] */ LONG_PTR wParam,
		/* [in] */ LONG_PTR lParam);

	virtual HRESULT STDMETHODCALLTYPE SetWindowPosition(
		/* [in] */ long Left,
		/* [in] */ long Top,
		/* [in] */ long Width,
		/* [in] */ long Height);

	virtual HRESULT STDMETHODCALLTYPE GetWindowPosition(
		/* [out] */ __RPC__out long *pLeft,
		/* [out] */ __RPC__out long *pTop,
		/* [out] */ __RPC__out long *pWidth,
		/* [out] */ __RPC__out long *pHeight);

	virtual HRESULT STDMETHODCALLTYPE GetMinIdealImageSize(
		/* [out] */ __RPC__out long *pWidth,
		/* [out] */ __RPC__out long *pHeight);

	virtual HRESULT STDMETHODCALLTYPE GetMaxIdealImageSize(
		/* [out] */ __RPC__out long *pWidth,
		/* [out] */ __RPC__out long *pHeight);

	virtual HRESULT STDMETHODCALLTYPE GetRestorePosition(
		/* [out] */ __RPC__out long *pLeft,
		/* [out] */ __RPC__out long *pTop,
		/* [out] */ __RPC__out long *pWidth,
		/* [out] */ __RPC__out long *pHeight);

	virtual HRESULT STDMETHODCALLTYPE HideCursor(
		/* [in] */ long HideCursor);

	virtual HRESULT STDMETHODCALLTYPE IsCursorHidden(
		/* [out] */ __RPC__out long *CursorHidden);
};
