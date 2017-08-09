#include "positionclass.h"

PositionClass::PositionClass()
{
	m_position = D3DXVECTOR3(0, 0, 0);

	m_rotation = D3DXVECTOR3(0, 0, 0);


	m_frameTime = 0.0f;

	m_leftwardSpeed = 0.0f;
	m_rightwardSpeed = 0.0f;
	m_forwardSpeed = 0.0f;
	m_backwardSpeed = 0.0f;
	m_upwardSpeed = 0.0f;
	m_downwardSpeed = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed = 0.0f;
	m_lookDownSpeed = 0.0f;

	m_posSpeed = D3DXVECTOR3(0, 0, 0);
	m_rotSpeed = D3DXVECTOR3(0, 0, 0);

	m_moveSpeed = 1.0f;
	m_posAnimationSpeed = 200;
	m_rotAnimationSpeed = 400;
	m_posTempSpeed = 0;

	for (int i = 0; i < NUM_OF_MOVES;i++)
		m_isMoving[i] = 0;
}


PositionClass::PositionClass(const PositionClass& other)
{
}


PositionClass::~PositionClass()
{
}

void PositionClass::SetPosition(D3DXVECTOR3 position)
{
	//set borders to height
	if (position.y < 10.0f)
		position.y = 10.0f;
	if (position.y > 11736.0f)
		position.y = 11736.0f;

	m_posSpeed = -(m_position - position) / m_posAnimationSpeed;
	m_posTempSpeed = m_posAnimationSpeed;

	//m_positionX = x;
	//m_positionY = y;
	//m_positionZ = z;

	m_moveSpeed = 0.5f + m_position.y / 300.0f;

	return;
}


void PositionClass::SetRotation(D3DXVECTOR3 rotation)
{
	m_rotSpeed = -(m_rotation - rotation) / m_rotAnimationSpeed;
	m_rotTempSpeed = m_rotAnimationSpeed;


	//m_rotationX = x;
	//m_rotationY = y;
	//m_rotationZ = z;
	return;
}

D3DXVECTOR3 PositionClass::GetPosition()
{
	return m_position;
}

D3DXVECTOR3 PositionClass::GetRealPosition()
{
	//get position with unfinished animations
	return m_position + m_posTempSpeed*m_posSpeed;
}


D3DXVECTOR3 PositionClass::GetRotation()
{
	return m_rotation;
}

D3DXVECTOR3 PositionClass::GetRealRotation()
{
	//get rotation with unfinished animations
	return m_rotation + m_posTempSpeed*m_rotSpeed;
}

void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}

void PositionClass::setMove(int ind, bool type)
{
	if (ind < NUM_OF_MOVES)
		m_isMoving[ind] = type;
}

void PositionClass::Move()
{
	

	//update position animation
	if (m_posTempSpeed < m_frameTime)
	{
		m_position += m_posSpeed * m_posTempSpeed;
		m_posTempSpeed = 0;
	}
	else
	{
		m_position += m_posSpeed * m_frameTime;
		m_posTempSpeed -= m_frameTime;
	}
	//update rotation animation
	if (m_rotTempSpeed < m_frameTime)
	{
		m_rotation += m_rotSpeed * m_rotTempSpeed;
		m_rotTempSpeed = 0;
	}
	else
	{
		m_rotation += m_rotSpeed * m_frameTime;
		m_rotTempSpeed -= m_frameTime;
	}

	MoveLeftward(m_isMoving[0]);
	MoveRightward(m_isMoving[1]);
	MoveForward(m_isMoving[2]);
	MoveBackward(m_isMoving[3]);
	MoveUpward(m_isMoving[4]);
	MoveDownward(m_isMoving[5]);
	TurnLeft(m_isMoving[6]);
	TurnRight(m_isMoving[7]);
	LookUpward(m_isMoving[8]);
	LookDownward(m_isMoving[9]);
}

void PositionClass::MoveLeftward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_leftwardSpeed += m_frameTime * 0.004f;

		if (m_leftwardSpeed > (m_frameTime * 0.2f))
		{
			m_leftwardSpeed = m_frameTime * 0.2f;
		}
	}
	else
	{
		m_leftwardSpeed -= m_frameTime * 0.007f;

		if (m_leftwardSpeed < 0.0f)
		{
			m_leftwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.

	radians = (m_rotation.y - 90) * 0.0174532925f;

	// Update the position.
	m_position.x += sinf(radians) * m_leftwardSpeed * m_moveSpeed;
	m_position.z += cosf(radians) * m_leftwardSpeed * m_moveSpeed;

	//m_positionX -= m_leftwardSpeed;
}

void PositionClass::MoveRightward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_rightwardSpeed += m_frameTime * 0.004f;

		if (m_rightwardSpeed > (m_frameTime * 0.2f))
		{
			m_rightwardSpeed = m_frameTime * 0.2f;
		}
	}
	else
	{
		m_rightwardSpeed -= m_frameTime * 0.007f;

		if (m_rightwardSpeed < 0.0f)
		{
			m_rightwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = (m_rotation.y + 90) * 0.0174532925f;

	// Update the position.
	m_position.x += sinf(radians) * m_rightwardSpeed * m_moveSpeed;
	m_position.z += cosf(radians) * m_rightwardSpeed * m_moveSpeed;

	//m_positionX += m_rightwardSpeed;
}

void PositionClass::MoveForward(bool keydown)
{
	float radians;


	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_forwardSpeed += m_frameTime * 0.004f;

		if (m_forwardSpeed > (m_frameTime * 0.2f))
		{
			m_forwardSpeed = m_frameTime * 0.2f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 0.007f;

		if (m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotation.y * 0.0174532925f;

	// Update the position.
	m_position.x += sinf(radians) * m_forwardSpeed * m_moveSpeed;
	m_position.z += cosf(radians) * m_forwardSpeed * m_moveSpeed;

	return;
}

void PositionClass::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_backwardSpeed += m_frameTime * 0.004f;

		if (m_backwardSpeed > (m_frameTime * 0.2f))
		{
			m_backwardSpeed = m_frameTime * 0.2f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 0.007f;

		if (m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotation.y * 0.0174532925f;

	// Update the position.
	m_position.x -= sinf(radians) * m_backwardSpeed * m_moveSpeed;
	m_position.z -= cosf(radians) * m_backwardSpeed * m_moveSpeed;

	return;
}

void PositionClass::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_upwardSpeed += m_frameTime * 0.005f;

		if (m_upwardSpeed > (m_frameTime * 0.2f))
		{
			m_upwardSpeed = m_frameTime * 0.2f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 0.01f;

		if (m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_position.y += m_upwardSpeed * m_moveSpeed;

	return;
}

void PositionClass::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_downwardSpeed += m_frameTime * 0.005f;

		if (m_downwardSpeed > (m_frameTime * 0.2f))
		{
			m_downwardSpeed = m_frameTime * 0.2f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 0.01f;

		if (m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_position.y -= m_downwardSpeed * m_moveSpeed;
	//if (m_positionY < 10.0f)
	//m_positionY = 10.0f;

	return;
}

void PositionClass::TurnLeft(bool keydown)
{
	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if (m_leftTurnSpeed > (m_frameTime * 0.09f))
		{
			m_leftTurnSpeed = m_frameTime * 0.09f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime* 0.005f;

		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotation.y -= m_leftTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if (m_rotation.y < 0.0f)
	{
		m_rotation.y += 360.0f;
	}

	return;
}

void PositionClass::TurnRight(bool keydown)
{
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if (m_rightTurnSpeed > (m_frameTime * 0.09f))
		{
			m_rightTurnSpeed = m_frameTime * 0.09f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime* 0.005f;

		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotation.y += m_rightTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if (m_rotation.y > 360.0f)
	{
		m_rotation.y -= 360.0f;
	}

	return;
}


void PositionClass::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.01f;

		if (m_lookUpSpeed > (m_frameTime * 0.09f))
		{
			m_lookUpSpeed = m_frameTime * 0.09f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime* 0.005f;

		if (m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotation.x -= m_lookUpSpeed;

	// Keep the rotation in the 0 to 360 range.
	if (m_rotation.x < 0.0f)
	{
		m_rotation.x += 360.0f;
	}

	return;
}


void PositionClass::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.01f;

		if (m_lookDownSpeed > (m_frameTime * 0.09f))
		{
			m_lookDownSpeed = m_frameTime * 0.09f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime* 0.005f;

		if (m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotation.x += m_lookDownSpeed;

	// Keep the rotation in the 0 to 360 range.
	if (m_rotation.x > 360.0f)
	{
		m_rotation.x -= 360.0f;
	}

	return;
}