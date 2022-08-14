#include <cassert>
#include <fstream>
#include <vector>
#include "ImageExam.h"

#pragma comment(lib, "WindowsCodecs.lib")

HRESULT ImageExam::Initialize(HINSTANCE hInstance, int showCmd, LPCWSTR title, UINT width, UINT height)
{
    HRESULT hr;
    hr = CoInitialize(nullptr);
    if (FAILED(hr))
    {
        assert(false);
        return E_FAIL;
    }
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(mcpWICFactory.GetAddressOf()));
    if (FAILED(hr))
    {
        assert(false);
        return E_FAIL;
    }
    D2DFramework::Initialize(hInstance, showCmd);
    
    hr = loadWICImage(L"Data/32.bmp", mcpBitmap.GetAddressOf());
    if (FAILED(hr))
    {
        assert(false);
        return E_FAIL;
    }
    
    return S_OK;
}

void ImageExam::Render()
{
    mcpRenderTarget->BeginDraw();
    mcpRenderTarget->Clear(D2D1::ColorF(0.f, 0.2f, 0.4f, 1.f));

    mcpRenderTarget->DrawBitmap(mcpBitmap.Get());

    mcpRenderTarget->EndDraw();
}

HRESULT ImageExam::loadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
    Microsoft::WRL::ComPtr<IWICBitmapDecoder> cpBmDecoder;
    HRESULT hr;
    // 1. 디코더 생성
    hr = mcpWICFactory->CreateDecoderFromFilename(
        filename,
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        cpBmDecoder.GetAddressOf()
    );
    AssertIfFailed(hr);
    
    // 2. 디코더에서 프레임 획득
    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> cpFrame;
    hr = cpBmDecoder->GetFrame(0, cpFrame.GetAddressOf());
    AssertIfFailed(hr);

    // 3. 컨버터를 사용해 데이터를 변환
    Microsoft::WRL::ComPtr<IWICFormatConverter> cpConverter;
    hr = mcpWICFactory->CreateFormatConverter(cpConverter.GetAddressOf());
    AssertIfFailed(hr);

    hr = cpConverter->Initialize(
        cpFrame.Get(),
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0,
        WICBitmapPaletteTypeCustom
    );
    AssertIfFailed(hr);

    // 4. 변환된 데이터에서 비트맵 생성
    hr = mcpRenderTarget->CreateBitmapFromWicBitmap(
        cpConverter.Get(),
        mcpBitmap.ReleaseAndGetAddressOf()
    );
    AssertIfFailed(hr);


    return S_OK;
}

HRESULT ImageExam::loadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);

    BITMAPFILEHEADER bitFileHeader;
    BITMAPINFOHEADER bitInfoHeader;

    ifs.read(reinterpret_cast<char*>(&bitFileHeader), sizeof(BITMAPFILEHEADER));
    ifs.read(reinterpret_cast<char*>(&bitInfoHeader), sizeof(BITMAPINFOHEADER));

    if (bitFileHeader.bfType != 0x4D42)
    {
        assert(false);
        return E_FAIL;
    }

    if (bitInfoHeader.biBitCount != 32)
    {
        assert(false);
        return E_FAIL;
    }

    std::vector<char> pPixels(bitInfoHeader.biSizeImage);

    ifs.seekg(bitFileHeader.bfOffBits);

    //ifs.read(&pPixels[0], bitInfoHeader.biSizeImage);
    int pitch = bitInfoHeader.biWidth * (bitInfoHeader.biBitCount / 8);

    // 비트맵 거꾸로 된거 뒤집기 위해서 역순으로 읽기.
    //for (int y = bitInfoHeader.biHeight - 1; y >= 0; --y)
    //{
    //    ifs.read(&pPixels[y * pitch], pitch);
    //}

    // 32.bmp 이미지 배경색 수동 제거 하기.
    int idx{};
    for (int y = bitInfoHeader.biHeight - 1; y >= 0; --y)
    {
        idx = y * pitch;
        for (int x = 0; x < bitInfoHeader.biWidth; ++x)
        {
            char r{}, g{}, b{}, a{};
            ifs.read(&b, 1);
            ifs.read(&g, 1);
            ifs.read(&r, 1);
            ifs.read(&a, 1);

            if (r == 30 || g == 199 || r == 250)
            {
                r = g = b = a = 0;
            }

            pPixels[idx++] = b;
            pPixels[idx++] = g;
            pPixels[idx++] = r;
            pPixels[idx++] = a;
            
        }
    }


    ifs.close();

    HRESULT hr = mcpRenderTarget->CreateBitmap(
        D2D1::SizeU(bitInfoHeader.biWidth, bitInfoHeader.biHeight),
        D2D1::BitmapProperties(
            D2D1::PixelFormat(
                DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED
            )
        ),
        ppBitmap
    );

    if (FAILED(hr))
    {
        assert(false);
        return E_FAIL;
    }

    (*ppBitmap)->CopyFromMemory(
        nullptr,
        &pPixels[0],
        pitch
    );

    return S_OK;
}

void ImageExam::Release()
{
    mcpBitmap.Reset();
    mcpWICFactory.Reset();
    CoUninitialize();
    D2DFramework::Release();

}
