////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include "GlobalManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(HWND hwnd, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(D3DXVECTOR4 color);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11DepthStencilView* GetDepthStencilView();

	void GetProjectionMatrix(D3DXMATRIX& projectionMatrix);
	void GetWorldMatrix(D3DXMATRIX& worldMatrix);
	void GetOrthoMatrix(D3DXMATRIX& orthoMatrix);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void SetBackBufferRenderTarget();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

private:
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;

	ID3D11DepthStencilState* m_depthDisabledStencilState;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};

#endif