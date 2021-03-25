#include "Material.h"
#include "WICTextureLoader.h"
#include <d3dcompiler.h>
#include <DDSTextureLoader.h>

#include "Utils.h"
#include<string>

int Material::init(ID3D11Device* pD3DDevice, LPCWSTR texture_name, ShaderDetails* parameters, LPCWSTR normalTextureName)
{
	_pParameters = parameters;
	int error = createVertexShader(pD3DDevice);
	if (error != 0) return error;

	error = createPixelShader(pD3DDevice);
	if (error != 0) return error;

	error = createMaterialConstantBuffer(pD3DDevice);
	if (error != 0) return error;

	error = createUpdatedBuffer(pD3DDevice);
	if (error != 0) return error;

	error = createTextureSampler(pD3DDevice, texture_name, normalTextureName);
	if (error != 0) return error;

	error = createPixelShaderBuffer(pD3DDevice);
	if (error != 0) return error;

	return 0;
}

void Material::render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix4x4, Camera* camera) const
{
	pD3DDeviceContext->IASetInputLayout(_pInputLayout);
	pD3DDeviceContext->VSSetShader(_pVertexShader, nullptr, 0);
	pD3DDeviceContext->PSSetShader(_pPixelShader, nullptr, 0);

	setMaterialConstantBuffer(pD3DDeviceContext);
	setUpdatedBuffer(pD3DDeviceContext, worldMatrix4x4, camera);

	pD3DDeviceContext->PSSetShaderResources(0, 1, &_pMainTexture);
	pD3DDeviceContext->PSSetShaderResources(1, 1, &_pNormalTexture);
	pD3DDeviceContext->PSSetSamplers(0, 1, &_pMainSampler);
	pD3DDeviceContext->PSSetSamplers(1, 1, &_pNormalSampler);
}


void Material::deInit()
{
	safeRelease<ID3D11VertexShader>(_pVertexShader);
	safeRelease<ID3D11PixelShader>(_pPixelShader);
	safeRelease<ID3D11ShaderResourceView>(_pMainTexture);
	safeRelease<ID3D11ShaderResourceView>(_pNormalTexture);
	safeRelease<ID3D11SamplerState>(_pMainSampler);
	safeRelease<ID3D11SamplerState>(_pNormalSampler);
	safeRelease<ID3D11Buffer>(_pMateriaConstantBuffer);
	safeRelease<ID3D11Buffer>(_pPixelShaderBuffer);
	safeRelease<ID3D11InputLayout>(_pInputLayout);
	safeRelease<ID3D11Buffer>(_pUpdatedBuffer);
}

void Material::setLight(ID3D11DeviceContext* pD3DDeviceContext, Light& lightData) const
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	const auto hr = pD3DDeviceContext->Map(_pPixelShaderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	auto* pBuffer = static_cast<PixelShaderBuffer*>(data.pData);
	pBuffer->LightData = lightData;

	pD3DDeviceContext->Unmap(_pPixelShaderBuffer, 0);

	pD3DDeviceContext->PSSetConstantBuffers(0, 1, &_pPixelShaderBuffer);
}

int Material::createVertexShader(ID3D11Device* pD3DDevice)
{
	ID3DBlob* pCompiledCode = nullptr;
	std::wstring compileName = std::wstring(_pParameters->VertexShaderName) + L".cso";

	HRESULT hr = D3DReadFileToBlob(compileName.c_str(), &pCompiledCode);

	if (FAILED(hr))
	{
		std::wstring rawName = std::wstring(_pParameters->VertexShaderName) + L".hlsl";
		hr = D3DCompileFromFile(rawName.c_str(), 
			nullptr, 
			nullptr, 
			"main", 
			"vs_4_0", 
			0, 0, 
			&pCompiledCode, 
			nullptr
		);
	}
	
	if (FAILED(hr)) return 40;

	hr = pD3DDevice->CreateVertexShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pVertexShader);
	if (FAILED(hr)) return 42;

	const int error = createInputLayout(pD3DDevice, pCompiledCode);
	if (error != 0) return error;

	safeRelease<ID3DBlob>(pCompiledCode);

	return 0;
}

int Material::createPixelShader(ID3D11Device* pD3DDevice)
{
	ID3DBlob* pCompiledCode = nullptr;
	std::wstring compileName = std::wstring(_pParameters->PixelShaderName) + L".cso";

	HRESULT hr = D3DReadFileToBlob(compileName.c_str(), &pCompiledCode);

	if (FAILED(hr))
	{
		std::wstring rawName = std::wstring(_pParameters->PixelShaderName) + L".hlsl";
		hr = D3DCompileFromFile(rawName.c_str(), 
			nullptr, 
			nullptr, 
			"main", 
			"ps_4_0", 
			0, 0, 
			&pCompiledCode, 
			nullptr
		);
	}
	
	if (FAILED(hr)) return 46;

	hr = pD3DDevice->CreatePixelShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pPixelShader);
	if (FAILED(hr)) return 47;

	safeRelease<ID3DBlob>(pCompiledCode);

	return 0;
}

int Material::createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob)
{
	D3D11_INPUT_ELEMENT_DESC elements[6] = {};

	elements[0].SemanticName = "POSITION";
	elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	elements[1].SemanticName = "COLOR";
	elements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	elements[2].SemanticName = "NORMAL";
	elements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	elements[3].SemanticName = "TEXCOORD";
	elements[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	elements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	elements[4].SemanticName = "TEXCOORD";
	elements[4].SemanticIndex = 1;
	elements[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	elements[5].SemanticName = "TEXCOORD";
	elements[5].SemanticIndex = 2;
	elements[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;


	HRESULT hr = pD3DDevice->CreateInputLayout(elements, 6, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &_pInputLayout);

	if (FAILED(hr)) return 44;

	return 0;
}

int Material::createMaterialConstantBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(MaterialConstantBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	const auto hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_pMateriaConstantBuffer);
	if (FAILED(hr)) return 46;

	return 0;
}

void Material::setMaterialConstantBuffer(ID3D11DeviceContext* pD3DDeviceContext) const
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	const auto hr = pD3DDeviceContext->Map(_pMateriaConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	 XMVECTOR tiling_offset = { _pParameters->Tiling.x, _pParameters->Tiling.y,_pParameters->Offset.x,_pParameters->Offset.y };
	 XMVECTOR specular_glossiness = { _pParameters->SpecularPower.x,_pParameters->Glossiness.x,0,0 };

	auto* pBuffer = static_cast<MaterialConstantBuffer*>(data.pData);

	XMStoreFloat4(&pBuffer->TilingOffset, tiling_offset);
	XMStoreFloat4(&pBuffer->SpecularGlossiness, specular_glossiness);

	pD3DDeviceContext->Unmap(_pMateriaConstantBuffer, 0);
	pD3DDeviceContext->PSSetConstantBuffers(1, 1, &_pMateriaConstantBuffer);
}

void Material::setUpdatedBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix4x4, Camera* camera) const
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_pUpdatedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	XMMATRIX world_matrix = XMLoadFloat4x4(worldMatrix4x4);
	XMFLOAT4X4 view_matrix = camera->getViewMatrix();
	XMFLOAT4X4 projection_matrix = camera->getProjectionMatrix();
	XMMATRIX wvp_matrix = world_matrix * XMLoadFloat4x4(&view_matrix) * XMLoadFloat4x4(&projection_matrix);

	auto* pBuffer = static_cast<UpdatedBuffer*>(data.pData);
	XMStoreFloat4x4(&pBuffer->WorldMatrix, world_matrix);
	XMStoreFloat4x4(&pBuffer->WorldViewProjectionMatrix, wvp_matrix);

	pBuffer->WorldSpaceCameraPos = camera->getPos();

	pD3DDeviceContext->Unmap(_pUpdatedBuffer, 0);

	pD3DDeviceContext->VSSetConstantBuffers(1, 1, &_pUpdatedBuffer);

	pD3DDeviceContext->PSSetConstantBuffers(2, 1, &_pUpdatedBuffer);
}

int Material::createTextureSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName, LPCWSTR normalTextureName)
{
	HRESULT hr;
	HRESULT hrNormal;
	
	if(hasEnding(textureName, L".dds"))
	{
		hr = CreateDDSTextureFromFile(pD3DDevice, textureName, nullptr, &_pMainTexture);
		
		if(normalTextureName != NULL)
			hrNormal = CreateDDSTextureFromFile(pD3DDevice, normalTextureName, nullptr, &_pNormalTexture);
	}
	else
	{
		hr = CreateWICTextureFromFile(pD3DDevice, textureName, nullptr, &_pMainTexture);
		
		if (normalTextureName != NULL)
			hrNormal = CreateWICTextureFromFile(pD3DDevice, normalTextureName, nullptr, &_pNormalTexture);
	}
	
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	hr = pD3DDevice->CreateSamplerState(&desc, &_pMainSampler);
	hrNormal = pD3DDevice->CreateSamplerState(&desc, &_pNormalSampler);
	if (FAILED(hr)) return 49;
	if (FAILED(hrNormal)) return 499;

	return 0;
}

bool Material::hasEnding(std::wstring const& fullString, std::wstring const& ending) {

	if (fullString.length() >= ending.length())
	{
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	return false;
}

int Material::createUpdatedBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(UpdatedBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_pUpdatedBuffer);
	if (FAILED(hr)) return 46;

	return 0;
}

int Material::createPixelShaderBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(PixelShaderBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	const auto hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_pPixelShaderBuffer);
	if (FAILED(hr)) return 47;

	return 0;
}
