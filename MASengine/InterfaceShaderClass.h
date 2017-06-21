///////////////////////////////////////////////////////////////////////////////
// Filename: InterfaceShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INTERFACEHADERCLASS_H_
#define _INTERFACEHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <d3dx10math.h>
#include <fstream>
#include "GlobalManagerClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class InterfaceShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
	struct ParamsBufferType
	{
		D3DXVECTOR4 selCol;
		float vertPercent;
		float horPercent;
		float selIntensivity;
		float transparency;
	};
public:
	InterfaceShaderClass();
	InterfaceShaderClass(const InterfaceShaderClass&);
	~InterfaceShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, float vertPercent, float horPercent, D3DXVECTOR4 selCol,
		float selIntensivity, float transparency);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, float vertPercent, float horPercent, D3DXVECTOR4 selCol,
		float selIntensivity, float transparency);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_paramsBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif