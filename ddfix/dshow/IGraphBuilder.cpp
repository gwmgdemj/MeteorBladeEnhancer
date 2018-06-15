#include "dshow.h"
#include <d3d9.h>
#include <vmr9.h>
HRESULT m_IGraphBuilder::QueryInterface(THIS_ REFIID riid, LPVOID * ppvObj)
{
	auto hr = ProxyInterface->QueryInterface(riid, ppvObj);
	if (riid == IID_IMediaControl)
	{

	}
	else if (riid == IID_IMediaSeeking)
	{

	}
	else if (riid == IID_IMediaEventEx)
	{

	}
	else if (riid == IID_IBasicVideo)
	{

	}
	else if (riid == IID_IBasicAudio)
	{

	}
	else if (riid == IID_IVideoWindow)
	{
		*ppvObj = ProxyAddressLookupTable.FindAddress<m_IVideoWindow>(*ppvObj);
	}
	else
	{
		int a = 0;
		a = 1;
	}
	return hr;
}

ULONG m_IGraphBuilder::AddRef(THIS)
{
	return ProxyInterface->AddRef();
}


ULONG m_IGraphBuilder::Release(THIS)
{
	return ProxyInterface->Release();
}


HRESULT m_IGraphBuilder::AddFilter(/* [in] */ IBaseFilter *pFilter, /* [string][in] */ LPCWSTR pName)
{
	return ProxyInterface->AddFilter(pFilter, pName);
}

HRESULT m_IGraphBuilder::RemoveFilter(/* [in] */ IBaseFilter *pFilter)
{
	return ProxyInterface->RemoveFilter(pFilter);
}

HRESULT m_IGraphBuilder::EnumFilters(/* [annotation][out] */ _Out_ IEnumFilters **ppEnum)
{
	return ProxyInterface->EnumFilters(ppEnum);
}

HRESULT m_IGraphBuilder::FindFilterByName(/* [string][in] */ LPCWSTR pName, /* [annotation][out] */ _Out_ IBaseFilter **ppFilter)
{
	return ProxyInterface->FindFilterByName(pName, ppFilter);
}

HRESULT m_IGraphBuilder::ConnectDirect(/* [in] */ IPin *ppinOut, /* [in] */ IPin *ppinIn, /* [annotation][unique][in] */ _In_opt_ const AM_MEDIA_TYPE *pmt)
{
	return ProxyInterface->ConnectDirect(ppinOut, ppinIn, pmt);
}

HRESULT m_IGraphBuilder::Reconnect(/* [in] */ IPin *ppin)
{
	return ProxyInterface->Reconnect(ppin);
}

HRESULT m_IGraphBuilder::Disconnect(/* [in] */ IPin *ppin)
{
	return ProxyInterface->Disconnect(ppin);
}

HRESULT m_IGraphBuilder::SetDefaultSyncSource(void)
{
	return ProxyInterface->SetDefaultSyncSource();
}

HRESULT m_IGraphBuilder::Connect(/* [in] */ IPin *ppinOut, /* [in] */ IPin *ppinIn)
{
	return ProxyInterface->Connect(ppinOut, ppinIn);
}

HRESULT m_IGraphBuilder::Render(/* [in] */ IPin *ppinOut)
{
	return ProxyInterface->Render(ppinOut);
}

HRESULT m_IGraphBuilder::RenderFile(/* [in] */ LPCWSTR lpcwstrFile, /* [annotation][unique][in] */ _In_opt_ LPCWSTR lpcwstrPlayList)
{
	IBaseFilter* vmr9Filter = nullptr;
	IVMRWindowlessControl9* vmr9Ctrl = nullptr;
	auto hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&vmr9Filter);
	hr = AddFilter(vmr9Filter, L"Video Mixing Renderer 9");
	if (SUCCEEDED(hr))
	{
		// Test VMRConfig, VMRMonitorConfig
		IVMRFilterConfig9* pConfig;
		HRESULT hRes2 = vmr9Filter->QueryInterface(IID_IVMRFilterConfig9, (LPVOID *)&pConfig);
		if (SUCCEEDED(hRes2))
		{
			hRes2 = pConfig->SetNumberOfStreams(1);
			hRes2 = pConfig->SetRenderingMode(VMR9Mode_Windowless);
			//hRes2 = pConfig->SetRenderingPrefs(RenderPrefs_AllowOverlays);
			pConfig->Release();
		}

		IVMRMonitorConfig9* pMonitorConfig;
		HRESULT hRes3 = vmr9Filter->QueryInterface(IID_IVMRMonitorConfig9, (LPVOID *)&pMonitorConfig);
		if (SUCCEEDED(hRes3))
		{
			UINT iCurrentMonitor;
			HRESULT hr4 = pMonitorConfig->GetMonitor(&iCurrentMonitor);
			pMonitorConfig->Release();
		}

		hr = vmr9Filter->QueryInterface(IID_IVMRWindowlessControl9, (LPVOID *)&vmr9Ctrl);

		if (SUCCEEDED(hr))
		{
			HWND hwnd = GetActiveWindow();
			HRESULT hr = vmr9Ctrl->SetVideoClippingWindow(hwnd);
			hr = vmr9Ctrl->SetAspectRatioMode(VMR_ARMODE_NONE);
		}
		else
		{
			if (vmr9Ctrl)
			{
				vmr9Ctrl->Release();
				vmr9Ctrl = NULL;
			}
		}
	}

	if (vmr9Filter)
	{
		vmr9Filter->Release();
	}

	hr = ProxyInterface->RenderFile(lpcwstrFile, lpcwstrPlayList);
	return hr;
}

HRESULT m_IGraphBuilder::AddSourceFilter(/* [in] */ LPCWSTR lpcwstrFileName, /* [annotation][unique][in] */ _In_opt_ LPCWSTR lpcwstrFilterName, /* [annotation][out] */ _Out_ IBaseFilter **ppFilter)
{
	return ProxyInterface->AddSourceFilter(lpcwstrFileName, lpcwstrFilterName, ppFilter);
}

HRESULT m_IGraphBuilder::SetLogFile(/* [in] */ DWORD_PTR hFile)
{
	return ProxyInterface->SetLogFile(hFile);
}

HRESULT m_IGraphBuilder::Abort(void)
{
	return ProxyInterface->Abort();
}

HRESULT m_IGraphBuilder::ShouldOperationContinue(void)
{
	return ProxyInterface->ShouldOperationContinue();
}
