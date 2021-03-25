#include "GameObject.h"
#include "D3D11.h"

int GameObject::init(Material* material, Mesh* mesh)
{
	XMStoreFloat4x4(&_worldMatrix, XMMatrixIdentity());
	_material = material;
	_mesh = mesh;
	return 0;
}

void GameObject::update()
{
	XMMATRIX translation = XMMatrixTranslation(_posX, _posY, _posZ);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(_rotX, _rotY, _rotZ);
	XMMATRIX scale = XMMatrixScaling(_scaleX, _scaleY, _scaleZ);

	XMStoreFloat4x4(&_worldMatrix, scale * rotation * translation);
}

void GameObject::render(ID3D11DeviceContext* pD3DDeviceContext, Camera* camera)
{
	_material->render(pD3DDeviceContext, &_worldMatrix, camera);
	_mesh->render(pD3DDeviceContext);
}

void GameObject::setPos(float X, float Y, float Z)
{
	_posX = X;
	_posY = Y;
	_posZ = Z;
}

void GameObject::setScale(float X, float Y, float Z)
{
	_scaleX = X;
	_scaleY = Y;
	_scaleZ = Z;
}

void GameObject::changeRotation(float X, float Y, float Z)
{
	_rotX += X;
	_rotY += Y;
	_rotZ += Z;
}