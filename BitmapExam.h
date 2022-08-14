#pragma once
#include <memory>
#include "D2DFramework.h"
struct IntVector
{
public:
	int x;
	int y;
};

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
	void drawPixelToBuffer(const int x, const int y, const D2D1::ColorF color);
	void drawRectangle(const int left, const int top, const int right, const int bottom, const D2D1::ColorF color);
	void drawCircle(const int x, const int y, const int radius, const D2D1::ColorF color);
	void drawLine(IntVector v1, IntVector v2, const D2D1::ColorF color);
	void swap(IntVector& v1, IntVector& v2);
private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mcpFrameBuffer{};
	std::unique_ptr<UINT8[]> mspBackBuffer;

};

