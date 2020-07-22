#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

// �������
class ICamera
{
public:
	ICamera();
	virtual ~ICamera() = 0;

	// ��ȡ���������
	DirectX::XMVECTOR GetPositionXM() const;
	DirectX::XMFLOAT3 GetPosition() const;

	// ��ȡ�۲�ռ�����������
	DirectX::XMVECTOR GetRightXM() const;
	DirectX::XMFLOAT3 GetRight() const;
	DirectX::XMVECTOR GetUpXM() const;
	DirectX::XMFLOAT3 GetUp() const;
	DirectX::XMVECTOR GetLookXM() const;
	DirectX::XMFLOAT3 GetLook() const;

	// ��ȡ��׶����Ϣ
	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	// ��ȡ����
	DirectX::XMMATRIX GetView() const;
	DirectX::XMMATRIX GetProj() const;
	DirectX::XMMATRIX GetViewProj() const;

	// ������׶��
	void SetFrustum(float fovY, float aspect, float nearZ, float farZ);

	// �������ת
	virtual void RotateUpDown(float angle) = 0;
	virtual void RotateLeftRight(float angle) = 0;

	// ���¹۲����
	virtual void UpdateViewMatrix() = 0;
protected:
	// �����������͹۲�ռ�������
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRight;
	DirectX::XMFLOAT3 mUp;
	DirectX::XMFLOAT3 mLook;

	// ��׶������
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	// �۲�����Ͷ�����
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;
};

// �����ģʽ
enum CameraMode
{
	FirstPerson = 0,
	ThirdPerson = 1,
};

// ��һ�˳��������Ŀ�������ƶ������Կ�������ĳ���
class FirstPersonCamera : public ICamera
{
public:
	FirstPersonCamera();
	~FirstPersonCamera();

	// ��������������ꡢ�۲�������
	void XM_CALLCONV Init(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	void Init(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& worldUp);

	// �������������
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& pos);

	// ����ɨ�裨�ƹ۲�����ϵ��X�ᣩ
	void RotateUpDown(float angle);

	// ����ɨ�裨����������ϵ��Y�ᣩ
	void RotateLeftRight(float angle);

	// ���¹۲����
	void UpdateViewMatrix();
};

// �����˳��������Զ����Ŀ�꣬��Ŀ�걣��һ�����룬��Ŀ�����ҡ�������ת
class ThirdPersonCamera : public ICamera
{
public:
	ThirdPersonCamera();
	~ThirdPersonCamera();

	void Init(float distance, float phi, float theta, float distanceMin, float distanceMax);

	// ��������Զ
	void ChangeDistance(float d);

	// ��Ŀ��������ת
	void RotateUpDown(float angle);
	// ��Ŀ��������ת
	void RotateLeftRight(float angle);

	// ����Ŀ������
	void SetTargetPosition(const DirectX::XMFLOAT3& target);

	// ���õ�Ŀ��ľ���
	void SetDistance(float distance);
	// ���õ�Ŀ��ľ�������
	void SetDistanceMinMax(float distanceMin, float distanceMax);

	// ���ýǶ�
	void SetPhi(float phi);
	void SetTheta(float theta);

	// ���¹۲����
	void UpdateViewMatrix();

private:
	DirectX::XMFLOAT3 mTarget;

	// �����������ľ��������
	float mDistanceMin;
	float mDistanceMax;

	// ��������ϵ����
	float mDistance;
	float mPhi;
	float mTheta;
};