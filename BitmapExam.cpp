#define _USE_MATH_DEFINES
#include <cassert>
#include <cmath>
#include "BitmapExam.h"

HRESULT BitmapExam::Initialize(HINSTANCE hInstance, int showCmd, LPCWSTR title, UINT width, UINT height)
{
    D2DFramework::Initialize(hInstance, showCmd);

    mspBackBuffer = std::make_unique<UINT8[]>(
        BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTE_COUNT        
    );

    HRESULT hr = mcpRenderTarget->CreateBitmap(
        D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT),
        D2D1::BitmapProperties(
            D2D1::PixelFormat(
                DXGI_FORMAT_R8G8B8A8_UNORM,
                D2D1_ALPHA_MODE_IGNORE
            )
        ),
        mcpFrameBuffer.GetAddressOf()
    );

    if (FAILED(hr))
    {
        assert(false);
    }

    return S_OK;
}

void BitmapExam::Render()
{
    HRESULT hr;
    mcpRenderTarget->BeginDraw();
    mcpRenderTarget->Clear(D2D1::ColorF(0.f, 0.2f, 0.4f, 1.f));
    clearBuffer(D2D1::ColorF(D2D1::ColorF::LightPink));

    drawLine(IntVector{ 50, 50 }, IntVector{ 250, 100 }, D2D1::ColorF(D2D1::ColorF::Black));
    presentBuffer();

    mcpRenderTarget->DrawBitmap(mcpFrameBuffer.Get());

    hr = mcpRenderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        CreateDeviceResources();
    };
}

void BitmapExam::presentBuffer()
{
    mcpFrameBuffer->CopyFromMemory(
        nullptr,
        &mspBackBuffer[0],
        BITMAP_WIDTH * BITMAP_BYTE_COUNT
    );
}

void BitmapExam::clearBuffer(const D2D1::ColorF color)
{
    UINT8* pCurrent = &mspBackBuffer[0];
    for (int count = 0; count < BITMAP_WIDTH * BITMAP_HEIGHT; ++count)
    {
        *pCurrent = static_cast<UINT8>(color.r * UCMAX);
        *(pCurrent + 1) = static_cast<UINT8>(color.g * UCMAX);
        *(pCurrent + 2) = static_cast<UINT8>(color.b * UCMAX);
        *(pCurrent + 3) = static_cast<UINT8>(color.a * UCMAX);

        pCurrent += BITMAP_BYTE_COUNT;
    }
}

void BitmapExam::drawPixelToBuffer(const int x, const int y, const D2D1::ColorF color)
{
    int pitch = BITMAP_WIDTH * BITMAP_BYTE_COUNT;
    int idx = (y * pitch) + (x * BITMAP_BYTE_COUNT);
    mspBackBuffer[idx] = static_cast<UINT8>(color.r * UCMAX);
    mspBackBuffer[idx + 1] = static_cast<UINT8>(color.g * UCMAX);
    mspBackBuffer[idx + 2] = static_cast<UINT8>(color.b * UCMAX);
    mspBackBuffer[idx + 3] = static_cast<UINT8>(color.a * UCMAX);
}

void BitmapExam::drawRectangle(const int left, const int top, const int right, const int bottom, const D2D1::ColorF color)
{
    if (top >= bottom || left >= right)
    {
        assert(false);
    }
    // 150, 150, 200, 200
    for (int i = 0; i < bottom - top; ++i)
    {
        for (int j = 0; j < right - left; ++j)
        {
            drawPixelToBuffer(j + left, i + top, color);
        }
    }
}

void BitmapExam::drawCircle(const int x, const int y, const int radius, const D2D1::ColorF color)
{

    for (float angle = 0.f; angle <= 360.f; angle += 0.1f)
    {
        int drawX = static_cast<int>((cos((angle / 180) * M_PI) * radius) + x);
        int drawY = static_cast<int>((sin((angle / 180) * M_PI) * radius) + y);
        drawPixelToBuffer(drawX, drawY, color);
    }
}

void BitmapExam::drawLine(IntVector v1, IntVector v2, const D2D1::ColorF color)
{
    if (v2.y - v1.y == 0 && v2.x - v1.x == 0)
    {
        return;
    }

    // v2.y - v1.y / v1.x - v2.x == m
    float m = static_cast<float>((v2.y - v1.y)) / (v2.x - v1.x);


    // Vertical line
    if (v1.x - v2.x == 0)
    {
        if (v1.y > v2.y)
        {
            swap(v1, v2);
        }

        for (int j = v1.y; j <= v2.y; ++j)
        {
            drawPixelToBuffer(v1.x, j, color);
        }

        return;
    }


    if (v1.x > v2.x)
    {
        swap(v1, v2);
    }

    for (int i = v1.x; i <= v2.x; ++i)
    {
        drawPixelToBuffer(i, static_cast<int>((m * (i - v1.x) + v1.y)), color);
    }
    
}

void BitmapExam::swap(IntVector& v1, IntVector& v2)
{
    IntVector tmp = v1;
    v1 = v2;
    v2 = tmp;
}
