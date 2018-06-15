#pragma once

namespace ND3D9
{
	typedef struct _D3DLIGHT9 D3DLIGHT9;
}

class m_IDirect3DLight : public IDirect3DLight, public AddressLookupTableObject
{
private:
	IDirect3DLight *ProxyInterface;
	ULONG Refs;
	ND3D9::D3DLIGHT9* m_light9;
public:
	m_IDirect3DLight(IDirect3DLight *aOriginal, void *temp) 
		: ProxyInterface(aOriginal)
		, Refs(1)
		, m_light9(nullptr)
	{
		ProxyAddressLookupTable.SaveAddress(this, ProxyInterface);
	}
	~m_IDirect3DLight();

	IDirect3DLight *GetProxyInterface() { return ProxyInterface; }
	ND3D9::D3DLIGHT9* GetLight9() const { return m_light9; }

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDirect3DLight methods ***/
	STDMETHOD(Initialize)(THIS_ LPDIRECT3D);
	STDMETHOD(SetLight)(THIS_ LPD3DLIGHT);
	STDMETHOD(GetLight)(THIS_ LPD3DLIGHT);
};
