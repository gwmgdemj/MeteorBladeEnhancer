#include "dshow.h"

HRESULT m_IFilterGraph::QueryInterface(THIS_ REFIID riid, LPVOID * ppvObj)
{
	return ProxyInterface->QueryInterface(riid, ppvObj);
}

ULONG m_IFilterGraph::AddRef(THIS)
{
	return ProxyInterface->AddRef();
}


ULONG m_IFilterGraph::Release(THIS)
{
	return ProxyInterface->Release();
}


HRESULT m_IFilterGraph::AddFilter(/* [in] */ IBaseFilter *pFilter, /* [string][in] */ LPCWSTR pName)
{
	return ProxyInterface->AddFilter(pFilter, pName);
}

HRESULT m_IFilterGraph::RemoveFilter(/* [in] */ IBaseFilter *pFilter)
{
	return ProxyInterface->RemoveFilter(pFilter);
}

HRESULT m_IFilterGraph::EnumFilters(/* [annotation][out] */ _Out_ IEnumFilters **ppEnum)
{
	return ProxyInterface->EnumFilters(ppEnum);
}

HRESULT m_IFilterGraph::FindFilterByName(/* [string][in] */ LPCWSTR pName, /* [annotation][out] */ _Out_ IBaseFilter **ppFilter)
{
	return ProxyInterface->FindFilterByName(pName, ppFilter);
}

HRESULT m_IFilterGraph::ConnectDirect(/* [in] */ IPin *ppinOut, /* [in] */ IPin *ppinIn, /* [annotation][unique][in] */ _In_opt_ const AM_MEDIA_TYPE *pmt)
{
	return ProxyInterface->ConnectDirect(ppinOut, ppinIn, pmt);
}

HRESULT m_IFilterGraph::Reconnect(/* [in] */ IPin *ppin)
{
	return ProxyInterface->Reconnect(ppin);
}

HRESULT m_IFilterGraph::Disconnect(/* [in] */ IPin *ppin)
{
	return ProxyInterface->Disconnect(ppin);
}

HRESULT m_IFilterGraph::SetDefaultSyncSource(void)
{
	return ProxyInterface->SetDefaultSyncSource();
}
