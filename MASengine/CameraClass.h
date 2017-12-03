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
\defgroup Systems ¬спомогательные графические системы
\ingroup Graphics
@{
*/
/*!
 амера
*/
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	//! ”становка позици¤ камеры \param[in] pos - координаты камеры
	void SetPosition(D3DXVECTOR3 pos);
	//! ”становка поворота камеры \param[in] rot - координаты поворота камеры (ypr)
	void SetRotation(D3DXVECTOR3 rot);

	//! ѕолучение позици¤ камеры \return координаты модели
	D3DXVECTOR3 GetPosition();
	//! ѕолучение поворота камеры \return координаты поворота модели (ypr)
	D3DXVECTOR3 GetRotation();

	//! ќбновление видовой матрицы в зависимости от позиции и поворота
	void Render();
	//! ѕолучение видовой матрицы \param[in] viewMatrix - полученна¤ видова¤ матрица
	void GetViewMatrix(D3DXMATRIX& viewMatrix);

	//! ќбновление видовой отраженной матрицы \param[in] height - высота отражени¤
	void RenderReflection(float height);
	//! ѕолучение отраженной видовой матрицы \return полученна¤ видова¤ отраженна¤ матрица
	D3DXMATRIX GetReflectionViewMatrix();

private:
	D3DXVECTOR3 m_position; //!<ѕозици¤ камеры
	D3DXVECTOR3 m_rotation; //!<ѕоворот камеры
	D3DXMATRIX m_viewMatrix; //!<¬идова¤ матрица
	D3DXMATRIX m_reflectionViewMatrix;  //!<¬идова¤ отраженна¤ матрица
};
/*! @} */
#endif