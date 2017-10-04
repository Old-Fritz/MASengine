#include "FrustumClass.h"

FrustumClass::FrustumClass()
{
}

FrustumClass::FrustumClass(const FrustumClass &)
{
}

FrustumClass::~FrustumClass()
{
}

void FrustumClass::ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix)
{
	float zMinimum, r;
	D3DXMATRIX matrix;


	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	D3DXMatrixMultiply(&matrix, &viewMatrix, &projectionMatrix);

	// Calculate near plane of frustum.
	m_planes[0].a = matrix._14 + matrix._13;
	m_planes[0].b = matrix._24 + matrix._23;
	m_planes[0].c = matrix._34 + matrix._33;
	m_planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	// Calculate far plane of frustum.
	m_planes[1].a = matrix._14 - matrix._13;
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	// Calculate left plane of frustum.
	m_planes[2].a = matrix._14 + matrix._11;
	m_planes[2].b = matrix._24 + matrix._21;
	m_planes[2].c = matrix._34 + matrix._31;
	m_planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	// Calculate right plane of frustum.
	m_planes[3].a = matrix._14 - matrix._11;
	m_planes[3].b = matrix._24 - matrix._21;
	m_planes[3].c = matrix._34 - matrix._31;
	m_planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	// Calculate top plane of frustum.
	m_planes[4].a = matrix._14 - matrix._12;
	m_planes[4].b = matrix._24 - matrix._22;
	m_planes[4].c = matrix._34 - matrix._32;
	m_planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	// Calculate bottom plane of frustum.
	m_planes[5].a = matrix._14 + matrix._12;
	m_planes[5].b = matrix._24 + matrix._22;
	m_planes[5].c = matrix._34 + matrix._32;
	m_planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);

	return;
}

bool FrustumClass::CheckPoints(D3DXVECTOR3 * points, int size)
{
	int b = 0;
	// Check if any one point of the cube is in the view frustum.
	for (int i = 0;i < 6;i++)
	{
		for (int j = 0;j < size;j++)
		{
			if (D3DXPlaneDotCoord(&m_planes[i], &points[j]) >= 0.0f)
			{
				b = 1;
				break;
			}
		}
		if (!b)
			return false;
		b = 0;
	}

	return true;
}

bool FrustumClass::CheckPoint(D3DXVECTOR3 point)
{
	// Check if point is in the view frustum.
	for (int i = 0; i<6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &point) < 0.0f)
			return false;
	}

	return true;
}

bool FrustumClass::CheckBox(D3DXVECTOR3 * points)
{
	// Check if any one point of the cube is in the view frustum.
	for (int i = 0; i<6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &points[0]) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &points[1]) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &points[2]) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &points[3]) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &points[4]) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &points[5]) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &points[6]) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &points[7]) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}
