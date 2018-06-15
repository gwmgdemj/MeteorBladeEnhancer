#pragma once

class m_IGraphBuilder : public IGraphBuilder, public AddressLookupTableObject
{
private:
	IGraphBuilder *ProxyInterface;

public:
	m_IGraphBuilder(IGraphBuilder *aOriginal, void *temp) : ProxyInterface(aOriginal)
	{
		ProxyAddressLookupTable.SaveAddress(this, ProxyInterface);
	}
	~m_IGraphBuilder()
	{
		ProxyAddressLookupTable.DeleteAddress(this);
	}

	IGraphBuilder *GetProxyInterface() { return ProxyInterface; }

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IFilterGraph methods ***/
	virtual HRESULT STDMETHODCALLTYPE AddFilter(
		/* [in] */ IBaseFilter *pFilter,
		/* [string][in] */ LPCWSTR pName);

	virtual HRESULT STDMETHODCALLTYPE RemoveFilter(
		/* [in] */ IBaseFilter *pFilter);

	virtual HRESULT STDMETHODCALLTYPE EnumFilters(
		/* [annotation][out] */
		_Out_  IEnumFilters **ppEnum);

	virtual HRESULT STDMETHODCALLTYPE FindFilterByName(
		/* [string][in] */ LPCWSTR pName,
		/* [annotation][out] */
		_Out_  IBaseFilter **ppFilter);

	virtual HRESULT STDMETHODCALLTYPE ConnectDirect(
		/* [in] */ IPin *ppinOut,
		/* [in] */ IPin *ppinIn,
		/* [annotation][unique][in] */
		_In_opt_  const AM_MEDIA_TYPE *pmt);

	virtual HRESULT STDMETHODCALLTYPE Reconnect(
		/* [in] */ IPin *ppin);

	virtual HRESULT STDMETHODCALLTYPE Disconnect(
		/* [in] */ IPin *ppin);

	virtual HRESULT STDMETHODCALLTYPE SetDefaultSyncSource(void);

	/*** IGraphBuilder methods ***/
	virtual HRESULT STDMETHODCALLTYPE Connect(
		/* [in] */ IPin *ppinOut,
		/* [in] */ IPin *ppinIn);

	virtual HRESULT STDMETHODCALLTYPE Render(
		/* [in] */ IPin *ppinOut);

	virtual HRESULT STDMETHODCALLTYPE RenderFile(
		/* [in] */ LPCWSTR lpcwstrFile,
		/* [annotation][unique][in] */
		_In_opt_  LPCWSTR lpcwstrPlayList);

	virtual HRESULT STDMETHODCALLTYPE AddSourceFilter(
		/* [in] */ LPCWSTR lpcwstrFileName,
		/* [annotation][unique][in] */
		_In_opt_  LPCWSTR lpcwstrFilterName,
		/* [annotation][out] */
		_Out_  IBaseFilter **ppFilter);

	virtual HRESULT STDMETHODCALLTYPE SetLogFile(
		/* [in] */ DWORD_PTR hFile);

	virtual HRESULT STDMETHODCALLTYPE Abort(void);

	virtual HRESULT STDMETHODCALLTYPE ShouldOperationContinue(void);
};
