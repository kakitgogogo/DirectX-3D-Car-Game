#pragma once

#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Effects.h"
#include "Utils.h"
#include "GameTimer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")

// D3DApp��ο� ��Introduction to 3D Game Programming with DirectX11�� �Ŀ��
// �û��������ʼ�����ڡ���ʼ��Direct3D����Ϣ�¼�����ȷ�����ʵ��
// �̳�D3DAppʵ�����ǵ���ϷӦ��
class D3DApp {
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	// ��ȡӦ��ʵ���ľ��
	HINSTANCE AppInst() const;
	// ��ȡ���ھ��
	HWND MainWnd() const;
	// ��ȡ���ڿ�߱�
	float AspectRatio() const;

	// ִ����Ϣ�¼�����Ⱦ��ѭ��
	int Run();

	// ���ڡ�Direct3D����Դ�ĳ�ʼ��
	virtual bool Init();
	// ���ڴ�С�䶯�ص�����
	virtual void OnResize();
	// ��Ϣ�¼�������
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ����ÿһ֡�ı���
	virtual void UpdateScene(float dt) = 0;
	// ����ÿһ֡
	virtual void DrawScene() = 0;

	// ����¼��ص�����
	virtual void OnMouseDown(WPARAM wParam, int x, int y) {}
	virtual void OnMouseUp(WPARAM wParam, int x, int y) {}
	virtual void OnMouseMove(WPARAM wParam, int x, int y) {}
	virtual void OnMouseWheel(WPARAM wParam, int x, int y) {}

protected:
	// ��ʼ������
	bool InitMainWindow();
	// ��ʼ��Direct3D
	bool InitDirect3D();

	// ����֡��
	void CalculateFrameStats();

protected:
	HINSTANCE mhAppInst;    // Ӧ��ʵ�����
	HWND mhMainWnd;         // ���ھ��
	bool mAppPaused;        // Ӧ���Ƿ���ͣ
	bool mMinimized;        // Ӧ���Ƿ���С��
	bool mMaximized;        // Ӧ���Ƿ����
	bool mResizing;         // ���ڴ�С�Ƿ����ڸı�
	UINT m4xMsaaQuality;    // MSAA֧�ֵ������ȼ�

	GameTimer mTimer;       // ��ʱ��

	D3D_DRIVER_TYPE md3dDriveType;               // D3D��������
	ID3D11Device* md3dDevice;                    // D3D11�豸
	ID3D11DeviceContext* md3dImmediateContext;   // D3D11�豸������
	IDXGISwapChain* mSwapChain;                  // D3D11������
	ID3D11Texture2D* mDepthStencilBuffer;        // ���ģ�建����
	ID3D11RenderTargetView* mRenderTargetView;   // ��ȾĿ����ͼ
	ID3D11DepthStencilView* mDepthStencilView;   // ���ģ����ͼ
	D3D11_VIEWPORT mScreenViewport;              // �ӿ�

	std::wstring mMainWndCaption;                // ���ڱ���
	int mClientWidth;                            // ���ڿ��
	int mClientHeight;                           // ���ڸ߶�
	bool mEnable4xMsaa;                          // �Ƿ���MSAA
};
