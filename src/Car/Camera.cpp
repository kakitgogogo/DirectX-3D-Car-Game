#include "Camera.h"
using namespace DirectX;

ICamera::ICamera()
{
	SetFrustum(0.25f*XM_PI, 1.0f, 1.0f, 1000.0f);
}

ICamera::~ICamera() {}

DirectX::XMVECTOR ICamera::GetPositionXM() const
{
	return XMLoadFloat3(&mPosition);
}

DirectX::XMFLOAT3 ICamera::GetPosition() const
{
	return mPosition;
}

DirectX::XMVECTOR ICamera::GetRightXM() const
{
	return XMLoadFloat3(&mRight);
}

DirectX::XMFLOAT3 ICamera::GetRight() const
{
	return mRight;
}

DirectX::XMVECTOR ICamera::GetUpXM() const
{
	return XMLoadFloat3(&mUp);
}

DirectX::XMFLOAT3 ICamera::GetUp() const
{
	return mUp;
}

DirectX::XMVECTOR ICamera::GetLookXM() const
{
	return XMLoadFloat3(&mLook);
}

DirectX::XMFLOAT3 ICamera::GetLook() const
{
	return mLook;
}

float ICamera::GetNearWindowWidth() const
{
	return mAspect * mNearWindowHeight;
}

float ICamera::GetNearWindowHeight() const
{
	return mNearWindowHeight;
}

float ICamera::GetFarWindowWidth() const
{
	return mAspect * mFarWindowHeight;
}

float ICamera::GetFarWindowHeight() const
{
	return mFarWindowHeight;
}

DirectX::XMMATRIX ICamera::GetView() const
{
	return XMLoadFloat4x4(&mView);
}

DirectX::XMMATRIX ICamera::GetProj() const
{
	return XMLoadFloat4x4(&mProj);
}

DirectX::XMMATRIX ICamera::GetViewProj() const
{
	return XMLoadFloat4x4(&mView) * XMLoadFloat4x4(&mProj);
}

void ICamera::SetFrustum(float fovY, float aspect, float nearZ, float farZ)
{
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = nearZ;
	mFarZ = farZ;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	// ʹ��XMMatrixPerspectiveFovLH��������ͶӰ����
	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ));
}

//
// ��һ�˳��������ʵ��
//

FirstPersonCamera::FirstPersonCamera() {}

FirstPersonCamera::~FirstPersonCamera() {}

void XM_CALLCONV FirstPersonCamera::Init(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp)
{
	XMVECTOR look = XMVector3Normalize(target - pos);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(worldUp, look));
	XMVECTOR up = XMVector3Cross(look, right);  // L��R���ഹֱ�����Ϊ��λ����

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, look);
	XMStoreFloat3(&mRight, right);
	XMStoreFloat3(&mUp, up);
}

void FirstPersonCamera::Init(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& worldUp)
{
	Init(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&worldUp));
}

void FirstPersonCamera::SetPosition(float x, float y, float z)
{
	SetPosition(XMFLOAT3(x, y, z));
}

void FirstPersonCamera::SetPosition(const DirectX::XMFLOAT3& pos)
{
	mPosition = pos;
}

void FirstPersonCamera::RotateUpDown(float angle)
{
	// �ƹ۲�����ϵ��right����ת�����¼���up���look��
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);
	XMVECTOR up = XMVector3TransformNormal(XMLoadFloat3(&mUp), R);
	XMVECTOR look = XMVector3TransformNormal(XMLoadFloat3(&mLook), R);

	XMStoreFloat3(&mUp, up);
	XMStoreFloat3(&mLook, look);
}

void FirstPersonCamera::RotateLeftRight(float angle)
{
	// ����������ϵ��z����ת�����¼���۲�����ϵ��right�ᡢup���look��
	XMMATRIX R = XMMatrixRotationY(angle);
	XMVECTOR right = XMVector3TransformNormal(XMLoadFloat3(&mRight), R);
	XMVECTOR up = XMVector3TransformNormal(XMLoadFloat3(&mUp), R);
	XMVECTOR look = XMVector3TransformNormal(XMLoadFloat3(&mLook), R);

	XMStoreFloat3(&mRight, right);
	XMStoreFloat3(&mUp, up);
	XMStoreFloat3(&mLook, look);
}

void FirstPersonCamera::UpdateViewMatrix()
{
	XMVECTOR right = XMLoadFloat3(&mRight);
	XMVECTOR up = XMLoadFloat3(&mUp);
	XMVECTOR look = XMLoadFloat3(&mLook);
	XMVECTOR pos = XMLoadFloat3(&mPosition);

	// ��������ϵԭ���ڹ۲�����ϵ��λ��
	float x = -XMVectorGetX(XMVector3Dot(pos, right));
	float y = -XMVectorGetX(XMVector3Dot(pos, up));
	float z = -XMVectorGetX(XMVector3Dot(pos, look));

	// ���¹۲����
	mView = {
		mRight.x, mUp.x, mLook.x, 0.0f,
		mRight.y, mUp.y, mLook.y, 0.0f,
		mRight.z, mUp.z, mLook.z, 0.0f,
		x, y, z, 1.0f
	};
}

//
// �����˳��������ʵ��
//

ThirdPersonCamera::ThirdPersonCamera() {}

ThirdPersonCamera::~ThirdPersonCamera() {}

void ThirdPersonCamera::Init(float distance, float phi, float theta, float distanceMin, float distanceMax)
{
	mDistance = distance; 
	mPhi = phi; 
	mTheta = theta;
	mDistanceMin = distanceMin; 
	mDistanceMax = distanceMax;
	mTarget = { 0.0f, 0.0f, 0.0f };
}

void ThirdPersonCamera::ChangeDistance(float d)
{
	SetDistance(mDistance + d);
}

void ThirdPersonCamera::RotateUpDown(float angle)
{
	SetPhi(mPhi - angle);
}

void ThirdPersonCamera::RotateLeftRight(float angle)
{
	SetTheta(mTheta - angle);
}

void ThirdPersonCamera::SetTargetPosition(const DirectX::XMFLOAT3& target)
{
	mTarget = target;
}

void ThirdPersonCamera::SetDistance(float distance)
{
	// ��Ŀ��ľ����޶���[mDistanceMin, mDistanceMax]
	mDistance = min(max(distance, mDistanceMin), mDistanceMax);
}

void ThirdPersonCamera::SetDistanceMinMax(float distanceMin, float distanceMax)
{
	mDistanceMin = distanceMin;
	mDistanceMax = distanceMax;
}

void ThirdPersonCamera::SetPhi(float phi)
{
	// ������Ұ�Ƕ��޶���[pi/6, pi/2]
	mPhi = min(max(phi, XM_PI / 6), XM_PIDIV2);
}

void ThirdPersonCamera::SetTheta(float theta)
{
	// ������Ұ�Ƕ��޶���[-pi, pi]
	mTheta = XMScalarModAngle(theta);
}

void ThirdPersonCamera::UpdateViewMatrix()
{
	// ��������ϵ
	float x = mTarget.x + mDistance * sinf(mPhi) * cosf(mTheta);
	float z = mTarget.z + mDistance * sinf(mPhi) * sinf(mTheta);
	float y = mTarget.y + mDistance * cosf(mPhi);
	mPosition = { x, y, z };

	XMVECTOR pos = XMLoadFloat3(&mPosition);
	XMVECTOR look = XMVector3Normalize(XMLoadFloat3(&mTarget) - pos);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), look));
	XMVECTOR up = XMVector3Cross(look, right);

	// ��������
	XMStoreFloat3(&mRight, right);
	XMStoreFloat3(&mUp, up);
	XMStoreFloat3(&mLook, look);

	// ��������ϵԭ���ڹ۲�����ϵ��λ��
	x = -XMVectorGetX(XMVector3Dot(pos, right));
	y = -XMVectorGetX(XMVector3Dot(pos, up));
	z = -XMVectorGetX(XMVector3Dot(pos, look));

	mView = {
		mRight.x, mUp.x, mLook.x, 0.0f,
		mRight.y, mUp.y, mLook.y, 0.0f,
		mRight.z, mUp.z, mLook.z, 0.0f,
		x, y, z, 1.0f
	};
}