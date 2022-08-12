#include <cassert>
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

    drawPixelToBuffer(100, 100, D2D1::ColorF::Black);
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

void BitmapExam::clearBuffer(D2D1::ColorF color)
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

void BitmapExam::drawPixelToBuffer(int x, int y, D2D1::ColorF color)
{
    int pitch = BITMAP_WIDTH * BITMAP_BYTE_COUNT;
    int idx = (x + BITMAP_BYTE_COUNT) + (y * pitch);
    mspBackBuffer[idx] = static_cast<UINT8>(color.r * UCMAX);
    mspBackBuffer[idx + 1] = static_cast<UINT8>(color.g * UCMAX);
    mspBackBuffer[idx + 2] = static_cast<UINT8>(color.b * UCMAX);
    mspBackBuffer[idx + 3] = static_cast<UINT8>(color.a * UCMAX);


}
