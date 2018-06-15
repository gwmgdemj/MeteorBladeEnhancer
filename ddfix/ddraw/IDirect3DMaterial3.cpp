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


void m_IDirect3DMaterial3::GetMaterial9(ND3D9::D3DMATERIAL9* mat9) const
{
	mat9->Diffuse = m_matDef.diffuse;
	mat9->Ambient = m_matDef.ambient;
	mat9->Specular = m_matDef.specular;
	mat9->Emissive = m_matDef.emissive;
	mat9->Power = m_matDef.power;
}

HRESULT m_IDirect3DMaterial3::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	if ((riid == IID_IDirect3DMaterial3 || riid == IID_IUnknown) && ppvObj)
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

ULONG m_IDirect3DMaterial3::AddRef()
{
	return ++Refs;
}

ULONG m_IDirect3DMaterial3::Release()
{
	ULONG x = --Refs;

	if (x == 0)
	{
		delete this;
	}

	return x;
}

HRESULT m_IDirect3DMaterial3::SetMaterial(LPD3DMATERIAL a)
{
	if (a)
	{
		m_matDef = *a;
		
		//return ND3D9::D3D9Context::Instance()->GetDevice()->SetMaterial(&matDef9);
		return DD_OK;
	}
	else
	{
		return DDERR_INVALIDPARAMS;
	}
}

HRESULT m_IDirect3DMaterial3::GetMaterial(LPD3DMATERIAL a)
{
	*a = m_matDef;
	return DD_OK;
}

HRESULT m_IDirect3DMaterial3::GetHandle(LPDIRECT3DDEVICE3 a, LPD3DMATERIALHANDLE b)
{
	// 存储this指针
	*b = (DWORD)this;
	return DD_OK;
}
