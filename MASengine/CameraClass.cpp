////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"

CameraClass::CameraClass()
{
	m_position = D3DXVECTOR3(0,0,0);

	m_rotation = D3DXVECTOR3(0, 0, 0);
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(D3DXVECTOR3 pos)
{
	m_position = pos;
	return;
}


void CameraClass::SetRotation(D3DXVECTOR3 rot)
{
	m_rotation = rot;
	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return m_position;
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return m_rotation;
}

void CameraClass::Render()
{
	// update view matrix with new position and rotation
	m_viewMatrix = createViewMatrix(m_position, m_rotation);

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void CameraClass::RenderReflection(float height)
{
	D3DXVECTOR3 up, position, lookAt;
	float radians;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	// For planar reflection invert the Y position of the camera.
	position = m_position;
	position.y = -m_position.y + (height * 2.0f);

	// Calculate the rotation in radians.
	radians = m_rotation.y * 0.0174532925f;

	// Setup where the camera is looking.
	lookAt.x = sinf(radians) + m_position.y;
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + m_position.z;

	// Create the reflection view matrix from the three vectors.
	D3DXMatrixLookAtLH(&m_reflectionViewMatrix, &position, &lookAt, &up);

	return;
}

D3DXMATRIX CameraClass::GetReflectionViewMatrix()
{
	return m_reflectionViewMatrix;
}

D3DXMATRIX CameraClass::createViewMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	D3DXVECTOR3 up, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX resultMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rot.x * 0.0174532925f;
	yaw = rot.y * 0.0174532925f;
	roll = rot.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = pos + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&resultMatrix, &pos, &lookAt, &up);

	return resultMatrix;
}
