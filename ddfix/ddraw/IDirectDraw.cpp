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
#include <assert.h>
#include "../D3D9Context.h"

// code is taken from project DXGL
const D3DDEVICEDESC7 d3ddesc_default =
{
	D3DDEVCAPS_CANBLTSYSTONONLOCAL | D3DDEVCAPS_CANRENDERAFTERFLIP | D3DDEVCAPS_DRAWPRIMTLVERTEX |
	D3DDEVCAPS_FLOATTLVERTEX | D3DDEVCAPS_TEXTURENONLOCALVIDMEM | D3DDEVCAPS_TEXTURESYSTEMMEMORY |
	D3DDEVCAPS_TEXTUREVIDEOMEMORY | D3DDEVCAPS_TLVERTEXSYSTEMMEMORY | D3DDEVCAPS_TLVERTEXVIDEOMEMORY |
	D3DDEVCAPS_EXECUTESYSTEMMEMORY | D3DDEVCAPS_EXECUTEVIDEOMEMORY, // dwDevCaps
{ //dpcLineCaps
	sizeof(D3DPRIMCAPS),
	D3DPMISCCAPS_CULLCCW | D3DPMISCCAPS_CULLCW | D3DPMISCCAPS_CULLNONE, // dwMiscCaps
	D3DPRASTERCAPS_SUBPIXEL | D3DPRASTERCAPS_ZTEST | D3DPRASTERCAPS_FOGRANGE | D3DPRASTERCAPS_FOGTABLE |
	D3DPRASTERCAPS_FOGVERTEX | D3DPRASTERCAPS_WFOG, //dwRasterCaps
	D3DPCMPCAPS_ALWAYS | D3DPCMPCAPS_EQUAL | D3DPCMPCAPS_GREATER | D3DPCMPCAPS_GREATEREQUAL |
	D3DPCMPCAPS_LESS | D3DPCMPCAPS_LESSEQUAL | D3DPCMPCAPS_NEVER | D3DPCMPCAPS_NOTEQUAL, //dwZCmpCaps
	D3DPBLENDCAPS_ZERO | D3DPBLENDCAPS_ONE | D3DPBLENDCAPS_SRCCOLOR | D3DPBLENDCAPS_INVSRCCOLOR |
	D3DPBLENDCAPS_SRCALPHA | D3DPBLENDCAPS_INVSRCALPHA | D3DPBLENDCAPS_DESTALPHA |
	D3DPBLENDCAPS_INVDESTALPHA | D3DPBLENDCAPS_DESTCOLOR | D3DPBLENDCAPS_INVDESTCOLOR |
	D3DPBLENDCAPS_SRCALPHASAT | D3DPBLENDCAPS_BOTHSRCALPHA | D3DPBLENDCAPS_BOTHINVSRCALPHA, //dwSrcBlendCaps
	D3DPBLENDCAPS_ZERO | D3DPBLENDCAPS_ONE | D3DPBLENDCAPS_SRCCOLOR | D3DPBLENDCAPS_INVSRCCOLOR |
	D3DPBLENDCAPS_SRCALPHA | D3DPBLENDCAPS_INVSRCALPHA | D3DPBLENDCAPS_DESTALPHA |
	D3DPBLENDCAPS_INVDESTALPHA | D3DPBLENDCAPS_DESTCOLOR | D3DPBLENDCAPS_INVDESTCOLOR |
	D3DPBLENDCAPS_SRCALPHASAT | D3DPBLENDCAPS_BOTHSRCALPHA | D3DPBLENDCAPS_BOTHINVSRCALPHA, //dwDestBlendCaps
	D3DPCMPCAPS_ALWAYS | D3DPCMPCAPS_EQUAL | D3DPCMPCAPS_GREATER | D3DPCMPCAPS_GREATEREQUAL |
	D3DPCMPCAPS_LESS | D3DPCMPCAPS_LESSEQUAL | D3DPCMPCAPS_NEVER | D3DPCMPCAPS_NOTEQUAL, //dwAlphaCmpCaps
	D3DPSHADECAPS_ALPHAFLATBLEND | D3DPSHADECAPS_ALPHAGOURAUDBLEND | D3DPSHADECAPS_COLORFLATRGB |
	D3DPSHADECAPS_COLORGOURAUDRGB | D3DPSHADECAPS_FOGFLAT | D3DPSHADECAPS_FOGGOURAUD, //dwShadeCaps
	D3DPTEXTURECAPS_ALPHA | D3DPTEXTURECAPS_PERSPECTIVE | D3DPTEXTURECAPS_TRANSPARENCY, //dwTextureCaps
	D3DPTFILTERCAPS_NEAREST | D3DPTFILTERCAPS_LINEAR | D3DPTFILTERCAPS_MIPNEAREST |
	D3DPTFILTERCAPS_MIPLINEAR | D3DPTFILTERCAPS_LINEARMIPNEAREST | D3DPTFILTERCAPS_LINEARMIPLINEAR |
	D3DPTFILTERCAPS_MAGFLINEAR | D3DPTFILTERCAPS_MAGFPOINT | D3DPTFILTERCAPS_MINFLINEAR |
	D3DPTFILTERCAPS_MINFPOINT, //dwTextureFilterCaps
	D3DPTBLENDCAPS_ADD | D3DPTBLENDCAPS_COPY | D3DPTBLENDCAPS_DECAL | D3DPTBLENDCAPS_DECALALPHA |
	D3DPTBLENDCAPS_MODULATE | D3DPTBLENDCAPS_MODULATEALPHA, //dwTextureBlendCaps
	D3DPTADDRESSCAPS_CLAMP | D3DPTADDRESSCAPS_INDEPENDENTUV | D3DPTADDRESSCAPS_MIRROR |
	D3DPTADDRESSCAPS_WRAP, //dwTextureAddressCaps
	0, //dwStippleWidth
	0  //dwStippleHeight
},
	{ //dpcTriCaps
		sizeof(D3DPRIMCAPS),
		D3DPMISCCAPS_CULLCCW | D3DPMISCCAPS_CULLCW | D3DPMISCCAPS_CULLNONE, // dwMiscCaps
		D3DPRASTERCAPS_SUBPIXEL | D3DPRASTERCAPS_ZTEST | D3DPRASTERCAPS_FOGRANGE | D3DPRASTERCAPS_FOGTABLE |
		D3DPRASTERCAPS_FOGVERTEX | D3DPRASTERCAPS_WFOG, //dwRasterCaps
	D3DPCMPCAPS_ALWAYS | D3DPCMPCAPS_LESSEQUAL, //dwZCmpCaps
	D3DPBLENDCAPS_ZERO | D3DPBLENDCAPS_ONE | D3DPBLENDCAPS_SRCCOLOR | D3DPBLENDCAPS_INVSRCCOLOR |
	D3DPBLENDCAPS_SRCALPHA | D3DPBLENDCAPS_INVSRCALPHA | D3DPBLENDCAPS_DESTALPHA |
	D3DPBLENDCAPS_INVDESTALPHA | D3DPBLENDCAPS_DESTCOLOR | D3DPBLENDCAPS_INVDESTCOLOR |
	D3DPBLENDCAPS_SRCALPHASAT | D3DPBLENDCAPS_BOTHSRCALPHA | D3DPBLENDCAPS_BOTHINVSRCALPHA, //dwSrcBlendCaps
	D3DPBLENDCAPS_ZERO | D3DPBLENDCAPS_ONE | D3DPBLENDCAPS_SRCCOLOR | D3DPBLENDCAPS_INVSRCCOLOR |
	D3DPBLENDCAPS_SRCALPHA | D3DPBLENDCAPS_INVSRCALPHA | D3DPBLENDCAPS_DESTALPHA |
	D3DPBLENDCAPS_INVDESTALPHA | D3DPBLENDCAPS_DESTCOLOR | D3DPBLENDCAPS_INVDESTCOLOR |
	D3DPBLENDCAPS_SRCALPHASAT | D3DPBLENDCAPS_BOTHSRCALPHA | D3DPBLENDCAPS_BOTHINVSRCALPHA, //dwDestBlendCaps
	D3DPCMPCAPS_ALWAYS | D3DPCMPCAPS_EQUAL | D3DPCMPCAPS_GREATER | D3DPCMPCAPS_GREATEREQUAL |
	D3DPCMPCAPS_LESS | D3DPCMPCAPS_LESSEQUAL | D3DPCMPCAPS_NEVER | D3DPCMPCAPS_NOTEQUAL, //dwAlphaCmpCaps
	D3DPSHADECAPS_ALPHAFLATBLEND | D3DPSHADECAPS_ALPHAGOURAUDBLEND | D3DPSHADECAPS_COLORFLATRGB |
	D3DPSHADECAPS_COLORGOURAUDRGB | D3DPSHADECAPS_FOGFLAT | D3DPSHADECAPS_FOGGOURAUD, //dwShadeCaps
	D3DPTEXTURECAPS_ALPHA | D3DPTEXTURECAPS_PERSPECTIVE | D3DPTEXTURECAPS_TRANSPARENCY, //dwTextureCaps
	D3DPTFILTERCAPS_NEAREST | D3DPTFILTERCAPS_LINEAR | D3DPTFILTERCAPS_MIPNEAREST |
	D3DPTFILTERCAPS_MIPLINEAR | D3DPTFILTERCAPS_LINEARMIPNEAREST | D3DPTFILTERCAPS_LINEARMIPLINEAR |
	D3DPTFILTERCAPS_MAGFLINEAR | D3DPTFILTERCAPS_MAGFPOINT | D3DPTFILTERCAPS_MINFLINEAR |
	D3DPTFILTERCAPS_MINFPOINT, //dwTextureFilterCaps
	D3DPTBLENDCAPS_ADD | D3DPTBLENDCAPS_COPY | D3DPTBLENDCAPS_DECAL | D3DPTBLENDCAPS_DECALALPHA |
	D3DPTBLENDCAPS_MODULATE | D3DPTBLENDCAPS_MODULATEALPHA, //dwTextureBlendCaps
	D3DPTADDRESSCAPS_CLAMP | D3DPTADDRESSCAPS_INDEPENDENTUV | D3DPTADDRESSCAPS_MIRROR |
	D3DPTADDRESSCAPS_WRAP, //dwTextureAddressCaps
	0, //dwStippleWidth
	0  //dwStippleHeight
	},
	DDBD_16 | DDBD_24 | DDBD_32, //dwDeviceRenderBitDepth 
	DDBD_16 | DDBD_24 | DDBD_32, //dwDeviceZBufferBitDepth
	1, //dwMinTextureWidth
	1, //dwMinTextureHeight
	2560, //dwMaxTextureWidth
	2560, //dwMaxTextureHeight
	0, //dwMaxTextureRepeat
	0, //dwMaxTextureAspectRatio
	1, //dwMaxAnisotropy
	0.0f, //dvGuardBandLeft
	0.0f, //dvGuardBandTop
	0.0f, //dvGuardBandRight
	0.0f, //dvGuardBandBottom
	0.0f, //dvExtentsAdjust 
	0, //dwStencilCaps
	8, //dwFVFCaps 
	D3DTEXOPCAPS_DISABLE | D3DTEXOPCAPS_SELECTARG1 | D3DTEXOPCAPS_SELECTARG2 | D3DTEXOPCAPS_MODULATE |
	D3DTEXOPCAPS_MODULATE2X | D3DTEXOPCAPS_MODULATE4X | D3DTEXOPCAPS_ADD |
	D3DTEXOPCAPS_ADDSIGNED | D3DTEXOPCAPS_ADDSIGNED2X | D3DTEXOPCAPS_SUBTRACT |
	D3DTEXOPCAPS_ADDSMOOTH | D3DTEXOPCAPS_BLENDDIFFUSEALPHA | D3DTEXOPCAPS_BLENDTEXTUREALPHA |
	D3DTEXOPCAPS_BLENDTEXTUREALPHAPM | D3DTEXOPCAPS_BLENDCURRENTALPHA, //dwTextureOpCaps
	8, //wMaxTextureBlendStages
	8, //wMaxSimultaneousTextures
	8, //dwMaxActiveLights
	0.0f, //dvMaxVertexW 
	IID_IDirect3DHALDevice, //deviceGUID
	0, //wMaxUserClipPlanes
	0, //wMaxVertexBlendMatrices
	D3DVTXPCAPS_DIRECTIONALLIGHTS | D3DVTXPCAPS_POSITIONALLIGHTS | D3DVTXPCAPS_VERTEXFOG, //dwVertexProcessingCaps 
	0,0,0,0 //dwReserved1 through dwReserved4
};

const D3DDEVICEDESC d3ddesc3_default =
{
	sizeof(D3DDEVICEDESC), // dwSize
	D3DDD_BCLIPPING | D3DDD_COLORMODEL | D3DDD_DEVCAPS | D3DDD_DEVICERENDERBITDEPTH |
	D3DDD_DEVICEZBUFFERBITDEPTH | D3DDD_LIGHTINGCAPS | D3DDD_LINECAPS | D3DDD_MAXBUFFERSIZE |
	D3DDD_MAXVERTEXCOUNT | D3DDD_TRANSFORMCAPS | D3DDD_TRICAPS, // dwFlags
	D3DCOLOR_RGB, // dcmColorModel
	d3ddesc_default.dwDevCaps,
{ //dtcTransformCaps
	sizeof(D3DTRANSFORMCAPS), //dwSize
	0 // dwCaps
},
FALSE, //bClipping
{ // dlcLightingCaps
	sizeof(D3DLIGHTINGCAPS), //dwSize
	D3DLIGHTCAPS_DIRECTIONAL | D3DLIGHTCAPS_POINT | D3DLIGHTCAPS_SPOT, // dwCaps
	D3DLIGHTINGMODEL_RGB, // dwLightingModel
	8  //dwNumLights
},
d3ddesc_default.dpcLineCaps,
d3ddesc_default.dpcTriCaps,
d3ddesc_default.dwDeviceRenderBitDepth,
d3ddesc_default.dwDeviceZBufferBitDepth,
0, // dwMaxBufferSize
65536, // dwMaxVertexCount
d3ddesc_default.dwMinTextureWidth,
d3ddesc_default.dwMinTextureHeight,
d3ddesc_default.dwMaxTextureWidth,
d3ddesc_default.dwMaxTextureHeight,
0, // dwMinStippleWidth
32, // dwMaxStippleWidth
0, // dwMinStippleHeight
32, // dwMaxStippleHeight
d3ddesc_default.dwMaxTextureRepeat,
d3ddesc_default.dwMaxTextureAspectRatio,
d3ddesc_default.dwMaxAnisotropy,
d3ddesc_default.dvGuardBandLeft,
d3ddesc_default.dvGuardBandTop,
d3ddesc_default.dvGuardBandRight,
d3ddesc_default.dvGuardBandBottom,
d3ddesc_default.dvExtentsAdjust,
d3ddesc_default.dwStencilCaps,
d3ddesc_default.dwFVFCaps,
d3ddesc_default.dwTextureOpCaps,
d3ddesc_default.wMaxTextureBlendStages,
d3ddesc_default.wMaxSimultaneousTextures
};


m_IDirectDraw::m_IDirectDraw(IDirectDraw *aOriginal, std::shared_ptr<WrapperLookupTable<void>> wrapperAddressLookupTable) 
	: ProxyInterface(aOriginal)
	, Refs(1)
	, WrapperAddressLookupTable(wrapperAddressLookupTable)
	, m_d3dDesc(d3ddesc3_default)
{
	ProxyAddressLookupTable.SaveAddress(this, ProxyInterface);
	WrapperAddressLookupTable->SaveWrapper(this, IID_IDirectDraw);
}


m_IDirectDraw::~m_IDirectDraw()
{
	ProxyAddressLookupTable.DeleteAddress(this);
	WrapperAddressLookupTable->DeleteWrapper(IID_IDirectDraw);	
}

HRESULT m_IDirectDraw::QueryInterface(REFIID riid, LPVOID FAR * ppvObj)
{
	if ((riid == IID_IDirectDraw || riid == CLSID_DirectDraw || riid == IID_IUnknown) && ppvObj)
	{
		AddRef();

		*ppvObj = this;

		return S_OK;
	}

	if (riid == IID_IDirectDraw4)
	{
		*ppvObj = WrapperAddressLookupTable->FindWrapper<m_IDirectDraw4>(IID_IDirectDraw4);
	}
	else if (riid == IID_IDirect3D3)
	{
		*ppvObj = WrapperAddressLookupTable->FindWrapper<m_IDirect3D3>(IID_IDirect3D3);
	}
	else
	{
		assert(false);
	}

	return S_OK;
}

ULONG m_IDirectDraw::AddRef()
{
	return ++Refs;
	return ProxyInterface->AddRef();
}

ULONG m_IDirectDraw::Release()
{
	//ULONG x = ProxyInterface->Release();
	ULONG x = --Refs;

	if (x == 0)
	{
		delete this;
	}

	return x;
}

HRESULT m_IDirectDraw::Compact()
{
	return ProxyInterface->Compact();
}

HRESULT m_IDirectDraw::CreateClipper(DWORD a, LPDIRECTDRAWCLIPPER FAR * b, IUnknown FAR * c)
{
	HRESULT hr = ProxyInterface->CreateClipper(a, b, c);

	if (SUCCEEDED(hr))
	{
		*b = ProxyAddressLookupTable.FindAddress<m_IDirectDrawClipper>(*b);
	}

	return hr;
}

HRESULT m_IDirectDraw::CreatePalette(DWORD a, LPPALETTEENTRY b, LPDIRECTDRAWPALETTE FAR * c, IUnknown FAR * d)
{
	HRESULT hr = ProxyInterface->CreatePalette(a, b, c, d);

	if (SUCCEEDED(hr))
	{
		*c = ProxyAddressLookupTable.FindAddress<m_IDirectDrawPalette>(*c);
	}

	return hr;
}

HRESULT m_IDirectDraw::CreateSurface(LPDDSURFACEDESC a, LPDIRECTDRAWSURFACE FAR * b, IUnknown FAR * c)
{
	HRESULT hr = ProxyInterface->CreateSurface(a, b, c);

	if (SUCCEEDED(hr))
	{
		*b = ProxyAddressLookupTable.FindAddress<m_IDirectDrawSurface>(*b);
	}

	return hr;
}

HRESULT m_IDirectDraw::DuplicateSurface(LPDIRECTDRAWSURFACE a, LPDIRECTDRAWSURFACE FAR * b)
{
	if (a)
	{
		a = static_cast<m_IDirectDrawSurface *>(a)->GetProxyInterface();
	}

	HRESULT hr = ProxyInterface->DuplicateSurface(a, b);

	if (SUCCEEDED(hr))
	{
		*b = ProxyAddressLookupTable.FindAddress<m_IDirectDrawSurface>(*b);
	}

	return hr;
}

HRESULT m_IDirectDraw::EnumDisplayModes(DWORD a, LPDDSURFACEDESC b, LPVOID c, LPDDENUMMODESCALLBACK d)
{
	return ProxyInterface->EnumDisplayModes(a, b, c, d);
}

HRESULT m_IDirectDraw::EnumSurfaces(DWORD a, LPDDSURFACEDESC b, LPVOID c, LPDDENUMSURFACESCALLBACK d)
{
	ENUMSURFACE CallbackContext;
	CallbackContext.lpContext = c;
	CallbackContext.lpCallback = d;

	return ProxyInterface->EnumSurfaces(a, b, &CallbackContext, m_IDirectDrawEnumSurface::EnumSurfaceCallback);
}

HRESULT m_IDirectDraw::FlipToGDISurface()
{
	return ProxyInterface->FlipToGDISurface();
}

HRESULT m_IDirectDraw::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps)
{
	// code is taken from project DXGL
	DDCAPS_DX7 ddCaps;
	ZeroMemory(&ddCaps, sizeof(DDCAPS_DX7));
	if (lpDDDriverCaps) 
		ddCaps.dwSize = lpDDDriverCaps->dwSize;
	else if (lpDDHELCaps) 
		ddCaps.dwSize = lpDDHELCaps->dwSize;
	else 
		return DDERR_INVALIDPARAMS;
	if (ddCaps.dwSize > sizeof(DDCAPS_DX7)) 
		ddCaps.dwSize = sizeof(DDCAPS_DX7);
	if (ddCaps.dwSize < sizeof(DDCAPS_DX3)) 
		return DDERR_INVALIDPARAMS;
	ddCaps.dwCaps = DDCAPS_BLT | DDCAPS_BLTCOLORFILL | DDCAPS_BLTDEPTHFILL | DDCAPS_BLTSTRETCH |
		DDCAPS_COLORKEY | DDCAPS_GDI | DDCAPS_PALETTE | DDCAPS_CANBLTSYSMEM |
		DDCAPS_3D | DDCAPS_CANCLIP | DDCAPS_CANCLIPSTRETCHED | DDCAPS_READSCANLINE;
	ddCaps.dwCaps2 = DDCAPS2_CANRENDERWINDOWED | DDCAPS2_WIDESURFACES | DDCAPS2_NOPAGELOCKREQUIRED |
		DDCAPS2_FLIPINTERVAL | DDCAPS2_FLIPNOVSYNC | DDCAPS2_NONLOCALVIDMEM;
	ddCaps.dwFXCaps = DDFXCAPS_BLTSHRINKX | DDFXCAPS_BLTSHRINKY |
		DDFXCAPS_BLTSTRETCHX | DDFXCAPS_BLTSTRETCHY | DDFXCAPS_BLTMIRRORLEFTRIGHT |
		DDFXCAPS_BLTMIRRORUPDOWN | DDFXCAPS_BLTROTATION90;
	ddCaps.dwPalCaps = DDPCAPS_8BIT | DDPCAPS_PRIMARYSURFACE | DDPCAPS_ALLOW256;
	ddCaps.ddsOldCaps.dwCaps = ddCaps.ddsCaps.dwCaps =
		DDSCAPS_BACKBUFFER | DDSCAPS_COMPLEX | DDSCAPS_FLIP |
		DDSCAPS_FRONTBUFFER | DDSCAPS_OFFSCREENPLAIN | DDSCAPS_PALETTE |
		DDSCAPS_SYSTEMMEMORY | DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE |
		DDSCAPS_NONLOCALVIDMEM | DDSCAPS_LOCALVIDMEM | DDSCAPS_TEXTURE |
		DDSCAPS_MIPMAP;
	ddCaps.ddsCaps.dwCaps2 = DDSCAPS2_MIPMAPSUBLEVEL;
	ddCaps.dwCKeyCaps = DDCKEYCAPS_SRCBLT | DDCKEYCAPS_DESTBLT;

	if (lpDDDriverCaps)
	{
		if (lpDDDriverCaps->dwSize > sizeof(DDCAPS_DX7)) lpDDDriverCaps->dwSize = sizeof(DDCAPS_DX7);
		memcpy(lpDDDriverCaps, &ddCaps, lpDDDriverCaps->dwSize);
	}
	if (lpDDHELCaps)
	{
		if (lpDDHELCaps->dwSize > sizeof(DDCAPS_DX7)) lpDDHELCaps->dwSize = sizeof(DDCAPS_DX7);
		memcpy(lpDDHELCaps, &ddCaps, lpDDHELCaps->dwSize);
	}

	return S_OK;
	return ProxyInterface->GetCaps(lpDDDriverCaps, lpDDHELCaps);
}

HRESULT m_IDirectDraw::GetDisplayMode(LPDDSURFACEDESC a)
{
	return ProxyInterface->GetDisplayMode(a);
}

HRESULT m_IDirectDraw::GetFourCCCodes(LPDWORD a, LPDWORD b)
{
	return ProxyInterface->GetFourCCCodes(a, b);
}

HRESULT m_IDirectDraw::GetGDISurface(LPDIRECTDRAWSURFACE FAR * a)
{
	HRESULT hr = ProxyInterface->GetGDISurface(a);

	if (SUCCEEDED(hr))
	{
		*a = ProxyAddressLookupTable.FindAddress<m_IDirectDrawSurface>(*a);
	}

	return hr;
}

HRESULT m_IDirectDraw::GetMonitorFrequency(LPDWORD a)
{
	return ProxyInterface->GetMonitorFrequency(a);
}

HRESULT m_IDirectDraw::GetScanLine(LPDWORD a)
{
	return ProxyInterface->GetScanLine(a);
}

HRESULT m_IDirectDraw::GetVerticalBlankStatus(LPBOOL a)
{
	return ProxyInterface->GetVerticalBlankStatus(a);
}

HRESULT m_IDirectDraw::Initialize(GUID FAR * a)
{
	return ProxyInterface->Initialize(a);
}

HRESULT m_IDirectDraw::RestoreDisplayMode()
{
	return ProxyInterface->RestoreDisplayMode();
}

HRESULT m_IDirectDraw::SetCooperativeLevel(HWND a, DWORD b)
{
	return ProxyInterface->SetCooperativeLevel(a, b);
}

HRESULT m_IDirectDraw::SetDisplayMode(DWORD a, DWORD b, DWORD c)
{
	return ProxyInterface->SetDisplayMode(a, b, c);
}

HRESULT m_IDirectDraw::WaitForVerticalBlank(DWORD a, HANDLE b)
{
	return ProxyInterface->WaitForVerticalBlank(a, b);
}
