#include <windows.h>
#include <ddraw.h>
#include "dinput/dinput.h"
#undef genericQueryInterface
#include "ddraw/ddraw.h"
#undef genericQueryInterface
#include "dshow/dshow.h"
#undef genericQueryInterface
#include <minhook/include/MinHook.h>

#include <string>
#include <functional>
#include <list>
#include <map>

std::ofstream Log::LOG("ddfix.log");
AddressLookupTable<void> ProxyAddressLookupTable = AddressLookupTable<void>(nullptr);

class MinHookPP
{
	friend class MinHookPPMgr;
public:
	MinHookPP(HMODULE module, const char* procName, void* detoursFunction)
	{
		m_hmodule = module;
		m_address = GetProcAddress(module, procName);
		m_procName = procName;
		m_detoursFunction = detoursFunction;

		m_orignal = nullptr;
		m_created = false;
		m_enabled = false;
	}

	~MinHookPP()
	{
		if (m_created)
		{
			RemoveHook();
		}
	}

	bool CreateHook()
	{
		bool result = false;
		if (!m_created)
		{
			MH_STATUS status = MH_CreateHook(m_address, m_detoursFunction, &m_orignal);
			if (status == MH_STATUS::MH_OK)
			{
				m_created = true;
				m_enabled = false;

				result = true;
			}
		}
		return result;
	}

	bool Enable()
	{
		bool result = false;
		if (m_created && !m_enabled)
		{
			MH_STATUS status = MH_EnableHook(m_address);
			if (status == MH_STATUS::MH_OK)
			{
				m_enabled = true;

				result = true;
			}
		}
		return result;
		
	}

	bool Disable()
	{
		bool result = false;
		if (m_created && m_enabled)
		{
			MH_STATUS status = MH_DisableHook(m_address);
			if (status == MH_STATUS::MH_OK)
			{
				m_enabled = false;

				result = true;
			}
		}
		return result;
	}

	bool RemoveHook()
	{
		bool result = false;
		if (m_created)
		{
			MH_STATUS status = MH_RemoveHook(m_address);
			if (status == MH_STATUS::MH_OK)
			{
				m_created = false;
				m_enabled = false;

				result = true;
			}
		}
		return result;
	}

	template<typename T>
	T* GetOrignalFunctionAddress() const
	{
		if (m_created && m_enabled)
		{
			return static_cast<T*>(m_orignal);
		}
		else
		{
			return nullptr;
		}
	}
private:
	HMODULE m_hmodule;
	void* m_address;
	std::string m_procName;
	void* m_detoursFunction;
	void* m_orignal;

	bool m_created;
	bool m_enabled;
	
};

class MinHookPPMgr final
{
	MinHookPPMgr()
	{
		MH_Initialize();
	}
public:
	static MinHookPPMgr* Instance()
	{
		static MinHookPPMgr mgr;
		return &mgr;
	}

	~MinHookPPMgr()
	{
		m_hookers.clear();
		MH_Uninitialize();
	}

	MinHookPP* CreateHooker(HMODULE module, const char* procName, void* detoursFunction)
	{
		MinHookPP* result = nullptr;
		if (HasHooked(module, procName))
		{
			result = nullptr;
		}
		else
		{
			m_hookers.emplace_back(module, procName, detoursFunction);
			result = &m_hookers.back();
		}
		return result;
	}

	bool HasHooked(HMODULE module, const char* procName) const
	{
		bool result = false;
		for (auto& hooker : m_hookers)
		{
			if (hooker.m_hmodule == module && hooker.m_procName == procName)
			{
				result = true;
				break;
			}
		}

		return result;
	}

	MinHookPP* GetHooker(HMODULE module, const char* procName)
	{
		MinHookPP* result = nullptr;
		for (auto& hooker : m_hookers)
		{
			if ((module ? ( hooker.m_hmodule == module ) : true)  && hooker.m_procName == procName)
			{
				result = &hooker;
				break;
			}
		}

		return result;
	}

private:
	std::list<MinHookPP> m_hookers;
};

namespace DDRAW_HOOK
{
	static void* m_acquireDDThreadLock;
	static void* m_d3DParseUnknownCommand;
	static void* m_dDInternalLock;
	static void* m_dDInternalUnlock;
	static void* m_directDrawCreate;
	static void* m_directDrawEnumerateA;
	static void* m_releaseDDThreadLock;

	static void CollectOrignalProcAddress()
	{
		char path[MAX_PATH];
		GetSystemDirectoryA(path, MAX_PATH);
		strcat_s(path, "\\ddraw.dll");
		HMODULE ddraw_original = LoadLibraryA(path);
		m_acquireDDThreadLock = GetProcAddress(ddraw_original, "AcquireDDThreadLock");
		m_d3DParseUnknownCommand = GetProcAddress(ddraw_original, "D3DParseUnknownCommand");
		m_dDInternalLock = GetProcAddress(ddraw_original, "DDInternalLock");
		m_dDInternalUnlock = GetProcAddress(ddraw_original, "DDInternalUnlock");
		m_directDrawCreate = GetProcAddress(ddraw_original, "DirectDrawCreate");
		m_directDrawEnumerateA = GetProcAddress(ddraw_original, "DirectDrawEnumerateA");
		m_releaseDDThreadLock = GetProcAddress(ddraw_original, "ReleaseDDThreadLock");
	}

	void __declspec(naked) FakeAcquireLock() {
		__asm jmp DDRAW_HOOK::m_acquireDDThreadLock;
	}
	void __declspec(naked) FakeParseUnknown() {
		__asm jmp DDRAW_HOOK::m_d3DParseUnknownCommand;
	}
	void __declspec(naked) FakeInternalLock() {
		__asm jmp DDRAW_HOOK::m_dDInternalLock;
	}
	void __declspec(naked) FakeInternalUnlock() {
		__asm jmp DDRAW_HOOK::m_dDInternalUnlock;
	}
	void __declspec(naked) FakeReleaseLock() {
		__asm jmp DDRAW_HOOK::m_releaseDDThreadLock;
	}

	HRESULT WINAPI FakeDirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter) {
		//HRESULT hr = reinterpret_cast<decltype(DirectDrawCreate)*>(DDRAW_HOOK::m_directDrawCreate)(lpGUID, lplpDD, pUnkOuter);
// 		if (SUCCEEDED(hr))
// 		{
// 			*lplpDD = ProxyAddressLookupTable.FindAddress<m_IDirectDraw>(*lplpDD);
// 		}
// 		return hr;

		std::shared_ptr<WrapperLookupTable<void>> WrapperAddressLookupTable;
		WrapperAddressLookupTable = std::make_shared<WrapperLookupTable<void>>(nullptr);
		*lplpDD = WrapperAddressLookupTable->FindWrapper<m_IDirectDraw>(IID_IDirectDraw);
		return S_OK;
	}

	void __declspec(naked) FakeDirectDrawEnumerateA() {
		__asm jmp DDRAW_HOOK::m_directDrawEnumerateA;
	}
};

namespace DINPUT_HOOK
{
	static HRESULT WINAPI FakeDirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA *ppDI, LPUNKNOWN punkOuter)
	{
		auto hooker = MinHookPPMgr::Instance()->GetHooker(nullptr, "DirectInputCreateA");
		HRESULT hr = hooker->GetOrignalFunctionAddress<decltype(DirectInputCreateA)>()(hinst, dwVersion, ppDI, punkOuter);

		if (SUCCEEDED(hr))
		{
			*ppDI = ProxyAddressLookupTable.FindAddress<m_IDirectInputA>(*ppDI);
		}

		return hr;
	}

	static HRESULT WINAPI FakeDirectInputCreateEx(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
	{
		auto hooker = MinHookPPMgr::Instance()->GetHooker(nullptr, "DirectInputCreateEx");
		HRESULT WINAPI MyDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, LPVOID *ppvOut, LPUNKNOWN punkOuter);

		HRESULT hr = MyDirectInput8Create(hinst, 0x0800, ppvOut, punkOuter);

		if (SUCCEEDED(hr))
		{
			genericDinputQueryInterface(riidltf, ppvOut);
		}

		return hr;
	}

	static void Hook()
	{
		char path[MAX_PATH];
		GetSystemDirectoryA(path, MAX_PATH);
		strcat_s(path, "\\dinput.dll");
		HMODULE dinput = LoadLibraryA(path);
		std::vector<MinHookPP*> hookers;
		hookers.push_back(MinHookPPMgr::Instance()->CreateHooker(dinput, "DirectInputCreateA", &FakeDirectInputCreateA));
		hookers.push_back(MinHookPPMgr::Instance()->CreateHooker(dinput, "DirectInputCreateEx", &FakeDirectInputCreateEx));
		for(auto hooker : hookers)
		{
			hooker->CreateHook();
			hooker->Enable();
		}
	}

};

namespace DSHOW_HOOK
{
	HRESULT WINAPI FakeCoCreateInstance(_In_ REFCLSID rclsid, _In_opt_ LPUNKNOWN pUnkOuter, _In_ DWORD dwClsContext, _In_ REFIID riid, _COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID FAR* ppv)
	{
		auto hooker = MinHookPPMgr::Instance()->GetHooker(nullptr, "CoCreateInstance");
		HRESULT hr = hooker->GetOrignalFunctionAddress<decltype(CoCreateInstance)>()(rclsid, pUnkOuter, dwClsContext, riid, ppv);

		if (SUCCEEDED(hr))
		{
			if (rclsid == CLSID_FilterGraph)
			{
				if (riid == IID_IFilterGraph)
				{
					*ppv = ProxyAddressLookupTable.FindAddress<m_IFilterGraph>(*ppv);
				}
				else if (riid == IID_IGraphBuilder)
				{
					*ppv = ProxyAddressLookupTable.FindAddress<m_IGraphBuilder>(*ppv);
				}
			}
			if (riid == IID_IBaseFilter)
			{
				int a = 0;
				a = 1;
			}
		}

		return hr;
		
	}

	static void Hook()
	{
		char path[MAX_PATH];
		GetSystemDirectoryA(path, MAX_PATH);
		strcat_s(path, "\\Ole32.dll");
		HMODULE dinput = LoadLibraryA(path);
		std::vector<MinHookPP*> hookers;
		hookers.push_back(MinHookPPMgr::Instance()->CreateHooker(dinput, "CoCreateInstance", &FakeCoCreateInstance));
		for (auto hooker : hookers)
		{
			hooker->CreateHook();
			hooker->Enable();
		}
	}
}


BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DDRAW_HOOK::CollectOrignalProcAddress();
		DINPUT_HOOK::Hook(); // 如果不Hook，会导致调试困难
		DSHOW_HOOK::Hook(); // 避免DShow里使用DX6的接口
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	default:
		break;
	}
	return TRUE;
}