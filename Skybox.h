#pragma once
#include "Material.h"
#include "WICTextureLoader.h"
#include <d3dcompiler.h>
#include "Utils.h"
#include<string>
#include "DDSTextureLoader.h"


class Skybox
{
public:
    int init(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pD3DDeviceContext, LPCWSTR textureName);
    void de_init();

    int createTextureAndSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName);

private:

    ID3D11ShaderResourceView* _pSkyboxTexture = nullptr;
    ID3D11SamplerState* _pSkyboxSampler = nullptr;
};