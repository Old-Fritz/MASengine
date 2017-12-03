////////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <string>
#include <vector>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "PathManagerClass.h"
#include"TextureManagerClass.h"
#include <D3DX10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: FontClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Resources
@{
*/
/*!
Шрифт
*/
class FontClass
{
private:
	//! тип для одной буквы шрифта, определяющий положение на текстуре
	struct FontType
	{
		float left, right; //!<Левая и правая границы символа на текстуре
		int size; //!< Размеры символа
	};
	//! Вершина для модели текстуры
	struct VertexType
	{
		D3DXVECTOR3 position; //!<Позиция вершины в пространстве
		D3DXVECTOR2 texture;  //!<Текстурные координаты в вершине
	};
	//!Совокупность данных об шрифте
	struct FontGroupType
	{
		std::map<int, FontType*> fonts; //!<Данные о шрифте, включающие числовой код для каждого символа

		FontType& operator [] (int ind)
		{
			//check for existing of symbol in map
			if (fonts.find(ind) == fonts.end())
				return *fonts[0];
			else
				return *fonts[ind];
		}
		//!Добавление нового символа в шрифт \param[in] ind - код символа \param[in] font - данные о символе
		void emplace(int ind, FontType* font)
		{
			fonts.emplace(std::pair<int, FontType*>(ind, font)); 
		}
		void Shutdown()
		{
			while (!fonts.empty())
			{
				::operator delete(fonts.begin()->second, sizeof(FontType), 4);
				fonts.begin()->second = 0;
				fonts.erase(fonts.begin());
			}
		}
	};

public:
	//! Тип расположение текста в блоке
	enum orient { LEFT, MIDDLE, RIGHT };
public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	//! Создание шрифта \param[in] device - графическое устройство \param[in] filename - путь до файла шрифта \return false, если были ошибки
	bool Initialize(ID3D11Device* device, PathClass* filename);
	void Shutdown();

	//! Получение текстуры шрифта \return текстуру в виде ресурса directx
	ID3D11ShaderResourceView* GetTexture(); 
	//! Создание модели шрифта \param[out] vertices - полученный массив вершин \param[in] sentence - текст  \param[in] drawX, drawY - позиция модели 
	//! \param[in] size - размер текста (величина букв) \param[in] width - ширина блока \param[in] orientation - расположение текста в блоке
	void BuildVertexArray(void* vertices, const std::wstring& sentence, float drawX, float drawY, float size, float width, orient orientation);

private:
	//! Загрузка данных о шрифте из файла  \param[in] filename - путь до файла данных шрифта \return false, если были ошибки
	bool LoadFontData(PathClass* filename);
	//! Освобождение памяти, занимаемой данными о шрифте
	void ReleaseFontData();
	//! Разделение строки на массив строк \param[in] sentence - текст  \param[in] size - размер текста (величина букв) \param[in] width - ширина блока
	//! \return массив пар (строка, ширина строки)
	std::vector<std::pair<std::wstring, int>> separateStrings(const std::wstring& sentence, float size, float width);

private:
	int m_maxSize; //!<Ширина самого большого символа
	FontGroupType m_Font; //!<Данные о шрифте
	PathClass* m_filename; //!<Путь до текстуры

	int m_SymbolsNum; //!<Количество символов в шрифте
	int m_SymbolsHeight; //!<Базовая высота символов
};
/*! @} */
#endif
