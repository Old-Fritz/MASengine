///////////////////////////////////////////////////////////////////////////////
// Filename: ModelShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELSHADERCLASS_H_
#define _MODELSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <d3dx10math.h>
#include <fstream>
#include "GlobalManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelShaderClass
////////////////////////////////////////////////////////////////////////////////
class ModelShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
		D3DXVECTOR3 cameraPosition;
	};
	struct ParamsBufferType
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR4 specularColor;
		D3DXVECTOR3 lightDirection;
		float specularPower;
		D3DXVECTOR3 cameraPosition;
	};
public:
	ModelShaderClass();
	ModelShaderClass(const ModelShaderClass&);
	~ModelShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd, PathClass* filenameVS, PathClass* filenamePS);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
		D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
		D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower);
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