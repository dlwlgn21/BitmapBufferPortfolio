#pragma once
#include <wincodec.h>
#include "D2DFramework.h"
class ImageExam final : public D2DFramework
{
public:
	virtual HRESULT Initialize(HINSTANCE hInstance, int showCmd, LPCWSTR title = L"D2DExample", UINT width = 1024, UINT height = 768) override;
	virtual void Render() override;
	virtual void Release() override;

private:
	HRESULT loadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
	HRESULT loadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);

private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mcpBitmap;
	Microsoft::WRL::ComPtr<IWICImagingFactory> mcpWICFactory;


};

