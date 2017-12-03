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

/*!
\addtogroup Graphics
@{
*/
/*!
Управление API DirectX 11
*/
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	//! Инициализация API DirectX 11 \param[in] hwnd - идентификатор окна \param[in] screenDepth, screenNear - границы видимости \return false, если были ошибки
	bool Initialize(HWND hwnd, float screenDepth, float screenNear);
	void Shutdown();

	//! Организация новой сцены, выполняется каждый кадр \param[in] color - цвет заднего фона
	void BeginScene(D3DXVECTOR4 color);
	//! Окончание организации новой сцены и рендернинг результата
	void EndScene();

	//! Получение графического устройства \return графическое устройство
	ID3D11Device* GetDevice();
	//! Тоже получение графического устройства, но для других целей \return графическое устройство 2
	ID3D11DeviceContext* GetDeviceContext();
	//! Получение Depth Stencil View \return основной Depth Stencil View
	ID3D11DepthStencilView* GetDepthStencilView();

	//! Получение проекционной матрицы \param[out] projectionMatrix проекционная матрица
	void GetProjectionMatrix(D3DXMATRIX& projectionMatrix);
	//! Получение матрицы мира \param[out] worldMatrix матрица мира
	void GetWorldMatrix(D3DXMATRIX& worldMatrix);
	//! Получение ортоматрицы \param[out] orthoMatrix ортоматрица
	void GetOrthoMatrix(D3DXMATRIX& orthoMatrix);

	//! Получение данных о видеокарте \param[out] cardName - название видеокарты \param[out] memory - видеопамять
	void GetVideoCardInfo(char* cardName, int& memory);

	//! Включение Z-буффера
	void TurnZBufferOn();
	//! Отключение Z-буффера
	void TurnZBufferOff();

	//! Возвращение к оригинальной цели для рендринга
	void SetBackBufferRenderTarget();

	//! Включение AlphaBlending
	void TurnOnAlphaBlending();
	//! Отключение AlphaBlending
	void TurnOffAlphaBlending();

private:
	int m_videoCardMemory; //!<Видеопамять
	char m_videoCardDescription[128]; //!<Название видеокарты
	IDXGISwapChain* m_swapChain; //!<Цепочка кадров
	ID3D11Device* m_device; //!<Графическое устрйоство
	ID3D11DeviceContext* m_deviceContext; //!<Графическое устройство 2
	ID3D11RenderTargetView* m_renderTargetView; //!<Базовая цель для рендринга
	ID3D11Texture2D* m_depthStencilBuffer; 
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	D3DXMATRIX m_projectionMatrix; //!<Проекционная матрица
	D3DXMATRIX m_worldMatrix; //!<Матрица мира
	D3DXMATRIX m_orthoMatrix; //!<Ортоматрица

	ID3D11DepthStencilState* m_depthDisabledStencilState; 

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};
/*! @} */
#endif