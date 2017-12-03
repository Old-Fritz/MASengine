////////////////////////////////////////////////////////////////////////////////
// Filename: TextureManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _TEXTUREMANAGERCLASS_H_
#define _TEXTUREMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "TextureClass.h"
#include "PathManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup ResourceManagment
@{
*/
/*!
Класс для управления текстурами, синглтон
*/
class TextureManagerClass
{
public:
	void Shutdown();
	//! \brief Получить экземпляр класса
	static TextureManagerClass& getI();

	/*!
	Добавление текстуры \param[in] device - графическое устройство
	\param[in] filename - путь до текстуры
	\return false, если были ошибки
	*/
	bool addTexture(ID3D11Device* device, PathClass* filename);
	//! Удаление текстуры \param[in] filename - путь до текстуры
	void deleteTexture(PathClass* filename);
	//! Получение текстуры \param[in] filename - путь до текстуры \return графический ресурс, текстура
	ID3D11ShaderResourceView* getTexture(PathClass* filename);
	//! см. getTexture, но по хэшу пути
	ID3D11ShaderResourceView* getTexture(int hash);
	//! Получение массива текстур \param[in] hashes - массив хэшей путей до файлов \return массив графических ресурсов, текстур
	ID3D11ShaderResourceView** getTexturesArray(int* hashes, int num);
private:
	TextureManagerClass();
	TextureManagerClass(const TextureManagerClass&);
	~TextureManagerClass();
private:
	std::map<int, TextureClass*> m_textures; //!<Текстуры

	static TextureManagerClass* m_instance; //!<Единственный экземпляр класса
	
};
/*! @} */
#endif