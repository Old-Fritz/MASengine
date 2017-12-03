////////////////////////////////////////////////////////////////////////////////
// Filename: frustumclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FRUSTUMCLASS_H_
#define _FRUSTUMCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: FrustumClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Utils
@{
*/
/*!
Конус усечения для опредления элементов, входящих в область видимости
*/
class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	//! Создание конуса усучения \param[in] screenDepth - глубина экрана, задняя граница конуса \param[in] projectionMatrix, viewMatrix - матрицы с параметрами
	void ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix);
	//!Проверка точек  \param[in] points - масств точек на проверку  \param[in] size - размер массива точек  \param[in] position - смещение точек
	bool CheckPoints(D3DXVECTOR3* points, int size, D3DXVECTOR3 position);
	//!Проверка точки  \param[in] point - точка на проверку   \param[in] position - смещение точки
	bool CheckPoint(D3DXVECTOR3 point, D3DXVECTOR3 position);
	//!Проверка box  \param[in] points - масств точек box   \param[in] position - смещение box
	bool CheckBox(D3DXVECTOR3* points, D3DXVECTOR3 position);
private:
	D3DXPLANE m_planes[6]; //!<Плоскости, формирующие конус усечения
};
/*! @} */
#endif