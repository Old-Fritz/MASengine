////////////////////////////////////////////////////////////////////////////////
// Filename: meshClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHCLASS_H_
#define _MESHCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "HeightMapLoaderClass.h"
#include "BoxMeshClass.h"


///////////////////////
// MY CLASS INCLUDES //
///////////////////////



////////////////////////////////////////////////////////////////////////////////
// Class name: MeshClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Meshes Различные меши
\ingroup Resources
@{
*/
/*!
Базовый меш
*/
class MeshClass
{
private:
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
	MeshClass();
	MeshClass(const MeshClass&);
	~MeshClass();

	//! Создание меша \param[in] device - графическое устройство \param[in] filename - путь до файла меша
	//! \param[in] sizeX, sizeY, sizeZ - масштаб меша \return false, если были ошибки
	bool Initialize(ID3D11Device* device, PathClass* filename, int sizeX = 1, int sizeY = 1, int sizeZ = 1);
	void Shutdown();
	//! Установка меша в цепь рендринга \param[in] deviceContext - графическое устройство
	void Render(ID3D11DeviceContext* deviceContext);
	//! Установка меша box в цепь рендринга \param[in] deviceContext - графическое устройство
	void RenderBoxMesh(ID3D11DeviceContext* deviceContext);

	//! \brief Получение количества индексов вершин \return Кол-во индексов вершин основного меша, или один раз меша box, если вызывалось RenderBoxMesh()
	int GetIndexCount();

	//! Проверка меша на соответствие области видимости \param[in] frustum - усеченный конус \param[in] position - позиция меша \return false, если меш не входит в область видимости, true, если входит
	bool checkFrustum(FrustumClass* frustum, D3DXVECTOR3 position);
	//! Проверка меша на пересечение с лучом \param[in] deviceContext - графическое устройство  \param[in] rayOrigin - основание луча
	//! \param[in] rayDirection - направление луча \param[out] point - точка пересечения луча и меша ((0,0,0), если не было пересечения)
	//! \return false, если не было пересечений, true, если были
	bool intersect(ID3D11DeviceContext* deviceContext, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, D3DXVECTOR3& point);

	//static func
	//! Смещение матрицы \param[in, out] matrix - смещаемая матрица \param[in] transVector - вектор смещения
	static void translateMatrix(D3DXMATRIX& matrix, D3DXVECTOR3 transVector);
	//! Поворот матрицы \param[in, out] matrix - поворачиваемая матрица \param[in] rotVector - вектор поворота (ypr)
	static void rotateMatrix(D3DXMATRIX& matrix, D3DXVECTOR3 rotVector);
private:
	void ShutdownBuffers();
	//! Установка буферов в цепь рендринга \param[in] deviceContext - графическое устройство
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	//! Создание box меша  \param[in] device - графическое устройство \param[in] minPoint, maxPoint - крайние точки \return false, если были ошибки
	bool buildBoxMesh(ID3D11Device* device, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint);
	//! Нахождение крайних точек меша \param[in] vertices - массив вершин \param[out] minPoint, maxPoint - крайние точки
	void findExtrPoints(VertexType* vertices, D3DXVECTOR3& minPoint, D3DXVECTOR3& maxPoint);

	//! Создание массивов вершин и индексов \param[in] deviceContext - графическое устройство \param[out] vertices - массив вершин
	//! \param[out] indices массив индексов вершин \return false, если были ошибки
	bool getVertsAndInds(ID3D11DeviceContext* deviceContext, VertexType** verticies, unsigned long** indices);

	
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer; //!<Вершинный и индексный буферы
	int m_vertexCount, m_indexCount; //!<Количество вершин и индексов

	VertexType* m_verticies; //!< Данные вершин
	unsigned long* m_indicies; //!< Данные индексов

	BoxMeshClass* m_boxMesh; //!<меш box
	bool m_isBoxMeshRendering = 0; //! Необходимость в рендринге меша box

};
/*! @} */
#endif