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

// D3DApp类参考 《Introduction to 3D Game Programming with DirectX11》 的框架
// 该基类包括初始化窗口、初始化Direct3D、消息事件处理等方法的实现
// 继承D3DApp实现我们的游戏应用
class D3DApp {
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	// 获取应用实例的句柄
	HINSTANCE AppInst() const;
	// 获取窗口句柄
	HWND MainWnd() const;
	// 获取窗口宽高比
	float AspectRatio() const;

	// 执行消息事件、渲染的循环
	int Run();

	// 窗口、Direct3D等资源的初始化
	virtual bool Init();
	// 窗口大小变动回调函数
	virtual void OnResize();
	// 消息事件处理函数
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// 更新每一帧的变量
	virtual void UpdateScene(float dt) = 0;
	// 绘制每一帧
	virtual void DrawScene() = 0;

	// 鼠标事件回调函数
	virtual void OnMouseDown(WPARAM wParam, int x, int y) {}
	virtual void OnMouseUp(WPARAM wParam, int x, int y) {}
	virtual void OnMouseMove(WPARAM wParam, int x, int y) {}
	virtual void OnMouseWheel(WPARAM wParam, int x, int y) {}

protected:
	// 初始化窗口
	bool InitMainWindow();
	// 初始化Direct3D
	bool InitDirect3D();

	// 计算帧数
	void CalculateFrameStats();

protected:
	HINSTANCE mhAppInst;    // 应用实例句柄
	HWND mhMainWnd;         // 窗口句柄
	bool mAppPaused;        // 应用是否暂停
	bool mMinimized;        // 应用是否最小化
	bool mMaximized;        // 应用是否最大化
	bool mResizing;         // 窗口大小是否正在改变
	UINT m4xMsaaQuality;    // MSAA支持的质量等级

	GameTimer mTimer;       // 计时器

	D3D_DRIVER_TYPE md3dDriveType;               // D3D驱动类型
	ID3D11Device* md3dDevice;                    // D3D11设备
	ID3D11DeviceContext* md3dImmediateContext;   // D3D11设备上下文
	IDXGISwapChain* mSwapChain;                  // D3D11交换链
	ID3D11Texture2D* mDepthStencilBuffer;        // 深度模板缓冲区
	ID3D11RenderTargetView* mRenderTargetView;   // 渲染目标视图
	ID3D11DepthStencilView* mDepthStencilView;   // 深度模板视图
	D3D11_VIEWPORT mScreenViewport;              // 视口

	std::wstring mMainWndCaption;                // 窗口标题
	int mClientWidth;                            // 窗口宽度
	int mClientHeight;                           // 窗口高度
	bool mEnable4xMsaa;                          // 是否开启MSAA
};
