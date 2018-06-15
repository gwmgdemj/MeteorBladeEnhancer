/**
* Copyright (C) 2017 Elisha Riedlinger
*
* This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
* authors be held liable for any damages arising from the use of this software.
* Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
*      original  software. If you use this  software  in a product, an  acknowledgment in the product
*      documentation would be appreciated but is not required.
*   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
*      being the original software.
*   3. This notice may not be removed or altered from any source distribution.
*/

#include "ddraw.h"
#include "../D3D9Context.h"


m_IDirect3DViewport3::~m_IDirect3DViewport3()
{
	ProxyAddressLookupTable.DeleteAddress(this);
	if (m_viewport9)
	{
		delete m_viewport9;
		m_viewport9 = nullptr;
	}

	for (auto light : m_lights)
	{
		if (light)
			light->Release();
	}
	m_lights.fill(nullptr);
}

HRESULT m_IDirect3DViewport3::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	if ((riid == IID_IDirect3DViewport3 || riid == IID_IUnknown) && ppvObj)
	{
		AddRef();

		*ppvObj = this;

		return S_OK;
	}

	HRESULT hr = ProxyInterface->QueryInterface(riid, ppvObj);

	if (SUCCEEDED(hr))
	{
		genericQueryInterface(riid, ppvObj);
	}

	return hr;
}

ULONG m_IDirect3DViewport3::AddRef()
{
	return ++Refs;
}

ULONG m_IDirect3DViewport3::Release()
{
	ULONG x = --Refs;

	if (x == 0)
	{
		delete this;
	}

	return x;
}

HRESULT m_IDirect3DViewport3::Initialize(LPDIRECT3D a)
{
	if (a)
	{
		a = static_cast<m_IDirect3D *>(a)->GetProxyInterface();
	}

	return ProxyInterface->Initialize(a);
}

HRESULT m_IDirect3DViewport3::GetViewport(LPD3DVIEWPORT a)
{
	return ProxyInterface->GetViewport(a);
}

HRESULT m_IDirect3DViewport3::SetViewport(LPD3DVIEWPORT a)
{
	return ProxyInterface->SetViewport(a);
}

HRESULT m_IDirect3DViewport3::TransformVertices(DWORD a, LPD3DTRANSFORMDATA b, DWORD c, LPDWORD d)
{
	return ProxyInterface->TransformVertices(a, b, c, d);
}

HRESULT m_IDirect3DViewport3::LightElements(DWORD a, LPD3DLIGHTDATA b)
{
	return ProxyInterface->LightElements(a, b);
}

HRESULT m_IDirect3DViewport3::SetBackground(D3DMATERIALHANDLE a)
{
	return ProxyInterface->SetBackground(a);
}

HRESULT m_IDirect3DViewport3::GetBackground(LPD3DMATERIALHANDLE a, LPBOOL b)
{
	return ProxyInterface->GetBackground(a, b);
}

HRESULT m_IDirect3DViewport3::SetBackgroundDepth(LPDIRECTDRAWSURFACE a)
{
	if (a)
	{
		a = static_cast<m_IDirectDrawSurface *>(a)->GetProxyInterface();
	}

	return ProxyInterface->SetBackgroundDepth(a);
}

HRESULT m_IDirect3DViewport3::GetBackgroundDepth(LPDIRECTDRAWSURFACE * a, LPBOOL b)
{
	HRESULT hr = ProxyInterface->GetBackgroundDepth(a, b);

	if (SUCCEEDED(hr))
	{
		*a = ProxyAddressLookupTable.FindAddress<m_IDirectDrawSurface>(*a);
	}

	return hr;
}

HRESULT m_IDirect3DViewport3::Clear(DWORD a, LPD3DRECT b, DWORD c)
{
	return ProxyInterface->Clear(a, b, c);
}

HRESULT m_IDirect3DViewport3::AddLight(LPDIRECT3DLIGHT a)
{
	if (!a)
	{
		return DDERR_INVALIDPARAMS;
	}

	// TODO: 我不确定这样转换是否正确
	ND3D9::D3D9Context::Instance()->GetDevice()->SetRenderState(ND3D9::D3DRS_LIGHTING, TRUE);
	// TODO: not working on point light.
	auto light = static_cast<m_IDirect3DLight*>(a);
	for (size_t i = 0; i < m_lights.size(); i++)
	{
		if (m_lights[i])
			continue;
		m_lights[i] = light;
		light->AddRef();

		ND3D9::D3DLIGHT9 light9 = *light->GetLight9();
		auto device = WrapperAddressLookupTable->FindWrapperOnly<m_IDirect3DDevice3>(IID_IDirect3DDevice3);
		auto lightAmbient = device->GetLightAmbient();
		light9.Ambient = lightAmbient;
		ND3D9::D3D9Context::Instance()->GetDevice()->SetLight(i, &light9);
		ND3D9::D3D9Context::Instance()->GetDevice()->LightEnable(i, TRUE);
		break;
	}

	
	return DD_OK;
}

HRESULT m_IDirect3DViewport3::DeleteLight(LPDIRECT3DLIGHT a)
{
	if (!a)
	{
		return DDERR_INVALIDPARAMS;
	}


	auto light = static_cast<m_IDirect3DLight*>(a);
	int lightCountInScene = 0;
	for (size_t i = 0; i < m_lights.size(); i++)
	{
		if (m_lights[i])
			++lightCountInScene;
		if (m_lights[i] != a)
			continue;
		m_lights[i] = nullptr;
		light->Release();
		ND3D9::D3D9Context::Instance()->GetDevice()->LightEnable(i, FALSE);
		--lightCountInScene;
	}

	// TODO: 我不确定这样转换是否正确
	if (lightCountInScene == 0)
	{
		ND3D9::D3D9Context::Instance()->GetDevice()->SetRenderState(ND3D9::D3DRS_LIGHTING, FALSE);
		ND3D9::D3D9Context::Instance()->GetDevice()->SetRenderState(ND3D9::D3DRS_AMBIENT, 0x00ffffff);
	}
	return DD_OK;
}

HRESULT m_IDirect3DViewport3::NextLight(LPDIRECT3DLIGHT a, LPDIRECT3DLIGHT * b, DWORD c)
{
	if (a)
	{
		a = static_cast<m_IDirect3DLight *>(a)->GetProxyInterface();
	}

	HRESULT hr = ProxyInterface->NextLight(a, b, c);

	if (SUCCEEDED(hr))
	{
		*b = ProxyAddressLookupTable.FindAddress<m_IDirect3DLight>(*b);
	}

	return hr;
}

HRESULT m_IDirect3DViewport3::GetViewport2(LPD3DVIEWPORT2 a)
{
	if (!m_viewport9)
	{
		return DDERR_INVALIDPARAMS;
	}

	if (a)
	{
		D3DVIEWPORT2 viewport;

		viewport.dwSize = sizeof(viewport);
		viewport.dwX = m_viewport9->X;
		viewport.dwY = m_viewport9->Y;
		viewport.dwWidth = m_viewport9->Width;
		viewport.dwHeight = m_viewport9->Height;
		viewport.dvMinZ = m_viewport9->MinZ;
		viewport.dvMaxZ = m_viewport9->MaxZ;

		*a = viewport;
		return DD_OK;
	}
	else
	{
		return DDERR_INVALIDPARAMS;
	}
}

HRESULT m_IDirect3DViewport3::SetViewport2(LPD3DVIEWPORT2 a)
{
	if (a)
	{
		if (!m_viewport9)
			m_viewport9 = new ND3D9::D3DVIEWPORT9;

		m_viewport9->X = a->dwX;
		m_viewport9->Y = a->dwY;
		m_viewport9->Width = a->dwWidth;
		m_viewport9->Height = a->dwHeight;
		m_viewport9->MinZ = a->dvMinZ;
		m_viewport9->MaxZ = a->dvMaxZ;
	}
	else
	{
		delete m_viewport9;
		m_viewport9 = nullptr;
	}
	return DD_OK;
}

HRESULT m_IDirect3DViewport3::SetBackgroundDepth2(LPDIRECTDRAWSURFACE4 a)
{
	if (a)
	{
		a = static_cast<m_IDirectDrawSurface4 *>(a)->GetProxyInterface();
	}

	return ProxyInterface->SetBackgroundDepth2(a);
}

HRESULT m_IDirect3DViewport3::GetBackgroundDepth2(LPDIRECTDRAWSURFACE4 * a, LPBOOL b)
{
	// make compile pass
// 	HRESULT hr = ProxyInterface->GetBackgroundDepth2(a, b);
// 
// 	if (SUCCEEDED(hr))
// 	{
// 		*a = ProxyAddressLookupTable.FindAddress<m_IDirectDrawSurface4>(*a);
// 	}
// 
// 	return hr;
	return DDERR_GENERIC;
}

HRESULT m_IDirect3DViewport3::Clear2(DWORD dwCount, LPD3DRECT lpRects, DWORD dwFlags, DWORD dwColor, D3DVALUE dvZ, DWORD dwStencil)
{
	ND3D9::D3D9Context::Instance()->GetDevice()->Clear(dwCount, lpRects, dwFlags, dwColor, dvZ, dwStencil);
	return DD_OK;
}
