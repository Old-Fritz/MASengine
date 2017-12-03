////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "fontclass.h"
#include "FontShaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup InterfaceBlocks
@{
*/
/*!
Текст в интерфейсе
*/
class TextClass
{
private:
	//! Вершина меша
	struct VertexType
	{
		D3DXVECTOR3 position; //!<Позиция вершины в пространстве
		D3DXVECTOR2 texture; //!<Текстурные координаты в вершине
	};

public:
	//! Одно предложение текста
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer; //!<Вершинный и индексный буферы
		int vertexCount, indexCount; //!<Количество вершин и индексов
		int maxLength; //!<Максимальная длина строки
		D3DXVECTOR4 color; //!<Цвет текста
	};
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	//! Инициализация теста \param[in] device,deviceContext - графическое устройство /param[in] hwnd - id окна /param[in] screenWidth, screenHeight - ширина и высота экрана
	//!  \param[in] sentencesNum - количество предложений \param[in] maxLength - максимальная длина строки изображения  \param[in] orientation - положение текста (слева, справа, по центру)
	//! \param[in] fontFilename - файл шрифта \return false, если были ошибки
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, int sentencesNum,
		int maxLength, int orientation, PathClass* fontFilename);
	void Shutdown();
	//! Прорисовка текста \param[in] fontShader - шейдер текста \param[in] deviceContext - графическое устройство
	//! \param[in] worldMatrix, viewMatrix, baseViewMatrix - матрицы с параметрами \return false, если были ошибки
	bool Render(FontShaderClass* FontShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix);
	//! Изменение параметров текста  \param[in] deviceContext - графическое устройство \param[in] sentenceNum - индекс строки 
	//! \param[in] text - новый текст \param[in] positionX, positionY - новое положение на экране \param[in] size - размер символов 
	//! \param[in] width - ширина  блока с текстом \param[in] color - цвет текста   \return false, если были ошибки
	bool UpdateSentence(ID3D11DeviceContext* deviceContext, int sentenceNum, const std::wstring& text, int positionX, int positionY,
		float size, float width, D3DXVECTOR4 color);
private:
	//! Создание предложения  \param[in] device - графическое устройство \param[out] sentence - созданное предложение \param[шт] maxLength - максимальная длина строки \return false, если были ошибки
	bool InitializeSentence(ID3D11Device* device, SentenceType** sentence, int maxLength);
	//! Удаление предложения  \param[in] sentence - удаляемое предложение
	void ReleaseSentence(SentenceType** sentence);
	//! отрисовка предложения \param[in] fontShader - шейдер текста \param[in] deviceContext - графическое устройство \param[in] sentence - выводимое предложение
	//! \param[in] worldMatrix, viewMatrix, baseViewMatrix - матрицы с параметрами \return false, если были ошибки
	bool RenderSentence(FontShaderClass* FontShader, ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix,
		D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix);

private:
	PathClass* m_fontFilename; //!<Путь до файла со шрифтом
	int m_screenWidth, m_screenHeight; //!<Размеры экрана
	int m_sentencesNum; //!<Количество предложений
	int m_maxLength; //!<Максимальная длина строки
	SentenceType** m_sentences; //!<Предложения
	enum orient { LEFT, MIDDLE, RIGHT } m_orientation = LEFT;  //!<Ориентация текста

};
/*! @} */

#endif