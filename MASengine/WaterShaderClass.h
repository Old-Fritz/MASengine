﻿///////////////////////////////////////////////////////////////////////////////
// Filename: WaterShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _WATERSHADERCLASS_H_
#define _WATERSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <d3dx10math.h>
#include <fstream>
#include "GlobalManagerClass.h"
#include "LightClass.h"
#include "TextureManagerClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: WaterShaderClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Shaders
@{
*/
/*!
Шейдер воды
*/
class WaterShaderClass
{
private:
	//!Тип матричного буфера
	struct MatrixBufferType
	{
		D3DXMATRIX world; //!<Матрица мира
		D3DXMATRIX view; //!<Видовая матрица
		D3DXMATRIX projection; //!<Проекционная матрица
		D3DXVECTOR4 cameraPosition; //!<Позиция камеры
		D3DXVECTOR4 lightPosition1; //!<направление света
		D3DXVECTOR4 lightPosition2; //!<направление света
	};
	//!Тип параметрического буфера
	struct ParamsBufferType
	{
		D3DXVECTOR4 ambientColor1; //!<цвет обтекающего света
		D3DXVECTOR4 diffuseColor1; //!<цвет диффузного света
		D3DXVECTOR4 specularColor1; //!<цвет зеркального света
		D3DXVECTOR4 ambientColor2; //!<цвет обтекающего света
		D3DXVECTOR4 diffuseColor2; //!<цвет диффузного света
		D3DXVECTOR4 specularColor2; //!<цвет зеркального света
		D3DXVECTOR4 cameraPosition; //!<Позиция камеры
		float specularPower2; //!<мощность зеркального света
		float specularPower1; //!<мощность зеркального света
		float waterTranslation; //!<Смещение воды
	};
public:
	WaterShaderClass();
	WaterShaderClass(const WaterShaderClass&);
	~WaterShaderClass();

	//! Загрузка шейдера \param[in] device - графическое устрйоство \param[in] hwnd - ID окна \param[in] filenameVS - Расположение вершинного шейдера
	//! \param[in] filenamePS - Расположение пиксельного шейдера \return false, если были ошибки
	bool Initialize(ID3D11Device* device, HWND hwnd, PathClass* filenameVS, PathClass* filenamePS);
	void Shutdown();

	/*!
	Прорисовка модели \param[in] deviceContext - графическое устройство \param[in] indexCount - количество прорисовываемых вершин
	\param[in] worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix - матрицы с параметрами \param[in] texture - текстура модели \param[in] lights - источники света
	\param[in] cameraPosition - позиция камеры \param[in] waterTranslation - Смещение воды \return false, если были ошибки
	*/
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* depthTexture, ID3D11ShaderResourceView* provTtexture, ID3D11ShaderResourceView* skyTtexture,
		ID3D11ShaderResourceView* waterTexture, std::vector<LightClass::PointLightType*> lights, D3DXVECTOR3 cameraPosition, float waterTranslation);

private:
	//! Загрузка шейдера \param[in] device - графическое устрйоство \param[in] hwnd - ID окна \param[in] vsFilename - Расположение вершинного шейдера
	//! \param[in] psFilename - Расположение пиксельного шейдера \return false, если были ошибки
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
	void ShutdownShader();
	//! Вывести сообщение об ошибке \param[in] errorMessage - сообщение об ошибке \param[in] hwnd - ID окна \param[in] shaderFilename - имя файла шейдера
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	/*!
	Установка параметров шейдера \param[in] deviceContext - графическое устройство
	\param[in] worldMatrix, viewMatrix, projectionMatrix,reflectionMatrix - матрицы с параметрами \param[in] texture - текстура модели \param[in] lights - источники света
	\param[in] cameraPosition - позиция камеры \param[in] waterTranslation - Смещение воды \return false, если были ошибки
	*/
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, 
		ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* depthTexture, ID3D11ShaderResourceView* provTtexture, ID3D11ShaderResourceView* skyTtexture,
		ID3D11ShaderResourceView* waterTexture, std::vector<LightClass::PointLightType*> lights, D3DXVECTOR3 cameraPosition, float waterTranslation);
	//! Выполнение шейдера \param[in] deviceContext - графическое устройство \param[in] indexCount - количество прорисовываемых вершин
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader; //!<Вершинный шейдер
	ID3D11PixelShader* m_pixelShader; //!<Пиксельный шейдер
	ID3D11InputLayout* m_layout; //!<Макет
	ID3D11Buffer* m_matrixBuffer; //!<Матричный буфер
	ID3D11Buffer* m_paramsBuffer; //!<Параментрический буфер
	ID3D11SamplerState* m_sampleState[2]; //!<Образцовая форма
};
/*! @} */
#endif