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
class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(D3DXVECTOR3 position);
	void SetRotation(D3DXVECTOR3 rotation);

	void GetPosition(D3DXVECTOR3& position);
	void GetRealPosition(D3DXVECTOR3& positionz);
	void GetRotation(D3DXVECTOR3& rotation);
	void GetRealRotation(D3DXVECTOR3& rotation);

	void SetFrameTime(float time);

	//set move params
	void setMove(int ind, bool type);

	//Make moves
	void Move();
	void MoveLeftward(bool keydown);
	void MoveRightward(bool keydown);
	void MoveForward(bool keydown);
	void MoveBackward(bool keydown);
	void MoveUpward(bool keydown);
	void MoveDownward(bool keydown);
	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);
	void LookUpward(bool keydown);
	void LookDownward(bool keydown);

private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;

	float m_frameTime;
	int m_posAnimationSpeed; //ms of one animation for pos
	int m_rotAnimationSpeed; //ms of one animation for rot
	int m_rotTempSpeed; //ms for current ror animation
	int m_posTempSpeed; //ms for current pos animation

	bool m_isMoving[NUM_OF_MOVES];

	//speed of dif directions
	float m_moveSpeed;
	float m_leftwardSpeed, m_rightwardSpeed;
	float m_forwardSpeed, m_backwardSpeed;
	float m_upwardSpeed, m_downwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;

	//speed of animation on dif directions
	D3DXVECTOR3 m_posSpeed;
	D3DXVECTOR3 m_rotSpeed;
};

#endif