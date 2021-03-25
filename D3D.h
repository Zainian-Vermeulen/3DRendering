#pragma once
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

class D3D
{
public:
	int init(HWND hWnd, INT width, INT height, bool isWindowed);
	void beginScene();
	void endScene();
	void deInit();

	ID3D11Device* getDevice() { return _pD3DDevice; }
	ID3D11DeviceContext* getDeviceContext() { return _pD3DDeviceContext; }



private:
	// COM - Component Object Model
	ID3D11Device* _pD3DDevice = nullptr; // object to create direct 3d objects
	ID3D11DeviceContext* _pD3DDeviceContext = nullptr; // object to modify the render pipeline
	IDXGISwapChain* _pD3DSwapChain = nullptr; // holds front & back buffer references
	ID3D11RenderTargetView* _pRenderTargetView = nullptr; // target to render on (in this case the back buffer)
	ID3D11DepthStencilView* _pDepthStencilView = nullptr; // holds depth & stencil buffer reference
	ID3D11RasterizerState* _pRasterizerState = nullptr; // properties for the rasterizer stage
	D3D11_VIEWPORT _viewPort = {};
};

