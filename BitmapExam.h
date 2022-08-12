#pragma once
#include <memory>
#include "D2DFramework.h"
class BitmapExam final : public D2DFramework
{
public:
	const int BITMAP_WIDTH = 1024;
	const int BITMAP_HEIGHT = 768;
	const int BITMAP_BYTE_COUNT = 4;
	const int UCMAX = 255;


public:
	virtual HRESULT Initialize(HINSTANCE hInstance, int showCmd, LPCWSTR title = L"D2DExample", UINT width = 1024, UINT height = 768) override;
	virtual void Render() override;

private:
	void presentBuffer();
	void clearBuffer(D2D1::ColorF color);
	void drawPixelToBuffer(int x, int y, D2D1::ColorF color);

private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mcpFrameBuffer{};
	std::unique_ptr<UINT8[]> mspBackBuffer;

};

