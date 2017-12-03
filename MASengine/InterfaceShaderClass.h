///////////////////////////////////////////////////////////////////////////////
// Filename: InterfaceShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INTERFACESHADERCLASS_H_
#define _INTERFACESHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <d3dx10math.h>
#include <fstream>
#include "GlobalManagerClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: InterfaceShaderClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Shaders
@{
*/
/*!
Шейдер интерфейса
*/
class InterfaceShaderClass
{
private:
	//!Тип матричного буфера
	struct MatrixBufferType
	{
		D3DXMATRIX world; //!<Матрица мира
		D3DXMATRIX view; //!<Видовая матрица
		D3DXMATRIX projection; //!<Проекционная матрица
	};
	//!Тип параметрического буфера
	struct ParamsBufferType
	{
		D3DXVECTOR4 selCol; //!<Цвет выделения
		float vertPercent; //!<Отображаемая часть по вертикали
		float horPercent;  //!<Отображаемая часть по горизонтали
		float selIntensivity;  //!<Мощность выделения
		float transparency;  //!<Прозрачнсть
	};
public:
	InterfaceShaderClass();
	InterfaceShaderClass(const InterfaceShaderClass&);
	~InterfaceShaderClass();

	//! Загрузка шейдера \param[in] device - графическое устрйоство \param[in] hwnd - ID окна \param[in] filenameVS - Расположение вершинного шейдера
	//! \param[in] filenamePS - Расположение пиксельного шейдера \return false, если были ошибки
	bool Initialize(ID3D11Device* device, HWND hwnd, PathClass* filenameVS, PathClass* filenamePS);
	void Shutdown();
	/*!
	Прорисовка модели \param[in] deviceContext - графическое устройство \param[in] indexCount - количество прорисовываемых вершин
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами \param[in] texture - текстура интерфейса
	\param[in] vertPercent - Отображаемая часть по вертикали \param[in] horPercent - Отображаемая часть по горизонтали
	\param[in] selCol - Цвет выделения \param[in] selIntensivity - Мощность выделения \param[in] transparency - Прозрачнсть
	\return false, если были ошибки
	*/
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, float vertPercent, float horPercent, D3DXVECTOR4 selCol,
		float selIntensivity, float transparency);

private:
	//! Загрузка шейдера \param[in] device - графическое устрйоство \param[in] hwnd - ID окна \param[in] vsFilename - Расположение вершинного шейдера
	//! \param[in] psFilename - Расположение пиксельного шейдера \return false, если были ошибки
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
	void ShutdownShader();
	//! Вывести сообщение об ошибке \param[in] errorMessage - сообщение об ошибке \param[in] hwnd - ID окна \param[in] shaderFilename - имя файла шейдера
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	/*!
	Установка параметров шейдера \param[in] deviceContext - графическое устройство
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами \param[in] texture - текстура интерфейса
	\param[in] vertPercent - Отображаемая часть по вертикали \param[in] horPercent - Отображаемая часть по горизонтали
	\param[in] selCol - Цвет выделения \param[in] selIntensivity - Мощность выделения \param[in] transparency - Прозрачнсть
	\return false, если были ошибки
	*/
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, float vertPercent, float horPercent, D3DXVECTOR4 selCol,
		float selIntensivity, float transparency);
	//! Выполнение шейдера \param[in] deviceContext - графическое устройство \param[in] indexCount - количество прорисовываемых вершин
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader; //!<Вершинный шейдер
	ID3D11PixelShader* m_pixelShader; //!<Пиксельный шейдер
	ID3D11InputLayout* m_layout; //!<Макет
	ID3D11Buffer* m_matrixBuffer; //!<Матричный буфер
	ID3D11Buffer* m_paramsBuffer; //!<Параментрический буфер
	ID3D11SamplerState* m_sampleState; //!<Образцовая форма
};
/*! @} */
#endif