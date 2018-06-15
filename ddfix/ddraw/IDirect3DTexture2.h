#pragma once

namespace ND3D9
{
	struct IDirect3DTexture9;
	using Resource9Handle = int;
}

class m_IDirect3DTexture2 : public IDirect3DTexture2, public AddressLookupTableObject
{
private:
	IDirect3DTexture2 *ProxyInterface;
	ULONG Refs;
	m_IDirectDrawSurface4* m_surface;
	ND3D9::Resource9Handle m_tex9Handle;

public:
	m_IDirect3DTexture2(IDirect3DTexture2 *aOriginal, m_IDirectDrawSurface4 *temp);
	~m_IDirect3DTexture2();

	IDirect3DTexture2 *GetProxyInterface() { return ProxyInterface; }
	SmartPtr<ND3D9::IDirect3DTexture9> GetTexture9() const;

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDirect3DTexture2 methods ***/
	STDMETHOD(GetHandle)(THIS_ LPDIRECT3DDEVICE2, LPD3DTEXTUREHANDLE);
	STDMETHOD(PaletteChanged)(THIS_ DWORD, DWORD);
	STDMETHOD(Load)(THIS_ LPDIRECT3DTEXTURE2);
};
