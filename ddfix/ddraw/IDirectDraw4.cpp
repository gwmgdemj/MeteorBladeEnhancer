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

m_IDirectDraw4::m_IDirectDraw4(IDirectDraw4 *aOriginal, std::shared_ptr<WrapperLookupTable<void>> wrapperAddressLookupTable) 
	: ProxyInterface(aOriginal)
	, Refs(1)
	, WrapperAddressLookupTable(wrapperAddressLookupTable)
	, m_displayWidth(0)
	, m_displayHeight(0)
	, m_displayRefreshRate(0)
{
	Tex9LookupTable = new AddressLookupTable<m_IDirectDraw4>(this, false);
	ProxyAddressLookupTable.SaveAddress(this, ProxyInterface);
	WrapperAddressLookupTable->SaveWrapper(this, IID_IDirectDraw4);
	ND3D9::D3D9Context::Instance()->Initialize(GetActiveWindow());
}


m_IDirectDraw4::~m_IDirectDraw4()
{
	delete Tex9LookupTable;
	Tex9LookupTable = nullptr;
	ProxyAddressLookupTable.DeleteAddress(this);
	WrapperAddressLookupTable->DeleteWrapper(IID_IDirectDraw4);
	ND3D9::D3D9Context::Instance()->Uninitialize();
}

HRESULT m_IDirectDraw4::QueryInterface(REFIID riid, LPVOID FAR * ppvObj)
{
	if ((riid == IID_IDirectDraw4 || riid == IID_IUnknown) && ppvObj)
	{
		AddRef();

		*ppvObj = this;

		return S_OK;
	}

	if (riid == IID_IDirect3D3)
	{
		*ppvObj = WrapperAddressLookupTable->FindWrapper<m_IDirect3D3>(IID_IDirect3D3);
	}
	else
	{
		assert(false);
	}
	
	return S_OK;
}

ULONG m_IDirectDraw4::AddRef()
{
	return ++Refs;
	return ProxyInterface->AddRef();
}

ULONG m_IDirectDraw4::Release()
{
	//ULONG x = ProxyInterface->Release();
	ULONG x = --Refs;

	if (x == 0)
	{
		delete this;
	}

	return x;
}

HRESULT m_IDirectDraw4::Compact()
{
	return ProxyInterface->Compact();
}

HRESULT m_IDirectDraw4::CreateClipper(DWORD a, LPDIRECTDRAWCLIPPER FAR * b, IUnknown FAR * c)
{
	*b = new m_IDirectDrawClipper(0, 0);
	return S_OK;
}

HRESULT m_IDirectDraw4::CreatePalette(DWORD a, LPPALETTEENTRY b, LPDIRECTDRAWPALETTE FAR * c, IUnknown FAR * d)
{
	HRESULT hr = ProxyInterface->CreatePalette(a, b, c, d);

	if (SUCCEEDED(hr))
	{
		*c = ProxyAddressLookupTable.FindAddress<m_IDirectDrawPalette>(*c);
	}

	return hr;
}

HRESULT m_IDirectDraw4::CreateSurface(LPDDSURFACEDESC2 a, LPDIRECTDRAWSURFACE4 FAR * b, IUnknown FAR * c)
{
	*b = new m_IDirectDrawSurface4(nullptr, *a, nullptr, WrapperAddressLookupTable);
	return DD_OK;
}

HRESULT m_IDirectDraw4::DuplicateSurface(LPDIRECTDRAWSURFACE4 a, LPDIRECTDRAWSURFACE4 FAR * b)
{
	// make compile pass
// 	if (a)
// 	{
// 		a = static_cast<m_IDirectDrawSurface4 *>(a)->GetProxyInterface();
// 	}
// 
// 	HRESULT hr = ProxyInterface->DuplicateSurface(a, b);
// 
// 	if (SUCCEEDED(hr))
// 	{
// 		*b = ProxyAddressLookupTable.FindAddress<m_IDirectDrawSurface4>(*b);
// 	}
// 
// 	return hr;
	return DDERR_GENERIC;
}

void DiscardDuplicateModes(DEVMODE **array, DWORD *count)
{
	DEVMODE *newarray = (DEVMODE *)malloc(sizeof(DEVMODE)*(*count));
	if (!newarray) return;
	DWORD newcount = 0;
	bool match;
	for (DWORD x = 0; x < (*count); x++)
	{
		match = false;
		memcpy(&newarray[newcount], &(*array)[x], sizeof(DEVMODE));
		for (int y = newcount; y > 0; y--)
		{
			if ((*array)[x].dmBitsPerPel == newarray[y - 1].dmBitsPerPel &&
				(*array)[x].dmDisplayFrequency == newarray[y - 1].dmDisplayFrequency &&
				(*array)[x].dmPelsWidth == newarray[y - 1].dmPelsWidth &&
				(*array)[x].dmPelsHeight == newarray[y - 1].dmPelsHeight)
			{
				match = true;
				break;
			}
		}
		if (!match) newcount++;
	}
	DEVMODE *newarray2 = (DEVMODE*)realloc(newarray, sizeof(DEVMODE)*newcount);
	if (newarray2) newarray = newarray2;
	free(*array);
	*array = newarray;
	*count = newcount;
}

HRESULT m_IDirectDraw4::EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK2 lpEnumModesCallback)
{
	// code is taken from project DXGL
	if (!lpEnumModesCallback) return DDERR_INVALIDPARAMS;
	if (dwFlags & 0xFFFFFFFC) return DDERR_INVALIDPARAMS;
	BOOL match;
	BOOL scalemodes;
	DWORD modenum = 0;
	DWORD modemax = 128;
	DEVMODE mode;
	ZeroMemory(&mode, sizeof(DEVMODE));
	mode.dmSize = sizeof(DEVMODE);
	DEVMODE *modes = (DEVMODE*)malloc(128 * sizeof(DEVMODE));
	DEVMODE *tmp;
	if (!modes) return DDERR_OUTOFMEMORY;
	DDSURFACEDESC2 ddmode;
	ZeroMemory(&ddmode, sizeof(DDSURFACEDESC2));
	ddmode.dwSize = sizeof(DDSURFACEDESC2);
	ddmode.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH | DDSD_PIXELFORMAT | DDSD_REFRESHRATE;
	ddmode.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	scalemodes = FALSE;
	while (EnumDisplaySettings(NULL, modenum++, &mode))
	{
		
		modes[modenum - 1] = mode;
		if (modenum >= modemax)
		{
			modemax += 128;
			tmp = (DEVMODE*)realloc(modes, modemax * sizeof(DEVMODE));
			if (tmp == NULL)
			{
				free(modes);
				return DDERR_OUTOFMEMORY;
			}
			modes = tmp;
		}
	}
	DiscardDuplicateModes(&modes, &modenum);
	
	for (DWORD i = 0; i < modenum; i++)
	{
		match = TRUE;
		if (dwFlags & DDEDM_REFRESHRATES) ddmode.dwRefreshRate = modes[i].dmDisplayFrequency;
		else
		{
			ddmode.dwRefreshRate = 0;
			for (DWORD x = 0; x < i; x++)
				if ((modes[x].dmBitsPerPel == modes[i].dmBitsPerPel) &&
					(modes[x].dmPelsWidth == modes[i].dmPelsWidth) &&
					(modes[x].dmPelsHeight == modes[i].dmPelsHeight)) match = FALSE;
		}
		if (lpDDSurfaceDesc)
		{
			if (lpDDSurfaceDesc->dwFlags & DDSD_WIDTH)
				if (lpDDSurfaceDesc->dwWidth != modes[i].dmPelsWidth) match = FALSE;
			if (lpDDSurfaceDesc->dwFlags & DDSD_HEIGHT)
				if (lpDDSurfaceDesc->dwHeight != modes[i].dmPelsHeight) match = FALSE;
			if (lpDDSurfaceDesc->dwFlags & DDSD_PIXELFORMAT)
				if (lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount != modes[i].dmBitsPerPel) match = FALSE;
			if (lpDDSurfaceDesc->dwFlags & DDSD_REFRESHRATE)
				if (lpDDSurfaceDesc->dwRefreshRate != modes[i].dmDisplayFrequency) match = FALSE;
		}
		if (match)
		{
			if (modes[i].dmBitsPerPel == 8) ddmode.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
			else if (modes[i].dmBitsPerPel == 4) ddmode.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED4;
			else ddmode.ddpfPixelFormat.dwFlags = DDPF_RGB;
			if (modes[i].dmBitsPerPel == 8)
			{
				ddmode.ddpfPixelFormat.dwRBitMask = 0;
				ddmode.ddpfPixelFormat.dwGBitMask = 0;
				ddmode.ddpfPixelFormat.dwBBitMask = 0;
			}
			else if (modes[i].dmBitsPerPel == 15)
			{
				ddmode.ddpfPixelFormat.dwRBitMask = 0x7C00;
				ddmode.ddpfPixelFormat.dwGBitMask = 0x3E0;
				ddmode.ddpfPixelFormat.dwRBitMask = 0x1F;
			}
			else if (modes[i].dmBitsPerPel == 16)
			{
				ddmode.ddpfPixelFormat.dwRBitMask = 0xF800;
				ddmode.ddpfPixelFormat.dwGBitMask = 0x7E0;
				ddmode.ddpfPixelFormat.dwBBitMask = 0x1F;
			}
			else
			{
				ddmode.ddpfPixelFormat.dwRBitMask = 0xFF0000;
				ddmode.ddpfPixelFormat.dwGBitMask = 0xFF00;
				ddmode.ddpfPixelFormat.dwBBitMask = 0xFF;
			}
			ddmode.ddpfPixelFormat.dwRGBBitCount = modes[i].dmBitsPerPel;
			ddmode.dwWidth = modes[i].dmPelsWidth;
			ddmode.dwHeight = modes[i].dmPelsHeight;
			if (modes[i].dmBitsPerPel == 15) ddmode.lPitch = modes[i].dmPelsWidth * 2;
			else if (modes[i].dmBitsPerPel == 4) ddmode.lPitch = modes[i].dmPelsWidth / 2;
			else ddmode.lPitch = modes[i].dmPelsWidth * (modes[i].dmBitsPerPel / 8);
			if (lpEnumModesCallback(&ddmode, lpContext) == DDENUMRET_CANCEL) return DD_OK;
		}
	}
	free(modes);
	return DD_OK;
}

HRESULT m_IDirectDraw4::EnumSurfaces(DWORD a, LPDDSURFACEDESC2 b, LPVOID c, LPDDENUMSURFACESCALLBACK2 d)
{
	ENUMSURFACE2 CallbackContext;
	CallbackContext.lpContext = c;
	CallbackContext.lpCallback = d;

	return ProxyInterface->EnumSurfaces(a, b, &CallbackContext, m_IDirectDrawEnumSurface2::EnumSurface2Callback);
}

HRESULT m_IDirectDraw4::FlipToGDISurface()
{
	return DD_OK;
	//return ProxyInterface->FlipToGDISurface();
}

HRESULT m_IDirectDraw4::GetCaps(LPDDCAPS a, LPDDCAPS b)
{
	m_IDirectDraw* ddraw = WrapperAddressLookupTable->FindWrapper<m_IDirectDraw>(IID_IDirectDraw);
	HRESULT hr = ddraw->GetCaps(a, b);
	return hr;
	return ProxyInterface->GetCaps(a, b);
}

HRESULT m_IDirectDraw4::GetDisplayMode(LPDDSURFACEDESC2 lpDDSurfaceDesc2)
{
	// code is taken from project DXGL

	if (!lpDDSurfaceDesc2)
		return DDERR_INVALIDPARAMS;
	DDSURFACEDESC2 ddsdMode;
	ZeroMemory(&ddsdMode, sizeof(DDSURFACEDESC2));
	ddsdMode.dwSize = sizeof(DDSURFACEDESC2);
	ddsdMode.dwFlags = DDSD_REFRESHRATE | DDSD_PIXELFORMAT | DDSD_PITCH | DDSD_WIDTH | DDSD_HEIGHT;
	ddsdMode.ddpfPixelFormat.dwFlags = DDPF_RGB;
	DEVMODE currmode;
	ZeroMemory(&currmode, sizeof(DEVMODE));
	currmode.dmSize = sizeof(DEVMODE);
	bool fullscreen = false;
	int primarybpp = 32;
	if (fullscreen)
	{
		if (primarybpp == 8)
		{
			ddsdMode.ddpfPixelFormat.dwRBitMask = 0;
			ddsdMode.ddpfPixelFormat.dwGBitMask = 0;
			ddsdMode.ddpfPixelFormat.dwBBitMask = 0;
			ddsdMode.ddpfPixelFormat.dwFlags |= DDPF_PALETTEINDEXED8;
		}
		else if (primarybpp == 15)
		{
			ddsdMode.ddpfPixelFormat.dwRBitMask = 0x7C00;
			ddsdMode.ddpfPixelFormat.dwGBitMask = 0x3E0;
			ddsdMode.ddpfPixelFormat.dwRBitMask = 0x1F;
		}
		else if (primarybpp == 16)
		{
			ddsdMode.ddpfPixelFormat.dwRBitMask = 0xF800;
			ddsdMode.ddpfPixelFormat.dwGBitMask = 0x7E0;
			ddsdMode.ddpfPixelFormat.dwBBitMask = 0x1F;
		}
		else
		{
			ddsdMode.ddpfPixelFormat.dwRBitMask = 0xFF0000;
			ddsdMode.ddpfPixelFormat.dwGBitMask = 0xFF00;
			ddsdMode.ddpfPixelFormat.dwBBitMask = 0xFF;
		}
		ddsdMode.ddpfPixelFormat.dwRGBBitCount = 32;
		ddsdMode.dwWidth = m_displayWidth;
		ddsdMode.dwHeight = m_displayHeight;
		ddsdMode.dwRefreshRate = m_displayRefreshRate;
		//if (primarybpp == 15) ddsdMode.lPitch = NextMultipleOf4(primaryx * 2);
		//else if (primarybpp == 4) ddsdMode.lPitch = NextMultipleOf4(primaryx / 2);
		//else ddsdMode.lPitch = NextMultipleOf4(primaryx * (primarybpp / 8));
		if (lpDDSurfaceDesc2->dwSize < sizeof(DDSURFACEDESC)) return DDERR_INVALIDPARAMS;
		if (lpDDSurfaceDesc2->dwSize > sizeof(DDSURFACEDESC2))
			lpDDSurfaceDesc2->dwSize = sizeof(DDSURFACEDESC2);
		memcpy(lpDDSurfaceDesc2, &ddsdMode, lpDDSurfaceDesc2->dwSize);
		
		return DD_OK;
	}
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &currmode);
	if (currmode.dmBitsPerPel == 8) ddsdMode.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
	else if (currmode.dmBitsPerPel == 4) ddsdMode.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED4;
	else ddsdMode.ddpfPixelFormat.dwFlags = DDPF_RGB;
	if (currmode.dmBitsPerPel == 8)
	{
		ddsdMode.ddpfPixelFormat.dwRBitMask = 0;
		ddsdMode.ddpfPixelFormat.dwGBitMask = 0;
		ddsdMode.ddpfPixelFormat.dwBBitMask = 0;
	}
	else if (currmode.dmBitsPerPel == 15)
	{
		ddsdMode.ddpfPixelFormat.dwRBitMask = 0x7C00;
		ddsdMode.ddpfPixelFormat.dwGBitMask = 0x3E0;
		ddsdMode.ddpfPixelFormat.dwRBitMask = 0x1F;
	}
	else if (currmode.dmBitsPerPel == 16)
	{
		ddsdMode.ddpfPixelFormat.dwRBitMask = 0xF800;
		ddsdMode.ddpfPixelFormat.dwGBitMask = 0x7E0;
		ddsdMode.ddpfPixelFormat.dwBBitMask = 0x1F;
	}
	else
	{
		ddsdMode.ddpfPixelFormat.dwRBitMask = 0xFF0000;
		ddsdMode.ddpfPixelFormat.dwGBitMask = 0xFF00;
		ddsdMode.ddpfPixelFormat.dwBBitMask = 0xFF;
	}
	ddsdMode.ddpfPixelFormat.dwRGBBitCount = currmode.dmBitsPerPel;
	ddsdMode.dwWidth = currmode.dmPelsWidth;
	ddsdMode.dwHeight = currmode.dmPelsHeight;
	if (currmode.dmBitsPerPel == 15) ddsdMode.lPitch = currmode.dmPelsWidth * 2;
	else if (currmode.dmBitsPerPel == 4) ddsdMode.lPitch = currmode.dmPelsWidth / 2;
	else ddsdMode.lPitch = currmode.dmPelsWidth * (currmode.dmBitsPerPel / 8);
	if (lpDDSurfaceDesc2->dwSize < sizeof(DDSURFACEDESC)) return DDERR_INVALIDPARAMS;
	if (lpDDSurfaceDesc2->dwSize > sizeof(DDSURFACEDESC2))
		lpDDSurfaceDesc2->dwSize = sizeof(DDSURFACEDESC2);
	memcpy(lpDDSurfaceDesc2, &ddsdMode, lpDDSurfaceDesc2->dwSize);
	return DD_OK;
}

HRESULT m_IDirectDraw4::GetFourCCCodes(LPDWORD a, LPDWORD b)
{
	return ProxyInterface->GetFourCCCodes(a, b);
}

HRESULT m_IDirectDraw4::GetGDISurface(LPDIRECTDRAWSURFACE4 FAR * a)
{
	// make compile pass
// 	HRESULT hr = ProxyInterface->GetGDISurface(a);
// 
// 	if (SUCCEEDED(hr))
// 	{
// 		*a = ProxyAddressLookupTable.FindAddress<m_IDirectDrawSurface4>(*a);
// 	}
// 
// 	return hr;
	return DDERR_GENERIC;
}

HRESULT m_IDirectDraw4::GetMonitorFrequency(LPDWORD a)
{
	return ProxyInterface->GetMonitorFrequency(a);
}

HRESULT m_IDirectDraw4::GetScanLine(LPDWORD a)
{
	return ProxyInterface->GetScanLine(a);
}

HRESULT m_IDirectDraw4::GetVerticalBlankStatus(LPBOOL a)
{
	return ProxyInterface->GetVerticalBlankStatus(a);
}

HRESULT m_IDirectDraw4::Initialize(GUID FAR * a)
{
	return ProxyInterface->Initialize(a);
}

HRESULT m_IDirectDraw4::RestoreDisplayMode()
{
	return DD_OK;
	//return ProxyInterface->RestoreDisplayMode();
}

HRESULT m_IDirectDraw4::SetCooperativeLevel(HWND a, DWORD b)
{
	return DD_OK;
	return ProxyInterface->SetCooperativeLevel(a, b);
}

HRESULT m_IDirectDraw4::SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate, DWORD dwFlags)
{
	m_displayWidth = dwWidth;
	m_displayHeight = dwHeight;
	m_displayRefreshRate = dwRefreshRate;
	return DD_OK;
}

HRESULT m_IDirectDraw4::WaitForVerticalBlank(DWORD a, HANDLE b)
{
	return ProxyInterface->WaitForVerticalBlank(a, b);
}

HRESULT m_IDirectDraw4::GetAvailableVidMem(LPDDSCAPS2 a, LPDWORD b, LPDWORD c)
{
	return ProxyInterface->GetAvailableVidMem(a, b, c);
}

HRESULT m_IDirectDraw4::GetSurfaceFromDC(HDC a, LPDIRECTDRAWSURFACE4 * b)
{
	// make compile pass
// 	HRESULT hr = ProxyInterface->GetSurfaceFromDC(a, b);
// 
// 	if (SUCCEEDED(hr))
// 	{
// 		*b = ProxyAddressLookupTable.FindAddress<m_IDirectDrawSurface4>(*b);
// 	}
// 
// 	return hr;
	return DD_OK;
}

HRESULT m_IDirectDraw4::RestoreAllSurfaces()
{
	return ProxyInterface->RestoreAllSurfaces();
}

HRESULT m_IDirectDraw4::TestCooperativeLevel()
{
	return ProxyInterface->TestCooperativeLevel();
}

HRESULT m_IDirectDraw4::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER a, DWORD b)
{
	return ProxyInterface->GetDeviceIdentifier(a, b);
}
