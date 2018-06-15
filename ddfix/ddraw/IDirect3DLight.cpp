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

m_IDirect3DLight::~m_IDirect3DLight()
{
	ProxyAddressLookupTable.DeleteAddress(this);
	if (m_light9)
	{
		delete m_light9;
		m_light9 = nullptr;
	}
}

HRESULT m_IDirect3DLight::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	if ((riid == IID_IDirect3DLight || riid == IID_IUnknown) && ppvObj)
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

ULONG m_IDirect3DLight::AddRef()
{
	return ++Refs;
}

ULONG m_IDirect3DLight::Release()
{
	ULONG x = --Refs;

	if (x == 0)
	{
		delete this;
	}

	return x;
}

HRESULT m_IDirect3DLight::Initialize(LPDIRECT3D a)
{
	if (a)
	{
		a = static_cast<m_IDirect3D *>(a)->GetProxyInterface();
	}

	return ProxyInterface->Initialize(a);
}

HRESULT m_IDirect3DLight::SetLight(LPD3DLIGHT a)
{
	if (a)
	{
		if (!m_light9)
			m_light9 = new ND3D9::D3DLIGHT9;

		ND3D9::D3DLIGHT9 light9 = { 0 };
		light9.Type = (ND3D9::D3DLIGHTTYPE) a->dltType;
		light9.Diffuse = a->dcvColor;
		light9.Position = a->dvPosition;
		light9.Direction = a->dvDirection;
		light9.Range = a->dvRange;
		light9.Falloff = a->dvFalloff;
		light9.Attenuation0 = a->dvAttenuation0;
		light9.Attenuation1 = a->dvAttenuation1;
		light9.Attenuation2 = a->dvAttenuation2;
		light9.Theta = a->dvTheta;
		light9.Phi = a->dvPhi;

		*m_light9 = light9;
	}
	else
	{
		if (m_light9)
		{
			delete m_light9;
			m_light9 = nullptr;
		}
	}
	
	return DD_OK;
}

HRESULT m_IDirect3DLight::GetLight(LPD3DLIGHT a)
{
	return ProxyInterface->GetLight(a);
}
