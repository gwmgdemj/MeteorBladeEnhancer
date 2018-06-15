#pragma once

#include "SmartPointer.h"

#include <unordered_map>
#include <map>
#include <algorithm>
#include <memory>
#include <string>
#include <assert.h>

template <typename D>
class AddressLookupTable
{
public:
	explicit AddressLookupTable(D *pDevice, bool autoDelete = true) : pDevice(pDevice), AutoDelete(autoDelete){}
	~AddressLookupTable()
	{
		ConstructorFlag = true;
		if (AutoDelete)
		{
			for (const auto& entry : g_map)
			{
				entry.second->DeleteMe();
			}
		}
		g_map.clear();
	}

	template <typename T>
	T *FindAddress(void *Proxy)
	{
		if (Proxy == nullptr)
		{
			return nullptr;
		}

		auto it = g_map.find(Proxy);

		if (it != std::end(g_map))
		{
			return static_cast<T *>(it->second);
		}

		return new T(static_cast<T *>(Proxy), pDevice);
	}

	template <typename T>
	T *FindAddressOnly(void *Proxy)
	{
		if (Proxy == nullptr)
		{
			return nullptr;
		}

		auto it = g_map.find(Proxy);

		if (it != std::end(g_map))
		{
			return static_cast<T *>(it->second);
		}

		return nullptr;
	}

	template <typename T>
	void SaveAddress(T *Wrapper, void *Proxy)
	{
		if (Wrapper != nullptr && Proxy != nullptr)
		{
			g_map[Proxy] = Wrapper;
		}
	}

	template <typename T>
	void DeleteAddress(T *Wrapper)
	{
		if (Wrapper != nullptr && !ConstructorFlag)
		{
			auto it = std::find_if(g_map.begin(), g_map.end(),
				[Wrapper](std::pair<void*, class AddressLookupTableObject*> Map) -> bool { return Map.second == Wrapper; });

			if (it != std::end(g_map))
			{
				it = g_map.erase(it);
			}
		}
	}

private:
	bool ConstructorFlag = false;
	D *const pDevice;
	bool AutoDelete;
	std::unordered_map<void*, class AddressLookupTableObject*> g_map;
};

class AddressLookupTableObject
{
public:
	virtual ~AddressLookupTableObject() { }

	void DeleteMe()
	{
		delete this;
	}
};

template <typename D>
class WrapperLookupTable : public std::enable_shared_from_this<WrapperLookupTable<D>>
{
public:
	explicit WrapperLookupTable(D *pDevice) : pDevice(pDevice) {}
	~WrapperLookupTable()
	{
		ConstructorFlag = true;
		for (const auto& entry : g_wrapperMap)
		{
			entry.second->DeleteMe();
		}
	}

	template <typename T>
	T *FindWrapper(const IID& iid)
	{
		auto it = g_wrapperMap.find(StringFromIID(iid));

		if (it != std::end(g_wrapperMap))
		{
			return static_cast<T *>(it->second);
		}

		return new T(nullptr, shared_from_this());
	}

	template <typename T>
	T *FindWrapperOnly(const IID& iid)
	{
		auto it = g_wrapperMap.find(StringFromIID(iid));

		if (it != std::end(g_wrapperMap))
		{
			return static_cast<T *>(it->second);
		}

		return nullptr;
	}

	template <typename T>
	void SaveWrapper(T *Wrapper, const IID& iid)
	{
		if (Wrapper != nullptr)
		{
			assert(g_wrapperMap.count(StringFromIID(iid)) == 0);
			g_wrapperMap[StringFromIID(iid)] = Wrapper;
		}
	}

	void DeleteWrapper(const IID& iid)
	{
		if (!ConstructorFlag)
		{
			auto it = g_wrapperMap.find(StringFromIID(iid));

			if (it != std::end(g_wrapperMap))
			{
				it = g_wrapperMap.erase(it);
			}
		}
	}

	std::wstring StringFromIID(const IID& iid)
	{
		LPOLESTR str;
		::StringFromIID(iid, &str);
		std::wstring wstr((wchar_t*)str);
		CoTaskMemFree((LPVOID)str);
		return wstr;
	}

private:
	bool ConstructorFlag = false;
	D *const pDevice;
	std::unordered_map<std::wstring, class AddressLookupTableObject*> g_wrapperMap;
};