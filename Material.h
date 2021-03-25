#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

#include "Light.h"
#include"Camera.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

struct ShaderDetails
{
	XMFLOAT2 Tiling;
	XMFLOAT2 Offset;
	XMFLOAT2 SpecularPower;
	XMFLOAT2 Glossiness;
	LPCWSTR VertexShaderName;
	LPCWSTR PixelShaderName;
};

class Material
{
public:

	int init(ID3D11Device* pD3DDevice, LPCWSTR textureName, ShaderDetails* parameters, LPCWSTR normalTextureName);
	void render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix4x4, Camera* camera) const;
	void deInit();

	void setLight(ID3D11DeviceContext* pD3DDeviceContext, Light& lightData) const;

protected:

	// helper methods
	int createVertexShader(ID3D11Device* pD3DDevice);
	int createPixelShader(ID3D11Device* pD3DDevice);
	int createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob);
	int createMaterialConstantBuffer(ID3D11Device* pD3DDevice);
	int createUpdatedBuffer(ID3D11Device* pD3DDevice);
	void setMaterialConstantBuffer(ID3D11DeviceContext* pD3DDeviceContext) const;
	void setUpdatedBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix, Camera* camera) const;
	virtual int createTextureSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName, LPCWSTR normalTextureName);
	bool hasEnding(std::wstring const& fullString, std::wstring const& ending);
	int createPixelShaderBuffer(ID3D11Device* pD3DDevice);

	ID3D11VertexShader* _pVertexShader = nullptr;
	ID3D11PixelShader* _pPixelShader = nullptr;

	ID3D11Buffer* _pPixelShaderBuffer = nullptr;
	struct PixelShaderBuffer
	{
		Light LightData;
	};
	
	ID3D11InputLayout* _pInputLayout = nullptr;

	ID3D11ShaderResourceView* _pMainTexture = nullptr;
	ID3D11ShaderResourceView* _pNormalTexture = nullptr;
	
	ID3D11SamplerState* _pMainSampler = nullptr;
	ID3D11SamplerState* _pNormalSampler = nullptr;

	ID3D11Buffer* _pMateriaConstantBuffer = nullptr;
	struct MaterialConstantBuffer
	{
		XMFLOAT4 TilingOffset;
		XMFLOAT4 SpecularGlossiness;
	};

	ID3D11Buffer* _pUpdatedBuffer = nullptr;
	struct UpdatedBuffer
	{
		XMFLOAT4X4 WorldViewProjectionMatrix;
		XMFLOAT4X4 WorldMatrix;
		XMFLOAT3 WorldSpaceCameraPos;
		FLOAT Time;
	};

	ShaderDetails* _pParameters = nullptr;;
};