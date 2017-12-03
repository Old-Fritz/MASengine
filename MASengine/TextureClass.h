////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11.h>
//#include <wchar.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Resources Различные ресурсы
@{
*/
/*!
Текстура
*/
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	//! Создание текстуры \param[in] device - графическое устройство \param[in] filename - путь до файла текстуры \return false, если были ошибки
	bool Initialize(ID3D11Device* device, const WCHAR* filename);
	void Shutdown();
	//! Получение текстуры \return текстуру в виде ресурса directx
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture; //! текстура в виде ресурса directx
};
/*! @} */
#endif