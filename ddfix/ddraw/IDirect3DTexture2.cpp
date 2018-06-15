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

m_IDirect3DTexture2::m_IDirect3DTexture2(IDirect3DTexture2 *aOriginal, m_IDirectDrawSurface4 *surface)
	: ProxyInterface(aOriginal)
	, m_tex9Handle(0)
	, Refs(1)
	, m_surface(surface)
{
	ProxyAddressLookupTable.SaveAddress(this, ProxyInterface);

	DDSURFACEDESC2 desc;
	m_surface->GetSurfaceDesc(&desc);
	auto dwCaps = desc.ddsCaps.dwCaps;
	if (desc.dwWidth > 0 && desc.dwHeight > 0)
	{
		if (!(dwCaps & (DDSCAPS_MIPMAP)) ||
			((dwCaps & (DDSCAPS_MIPMAP)) && !(desc.ddsCaps.dwCaps2 & DDSCAPS2_MIPMAPSUBLEVEL))
			)
		{
			int mipmapCount = (desc.ddsCaps.dwCaps & DDSCAPS_MIPMAP) ? desc.dwMipMapCount : 0;

			m_tex9Handle = ND3D9::D3D9Context::Instance()->CreateTexture9(
				desc.dwWidth,
				desc.dwHeight,
				mipmapCount,
				mipmapCount ? 0 : D3DUSAGE_AUTOGENMIPMAP,
				ND3D9::D3DFMT_A8R8G8B8,
				ND3D9::D3DPOOL_MANAGED);
		}		
	}


}


m_IDirect3DTexture2::~m_IDirect3DTexture2()
{
	ProxyAddressLookupTable.DeleteAddress(this);
	ND3D9::D3D9Context::Instance()->ReleaseResource9(m_tex9Handle);
	m_tex9Handle = 0;
}

SmartPtr<ND3D9::IDirect3DTexture9> m_IDirect3DTexture2::GetTexture9() const
{
	return ND3D9::D3D9Context::Instance()->GetResource9<ND3D9::IDirect3DTexture9>(m_tex9Handle, nullptr);
}

HRESULT m_IDirect3DTexture2::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	if ((riid == IID_IDirect3DTexture2 || riid == IID_IUnknown) && ppvObj)
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

ULONG m_IDirect3DTexture2::AddRef()
{
	return ++Refs;
}

ULONG m_IDirect3DTexture2::Release()
{
	ULONG x = --Refs;

	if (x == 0)
	{
		delete this;
	}

	return x;
}

HRESULT m_IDirect3DTexture2::GetHandle(LPDIRECT3DDEVICE2 a, LPD3DTEXTUREHANDLE b)
{
	if (a)
	{
		a = static_cast<m_IDirect3DDevice2 *>(a)->GetProxyInterface();
	}

	return ProxyInterface->GetHandle(a, b);
}

HRESULT m_IDirect3DTexture2::PaletteChanged(DWORD a, DWORD b)
{
	return ProxyInterface->PaletteChanged(a, b);
}

HRESULT m_IDirect3DTexture2::Load(LPDIRECT3DTEXTURE2 a)
{
	if (!a)
	{
		return DDERR_INVALIDPARAMS;
	}

	auto srcTex = static_cast<m_IDirect3DTexture2*>(a);
	auto destTex = this;	

	DDCOLORKEY srcColorKey;
	DDCOLORKEY destColorKey;

	bool hasSrcColorKey = false;
	bool hasDestColorKey = false;

	DDSURFACEDESC2 desc;
	if (SUCCEEDED(destTex->m_surface->GetColorKey(DDCKEY_SRCBLT, &srcColorKey)))
		hasSrcColorKey = true;
	if (SUCCEEDED(destTex->m_surface->GetColorKey(DDCKEY_DESTBLT, &destColorKey)))
		hasDestColorKey = true;

	m_surface->GetSurfaceDesc(&desc);

	int width = desc.dwWidth;
	int height = desc.dwHeight;

	ND3D9::D3DLOCKED_RECT srcLockedRect = { 0 };
	ND3D9::D3DLOCKED_RECT destLockedRect = { 0 };
	if(SUCCEEDED(destTex->GetTexture9()->LockRect(0, &destLockedRect, nullptr, 0) ))
	{
		if (SUCCEEDED(srcTex->GetTexture9()->LockRect(0, &srcLockedRect, nullptr, D3DLOCK_READONLY)))
		{
			char* srcLineBitsStart = (char*)srcLockedRect.pBits;
			char* destLineBitsStart = (char*)destLockedRect.pBits;
			for (int y = 0; y < height; y++)
			{
				D3DCOLOR* srcLineStart = (D3DCOLOR*)srcLineBitsStart;
				D3DCOLOR* destLineStart = (D3DCOLOR*)destLineBitsStart;
				for (int x = 0; x < width; x++)
				{
					D3DCOLOR srcPixel = srcLineStart[x];
					D3DCOLOR& destPixel = destLineStart[x];
					D3DCOLOR temp = srcPixel | 0xff000000;
					if (hasSrcColorKey)
					{
						if ((srcPixel & 0x00ffffff) == srcColorKey.dwColorSpaceLowValue)
						{
							temp = temp & 0x00ffffff;
						}
					}
					destPixel = temp;
				}
				srcLineBitsStart += srcLockedRect.Pitch;
				destLineBitsStart += destLockedRect.Pitch;
			}

			srcTex->GetTexture9()->UnlockRect(0);
		}
		else
		{
			assert(false);
		}
		destTex->GetTexture9()->UnlockRect(0);
	}
	else
	{
		assert(false);
	}

	//destTex->m_tex9->GenerateMipSubLevels();
	return DD_OK;

}
