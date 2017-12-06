////////////////////////////////////////////////////////////////////////////////
// Filename: rendertextureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERTEXTURECLASS_H_
#define _RENDERTEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: RenderTextureClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Systems
@{
*/
/*!
������ ����� � ��������
*/
class RenderTextureClass
{
public:
	RenderTextureClass();
	RenderTextureClass(const RenderTextureClass&);
	~RenderTextureClass();

	//! ������������� ��������  \param[in] device - ����������� ����������  \param[in] textureWidth, textureHeight - ������� �������� \return false, ���� ���� ������
	bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight);
	void Shutdown();

	//! ��������� ����� ���� ��� ��������� \param[in] deviceContext - ����������� ���������� \param[in] depthStencilView - ����� �������
	void SetRenderTarget(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView* depthStencilView);
	//! ������� ���� ��� ��������� \param[in] deviceContext - ����������� ���������� \param[in] depthStencilView - ����� ������� \param[in] color - ���� ����
	void ClearRenderTarget(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView* depthStencilView, D3DXVECTOR4 color);
	//! ��������� �������� � ���� ������� Directx \return �������� � ���� ������� directx
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Texture2D* m_renderTargetTexture;  //!< ���� ��� ���������
	ID3D11RenderTargetView* m_renderTargetView; //!< �������� ���� ��� ���������
	ID3D11ShaderResourceView* m_shaderResourceView; //!< �������� � ���� ������� directx
};

#endif
