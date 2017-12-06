////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include<d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Systems Вспомогательные графические системы
\ingroup Graphics
@{
*/
/*!
 Камера
*/
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	//! Установка позици¤ камеры \param[in] pos - координаты камеры
	void SetPosition(D3DXVECTOR3 pos);
	//! Установка поворота камеры \param[in] rot - координаты поворота камеры (ypr)
	void SetRotation(D3DXVECTOR3 rot);

	//! Получение позиции камеры \return координаты модели
	D3DXVECTOR3 GetPosition();
	//! Получение поворота камеры \return координаты поворота модели (ypr)
	D3DXVECTOR3 GetRotation();

	//! Обновление видовой матрицы в зависимости от позиции и поворота
	void Render();
	//! Получение видовой матрицы \param[in] viewMatrix - полученная видовая матрица
	void GetViewMatrix(D3DXMATRIX& viewMatrix);

	//! Обновление видовой отраженной матрицы \param[in] height - высота отражения
	void RenderReflection(float height);
	//! Получение отраженной видовой матрицы \return полученная видовая отраженная матрица
	D3DXMATRIX GetReflectionViewMatrix();

	//! Создание специальной матрицы вида \param[in] pos - позиция камеры \param[in] rot - поворот камеры \return полученная матрица
	D3DXMATRIX createViewMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	D3DXVECTOR3 m_position; //!<Позиция камеры
	D3DXVECTOR3 m_rotation; //!<Поворот камеры
	D3DXMATRIX m_viewMatrix; //!<Видовая матрица
	D3DXMATRIX m_reflectionViewMatrix;  //!<Видовая отраженная матрица
};
/*! @} */
#endif