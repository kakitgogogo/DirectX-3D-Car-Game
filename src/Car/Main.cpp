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

	// ����ǰ��
	void CarWalk(float d);
	// ������ת
	void CarRotate(float angle);

	// ��ó����������
	XMMATRIX getCarWorldMatrix();

private:
	BasicEffect mBasicEffect;

	FirstPersonCamera mFirstPersonCamera;
	ThirdPersonCamera mThirdPersonCamera;
	CameraMode mCameraMode;
	ICamera* mCurrentCamera;

	bool mIsResized;              // �����Ƿ�ı��˴�С

	Model mGround;                // ����ģ��
	Model mCarBody;               // ����ģ��
	Model mCarWheelFrontLeft;     // ��ǰ��ģ��
	Model mCarWheelFrontRight;    // ��ǰ��ģ��
	Model mCarWheelBehindLeft;    // �����ģ��
	Model mCarWheelBehindRight;   // �Һ���ģ��

	XMFLOAT3 mCarPosition;        // ����λ��
	XMFLOAT3 mCarFront;           // ���ĳ���

	float mCarSpeed;              // ����
	float mCarSpeedMax;           // �����
	float mCarAcceleration;       // ���ļ��ٶ�
	float mFriction;              // ��������Ħ��ϵ��

	float mCarFrontWheelAngleX;   // ����ǰ���ĽǶ�
	float mCarFrontWheelAngleY;   // ����������ת�ĽǶ�

	DirectionalLight mDirLights[3];  // �ƹ�

	SkyBox mSkyBox;               // ��պ�
	SkyEffect mSkyEffect; 

	POINT mLastMousePos;          // ��һ�μ�¼����λ��
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
	// ��ʼ����Ϸģ��
	//

	// ����
	CreatePlane(100.0f, 100.0f, mGround.GetMeshData());
	mGround.SetBuffer(md3dDevice);
	mGround.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mGround.SetTexture(md3dDevice, L"Textures\\floor.dds");
	mGround.SetTexTransformMatrix(XMMatrixScaling(20.0f, 20.0f, 0.0f));

	// �����壺 ����
	CreateCube(2.0f, 2.0f, 4.0f, mCarBody.GetMeshData());
	mCarBody.SetBuffer(md3dDevice);
	mCarBody.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mCarBody.SetColor(Colors::White);

	// Բ���壺 ��ǰ����
	CreateCylinder(0.4f, 0.4f, 0.5f, 20, 20, mCarWheelFrontLeft.GetMeshData());
	mCarWheelFrontLeft.SetBuffer(md3dDevice);
	mCarWheelFrontLeft.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mCarWheelFrontLeft.SetColor(Colors::Gray);

	// Բ���壺 ��ǰ����
	CreateCylinder(0.4f, 0.4f, 0.5f, 20, 20, mCarWheelFrontRight.GetMeshData());
	mCarWheelFrontRight.SetBuffer(md3dDevice);
	mCarWheelFrontRight.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mCarWheelFrontRight.SetColor(Colors::Gray);

	// Բ���壺 �����
	CreateCylinder(0.4f, 0.4f, 0.5f, 20, 20, mCarWheelBehindLeft.GetMeshData());
	mCarWheelBehindLeft.SetBuffer(md3dDevice);
	mCarWheelBehindLeft.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mCarWheelBehindLeft.SetColor(Colors::Gray);

	// Բ���壺 �Һ���
	CreateCylinder(0.4f, 0.4f, 0.5f, 20, 20, mCarWheelBehindRight.GetMeshData());
	mCarWheelBehindRight.SetBuffer(md3dDevice);
	mCarWheelBehindRight.SetMaterial(XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f), XMFLOAT4());
	mCarWheelBehindRight.SetColor(Colors::Gray);

	//
	// ��ʼ���ƹ�
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
	// ��ʼ����պ�
	//

	mSkyBox.Init(md3dDevice, L"Textures\\skybox.dds", 5000.0f);

	//
	// ��ʼ�������
	//

	mFirstPersonCamera.Init(XMFLOAT3(mCarPosition.x, 4.0f, mCarPosition.z), XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	mThirdPersonCamera.Init(10.0f, 0.25f * XM_PI, -0.75f * XM_PI, 5.0f, 20.0f);

	// Ĭ�ϵ����˳������
	mCameraMode = CameraMode::ThirdPerson;
	mCurrentCamera = &mThirdPersonCamera;
}

void CarGameApp::OnResize()
{
	D3DApp::OnResize();

	// ������׶��
	mFirstPersonCamera.SetFrustum(0.25 * XM_PI, AspectRatio(), 1.0f, 1000.f);
	mThirdPersonCamera.SetFrustum(0.25 * XM_PI, AspectRatio(), 1.0f, 1000.f);

	mIsResized = true;
}

void CarGameApp::UpdateScene(float dt)
{
	// �������W�������ռ��ٶ������ٶȣ����û����W��������Ħ��ϵ�������ٶ�
	if (GetAsyncKeyState('W') & 0x8000)
	{
		mCarSpeed += (mCarAcceleration * dt);
	}
	else
	{
		mCarSpeed -= (mFriction * dt);
	}

	// �������S���������������ٶȽ����ٶ�
	if (GetAsyncKeyState('S') & 0x8000)
	{
		mCarSpeed -= (2.0f*mCarAcceleration * dt);
	}

	// ����ǰ�ٶ�ǰ��
	mCarSpeed = max(min(mCarSpeed, mCarSpeedMax), 0.0f);
	CarWalk(dt * mCarSpeed);

	// ���ֵ�ǰ���Ƕȣ��ɵ�ǰ�ٶȾ���ǰ���ٶ�
	mCarFrontWheelAngleX = XMScalarModAngle(mCarFrontWheelAngleX + (mCarSpeed / mCarSpeedMax) * XM_2PI * dt);

	// ����ת���������Ϊ0������ת��
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

	// ����V���ı�����ͷģʽ��ʹ��GetAsyncKeyState()����ֵ�����λ����ֹ����ʱ�ظ�����
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
		// ����ǵ�һ�˳�����ͷ�����������λ��
		mFirstPersonCamera.SetPosition(mCarPosition.x, 4.0f, mCarPosition.z);
		mCurrentCamera = &mFirstPersonCamera;
	}
	else
	{
		// ����ǵ����˳�����ͷ������Ŀ��λ��
		mThirdPersonCamera.SetTargetPosition(mCarPosition);
		mCurrentCamera = &mThirdPersonCamera;
	}

	// ���¹۲���������λ��
	mCurrentCamera->UpdateViewMatrix();
	mBasicEffect.SetEyePos(mCurrentCamera->GetPositionXM());
	mBasicEffect.SetViewMatrix(mCurrentCamera->GetView());

	// ������ڸı��˴�С������ͶӰ����
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

	// ��ճ���
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//
	// ������պ�
	//

	mSkyEffect.ResetRenderStates(md3dImmediateContext);
	mSkyBox.Draw(md3dImmediateContext, &mSkyEffect, mCurrentCamera);

	//
	// ���Ƶ���ͳ�
	//

	// ������Ⱦ״̬
	mBasicEffect.ResetRenderStates(md3dImmediateContext);

	// ���Ƶ���
	mGround.Draw(md3dImmediateContext, &mBasicEffect);

	XMMATRIX world = getCarWorldMatrix();

	// ���Ƴ���
	mCarBody.SetWorldMatrix(XMMatrixTranslation(0.0f, 1.4f, 0.0f) * world);
	mCarBody.Draw(md3dImmediateContext, &mBasicEffect);
	mCarBody.DrawShadow(md3dImmediateContext, &mBasicEffect, &mDirLights[0].Direction);

	// ������ǰ����
	mCarWheelFrontLeft.SetWorldMatrix(XMMatrixRotationZ(XM_PIDIV2) * XMMatrixRotationX(mCarFrontWheelAngleX) * XMMatrixRotationY(mCarFrontWheelAngleY) * XMMatrixTranslation(-1.0f, 0.4f, 1.0f) * world);
	mCarWheelFrontLeft.Draw(md3dImmediateContext, &mBasicEffect);
	mCarWheelFrontLeft.DrawShadow(md3dImmediateContext, &mBasicEffect, &mDirLights[0].Direction);

	// ������ǰ����
	mCarWheelFrontRight.SetWorldMatrix(XMMatrixRotationZ(XM_PIDIV2) * XMMatrixRotationX(mCarFrontWheelAngleX) * XMMatrixRotationY(mCarFrontWheelAngleY) * XMMatrixTranslation(1.0f, 0.4f, 1.0f) * world);
	mCarWheelFrontRight.Draw(md3dImmediateContext, &mBasicEffect);
	mCarWheelFrontRight.DrawShadow(md3dImmediateContext, &mBasicEffect, &mDirLights[0].Direction);

	// ���������
	mCarWheelBehindLeft.SetWorldMatrix(XMMatrixRotationZ(XM_PIDIV2) * XMMatrixRotationX(mCarFrontWheelAngleX) * XMMatrixTranslation(-1.0f, 0.4f, -1.0f) * world);
	mCarWheelBehindLeft.Draw(md3dImmediateContext, &mBasicEffect);
	mCarWheelBehindLeft.DrawShadow(md3dImmediateContext, &mBasicEffect, &mDirLights[0].Direction);

	// �����Һ���
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
	// ���ճ��ĳ���ǰ��
	XMVECTOR Pos = XMLoadFloat3(&mCarPosition);
	XMVECTOR Front = XMLoadFloat3(&mCarFront);
	XMVECTOR Dist = XMVectorReplicate(d);
	// DestPos = Dist * Front + SrcPos
	XMStoreFloat3(&mCarPosition, XMVectorMultiplyAdd(Dist, Front, Pos));
}

void CarGameApp::CarRotate(float rad)
{
	// ��Y����ת
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

	// ������Ϸѭ��
	return theApp.Run();
}