#include "Skybox.h"
#include <DDSTextureLoader.h>
#include "Utils.h"

int Skybox::init(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pD3DDeviceContext, LPCWSTR textureName)
{
	int error = createTextureAndSampler(pD3DDevice, textureName);
	if (error != 0) return error;

	pD3DDeviceContext->PSSetShaderResources(2, 1, &_pSkyboxTexture);

	pD3DDeviceContext->PSSetSamplers(2, 1, &_pSkyboxSampler);

	return 0;
}



int Skybox::createTextureAndSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName)
{
	HRESULT hr;

	hr = CreateDDSTextureFromFile(pD3DDevice, textureName, nullptr, &_pSkyboxTexture);


	if (FAILED(hr))
	{
		return 48;
	}
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	hr = pD3DDevice->CreateSamplerState(&desc, &_pSkyboxSampler);
	if (FAILED(hr)) return 49;

	return 0;
}

void Skybox::de_init()
{
	safeRelease(_pSkyboxSampler);
	safeRelease(_pSkyboxTexture);
}
