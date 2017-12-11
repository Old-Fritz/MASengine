////////////////////////////////////////////////////////////////////////////////
// Filename: SkyModelClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYMODELCLASS_H_
#define _SKYMODELCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "ProvManagerClass.h"
#include "GlobalManagerClass.h"
#include "ProvRegionManagerClass.h"
#include "SkyShaderClass.h"
#include "MeshClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: SkyModelClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Blocks Блоки ландшафта
\ingroup Graphics
@{
*/
/*!
Блок с ландшафтом, оснвгая структурная единица
*/
class SkyModelClass
{
public:
	SkyModelClass();
	SkyModelClass(const SkyModelClass&);
	~SkyModelClass();

	//! Загрузка блока \param[in] device, deviceContext - графическое устройство \param[in] skyFilename - путь до файла неба  \return false, если были ошибки
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PathClass* skyFilename);
	void Shutdown();

	/*!
	Прорисовка блок \param[in] deviceContext - графическое устройство \param[in] skyShader - шейдер неба 
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрам \return false, если были ошибки
	*/
	bool Render(ID3D11DeviceContext* deviceContext, SkyShaderClass* skyShader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
private:
	//! Получение данных из файла \param[in] blockFilename - путь до файла блока \return false, если были ошибки
	bool readFromFile(PathClass* filename);
private:
	int m_id; //!< ID блока

	float m_skyWidth, m_skyHeight;  //!<Ширина и высота блока 
	int m_meshHash; //!<Хэши меша плоскости неба
	PathClass* m_meshFilename; //!<Путь до файла меша плоскости неба

	//textures
	int m_texture1Hash; //!<Хэш текстуры облаков первого уровня
	PathClass* m_texture1Filename; //!<Путь до текстуры облаков первого уровня

	int m_texture2Hash; //!<Хэш текстуры облаков второго уровня
	PathClass* m_texture2Filename; //!<Путь до текстуры облаков второго уровня

	//info from file
	D3DXVECTOR3 m_position; //!<Позиция неба на карте
};
/*! @} */
#endif
