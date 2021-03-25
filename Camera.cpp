#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;

int Camera::init(INT screenWidth, INT screenHeight)
{
	_cameraPos =  { 0.0f, 5.0f, -20.0f};
	_focusPos = { 0.0f, 0.0f, 0.0f };
	_cameraRotation = { 0.0f, 0.0f, 0.0f };
	
	setProjectionMatrix(90.0f, static_cast<FLOAT>(screenWidth) / static_cast<FLOAT>(screenHeight), 0.3f, 1000.0f);
	updateViewMatrix();


	return 0;
}

void Camera::update(float deltaTime)
{
	rotateCamera(0.15f, deltaTime);
	
	updateViewMatrix();
}

void Camera::rotateCamera(float degrees, float deltaTime)
{
	XMMATRIX xmmatrix = XMMatrixRotationRollPitchYaw(degrees * deltaTime * -1, degrees * deltaTime, 0);

	XMVECTOR xmvector = XMVector3Transform(XMLoadFloat3(&_cameraPos), xmmatrix);

	XMStoreFloat3(&_cameraPos, xmvector);
}

void Camera::setProjectionMatrix(float degrees, float aspectRatio, float nearClippingZ, float farClippingZ)
{
	float radians = degrees / (180.0f * XM_PI);
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(radians, aspectRatio, nearClippingZ, farClippingZ);
	XMStoreFloat4x4(&_projectionMatrix, projectionMatrix);
}

void Camera::changePos(XMFLOAT3 offset)
{
	XMVECTOR vector = XMLoadFloat3(&offset);
	
	 XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(_cameraRotation.x, _cameraRotation.y, _cameraRotation.z);
	 XMVECTOR vector2 = XMVector3Transform(vector, rotationMatrix);
	
	_cameraPos.x = XMVectorGetX(vector2);
	_cameraPos.y = XMVectorGetY(vector2);
	_cameraPos.z = XMVectorGetZ(vector2);
}

void Camera::updateViewMatrix()
{
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(_cameraRotation.x, _cameraRotation.y, _cameraRotation.z);
	XMVECTOR target = XMVectorZero();
	
	XMVECTOR upDirection = XMVector3TransformCoord(XMLoadFloat3(&DEFAULT_UP_VECTOR), rotationMatrix);
	const auto view_matrix = XMMatrixLookAtLH(XMLoadFloat3(&_cameraPos), target, upDirection);
	XMStoreFloat4x4(&_viewMatrix, view_matrix);
}

const XMFLOAT4X4& Camera::getViewMatrix() const
{
	return _viewMatrix;
}

const XMFLOAT4X4& Camera::getProjectionMatrix() const
{
	return _projectionMatrix;
}

XMFLOAT3 Camera::getPos() const
{
	return _cameraPos;
}
