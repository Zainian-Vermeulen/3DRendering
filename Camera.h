#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	int init(INT screenWidth, INT screenHeight);
	void update(float deltaTime);
	void rotateCamera(float degrees, float deltaTime);
	void setProjectionMatrix(float degrees, float aspectRatio, float nearClippingZ, float farClippingZ);
	void updateViewMatrix();
	void changePos(XMFLOAT3 offset);
	XMFLOAT3 getPos() const;

	const XMFLOAT4X4& getViewMatrix() const;
	const XMFLOAT4X4& getProjectionMatrix() const;

	XMFLOAT4X4 getViewMatrix() { return _viewMatrix; }
	XMFLOAT4X4 getProjectionMatrix() { return _projectionMatrix; }

private:
	XMFLOAT3 _cameraPos = {};
	XMFLOAT3 _cameraRotation = {};
	XMFLOAT3 _focusPos{};
	
	const XMFLOAT3 DEFAULT_UP_VECTOR{ 0.0f, 1.0f, 0.0f };
	
	XMFLOAT4X4 _viewMatrix = {};
	XMFLOAT4X4 _projectionMatrix = {};
};

