////////////////////////////////////////////////////////////////////////////////
// Filename: HeightMapLoaderClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _HEIGHTMAPLOADERCLASS_H_
#define _HEIGHTMAPLOADERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "MeshLoaderClass.h"
#include <map>

////////////////////////////////////////////////////////////////////////////////
// Class name: HeightMapLoaderClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Loaders
@{
*/
/*!
Загрузчик карт высот
*/
class HeightMapLoaderClass : public MeshLoaderClass
{
public:
	HeightMapLoaderClass();
	HeightMapLoaderClass(const HeightMapLoaderClass&);
	~HeightMapLoaderClass();

	//! Загрузка меша из файла и прменение масштаба \param[in] filename - путь до файла меша \param[in] sizeX, sizeY, sizeZ - размеры карты высот \return false, если были ошибки
	 bool loadModel(PathClass* filename, float sizeX = 0, float sizeY = 0, float sizeZ = 0);
	
	//! Подсчет кол-ва вершин и индексов \param[out] vertexCount - кол-во вершин \param[out] indexCount - кол-во индексов
	 void calcVertAndIndCount(int& vertexCount, int& indexCount);

	 //! Инициализация буферов \param[in] device - графическое устройство \param[in] vertices - массив вершин 
	 //! \param[in] indices - массив индексов вершин \return false, если были
	 bool createBuffers(ID3D11Device* device, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer, VertexType** verticies, unsigned long** indicies);

	void Shutdown();
private:
	//! Создание массивов вершин и индексов \param[in] deviceContext - графическое устройство \param[out] vertices - массив вершин
	//! \param[out] indices массив индексов вершин \return false, если были ошибки
	bool createVertsAndInds(VertexType** vertices, unsigned long** indices);

	//! Загрузка карты высот из файла \param[in] filename - путь до файла меша \return false, если были ошибки
	bool loadHeightMap(PathClass* filename); 
	//! Изменение размеров карты высот \param[in] sizeX, sizeY, sizeZ - размеры карты высот
	void normalizeHeightMap(float sizeX, float sizeY, float sizeZ);
	//! Расчет нормальных векторов \return false, если были ошибки
	bool calculateNormals();
	//! Расчет координат текстур
	void calculateTextureCoordinates();
private:
	int m_terrainWidth, m_terrainHeight; //!<Ширина и высота карты высот
	static std::map<std::pair<int, int>, ID3D11Buffer*> m_indexBuffers; //!< Повторяющиеся буфферы индексов
};
/*! @} */
#endif