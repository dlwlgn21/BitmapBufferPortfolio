#include <sstream>
#include <cassert>
#include "D2DFramework.h"

#pragma comment (lib, "d2d1.lib")

HRESULT D2DFramework::InitD2D()
{
	HRESULT hr;

	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		mcpD2DFactory.GetAddressOf()
	);

	assert(hr == S_OK);

	RECT wr;
	GetClientRect(mHwnd, &wr);

	hr = mcpD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			mHwnd,
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)
		),
		mcpRenderTarget.GetAddressOf()
	);

	assert(hr == S_OK);

	return S_OK;
}

void D2DFramework::Release()
{
	mcpD2DFactory.Reset();
	mcpRenderTarget.Reset();
}

void D2DFramework::Render()
{
	HRESULT hr;
	mcpRenderTarget->BeginDraw();
	mcpRenderTarget->Clear(D2D1::ColorF(0.f, 0.2f, 0.4f, 1.f));



	hr = mcpRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}

int D2DFramework::GameLoop()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			Render();
		}
	}
	return static_cast<int>(msg.wParam);
}

HRESULT D2DFramework::Initialize(HINSTANCE hInstance, int showCmd, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr;

	hr = InitWindow(hInstance, showCmd, title, width, height);
	if (FAILED(hr))
	{
		assert(false);
	}
	hr = InitD2D();
	if (FAILED(hr))
	{
		assert(false);
	}
	return S_OK;
}

LRESULT D2DFramework::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	D2DFramework* pFramework = reinterpret_cast<D2DFramework*>((GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 1;
}

HRESULT D2DFramework::CreateDeviceResources()
{
	RECT wr;

	GetClientRect(mHwnd, &wr);

	D2D1_SIZE_U size = D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top);

	HRESULT hr = mcpD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(mHwnd, size),
		mcpRenderTarget.ReleaseAndGetAddressOf()
	);
	if (FAILED(hr))
	{
		assert(false);
	}

	return S_OK;
}

HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, int showCmd, LPCWSTR title, UINT width, UINT height)
{
	WNDCLASSEX wc;

	// STEP 1 - Register Window Class

	ZeroMemory(&wc, sizeof(wc));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = M_WINDOW_CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = D2DFramework::WndProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		assert(false);
	}
	RECT wr = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);


	// STEP 2 - CreateWindow
	mHwnd = CreateWindowEx(
		NULL,
		M_WINDOW_CLASS_NAME,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	assert(mHwnd != nullptr);

	SetWindowLongPtr(mHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ShowWindow(mHwnd, showCmd);
	UpdateWindow(mHwnd);

	return S_OK;
}
