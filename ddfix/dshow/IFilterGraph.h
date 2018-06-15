#pragma once

class m_IFilterGraph : public IFilterGraph, public AddressLookupTableObject
{
private:
	IFilterGraph *ProxyInterface;

public:
	m_IFilterGraph(IFilterGraph *aOriginal, void *temp) : ProxyInterface(aOriginal)
	{
		ProxyAddressLookupTable.SaveAddress(this, ProxyInterface);
	}
	~m_IFilterGraph()
	{
		ProxyAddressLookupTable.DeleteAddress(this);
	}

	IFilterGraph *GetProxyInterface() { return ProxyInterface; }

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IFilterGraph methods ***/
	HRESULT STDMETHODCALLTYPE AddFilter(
		/* [in] */ IBaseFilter *pFilter,
		/* [string][in] */ LPCWSTR pName);

	HRESULT STDMETHODCALLTYPE RemoveFilter(
		/* [in] */ IBaseFilter *pFilter);

	HRESULT STDMETHODCALLTYPE EnumFilters(
		/* [annotation][out] */
		_Out_  IEnumFilters **ppEnum);

	HRESULT STDMETHODCALLTYPE FindFilterByName(
		/* [string][in] */ LPCWSTR pName,
		/* [annotation][out] */
		_Out_  IBaseFilter **ppFilter);

	HRESULT STDMETHODCALLTYPE ConnectDirect(
		/* [in] */ IPin *ppinOut,
		/* [in] */ IPin *ppinIn,
		/* [annotation][unique][in] */
		_In_opt_  const AM_MEDIA_TYPE *pmt);

	HRESULT STDMETHODCALLTYPE Reconnect(
		/* [in] */ IPin *ppin);

	HRESULT STDMETHODCALLTYPE Disconnect(
		/* [in] */ IPin *ppin);

	HRESULT STDMETHODCALLTYPE SetDefaultSyncSource(void);
};
