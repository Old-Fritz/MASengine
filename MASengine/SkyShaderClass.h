///////////////////////////////////////////////////////////////////////////////
// Filename: SkyShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYSHADERCLASS_H_
#define _SKYSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <d3dx10math.h>
#include <fstream>
#include "GlobalManagerClass.h"
#include "LightClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelShaderClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Shaders
@{
*/
/*!
������ ����
*/
class SkyShaderClass
{
private:
	//!��� ���������� ������
	struct MatrixBufferType
	{
		D3DXMATRIX world; //!<������� ����
		D3DXMATRIX view; //!<������� �������
		D3DXMATRIX projection; //!<������������ �������
	};
	//!��� ���������������� ������
	struct ParamsBufferType
	{
		D3DXVECTOR2 firstTranslation; //!< ����� ������ ������ �������
		D3DXVECTOR2 secondTranslation; //!< ����� ������ ������ �������
		float brightness; //!< ����� �������
	};
public:
	SkyShaderClass();
	SkyShaderClass(const SkyShaderClass&);
	~SkyShaderClass();

	//! �������� ������� \param[in] device - ����������� ���������� \param[in] hwnd - ID ���� \param[in] filenameVS - ������������ ���������� �������
	//! \param[in] filenamePS - ������������ ����������� ������� \return false, ���� ���� ������
	bool Initialize(ID3D11Device* device, HWND hwnd, PathClass* filenameVS, PathClass* filenamePS);
	void Shutdown();
	/*!
	���������� ������ \param[in] deviceContext - ����������� ���������� \param[in] indexCount - ���������� ��������������� ������
	\param[in] worldMatrix, viewMatrix, projectionMatrix - ������� � ����������� \param[in] texture1 - �������� ������ ������ �������
	\param[in] texture2 - �������� ������ ������ �������  \param[in] firstTranslation - ����� ������ ������ �������
	\param[in] secondTranslation - ����� ������ ������ ������� \param[in] brightness - ����� ������� \return false, ���� ���� ������
	*/
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2, D3DXVECTOR2 firstTranslation, D3DXVECTOR2 secondTranslation,
		float brightness);

private:
	//! �������� ������� \param[in] device - ����������� ���������� \param[in] hwnd - ID ���� \param[in] vsFilename - ������������ ���������� �������
	//! \param[in] psFilename - ������������ ����������� ������� \return false, ���� ���� ������
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
	void ShutdownShader();
	//! ������� ��������� �� ������ \param[in] errorMessage - ��������� �� ������ \param[in] hwnd - ID ���� \param[in] shaderFilename - ��� ����� �������
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	/*!
	��������� ���������� ������� \param[in] deviceContext - ����������� ����������
	\param[in] worldMatrix, viewMatrix, projectionMatrix - ������� � ����������� \param[in] texture1 - �������� ������ ������ �������
	\param[in] texture2 - �������� ������ ������ �������  \param[in] firstTranslation - ����� ������ ������ �������
	\param[in] secondTranslation - ����� ������ ������ ������� \param[in] brightness - ����� ������� \return false, ���� ���� ������
	*/
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2, D3DXVECTOR2 firstTranslation, D3DXVECTOR2 secondTranslation,
		float brightness);

	//! ���������� ������� \param[in] deviceContext - ����������� ���������� \param[in] indexCount - ���������� ��������������� ������
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader; //!<��������� ������
	ID3D11PixelShader* m_pixelShader; //!<���������� ������
	ID3D11InputLayout* m_layout; //!<�����
	ID3D11Buffer* m_matrixBuffer; //!<��������� �����
	ID3D11Buffer* m_paramsBuffer; //!<���������������� �����
	ID3D11SamplerState* m_sampleState; //!<���������� �����
};
/*! @} */
#endif