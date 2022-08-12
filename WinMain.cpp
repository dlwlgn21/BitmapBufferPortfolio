#include <cassert>
#include <Windows.h>
#include <d2d1.h>
#include <math.h>
#include <sstream>
#include "BitmapExam.h"

#pragma comment (lib, "d2d1.lib")

BitmapExam myFramework;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,				// OS가 실행시킨 현재 인스턴스의 핸들
	_In_opt_ HINSTANCE hPrevInstance,		// 이전 인스턴스의 핸들. 항상 NULL
	_In_ LPSTR lpCmdLine,					// 명령줄. 실행파일 뒤에 추가 명령
	_In_ int nShowCmd						// 일반 윈도우로 표시할지, 전체 화면이나 최소화 화면으로 표시할지에 대한 값
)
{
	int ret = 0;
	HRESULT hr = myFramework.Initialize(hInstance, nShowCmd);
	if (SUCCEEDED(hr))
	{
		ret = myFramework.GameLoop();
	}
	else
	{
		assert(false);
	}

	myFramework.Release();

	return ret;

}
