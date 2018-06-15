#pragma once
#include <map>
#include <assert.h>

struct HWND__;
typedef struct HWND__ *HWND;

namespace ND3D9
{
#undef _D3D9_H_
#undef DIRECT3D_VERSION
#undef D3DFVF_POSITION_MASK
#undef D3DFVF_RESERVED2
#undef D3D_OK
#undef D3DERR_WRONGTEXTUREFORMAT
#undef D3DERR_UNSUPPORTEDCOLOROPERATION
#undef D3DERR_UNSUPPORTEDCOLORARG
#undef D3DERR_UNSUPPORTEDALPHAOPERATION
#undef D3DERR_UNSUPPORTEDALPHAARG
#undef D3DERR_TOOMANYOPERATIONS
#undef D3DERR_CONFLICTINGTEXTUREFILTER
#undef D3DERR_UNSUPPORTEDFACTORVALUE
#undef D3DERR_CONFLICTINGRENDERSTATE
#undef D3DERR_UNSUPPORTEDTEXTUREFILTER
#undef D3DERR_CONFLICTINGTEXTUREPALETTE
#define DIRECT3D_VERSION 0x0900

#include <d3d9.h>
#include "Common/SmartPointer.h"

	using Resource9Handle = int;

	class D3D9Context;

	struct IResource9Factory
	{
		virtual ~IResource9Factory() = default;

		virtual IUnknown* Create(D3D9Context* context) const = 0;
		virtual bool IsCreateInVideoMemory() const = 0;
		virtual std::string GetType() const = 0;
	};

	class D3D9Context final
	{
		struct Resource9Info
		{
			Resource9Handle handle;
			IResource9Factory* factory;
			IUnknown* pointer;
		};

	private:
		D3D9Context();
	public:
		~D3D9Context();
		static D3D9Context* Instance();

		void Initialize(::HWND hwnd);
		void Uninitialize();
		IDirect3DDevice9* GetDevice() const;
		IDirect3D9* GetD3D9() const;
		void GetBackBufferSize(int* width, int* height);
		void TagDeviceLost();
		bool IsDeviceLost() const;

		Resource9Handle CreateOffScreenSurface9(int width, int height, D3DFORMAT format, D3DPOOL pool);
		Resource9Handle CreateZBufferSurface9(int width, int height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multiSample, DWORD multisampleQuality, BOOL discard);
		Resource9Handle GetBackBuffer9();
		Resource9Handle CreateTexture9(int width, int height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool);

		template<class T>
		SmartPtr<T> GetResource9(Resource9Handle handle, std::string* pType)
		{
			SmartPtr<T> result;
			*(void**)&result = GetResource9(handle, pType);
			return result;
		}

		
		HRESULT ResetDevice();
		ULONG ReleaseResource9(Resource9Handle handle);

	private:
		IUnknown * GetResource9(Resource9Handle handle, std::string* pType)
		{
			// 不知道为什么定义如果写在cpp文件里，会导致编译失败
			IUnknown* result = nullptr;
			auto itor = m_resAllocated.find(handle);
			if (itor != m_resAllocated.end())
			{
				auto ptr = itor->second.pointer;
				ptr->AddRef();
				if (pType)
				{
					*pType = itor->second.factory->GetType();
				}
				result = ptr;
			}
			return result;
		}
		Resource9Handle LogResource(IResource9Factory* factory, IUnknown* pointer);
		void CalcBackBufferSize();
		HRESULT CreateDevice();
		void BuildD3DPresentParameters(D3DPRESENT_PARAMETERS &d3dpp);
		void RebuildResource9(Resource9Handle handle);
	private:

		IDirect3D9 * m_d3d9;
		IDirect3DDevice9* m_d3dDev9;

		std::map<Resource9Handle, Resource9Info> m_resAllocated;
		int m_resCountHistory;

		int m_backBufferWidth;
		int m_backBufferHeight;
		Resource9Handle m_backBuffer9Handle;

		bool m_deviceLost;

		::HWND m_hwnd;
	};
}