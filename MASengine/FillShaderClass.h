///////////////////////////////////////////////////////////////////////////////
// Filename: FontShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FILLSHADERCLASS_H_
#define _FILLSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <d3dx10math.h>
#include <fstream>
#include "GM.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: FontShaderClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Shaders 3D-������
\ingroup ShaderManagment
@{
*/
/*!
������, ������������ ������������ �����
*/
class FillShaderClass
{
private:
	//!��� ���������� ������
	struct MatrixBufferType
	{
		D3DXMATRIX world; //!<������� ����
		D3DXMATRIX view; //!<������� �������
		D3DXMATRIX projection; //!<������������ �������
		D3DXVECTOR4 clipPlane; //!<��������� ���������
	};
public:
	FillShaderClass();
	FillShaderClass(const FillShaderClass&);
	~FillShaderClass();

	//! �������� ������� \param[in] device - ����������� ���������� \param[in] hwnd - ID ���� \param[in] filenameVS - ������������ ���������� �������
	//! \param[in] filenamePS - ������������ ����������� ������� \return false, ���� ���� ������
	bool Initialize(ID3D11Device* device, HWND hwnd, PathClass* filenameVS, PathClass* filenamePS);
	void Shutdown();
	/*!
	���������� ������ \param[in] deviceContext - ����������� ���������� \param[in] indexCount - ���������� ��������������� ������
	\param[in] worldMatrix, viewMatrix, projectionMatrix - ������� � ����������� \param[in] clipPlane - ���������� ��������� \return false, ���� ���� ������
	*/
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR4 clipPlane);

private:
	//! �������� ������� \param[in] device - ����������� ���������� \param[in] hwnd - ID ���� \param[in] vsFilename - ������������ ���������� �������
	//! \param[in] psFilename - ������������ ����������� ������� \return false, ���� ���� ������
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
	void ShutdownShader();
	//! ������� ��������� �� ������ \param[in] errorMessage - ��������� �� ������ \param[in] hwnd - ID ���� \param[in] shaderFilename - ��� ����� �������
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	/*!
	��������� ���������� ������� \param[in] deviceContext - ����������� ����������
	\param[in] worldMatrix, viewMatrix, projectionMatrix - ������� � ����������� \param[in] clipPlane - ���������� ��������� \return false, ���� ���� ������
	*/
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR4 clipPlane);

	//! ���������� ������� \param[in] deviceContext - ����������� ���������� \param[in] indexCount - ���������� ��������������� ������
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader; //!<��������� ������
	ID3D11PixelShader* m_pixelShader; //!<���������� ������
	ID3D11InputLayout* m_layout; //!<�����
	ID3D11Buffer* m_matrixBuffer; //!<��������� �����
	ID3D11SamplerState* m_sampleState; //!<���������� �����
};
/*! @} */
#endif