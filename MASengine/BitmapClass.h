////////////////////////////////////////////////////////////////////////////////
// Filename: bitmapclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BITMAPCLASS_H_
#define _BITMAPCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include"GlobalManagerClass.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: BitmapClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup InterfaceBlocks Части элемента интерфейса
\ingroup Interface
@{
*/
/*!
Изображение в интерфейсе
*/
class BitmapClass
{
private:

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();

	//! Инициализация bitmap \param[in] device - графическое устройство /param[in] screenWidth, screenHeight - ширина и высота экрана
	//! /param[in] textureFilename - текстура изображения /param[in] bitmapHeight, bitmapWidth - размеры изображения \return false, если были ошибки
	bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight, PathClass* textureFilename,
		int bitmapWidth, int bitmapHeight);
	void Shutdown();
	//! Отрисовка изображения  \param[in] deviceContext - графическое устройств \param[in] positionX, positionY - позиция изображения \return false, если были ошибки
	bool Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY);

	//! Изменение текстуры изображения \param[in] device - графическое устройство /param[in] textureFilename - новая текстура изображения \return false, если были ошибки
	bool setNewTexture(ID3D11Device* device, PathClass* filename);
	//!  Получение количества индексов вершин \return Кол-во индексов вершин прямоугольника с изображением (6 по умолчанию)
	int GetIndexCount();
	//! Получение текстуры изображения \return текстуру в виде ресурса directx
	ID3D11ShaderResourceView* GetTexture();

private:
	//! Инициализация буферов \param[in] device - графическое устройство \return false, если были ошибки \return false, если были ошибки
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	//! Изменение буферов \param[in] deviceContext - графическое устройств \param[in] positionX, positionY - позиция изображения \return false, если были ошибки
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY);
	//! Установка буферов в цепь рендринга \param[in] deviceContext - графическое устройство
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer; //!<Вершинный и индексный буфера
	int m_vertexCount, m_indexCount; //!<Количество вершин и индексов
	int m_screenWidth, m_screenHeight; //!<Размеры экрана
	PathClass* m_filename; //!<Имя файла с текстурой

	int m_bitmapWidth, m_bitmapHeight; //!<Размеры изображения
	int m_previousPosX, m_previousPosY; //!<Предыдущее положение изображения
};
/*! @} */
#endif