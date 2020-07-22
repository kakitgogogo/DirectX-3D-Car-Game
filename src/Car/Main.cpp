#include <d3d11.h>
#include "D3DApp.h"
#include "Vertex.h"
#include "Geometry.h"
#include "RenderStates.h"
#include "Camera.h"
#include "Model.h"
#include "SkyBox.h"

using namespace DirectX;

class CarGameApp : public D3DApp 
{
public:
	CarGameApp(HINSTANCE hInstance);
	~CarGameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM wParam, int x, int y);
	void OnMouseUp(WPARAM wParam, int x, int y);
	void OnMouseMove(WPARAM wParam, int x, int y);
	void OnMouseWheel(WPARAM wParam, int x, int y);

private:
	void InitResource();

	// 车的前进
	void CarWalk(float d);
	// 车的旋转
	void CarRotate(float angle);

	// 获得车的世界矩阵
	XMMATRIX getCarWorldMatrix();

private:
	BasicEffect mBasicEffect;

	FirstPersonCamera mFirstPersonCamera;
	ThirdPersonCamera mThirdPersonCamera;
	CameraMode mCameraMode;
	ICamera* mCurrentCamera;

	bool mIsResized;              // 窗口是否改变了大小

	Model mGround;                // 地面模型
	Model mCarBody;               // 车身模型
	Model mCarWheelFrontLeft;     // 左前轮模型
	Model mCarWheelFrontRight;    // 右前轮模型
	Model mCarWheelBehindLeft;    // 左后轮模型
	Model mCarWheelBehindRight;   // 右后轮模型

	XMFLOAT3 mCarPosition;        // 车的位置
	XMFLOAT3 mCarFront;           // 车的朝向

	float mCarSpeed;              // 车速
	float mCarSpeedMax;           // 最大车速
	float mCarAcceleration;       // 车的加速度
	float mFriction;              // 车与地面的摩擦系数

	float mCarFrontWheelAngleX;   // 车轮前滚的角度
	float mCarFrontWheelAngleY;   // 车轮左右旋转的角度

	DirectionalLight mDirLights[3];  // 灯光

	SkyBox mSkyBox;               // 天空盒
	SkyEffect mSkyEffect; 

	POINT mLastMousePos;          // 上一次记录鼠标的位置
};

CarGameApp::CarGameApp(HINSTANCE hInstance)
	: D3DApp(hInstance), mIsResized(false),
	mCarPosition(0.0f, 0.0f, 0.0f),
	mCarFront(0.0f, 0.0f, 1.0f),
	mCarSpeed(0.0f), 
	mCarSpeedMax(6.0f), 
	mCarAcceleration(3.0f), 
	mFriction(1.5f), 
	mCarFrontWheelAngleX(0.0f),
	mCarFrontWheelAngleY(0.0f)
{
}

CarGameApp::~CarGameApp()
{
}

bool CarGameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	RenderStates::InitAll(md3dDevice);

	if (!mBasicEffect.InitAll(md3dDevice))
		return false;
	if (!mSkyEffect.InitAll(md3dDevice))
		return false;

	InitResource();

	return true;
}

void CarGameApp::InitResource()
{
	// 
	// 初始化游戏模型
	//

	// 地面
	CreatePlane(100.0f, 100.0f, mGround.GetMeshData());
	mGround.SetBuffer(md3dDevice);
	mGround.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mGround.SetTexture(md3dDevice, L"Textures\\floor.dds");
	mGround.SetTexTransformMatrix(XMMatrixScaling(20.0f, 20.0f, 0.0f));

	// 长方体： 车身
	CreateCube(2.0f, 2.0f, 4.0f, mCarBody.GetMeshData());
	mCarBody.SetBuffer(md3dDevice);
	mCarBody.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mCarBody.SetColor(Colors::White);

	// 圆柱体： 左前车轮
	CreateCylinder(0.4f, 0.4f, 0.5f, 20, 20, mCarWheelFrontLeft.GetMeshData());
	mCarWheelFrontLeft.SetBuffer(md3dDevice);
	mCarWheelFrontLeft.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mCarWheelFrontLeft.SetColor(Colors::Gray);

	// 圆柱体： 右前车轮
	CreateCylinder(0.4f, 0.4f, 0.5f, 20, 20, mCarWheelFrontRight.GetMeshData());
	mCarWheelFrontRight.SetBuffer(md3dDevice);
	mCarWheelFrontRight.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mCarWheelFrontRight.SetColor(Colors::Gray);

	// 圆柱体： 左后车轮
	CreateCylinder(0.4f, 0.4f, 0.5f, 20, 20, mCarWheelBehindLeft.GetMeshData());
	mCarWheelBehindLeft.SetBuffer(md3dDevice);
	mCarWheelBehindLeft.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mCarWheelBehindLeft.SetColor(Colors::Gray);

	// 圆柱体： 右后车轮
	CreateCylinder(0.4f, 0.4f, 0.5f, 20, 20, mCarWheelBehindRight.GetMeshData());
	mCarWheelBehindRight.SetBuffer(md3dDevice);
	mCarWheelBehindRight.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mCarWheelBehindRight.SetColor(Colors::Gray);

	//
	// 初始化灯光
	//

	mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);
	mBasicEffect.SetDirLights(mDirLights);

	//
	// 初始化天空盒
	//

	mSkyBox.Init(md3dDevice, L"Textures\\skybox.dds", 5000.0f);

	//
	// 初始化摄像机
	//

	mFirstPersonCamera.Init(XMFLOAT3(mCarPosition.x, 4.0f, mCarPosition.z), XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	mThirdPersonCamera.Init(10.0f, 0.25f * XM_PI, -0.75f * XM_PI, 5.0f, 20.0f);

	// 默认第三人称摄像机
	mCameraMode = CameraMode::ThirdPerson;
	mCurrentCamera = &mThirdPersonCamera;
}

void CarGameApp::OnResize()
{
	D3DApp::OnResize();

	// 更新视锥体
	mFirstPersonCamera.SetFrustum(0.25 * XM_PI, AspectRatio(), 1.0f, 1000.f);
	mThirdPersonCamera.SetFrustum(0.25 * XM_PI, AspectRatio(), 1.0f, 1000.f);

	mIsResized = true;
}

void CarGameApp::UpdateScene(float dt)
{
	// 如果按下W键，按照加速度增加速度；如果没按下W键，按照摩擦系数降低速度
	if (GetAsyncKeyState('W') & 0x8000)
	{
		mCarSpeed += (mCarAcceleration * dt);
	}
	else
	{
		mCarSpeed -= (mFriction * dt);
	}

	// 如果按下S键，按照两倍加速度降低速度
	if (GetAsyncKeyState('S') & 0x8000)
	{
		mCarSpeed -= (2.0f*mCarAcceleration * dt);
	}

	// 按当前速度前进
	mCarSpeed = max(min(mCarSpeed, mCarSpeedMax), 0.0f);
	CarWalk(dt * mCarSpeed);

	// 车轮的前滚角度，由当前速度决定前滚速度
	mCarFrontWheelAngleX = XMScalarModAngle(mCarFrontWheelAngleX + (mCarSpeed / mCarSpeedMax) * XM_2PI * dt);

	// 车的转向，如果车速为0，则不能转向
	mCarFrontWheelAngleY = 0.0f;
	if ((GetAsyncKeyState('A') & 0x8000) && mCarSpeed > 0.0f)
	{
		CarRotate(dt * -2.0f);
		mCarFrontWheelAngleY = -XM_PI / 9;
	}

	if ((GetAsyncKeyState('D') & 0x8000) && mCarSpeed > 0.0f)
	{
		CarRotate(dt * 2.0f);
		mCarFrontWheelAngleY = XM_PI / 9;
	}

	// 按下V，改变摄像头模式；使用GetAsyncKeyState()返回值的最低位来防止按下时重复触发
	SHORT btnState = GetAsyncKeyState('V');
	if ((btnState & 0x8000) && (btnState & 0x0001))
	{
		if (mCameraMode == CameraMode::FirstPerson) 
			mCameraMode = CameraMode::ThirdPerson;
		else
			mCameraMode = CameraMode::FirstPerson;
	}

	if (mCameraMode == CameraMode::FirstPerson)
	{
		// 如果是第一人称摄像头，更新摄像机位置
		mFirstPersonCamera.SetPosition(mCarPosition.x, 4.0f, mCarPosition.z);
		mCurrentCamera = &mFirstPersonCamera;
	}
	else
	{
		// 如果是第三人称摄像头，更新目标位置
		mThirdPersonCamera.SetTargetPosition(mCarPosition);
		mCurrentCamera = &mThirdPersonCamera;
	}

	// 更新观察矩阵、摄像机位置
	mCurrentCamera->UpdateViewMatrix();
	mBasicEffect.SetEyePos(mCurrentCamera->GetPositionXM());
	mBasicEffect.SetViewMatrix(mCurrentCamera->GetView());

	// 如果窗口改变了大小，更新投影矩阵
	if (mIsResized)
	{
		mBasicEffect.SetProjMatrix(mCurrentCamera->GetProj());
		mIsResized = false;
	}
}

void CarGameApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	// 清空场景
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//
	// 绘制天空盒
	//

	mSkyEffect.ResetRenderStates(md3dImmediateContext);
	mSkyBox.Draw(md3dImmediateContext, &mSkyEffect, mCurrentCamera);

	//
	// 绘制地面和车
	//

	// 重置渲染状态
	mBasicEffect.ResetRenderStates(md3dImmediateContext);

	// 绘制地面
	mGround.Draw(md3dImmediateContext, &mBasicEffect);

	XMMATRIX world = getCarWorldMatrix();

	// 绘制车身
	mCarBody.SetWorldMatrix(XMMatrixTranslation(0.0f, 1.4f, 0.0f) * world);
	mCarBody.Draw(md3dImmediateContext, &mBasicEffect);
	mCarBody.DrawShadow(md3dImmediateContext, &mBasicEffect, &mDirLights[0].Direction);

	// 绘制左前车轮
	mCarWheelFrontLeft.SetWorldMatrix(XMMatrixRotationZ(XM_PIDIV2) * XMMatrixRotationX(mCarFrontWheelAngleX) * XMMatrixRotationY(mCarFrontWheelAngleY) * XMMatrixTranslation(-1.0f, 0.4f, 1.0f) * world);
	mCarWheelFrontLeft.Draw(md3dImmediateContext, &mBasicEffect);
	mCarWheelFrontLeft.DrawShadow(md3dImmediateContext, &mBasicEffect, &mDirLights[0].Direction);

	// 绘制右前车轮
	mCarWheelFrontRight.SetWorldMatrix(XMMatrixRotationZ(XM_PIDIV2) * XMMatrixRotationX(mCarFrontWheelAngleX) * XMMatrixRotationY(mCarFrontWheelAngleY) * XMMatrixTranslation(1.0f, 0.4f, 1.0f) * world);
	mCarWheelFrontRight.Draw(md3dImmediateContext, &mBasicEffect);
	mCarWheelFrontRight.DrawShadow(md3dImmediateContext, &mBasicEffect, &mDirLights[0].Direction);

	// 绘制左后车轮
	mCarWheelBehindLeft.SetWorldMatrix(XMMatrixRotationZ(XM_PIDIV2) * XMMatrixRotationX(mCarFrontWheelAngleX) * XMMatrixTranslation(-1.0f, 0.4f, -1.0f) * world);
	mCarWheelBehindLeft.Draw(md3dImmediateContext, &mBasicEffect);
	mCarWheelBehindLeft.DrawShadow(md3dImmediateContext, &mBasicEffect, &mDirLights[0].Direction);

	// 绘制右后车轮
	mCarWheelBehindRight.SetWorldMatrix(XMMatrixRotationZ(XM_PIDIV2) * XMMatrixRotationX(mCarFrontWheelAngleX) * XMMatrixTranslation(1.0f, 0.4f, -1.0f) * world);
	mCarWheelBehindRight.Draw(md3dImmediateContext, &mBasicEffect);
	mCarWheelBehindRight.DrawShadow(md3dImmediateContext, &mBasicEffect, &mDirLights[0].Direction);

	HR(mSwapChain->Present(0, 0));
}

void CarGameApp::OnMouseDown(WPARAM wParam, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void CarGameApp::OnMouseUp(WPARAM wParam, int x, int y)
{
	ReleaseCapture();
}

void CarGameApp::OnMouseMove(WPARAM wParam, int x, int y) {
	if ((wParam & MK_LBUTTON) != 0) {
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		mCurrentCamera->RotateLeftRight(dx);
		mCurrentCamera->RotateUpDown(dy);
	}
	else if ((wParam & MK_RBUTTON) != 0) {
		float dx = 0.005f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f*static_cast<float>(y - mLastMousePos.y);

		mThirdPersonCamera.ChangeDistance(-dy);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void CarGameApp::OnMouseWheel(WPARAM wParam, int x, int y)
{
	WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
	short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

	if(mCameraMode == CameraMode::ThirdPerson)
		mThirdPersonCamera.ChangeDistance(-0.005f * static_cast<float>(zDelta));
}

void CarGameApp::CarWalk(float d)
{
	// 按照车的朝向前进
	XMVECTOR Pos = XMLoadFloat3(&mCarPosition);
	XMVECTOR Front = XMLoadFloat3(&mCarFront);
	XMVECTOR Dist = XMVectorReplicate(d);
	// DestPos = Dist * Front + SrcPos
	XMStoreFloat3(&mCarPosition, XMVectorMultiplyAdd(Dist, Front, Pos));
}

void CarGameApp::CarRotate(float rad)
{
	// 绕Y轴旋转
	XMMATRIX R = XMMatrixRotationY(rad);

	XMStoreFloat3(&mCarFront, XMVector3TransformNormal(XMLoadFloat3(&mCarFront), R));
}

XMMATRIX CarGameApp::getCarWorldMatrix()
{
	XMVECTOR F = XMLoadFloat3(&mCarFront);
	XMVECTOR U = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR P = XMLoadFloat3(&mCarPosition);
	XMVECTOR R = XMVector3Cross(U, F);

	XMFLOAT3 right;
	XMStoreFloat3(&right, R);

	return XMMatrixSet(
		right.x, right.y, right.z, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		mCarFront.x, mCarFront.y, mCarFront.z, 0.0f,
		mCarPosition.x, mCarPosition.y, mCarPosition.z, 1.0f
	);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) 
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	CarGameApp theApp(hInstance);

	if (!theApp.Init()) return 0;

	// 进入游戏循环
	return theApp.Run();
}