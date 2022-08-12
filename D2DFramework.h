#pragma once

#include <d2d1.h>
#include <wrl/client.h>

class D2DFramework
{
public:
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();
	virtual HRESULT Initialize(HINSTANCE hInstance, int showCmd, LPCWSTR title = L"D2DExample", UINT width = 1024, UINT height = 768);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

protected:
	virtual HRESULT CreateDeviceResources();

private:
	virtual HRESULT InitD2D();
	HRESULT InitWindow(HINSTANCE hInstance, int showCmd, LPCWSTR title, UINT width, UINT height);

protected:
	HWND mHwnd{};
	Microsoft::WRL::ComPtr<ID2D1Factory> mcpD2DFactory;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mcpRenderTarget;

private:
	const LPCWSTR M_WINDOW_CLASS_NAME{ L"D2DWindowClass" };
};