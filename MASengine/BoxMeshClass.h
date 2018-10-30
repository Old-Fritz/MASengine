////////////////////////////////////////////////////////////////////////////////
// Filename: BoxMeshClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BOXMESHCLASS_H_
#define _BOXMESHCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "PathManagerClass.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10math.h>
#include "MemoryManager.h"
#include "frustumClass.h"


///////////////////////
// MY CLASS INCLUDES //
///////////////////////



////////////////////////////////////////////////////////////////////////////////
// Class name: BoxMeshClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Meshes
@{
*/
/*!
Меш box
*/
class BoxMeshClass
{
private:
	//! Вершина меша
	struct VertexType
	{
		D3DXVECTOR3 position; //!<Позиция вершины в пространстве
		D3DXVECTOR2 texture; //!<Текстурные координаты в вершине
		D3DXVECTOR3 normal; //!<Нормальный вектор
	};
public:
	BoxMeshClass();
	BoxMeshClass(const BoxMeshClass&);
	~BoxMeshClass();

	//! Создание меша \param[in] device - графическое устройство \param[in] minPoint, maxPoint - крайние точки \return false, если были ошибки
	bool Initialize(ID3D11Device* device, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint);
	void Shutdown();
	//! Установка меша в цепь рендринга \param[in] deviceContext - графическое устройство
	void Render(ID3D11DeviceContext* deviceContext);

	//!Получение центра меша \return координаты центра
	D3DXVECTOR3 getCenter();
	//!Получение размера меша \return вектор, состоящий из размеров меша по трем осям
	D3DXVECTOR3 getSize();

	//! Проверка меша на соответствие области видимости \param[in] frustum - усеченный конус \param[in] position - позиция меша \return false, если меш не входит в область видимости, true, если входит
	bool checkFrustum(FrustumClass* frustum, D3DXVECTOR3 position);

	//! \brief Получение количества индексов вершин \return Кол-во индексов вершин основного меша, или один раз меша box, если вызывалось RenderBoxMesh()
	int GetIndexCount();

	//! Проверка меша на пересечение с лучом  \param[in] rayOrigin - основание луча \param[in] rayDirection - направление луча
	//! \return false, если не было пересечений, true, если были
	bool intersect(D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection);
	//! Проверка полигона на пересечение с лучом  \param[in] rayOrigin - основание луча \param[in] rayDirection - направление луча
	//! param[in] v0, v1, v2 - координаты полигона \param[out] res - результат теста, включающий расстояние точки пересечения от rayOrigin и барицентрические координаты
	//! \return false, если не было пересечений, true, если были
	bool triangleHitTest(D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3& res);
private:
	//! Создание массивов вершин и индексов \param[in] deviceContext - графическое устройство \param[out] indices массив индексов вершин \return false, если были ошибки
	bool createVertsAndInds(unsigned long** indices);
	//! Инициализация буферов \param[in] device - графическое устройство \return false, если были ошибки
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	//! Установка буферов в цепь рендринга \param[in] deviceContext - графическое устройство
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
private:
	D3DXVECTOR3* m_points; //!<Вершины меша
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer; //!<Вершинный и индексный буферы
	int m_vertexCount, m_indexCount; //!<Количество вершин и индексов
	D3DXVECTOR3 m_minPoint, m_maxPoint; //!<Крайние точки
};

#endif