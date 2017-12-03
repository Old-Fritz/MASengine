﻿///////////////////////////////////////////////////////////////////////////////
// Filename: TerrainShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINSHADERCLASS_H_
#define _TERRAINSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <d3dx10math.h>
#include <fstream>
#include "GlobalManagerClass.h"

const int NUM_OF_MAP_TEXTURES = 10;

////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainShaderClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Shaders
@{
*/
/*!
Шейдер блоков
*/
class TerrainShaderClass
{
private:
	//!Тип матричного буфера
	struct MatrixBufferType
	{
		D3DXMATRIX world; //!<Матрица мира
		D3DXMATRIX view; //!<Видовая матрица
		D3DXMATRIX projection; //!<Проекционная матрица
		D3DXVECTOR3 cameraPosition; //!<Позиция камеры
	};
	//!Тип параметрического буфера
	struct ParamsBufferType
	{
		D3DXVECTOR4 ambientColor; //!<цвет обтекающего света
		D3DXVECTOR4 diffuseColor; //!<цвет диффузного света
		D3DXVECTOR4 specularColor; //!<цвет зеркального света
		D3DXVECTOR4 provsColor[256]; //!<Цвета провинций
		D3DXVECTOR3 lightDirection; //!<направление света
		float specularPower; //!<мощность зеркального света
		D3DXVECTOR3 cameraPosition; //!<Позиция камеры
	};
public:
	TerrainShaderClass();
	TerrainShaderClass(const TerrainShaderClass&);
	~TerrainShaderClass();

	//! Загрузка шейдера \param[in] device - графическое устрйоство \param[in] hwnd - ID окна \param[in] filenameVS - Расположение вершинного шейдера
	//! \param[in] filenamePS - Расположение пиксельного шейдера \return false, если были ошибки
	bool Initialize(ID3D11Device* device, HWND hwnd, PathClass* filenameVS, PathClass* filenamePS);
	void Shutdown();
	/*!
	Прорисовка модели \param[in] deviceContext - графическое устройство \param[in] indexCount - количество прорисовываемых вершин
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами
	\param[in] texture - текстура с провами \param[in] physTexture - физическая карта \param[in] mapTextures - физические текстуры
	\param[in] lightDirection - направление света \param[in] ambientColor - цвет обтекающего света
	\param[in] diffuseColor - цвет диффузного света \param[in] cameraPosition - позиция камеры \param[in] specularColor - цвет зеркального света
	\param[in] specularPower - мощность зеркального света \param[in] provsColor - Цвета провинций
	\return false, если были ошибки
	*/
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* physTexture, ID3D11ShaderResourceView** mapTextures,
		D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
		D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower, D3DXVECTOR4* provsColor);

private:
	//! Загрузка шейдера \param[in] device - графическое устрйоство \param[in] hwnd - ID окна \param[in] vsFilename - Расположение вершинного шейдера
	//! \param[in] psFilename - Расположение пиксельного шейдера \return false, если были ошибки
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
	void ShutdownShader();
	//! Вывести сообщение об ошибке \param[in] errorMessage - сообщение об ошибке \param[in] hwnd - ID окна \param[in] shaderFilename - имя файла шейдера
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	/*!
	Установка параметров шейдера \param[in] deviceContext - графическое устройство
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами
	\param[in] texture - текстура с провами \param[in] physTexture - физическая карта \param[in] mapTextures - физические текстуры
	\param[in] lightDirection - направление света \param[in] ambientColor - цвет обтекающего света
	\param[in] diffuseColor - цвет диффузного света \param[in] cameraPosition - позиция камеры \param[in] specularColor - цвет зеркального света
	\param[in] specularPower - мощность зеркального света \param[in] provsColor - Цвета провинций
	\return false, если были ошибки
	*/
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* physTexture, ID3D11ShaderResourceView** mapTextures,
		D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
		D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower, D3DXVECTOR4* provsColor);
	//! Выполнение шейдера \param[in] deviceContext - графическое устройство \param[in] indexCount - количество прорисовываемых вершин
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader; //!<Вершинный шейдер
	ID3D11PixelShader* m_pixelShader; //!<Пиксельный шейдер
	ID3D11InputLayout* m_layout; //!<Макет
	ID3D11Buffer* m_matrixBuffer; //!<Матричный буфер
	ID3D11Buffer* m_paramsBuffer; //!<Параментрический буфер
	ID3D11SamplerState* m_sampleState[3]; //!<Образцовые форма
};
/*! @} */
#endif