#pragma once

namespace NDINPUT8
{
	struct IDirectInput8A;
}

class m_IDirectInput7A : public IDirectInput7A, public AddressLookupTableObject
{
private:
	NDINPUT8::IDirectInput8A *ProxyInterface;

public:
	m_IDirectInput7A(IDirectInput7A *aOriginal, void *temp) : ProxyInterface(reinterpret_cast<NDINPUT8::IDirectInput8A*>(aOriginal))
	{
		ProxyAddressLookupTable.SaveAddress(this, ProxyInterface);
	}
	~m_IDirectInput7A()
	{
		ProxyAddressLookupTable.DeleteAddress(this);
	}

	NDINPUT8::IDirectInput8A *GetProxyInterface() { return ProxyInterface; }

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDirectInput2A methods ***/
	STDMETHOD(CreateDevice)(THIS_ REFGUID, LPDIRECTINPUTDEVICEA *, LPUNKNOWN);
	STDMETHOD(EnumDevices)(THIS_ DWORD, LPDIENUMDEVICESCALLBACKA, LPVOID, DWORD);
	STDMETHOD(GetDeviceStatus)(THIS_ REFGUID);
	STDMETHOD(RunControlPanel)(THIS_ HWND, DWORD);
	STDMETHOD(Initialize)(THIS_ HINSTANCE, DWORD);
	STDMETHOD(FindDevice)(THIS_ REFGUID, LPCSTR, LPGUID);

	/*** IDirectInput7A methods ***/
	STDMETHOD(CreateDeviceEx)(THIS_ REFGUID, REFIID, LPVOID *, LPUNKNOWN);
};
