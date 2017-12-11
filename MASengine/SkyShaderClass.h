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
Шейдер неба
*/
class SkyShaderClass
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
		D3DXVECTOR2 firstTranslation; //!< Сдвиг первой группы облаков
		D3DXVECTOR2 secondTranslation; //!< Сдвиг второй группы облаков
		float brightness; //!< Общая яркость
	};
public:
	SkyShaderClass();
	SkyShaderClass(const SkyShaderClass&);
	~SkyShaderClass();

	//! Загрузка шейдера \param[in] device - графическое устрйоство \param[in] hwnd - ID окна \param[in] filenameVS - Расположение вершинного шейдера
	//! \param[in] filenamePS - Расположение пиксельного шейдера \return false, если были ошибки
	bool Initialize(ID3D11Device* device, HWND hwnd, PathClass* filenameVS, PathClass* filenamePS);
	void Shutdown();
	/*!
	Прорисовка модели \param[in] deviceContext - графическое устройство \param[in] indexCount - количество прорисовываемых вершин
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами \param[in] texture1 - текстура первой группы облаков
	\param[in] texture2 - текстура второй группы облаков  \param[in] firstTranslation - Сдвиг первой группы облаков
	\param[in] secondTranslation - Сдвиг второй группы облаков \param[in] brightness - Общая яркость \return false, если были ошибки
	*/
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2, D3DXVECTOR2 firstTranslation, D3DXVECTOR2 secondTranslation,
		float brightness);

private:
	//! Загрузка шейдера \param[in] device - графическое устрйоство \param[in] hwnd - ID окна \param[in] vsFilename - Расположение вершинного шейдера
	//! \param[in] psFilename - Расположение пиксельного шейдера \return false, если были ошибки
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
	void ShutdownShader();
	//! Вывести сообщение об ошибке \param[in] errorMessage - сообщение об ошибке \param[in] hwnd - ID окна \param[in] shaderFilename - имя файла шейдера
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	/*!
	Установка параметров шейдера \param[in] deviceContext - графическое устройство
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами \param[in] texture1 - текстура первой группы облаков
	\param[in] texture2 - текстура второй группы облаков  \param[in] firstTranslation - Сдвиг первой группы облаков
	\param[in] secondTranslation - Сдвиг второй группы облаков \param[in] brightness - Общая яркость \return false, если были ошибки
	*/
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2, D3DXVECTOR2 firstTranslation, D3DXVECTOR2 secondTranslation,
		float brightness);

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