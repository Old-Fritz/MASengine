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
Класс для управления текстурами
*/
class TextureManagerClass
{
public:
	void Shutdown();

	bool Initialize(ID3D11Device* device);

	/*!
	Добавление текстуры \param[in] device - графическое устройство
	\param[in] filename - путь до текстуры
	\return false, если были ошибки
	*/
	bool addTexture(ID3D11Device* device, PathClass* filename);
	//! Удаление текстуры \param[in] filename - путь до текстуры
	void deleteTexture(PathClass* filename);
	//! Удаление текстуры \param[in] filename - путь до текстуры
	void deleteTexture(int hash);
	/*!
	Получение текстуры и добавление, емли её нет  \param[in] device - графическое устройство
	\param[in] filename - путь до текстуры
	\return NULL, если были ошибки
	*/
	TextureClass* getTexture(ID3D11Device* device, PathClass* filename);
	//! Получение загруженной текстуры по её хэшу
	TextureClass* getTexture(int hash);
private:
	TextureManagerClass();
	TextureManagerClass(const TextureManagerClass&);
	~TextureManagerClass();
private:
	std::map<int, TextureClass*> m_textures; //!<Текстуры
};
/*! @} */
#endif