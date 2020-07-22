#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

// 摄像机类
class ICamera
{
public:
	ICamera();
	virtual ~ICamera() = 0;

	// 获取摄像机坐标
	DirectX::XMVECTOR GetPositionXM() const;
	DirectX::XMFLOAT3 GetPosition() const;

	// 获取观察空间坐标轴向量
	DirectX::XMVECTOR GetRightXM() const;
	DirectX::XMFLOAT3 GetRight() const;
	DirectX::XMVECTOR GetUpXM() const;
	DirectX::XMFLOAT3 GetUp() const;
	DirectX::XMVECTOR GetLookXM() const;
	DirectX::XMFLOAT3 GetLook() const;

	// 获取视锥体信息
	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	// 获取矩阵
	DirectX::XMMATRIX GetView() const;
	DirectX::XMMATRIX GetProj() const;
	DirectX::XMMATRIX GetViewProj() const;

	// 设置视锥体
	void SetFrustum(float fovY, float aspect, float nearZ, float farZ);

	// 摄像机旋转
	virtual void RotateUpDown(float angle) = 0;
	virtual void RotateLeftRight(float angle) = 0;

	// 更新观察矩阵
	virtual void UpdateViewMatrix() = 0;
protected:
	// 摄像机的坐标和观察空间坐标轴
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRight;
	DirectX::XMFLOAT3 mUp;
	DirectX::XMFLOAT3 mLook;

	// 视锥体属性
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	// 观察矩阵和投射矩阵
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;
};

// 摄像机模式
enum CameraMode
{
	FirstPerson = 0,
	ThirdPerson = 1,
};

// 第一人称摄像机随目标物体移动，可以控制相机的朝向
class FirstPersonCamera : public ICamera
{
public:
	FirstPersonCamera();
	~FirstPersonCamera();

	// 设置摄像机的坐标、观察坐标轴
	void XM_CALLCONV Init(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	void Init(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& worldUp);

	// 设置摄像机坐标
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& pos);

	// 上下扫描（绕观察坐标系的X轴）
	void RotateUpDown(float angle);

	// 左右扫描（绕世界坐标系的Y轴）
	void RotateLeftRight(float angle);

	// 更新观察矩阵
	void UpdateViewMatrix();
};

// 第三人称摄像机永远朝向目标，与目标保持一定距离，绕目标左右、上下旋转
class ThirdPersonCamera : public ICamera
{
public:
	ThirdPersonCamera();
	~ThirdPersonCamera();

	void Init(float distance, float phi, float theta, float distanceMin, float distanceMax);

	// 拉近或拉远
	void ChangeDistance(float d);

	// 绕目标上下旋转
	void RotateUpDown(float angle);
	// 绕目标左右旋转
	void RotateLeftRight(float angle);

	// 设置目标坐标
	void SetTargetPosition(const DirectX::XMFLOAT3& target);

	// 设置到目标的距离
	void SetDistance(float distance);
	// 设置到目标的距离限制
	void SetDistanceMinMax(float distanceMin, float distanceMax);

	// 设置角度
	void SetPhi(float phi);
	void SetTheta(float theta);

	// 更新观察矩阵
	void UpdateViewMatrix();

private:
	DirectX::XMFLOAT3 mTarget;

	// 摄像机与物体的距离的限制
	float mDistanceMin;
	float mDistanceMax;

	// 球面坐标系参数
	float mDistance;
	float mPhi;
	float mTheta;
};