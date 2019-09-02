////////////////////////////////////////////////////////////////////////////////
// Filename: MeshLoaderClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _MESHLOADERCLASS_H_
#define _MESHLOADERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "PathManagerClass.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10math.h>
#include "MemoryManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: MeshLoaderClass
////////////////////////////////////////////////////////////////////////////////
/*!
\defgroup Loaders Загрузчики мешей
\ingroup Meshes
@{
*/
/*!
Базовый загрузчик 
*/
class MeshLoaderClass
{
public:
	//! Вершина меша
	struct VertexType
	{
		D3DXVECTOR3 position; //!<Позиция вершины в пространстве
		D3DXVECTOR2 texture; //!<Текстурные координаты в вершине
		D3DXVECTOR3 normal; //!<Нормальный вектор
	};
	//! Помежуточный тип данных для обозначения вершины
	struct ModelType
	{
		float x, y, z;  //!<Позиция вершины в пространстве
		float tu, tv; //!<Текстурные координаты в вершине
		float nx, ny, nz; //!<Нормальный вектор
	};
public:
	MeshLoaderClass();
	MeshLoaderClass(const MeshLoaderClass&);
	~MeshLoaderClass();

	//! Подсчет кол-ва вершин и индексов \param[out] vertexCount - кол-во вершин \param[out] indexCount - кол-во индексов
	virtual void calcVertAndIndCount(int& vertexCount, int& indexCount);

	//! Инициализация буферов \param[in] device - графическое устройство \param[in] vertices - массив вершин 
	//! \param[in] indices - массив индексов вершин \return false, если были
	virtual bool createBuffers(ID3D11Device* device, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer, VertexType** verticies, unsigned long** indicies);

	//! Загрузка меша из файла и прменение масштаба \param[in] filename - путь до файла меша \param[in] sizeX, sizeY, sizeZ - масштаб меша \return false, если были ошибки
	virtual bool loadModel(PathClass* filename, float sizeX = 1, float sizeY = 1, float sizeZ = 1);
	
	virtual void getExtrPoints(D3DXVECTOR3& minPoint, D3DXVECTOR3& maxPoint);
	

	virtual void Shutdown();
protected:
	//! Создание массивов вершин и индексов \param[in] deviceContext - графическое устройство \param[out] vertices - массив вершин
	//! \param[out] indices массив индексов вершин \return false, если были ошибки
	virtual bool createVertsAndInds(VertexType** vertices, unsigned long** indices);

	virtual void findExtrPoints(VertexType* vertices);
	
	//! Загрузка меша из файла \param[in] filename - путь до файла меша \return false, если были ошибки
	bool loadMesh(PathClass* filename);
	//! Масштабировать меш \param[in] sizeX, sizeY, sizeZ - масштаб меша
	void normalizeMesh(float scaleX, float scaleY, float scaleZ);
protected:
	ModelType* m_model; //!<Временные данные о загружаемом меше
	int m_vertexCount, m_indexCount; //!<Количество вершин и индексов

	D3DXVECTOR3 m_minPoint, m_maxPoint;

};
/*! @} */
#endif