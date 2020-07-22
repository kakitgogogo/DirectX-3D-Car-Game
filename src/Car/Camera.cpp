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

	// 使用XMMatrixPerspectiveFovLH函数计算投影矩阵
	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ));
}

//
// 第一人称摄像机的实现
//

FirstPersonCamera::FirstPersonCamera() {}

FirstPersonCamera::~FirstPersonCamera() {}

void XM_CALLCONV FirstPersonCamera::Init(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp)
{
	XMVECTOR look = XMVector3Normalize(target - pos);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(worldUp, look));
	XMVECTOR up = XMVector3Cross(look, right);  // L和R互相垂直，叉乘为单位向量

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
	// 绕观察坐标系的right轴旋转，重新计算up轴和look轴
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);
	XMVECTOR up = XMVector3TransformNormal(XMLoadFloat3(&mUp), R);
	XMVECTOR look = XMVector3TransformNormal(XMLoadFloat3(&mLook), R);

	XMStoreFloat3(&mUp, up);
	XMStoreFloat3(&mLook, look);
}

void FirstPersonCamera::RotateLeftRight(float angle)
{
	// 绕世界坐标系的z轴旋转，重新计算观察坐标系的right轴、up轴和look轴
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

	// 世界坐标系原点在观察坐标系的位置
	float x = -XMVectorGetX(XMVector3Dot(pos, right));
	float y = -XMVectorGetX(XMVector3Dot(pos, up));
	float z = -XMVectorGetX(XMVector3Dot(pos, look));

	// 更新观察矩阵
	mView = {
		mRight.x, mUp.x, mLook.x, 0.0f,
		mRight.y, mUp.y, mLook.y, 0.0f,
		mRight.z, mUp.z, mLook.z, 0.0f,
		x, y, z, 1.0f
	};
}

//
// 第三人称摄像机的实现
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
	// 与目标的距离限定在[mDistanceMin, mDistanceMax]
	mDistance = min(max(distance, mDistanceMin), mDistanceMax);
}

void ThirdPersonCamera::SetDistanceMinMax(float distanceMin, float distanceMax)
{
	mDistanceMin = distanceMin;
	mDistanceMax = distanceMax;
}

void ThirdPersonCamera::SetPhi(float phi)
{
	// 上下视野角度限定在[pi/6, pi/2]
	mPhi = min(max(phi, XM_PI / 6), XM_PIDIV2);
}

void ThirdPersonCamera::SetTheta(float theta)
{
	// 左右视野角度限定在[-pi, pi]
	mTheta = XMScalarModAngle(theta);
}

void ThirdPersonCamera::UpdateViewMatrix()
{
	// 球面坐标系
	float x = mTarget.x + mDistance * sinf(mPhi) * cosf(mTheta);
	float z = mTarget.z + mDistance * sinf(mPhi) * sinf(mTheta);
	float y = mTarget.y + mDistance * cosf(mPhi);
	mPosition = { x, y, z };

	XMVECTOR pos = XMLoadFloat3(&mPosition);
	XMVECTOR look = XMVector3Normalize(XMLoadFloat3(&mTarget) - pos);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), look));
	XMVECTOR up = XMVector3Cross(look, right);

	// 更新向量
	XMStoreFloat3(&mRight, right);
	XMStoreFloat3(&mUp, up);
	XMStoreFloat3(&mLook, look);

	// 世界坐标系原点在观察坐标系的位置
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