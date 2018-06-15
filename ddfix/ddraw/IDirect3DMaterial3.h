#pragma once

namespace ND3D9
{
	typedef struct _D3DMATERIAL9 D3DMATERIAL9;
}

class m_IDirect3DMaterial3 : public IDirect3DMaterial3, public AddressLookupTableObject
{
private:
	IDirect3DMaterial3 *ProxyInterface;
	ULONG Refs;
	D3DMATERIAL m_matDef;
public:
	m_IDirect3DMaterial3(IDirect3DMaterial3 *aOriginal, void *temp) 
		: ProxyInterface(aOriginal)
		, Refs(1)
		, m_matDef({0})
	{
		ProxyAddressLookupTable.SaveAddress(this, ProxyInterface);
	}
	~m_IDirect3DMaterial3()
	{
		ProxyAddressLookupTable.DeleteAddress(this);
	}

	IDirect3DMaterial3 *GetProxyInterface() { return ProxyInterface; }
	void GetMaterial9(ND3D9::D3DMATERIAL9* mat9) const;

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDirect3DMaterial3 methods ***/
	STDMETHOD(SetMaterial)(THIS_ LPD3DMATERIAL);
	STDMETHOD(GetMaterial)(THIS_ LPD3DMATERIAL);
	STDMETHOD(GetHandle)(THIS_ LPDIRECT3DDEVICE3, LPD3DMATERIALHANDLE);
};
