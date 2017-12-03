///////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>
#include <d3dx10math.h>


const int NUM_OF_MOVES = 10;

////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////

/*!
Позиция игрока
*/
class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	//! Установка позиции (происходит не мгновенно) \param[in] position - новая позиция 
	void SetPosition(D3DXVECTOR3 position);
	//! Установка поворота (происходит не мгновенно) \param[in] rotation - новый поворот
	void SetRotation(D3DXVECTOR3 rotation);

	//! Получение текущей позиции \return позиция игрока
	D3DXVECTOR3 GetPosition();
	//! Получение позиции с учетом незавершенной анимации \return позиция игрока
	D3DXVECTOR3 GetRealPosition();
	//! Получение текущего поворота \return поворот игрока
	D3DXVECTOR3 GetRotation();
	//! Получение поворота с учетом незавершенной анимации \return поворот игрока
	D3DXVECTOR3 GetRealRotation();

	//! Установка кадрового времени \param[in] time - Время, прошедшее за  предыдущий кадр
	void SetFrameTime(float time);

	//! Установка движения в одну из сторон \param[in] ind - индекс направления смещения \param[in] type - тип смещения (0-остановить, 1-начать)
	void setMove(int ind, bool type);

	//Make moves
	//! Расчитать смещение позиции
	void Move();
	//! Расчитать смещение влево \param[in] keydown - 1, если идет смещение, 0, если не идет
	void MoveLeftward(bool keydown);
	//! Расчитать смещение влево \param[in] keydown - 1, если идет смещение, 0, если не идет
	void MoveRightward(bool keydown);
	//! Расчитать смещение прямо \param[in] keydown - 1, если идет смещение, 0, если не идет
	void MoveForward(bool keydown);
	//! Расчитать смещение назад \param[in] keydown - 1, если идет смещение, 0, если не идет
	void MoveBackward(bool keydown);
	//! Расчитать смещение наверх \param[in] keydown - 1, если идет смещение, 0, если не идет
	void MoveUpward(bool keydown);
	//! Расчитать смещение вниз \param[in] keydown - 1, если идет смещение, 0, если не идет
	void MoveDownward(bool keydown);
	//! Расчитать поворот влево \param[in] keydown - 1, если идет поворот, 0, если не идет
	void TurnLeft(bool keydown);
	//! Расчитать поворот вправо \param[in] keydown - 1, если идет поворот, 0, если не идет
	void TurnRight(bool keydown);
	//! Расчитать поворот вверх \param[in] keydown - 1, если идет поворот, 0, если не идет
	void LookUpward(bool keydown);
	//! Расчитать поворот вниз \param[in] keydown - 1, если идет поворот, 0, если не идет
	void LookDownward(bool keydown);
	

private:
	D3DXVECTOR3 m_position; //!<Текущая позиция
	D3DXVECTOR3 m_rotation; //!<Текущий поворот

	float m_frameTime; //!<Время, прошедшее за  предыдущий кадр
	int m_posAnimationSpeed; //!<Время в мс для одной анимации по перемещению
	int m_rotAnimationSpeed; //!<Время в мс для одной анимации по повороту
	int m_rotTempSpeed; //!<Время в мс до конца текущей анимации по повороту
	int m_posTempSpeed; //!<Время в мс до конца текущей анимации по перемещению

	bool m_isMoving[NUM_OF_MOVES]; //!<Направления, по которым происходят смещения

	//speed of dif directions
	float m_moveSpeed; //!<Модификатор скорости, определяющий зависимость высоты от скорости смещений
	float m_leftwardSpeed, m_rightwardSpeed; //!<Текущая скорость смещения влево/вправо
	float m_forwardSpeed, m_backwardSpeed; //!<Текущая скорость смещения прямо/назад
	float m_upwardSpeed, m_downwardSpeed; //!<Текущая скорость смещения вверх/вниз
	float m_leftTurnSpeed, m_rightTurnSpeed; //!<Текущая скорость поворота влево/вправо
	float m_lookUpSpeed, m_lookDownSpeed; //!<Текущая скорость поворота вверх/вниз

	//speed of animation on dif directions
	D3DXVECTOR3 m_posSpeed;  //!<Текущая скорость анимации по позиции
	D3DXVECTOR3 m_rotSpeed;   //!<Текущая скорость анимации по повороту
};

#endif