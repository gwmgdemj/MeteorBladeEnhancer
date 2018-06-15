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

#include "dinput.h"

namespace NDINPUT8
{
#undef __DINPUT_INCLUDED__
#undef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#undef DIRECTINPUT_VERSION
}

HRESULT m_IDirectInputDevice7A::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	if ((riid == IID_IDirectInputDevice7A || riid == IID_IUnknown) && ppvObj)
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

ULONG m_IDirectInputDevice7A::AddRef()
{
	return ProxyInterface->AddRef();
}

ULONG m_IDirectInputDevice7A::Release()
{
	ULONG x = ProxyInterface->Release();

	if (x == 0)
	{
		delete this;
	}

	return x;
}

HRESULT m_IDirectInputDevice7A::GetCapabilities(LPDIDEVCAPS lpDIDevCaps)
{
	return ProxyInterface->GetCapabilities(reinterpret_cast<NDINPUT8::LPDIDEVCAPS>(lpDIDevCaps));
}

HRESULT m_IDirectInputDevice7A::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return ProxyInterface->EnumObjects(reinterpret_cast<NDINPUT8::LPDIENUMDEVICEOBJECTSCALLBACKA>(lpCallback), pvRef, dwFlags);
}

HRESULT m_IDirectInputDevice7A::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph)
{
	return ProxyInterface->GetProperty(rguidProp, reinterpret_cast<NDINPUT8::LPDIPROPHEADER>(pdiph));
}

HRESULT m_IDirectInputDevice7A::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph)
{
	return ProxyInterface->SetProperty(rguidProp, reinterpret_cast<NDINPUT8::LPCDIPROPHEADER>(pdiph));
}

HRESULT m_IDirectInputDevice7A::Acquire()
{
	return ProxyInterface->Acquire();
}

HRESULT m_IDirectInputDevice7A::Unacquire()
{
	return ProxyInterface->Unacquire();
}

HRESULT m_IDirectInputDevice7A::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
	return ProxyInterface->GetDeviceState(cbData, lpvData);
}

HRESULT m_IDirectInputDevice7A::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	return ProxyInterface->GetDeviceData(cbObjectData, reinterpret_cast<NDINPUT8::LPDIDEVICEOBJECTDATA>(rgdod), pdwInOut, dwFlags);
}

HRESULT m_IDirectInputDevice7A::SetDataFormat(LPCDIDATAFORMAT lpdf)
{
	return ProxyInterface->SetDataFormat(reinterpret_cast<NDINPUT8::LPCDIDATAFORMAT>(lpdf));
}

HRESULT m_IDirectInputDevice7A::SetEventNotification(HANDLE hEvent)
{
	return ProxyInterface->SetEventNotification(hEvent);
}

HRESULT m_IDirectInputDevice7A::SetCooperativeLevel(HWND hwnd, DWORD dwFlags)
{
	return ProxyInterface->SetCooperativeLevel(hwnd, dwFlags);
}

HRESULT m_IDirectInputDevice7A::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA pdidoi, DWORD dwObj, DWORD dwHow)
{
	return ProxyInterface->GetObjectInfo(reinterpret_cast<NDINPUT8::LPDIDEVICEOBJECTINSTANCEA>(pdidoi), dwObj, dwHow);
}

HRESULT m_IDirectInputDevice7A::GetDeviceInfo(LPDIDEVICEINSTANCEA pdidi)
{
	return ProxyInterface->GetDeviceInfo(reinterpret_cast<NDINPUT8::LPDIDEVICEINSTANCEA>(pdidi));
}

HRESULT m_IDirectInputDevice7A::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
	return ProxyInterface->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT m_IDirectInputDevice7A::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid)
{
	return ProxyInterface->Initialize(hinst, dwVersion, rguid);
}

HRESULT m_IDirectInputDevice7A::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT * ppdeff, LPUNKNOWN punkOuter)
{
	HRESULT hr = ProxyInterface->CreateEffect(rguid, reinterpret_cast<NDINPUT8::LPCDIEFFECT>(lpeff), reinterpret_cast<NDINPUT8::LPDIRECTINPUTEFFECT*>(ppdeff), punkOuter);

	if (SUCCEEDED(hr))
	{
		*ppdeff = ProxyAddressLookupTable.FindAddress<m_IDirectInputEffect>(*ppdeff);
	}

	return hr;
}

HRESULT m_IDirectInputDevice7A::EnumEffects(LPDIENUMEFFECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwEffType)
{
	return ProxyInterface->EnumEffects(reinterpret_cast<NDINPUT8::LPDIENUMEFFECTSCALLBACKA>(lpCallback), pvRef, dwEffType);
}

HRESULT m_IDirectInputDevice7A::GetEffectInfo(LPDIEFFECTINFOA pdei, REFGUID rguid)
{
	return ProxyInterface->GetEffectInfo(reinterpret_cast<NDINPUT8::LPDIEFFECTINFOA>(pdei), rguid);
}

HRESULT m_IDirectInputDevice7A::GetForceFeedbackState(LPDWORD pdwOut)
{
	return ProxyInterface->GetForceFeedbackState(pdwOut);
}

HRESULT m_IDirectInputDevice7A::SendForceFeedbackCommand(DWORD dwFlags)
{
	return ProxyInterface->SendForceFeedbackCommand(dwFlags);
}

HRESULT m_IDirectInputDevice7A::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl)
{
	ENUMEFFECT CallbackContext;
	CallbackContext.pvRef = pvRef;
	CallbackContext.lpCallback = lpCallback;

	return ProxyInterface->EnumCreatedEffectObjects(reinterpret_cast<NDINPUT8::LPDIENUMCREATEDEFFECTOBJECTSCALLBACK>(m_IDirectInputEnumEffect::EnumEffectCallback), &CallbackContext, fl);
}

HRESULT m_IDirectInputDevice7A::Escape(LPDIEFFESCAPE pesc)
{
	return ProxyInterface->Escape(reinterpret_cast<NDINPUT8::LPDIEFFESCAPE>(pesc));
}

HRESULT m_IDirectInputDevice7A::Poll()
{
	return ProxyInterface->Poll();
}

HRESULT m_IDirectInputDevice7A::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl)
{
	return ProxyInterface->SendDeviceData(cbObjectData, reinterpret_cast<NDINPUT8::LPCDIDEVICEOBJECTDATA>(rgdod), pdwInOut, fl);
}

HRESULT m_IDirectInputDevice7A::EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags)
{
	return ProxyInterface->EnumEffectsInFile(lpszFileName, reinterpret_cast<NDINPUT8::LPDIENUMEFFECTSINFILECALLBACK>(pec), pvRef, dwFlags);
}

HRESULT m_IDirectInputDevice7A::WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags)
{
	return ProxyInterface->WriteEffectToFile(lpszFileName, dwEntries, reinterpret_cast<NDINPUT8::LPDIFILEEFFECT>(rgDiFileEft), dwFlags);
}
