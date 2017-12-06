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
Рендер сцены в текстуру
*/
class RenderTextureClass
{
public:
	RenderTextureClass();
	RenderTextureClass(const RenderTextureClass&);
	~RenderTextureClass();

	//! Инициализация текстуры  \param[in] device - графическое устройство  \param[in] textureWidth, textureHeight - размеры текстуры \return false, если были ошибки
	bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight);
	void Shutdown();

	//! Установка новой цели для рендринга \param[in] deviceContext - графическое устройство \param[in] depthStencilView - буфер глубины
	void SetRenderTarget(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView* depthStencilView);
	//! Очистка цели для рендринга \param[in] deviceContext - графическое устройство \param[in] depthStencilView - буфер глубины \param[in] color - цвет фона
	void ClearRenderTarget(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView* depthStencilView, D3DXVECTOR4 color);
	//! Получение текстуры в виде ресурса Directx \return текстура в виде ресурса directx
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Texture2D* m_renderTargetTexture;  //!< Цель для рендринга
	ID3D11RenderTargetView* m_renderTargetView; //!< Описание цели для рендринга
	ID3D11ShaderResourceView* m_shaderResourceView; //!< Текстура в виде ресурса directx
};

#endif
