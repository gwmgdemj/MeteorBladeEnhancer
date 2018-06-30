#include "D3D9Context.h"
#include "Common/Logging.h"

using namespace ND3D9;

class OffscreenSurface9Factory final : public IResource9Factory
{
public:
	OffscreenSurface9Factory(int width, int height, D3DFORMAT format, D3DPOOL pool)
		: m_width(width)
		, m_height(height)
		, m_format(format)
		, m_pool(pool)
	{

	}

	virtual IUnknown* Create(D3D9Context* context) const override
	{
		IDirect3DSurface9* surface9 = nullptr;
		context->GetDevice()->CreateOffscreenPlainSurface(
			m_width,
			m_height,
			m_format,
			m_pool,
			&surface9,
			nullptr);
		return surface9;
	}

	virtual bool IsCreateInVideoMemory() const override
	{
		return m_pool == D3DPOOL_DEFAULT;
	}

	virtual std::string GetType() const override
	{
		return "OffScreen";
	}

private:
	int m_width;
	int m_height;
	D3DFORMAT m_format;
	D3DPOOL m_pool;
};

class ZBufferSurface9Factory final : public IResource9Factory
{
public:
	ZBufferSurface9Factory(int width, int height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multiSample, DWORD multisampleQuality, BOOL discard)
		: m_width(width)
		, m_height(height)
		, m_format(format)
		, m_multiSample(multiSample)
		, m_multisampleQuality(multisampleQuality)
		, m_discard(discard)
	{

	}

	virtual IUnknown* Create(D3D9Context* context) const override
	{
		IDirect3DSurface9* surface9 = nullptr;
		context->GetDevice()->CreateDepthStencilSurface(m_width, m_height, m_format, m_multiSample, m_multisampleQuality, m_discard, &surface9, nullptr);
		return surface9;
	}

	virtual bool IsCreateInVideoMemory() const override
	{
		return true;
	}

	virtual std::string GetType() const override
	{
		return "ZBuffer";
	}

private:
	int m_width;
	int m_height;
	D3DFORMAT m_format;
	D3DMULTISAMPLE_TYPE m_multiSample;
	DWORD m_multisampleQuality;
	BOOL m_discard;
};

class BackBuffer9Factory final : public IResource9Factory
{
public:
	BackBuffer9Factory()
	{

	}

	virtual IUnknown* Create(D3D9Context* context) const override
	{
		IDirect3DSurface9* backbuffer9 = nullptr;
		context->GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer9);
		return backbuffer9;
	}

	virtual bool IsCreateInVideoMemory() const override
	{
		return true;
	}

	virtual std::string GetType() const override
	{
		return "BackBuffer";
	}

};

class Texture9Factory final : public IResource9Factory
{
public:
	Texture9Factory(int width, int height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool)
		: m_width(width)
		, m_height(height)
		, m_levels(levels)
		, m_usage(usage)
		, m_format(format)
		, m_pool(pool)
	{

	}

	virtual IUnknown* Create(D3D9Context* context) const override
	{
		IDirect3DTexture9* tex9 = nullptr;
		context->GetDevice()->CreateTexture(m_width, m_height, m_levels, m_usage, m_format, m_pool, &tex9, nullptr);
		return tex9;
	}

	virtual bool IsCreateInVideoMemory() const override
	{
		return m_pool == D3DPOOL_DEFAULT;
	}

	virtual std::string GetType() const override
	{
		return "Texture";
	}

private:
	int m_width;
	int m_height;
	UINT m_levels;
	DWORD m_usage;
	D3DFORMAT m_format;
	D3DPOOL m_pool;
};

class RenderTarget9Factory final : public IResource9Factory
{
public:
	RenderTarget9Factory(int width, int height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multiSample, DWORD multisampleQuality, BOOL lockable)
		: m_width(width)
		, m_height(height)
		, m_format(format)
		, m_multiSample(multiSample)
		, m_multisampleQuality(multisampleQuality)
		, m_lockable(lockable)
	{

	}

	virtual IUnknown* Create(D3D9Context* context) const override
	{
		IDirect3DSurface9* surface9 = nullptr;
		context->GetDevice()->CreateRenderTarget(
			m_width,
			m_height,
			m_format,
			m_multiSample,
			m_multisampleQuality,
			m_lockable,
			&surface9,
			nullptr);
		return surface9;
	}

	virtual bool IsCreateInVideoMemory() const override
	{
		return true;
	}

	virtual std::string GetType() const override
	{
		return "RenderTarget";
	}

private:
	int m_width;
	int m_height;
	D3DFORMAT m_format;
	D3DMULTISAMPLE_TYPE m_multiSample;
	DWORD m_multisampleQuality;
	BOOL m_lockable;
};

class Sprite9Factory final : public IResource9Factory
{
public:
	Sprite9Factory()
	{

	}

	virtual IUnknown* Create(D3D9Context* context) const override
	{
		ID3DXSprite* sprite = nullptr;
		D3DXCreateSprite(context->GetDevice(), &sprite);
		return sprite;
	}

	virtual bool IsCreateInVideoMemory() const override
	{
		return true;
	}

	virtual std::string GetType() const override
	{
		return "Sprite";
	}
};

D3D9Context::~D3D9Context()
{
	logf("%d resource not releases.", m_resAllocated.size());
}

Resource9Handle D3D9Context::LogResource(IResource9Factory* factory, IUnknown* pointer)
{
	m_resCountHistory += 1;
	Resource9Handle handle = m_resCountHistory;
	Resource9Info info = { 0 };
	info.handle = handle;
	info.factory = factory;
	info.pointer = pointer;
	m_resAllocated[handle] = info;
	return handle;
}

IDirect3D9* D3D9Context::GetD3D9() const
{
	return m_d3d9;
}

IDirect3DDevice9* D3D9Context::GetDevice() const
{
	return m_d3dDev9;
}

void D3D9Context::Uninitialize()
{
	if (m_d3d9)
	{
		m_d3dDev9->Release();
		m_d3d9->Release();
	}
}

void D3D9Context::Initialize(::HWND hwnd)
{
	assert(!m_d3d9);
	m_hwnd = hwnd;

	m_d3d9 = Direct3DCreate9(DIRECT3D_VERSION);

	CreateDevice();

	if (TRUE)
	{
		// TODO: trick for game meteor blade
		//m_d3dDev9->SetRenderState(D3DRS_LIGHTING, TRUE);
		//m_d3dDev9->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
		//m_d3dDev9->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	}
}

D3D9Context* D3D9Context::Instance()
{
	static D3D9Context context;
	return &context;
}

D3D9Context::D3D9Context()
	: m_d3d9(nullptr)
	, m_d3dDev9(nullptr)
	, m_backBufferWidth(0)
	, m_backBufferHeight(0)
	, m_deviceLost(false)
	, m_hwnd(0)
	, m_resCountHistory(0)
	, m_backBuffer9Handle(0)
{
}

void D3D9Context::GetBackBufferSize(int* width, int* height)
{
	*width = m_backBufferWidth;
	*height = m_backBufferHeight;
}

void D3D9Context::TagDeviceLost()
{
	m_deviceLost = true;
}

bool D3D9Context::IsDeviceLost() const
{
	return m_deviceLost;
}

HRESULT D3D9Context::ResetDevice()
{
	auto testResult = m_d3dDev9->TestCooperativeLevel();
	if (testResult == D3D_OK)
	{
		return D3D_OK;
	}
	else if (testResult != D3DERR_DEVICENOTRESET)
	{
		return D3DERR_DEVICENOTRESET;
	}

	auto oldResAllocated = m_resAllocated;
	for (auto& kv : oldResAllocated)
	{
		const auto& info = kv.second;
		if (info.factory->IsCreateInVideoMemory())
		{
			auto ptr = GetResource9(info.handle, nullptr); // internal will AddRef
			ULONG refs = ptr->Release();
			refs = ptr->Release();
			assert(refs == 0);
		}
	}

	D3DPRESENT_PARAMETERS d3dpp;
	BuildD3DPresentParameters(d3dpp);
	HRESULT hr = m_d3dDev9->Reset(&d3dpp);
	if (SUCCEEDED(hr))
	{
		m_deviceLost = false;

		for (auto& kv : m_resAllocated)
		{
			const auto& info = kv.second;
			if (info.factory->IsCreateInVideoMemory())
			{
				RebuildResource9(info.handle);
			}
		}

		return D3D_OK;
	}
	else
	{
		return hr;
	}
}

void D3D9Context::CalcBackBufferSize()
{
	DEVMODE currmode;
	ZeroMemory(&currmode, sizeof(DEVMODE));
	currmode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &currmode);
	m_backBufferWidth = currmode.dmPelsWidth;
	m_backBufferHeight = currmode.dmPelsHeight;
}

HRESULT D3D9Context::CreateDevice()
{
	CalcBackBufferSize();
	D3DPRESENT_PARAMETERS d3dpp;
	BuildD3DPresentParameters(d3dpp);
	HRESULT hr = m_d3d9->CreateDevice(0, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_d3dDev9);

	auto backBufferFactory = new BackBuffer9Factory();
	auto backBuffer9 = backBufferFactory->Create(this);
	m_backBuffer9Handle = LogResource(backBufferFactory, backBuffer9);

	return hr;
}

void D3D9Context::BuildD3DPresentParameters(D3DPRESENT_PARAMETERS &d3dpp)
{
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.hDeviceWindow = (HWND)m_hwnd;
	d3dpp.BackBufferWidth = m_backBufferWidth;
	d3dpp.BackBufferHeight = m_backBufferHeight;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.EnableAutoDepthStencil = FALSE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

Resource9Handle D3D9Context::CreateOffScreenSurface9(int width, int height, D3DFORMAT format, D3DPOOL pool)
{
	auto factory = new OffscreenSurface9Factory(width, height, format, pool);
	auto ptr = factory->Create(this);
	return LogResource(factory, ptr);
}

Resource9Handle D3D9Context::CreateZBufferSurface9(int width, int height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multiSample, DWORD multisampleQuality, BOOL discard)
{
	auto factory = new ZBufferSurface9Factory(width, height, format, multiSample, multisampleQuality, discard);
	auto ptr = factory->Create(this);
	return LogResource(factory, ptr);
}

Resource9Handle D3D9Context::GetBackBuffer9()
{
	return m_backBuffer9Handle;
}

Resource9Handle D3D9Context::CreateTexture9(int width, int height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool)
{
	auto factory = new Texture9Factory(width, height, levels, usage, format, pool);
	auto ptr = factory->Create(this);
	return LogResource(factory, ptr);
}

ULONG D3D9Context::ReleaseResource9(Resource9Handle handle)
{
	assert(handle > 0);
	auto itor = m_resAllocated.find(handle);
	if (itor != m_resAllocated.end())
	{
		auto info = m_resAllocated.at(handle);
		auto refs = info.pointer->Release();
		if (refs == 0)
		{
			delete info.factory;
			m_resAllocated.erase(itor);
		}
		return refs;
	}
	else
	{
		assert(false);
		return 0;
	}
}

void D3D9Context::RebuildResource9(Resource9Handle handle)
{
	auto& info = m_resAllocated.at(handle);
	auto newPtr = info.factory->Create(this);
	info.pointer = newPtr;
}

Resource9Handle D3D9Context::CreateRenderTarget(int width, int height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multiSample, DWORD multisampleQuality, BOOL lockable)
{
	auto factory = new RenderTarget9Factory(width, height, format, multiSample, multisampleQuality, lockable);
	auto ptr = factory->Create(this);
	return LogResource(factory, ptr);
}

Resource9Handle D3D9Context::CreateSprite()
{
	auto factory = new Sprite9Factory();
	auto ptr = factory->Create(this);
	return LogResource(factory, ptr);
}

