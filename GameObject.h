#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Material.h"
#include "Mesh.h"

class GameObject
{
public:
	int init(Material* material, Mesh* mesh);
	void update();
	void render(ID3D11DeviceContext* pD3DDeviceContext, Camera* camera);

	void setPos(float X, float Y, float Z);
	void setRotation(float X, float Y, float Z);
	void changeRotation(float X, float Y, float Z);
	void setScale(float X, float Y, float Z);

	XMFLOAT4X4* getWorldMatrix() { return &_worldMatrix; }

private:
	XMFLOAT4X4 _worldMatrix = {};
	Material* _material = nullptr;
	Mesh* _mesh = nullptr;
	float _posX;
	float _posY;
	float _posZ;
	float _rotX;
	float _rotY;
	float _rotZ;
	float _scaleX = 1;
	float _scaleY = 1;
	float _scaleZ = 1;
};

