#include <cassert>
#include <Windows.h>
#include <d2d1.h>
#include <math.h>
#include <sstream>
#include "BitmapExam.h"

#pragma comment (lib, "d2d1.lib")

BitmapExam myFramework;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,				// OS�� �����Ų ���� �ν��Ͻ��� �ڵ�
	_In_opt_ HINSTANCE hPrevInstance,		// ���� �ν��Ͻ��� �ڵ�. �׻� NULL
	_In_ LPSTR lpCmdLine,					// �����. �������� �ڿ� �߰� ���
	_In_ int nShowCmd						// �Ϲ� ������� ǥ������, ��ü ȭ���̳� �ּ�ȭ ȭ������ ǥ�������� ���� ��
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
