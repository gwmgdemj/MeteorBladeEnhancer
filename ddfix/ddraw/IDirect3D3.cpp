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
#include <assert.h>

HRESULT m_IDirect3D3::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	if ((riid == IID_IDirect3D3 || riid == IID_IUnknown) && ppvObj)
	{
		AddRef();

		*ppvObj = this;

		return S_OK;
	}

	if (riid == IID_IDirect3DDevice3)
	{
		*ppvObj = new m_IDirect3DDevice3(nullptr, nullptr);
	}
	else
	{
		assert(false);
	}

	return S_OK;
}

ULONG m_IDirect3D3::AddRef()
{
	return ++Refs;
	return ProxyInterface->AddRef();
}

ULONG m_IDirect3D3::Release()
{
	//ULONG x = ProxyInterface->Release();
	ULONG x = --Refs;

	if (x == 0)
	{
		delete this;
	}

	return x;
}

HRESULT m_IDirect3D3::EnumDevices(LPD3DENUMDEVICESCALLBACK lpEnumDevicesCallback, LPVOID lpUserArg)
{
	if (!lpEnumDevicesCallback) 
		return DDERR_INVALIDPARAMS;
	m_IDirectDraw* ddraw = WrapperAddressLookupTable->FindWrapper<m_IDirectDraw>(IID_IDirectDraw);
	D3DDEVICEDESC desc = *ddraw->GetD3DDevice3Desc();
	
	UINT adapterCount = ND3D9::D3D9Context::Instance()->GetD3D9()->GetAdapterCount();
	for (UINT i = 0; i < adapterCount; i++)
	{
		ND3D9::D3DADAPTER_IDENTIFIER9 identifier;
		if (SUCCEEDED(ND3D9::D3D9Context::Instance()->GetD3D9()->GetAdapterIdentifier(i, 0, &identifier)))
		{
			auto guid = identifier.DeviceIdentifier;
			auto description = identifier.Description;
			auto deviceName = identifier.DeviceName;
			if (!SUCCEEDED(lpEnumDevicesCallback(&guid, description, deviceName, &desc, &desc, lpUserArg)))
			{
				return DDERR_GENERIC;
			};
		}
		else
		{
			return DDERR_GENERIC;
		}
	}
	
	return D3D_OK;
}

HRESULT m_IDirect3D3::CreateLight(LPDIRECT3DLIGHT * a, LPUNKNOWN b)
{
	*a = new m_IDirect3DLight(nullptr, nullptr);
	return DD_OK;
}

HRESULT m_IDirect3D3::CreateMaterial(LPDIRECT3DMATERIAL3 * a, LPUNKNOWN b)
{
	*a = new m_IDirect3DMaterial3(nullptr, nullptr);
	return DD_OK;
}

HRESULT m_IDirect3D3::CreateViewport(LPDIRECT3DVIEWPORT3 * a, LPUNKNOWN b)
{
	*a = new m_IDirect3DViewport3(nullptr, WrapperAddressLookupTable);
	return DD_OK;
}

HRESULT m_IDirect3D3::FindDevice(LPD3DFINDDEVICESEARCH a, LPD3DFINDDEVICERESULT b)
{
	return ProxyInterface->FindDevice(a, b);
}

HRESULT m_IDirect3D3::CreateDevice(REFCLSID a, LPDIRECTDRAWSURFACE4 b, LPDIRECT3DDEVICE3 * c, LPUNKNOWN d)
{
	*c = WrapperAddressLookupTable->FindWrapper<m_IDirect3DDevice3>(IID_IDirect3DDevice3);
	return D3D_OK;
}

HRESULT m_IDirect3D3::CreateVertexBuffer(LPD3DVERTEXBUFFERDESC a, LPDIRECT3DVERTEXBUFFER * b, DWORD c, LPUNKNOWN d)
{
	HRESULT hr = ProxyInterface->CreateVertexBuffer(a, b, c, d);

	if (SUCCEEDED(hr))
	{
		*b = ProxyAddressLookupTable.FindAddress<m_IDirect3DVertexBuffer>(*b);
	}

	return hr;
}

HRESULT m_IDirect3D3::EnumZBufferFormats(REFCLSID riidDevice, LPD3DENUMPIXELFORMATSCALLBACK lpEnumCallback, LPVOID lpContext)
{
	// code is taken from project DXGL
	DDPIXELFORMAT ddpf;
	ZeroMemory(&ddpf, sizeof(DDPIXELFORMAT));
	ddpf.dwSize = sizeof(DDPIXELFORMAT);
	ddpf.dwFlags = DDPF_ZBUFFER;
	ddpf.dwZBufferBitDepth = 16;
	ddpf.dwZBitMask = 0xffff;
	if (lpEnumCallback(&ddpf, lpContext) == D3DENUMRET_CANCEL) return D3D_OK;
	ddpf.dwZBufferBitDepth = 24;
	ddpf.dwZBitMask = 0xffffff00;
	if (lpEnumCallback(&ddpf, lpContext) == D3DENUMRET_CANCEL) return D3D_OK;
	ddpf.dwZBufferBitDepth = 32;
	if (lpEnumCallback(&ddpf, lpContext) == D3DENUMRET_CANCEL) return D3D_OK;
	ddpf.dwZBitMask = 0xffffffff;
	if (lpEnumCallback(&ddpf, lpContext) == D3DENUMRET_CANCEL) return D3D_OK;
// 	if (glDD7->renderer)
// 	{
// 		if (glDD7->renderer->ext->GLEXT_EXT_packed_depth_stencil || glDD7->renderer->ext->GLEXT_NV_packed_depth_stencil)
// 		{
// 			ddpf.dwZBufferBitDepth = 32;
// 			ddpf.dwStencilBitDepth = 8;
// 			ddpf.dwZBitMask = 0xffffff00;
// 			ddpf.dwStencilBitMask = 0xff;
// 			if (lpEnumCallback(&ddpf, lpContext) == D3DENUMRET_CANCEL) TRACE_RET(HRESULT, 23, D3D_OK);
// 			ddpf.dwZBitMask = 0x00ffffff;
// 			ddpf.dwStencilBitMask = 0xff000000;
// 			if (lpEnumCallback(&ddpf, lpContext) == D3DENUMRET_CANCEL) TRACE_RET(HRESULT, 23, D3D_OK);
// 		}
// 	}
// 	TRACE_EXIT(23, D3D_OK);
	return D3D_OK;
}

HRESULT m_IDirect3D3::EvictManagedTextures()
{
	return ND3D9::D3D9Context::Instance()->GetDevice()->EvictManagedResources();
}
