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
class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	void ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix);
	bool CheckPoints(D3DXVECTOR3* points, int size, D3DXVECTOR3 position);
	bool CheckPoint(D3DXVECTOR3 point, D3DXVECTOR3 position);
	bool CheckBox(D3DXVECTOR3* points, D3DXVECTOR3 position);
private:
	D3DXPLANE m_planes[6];
};

#endif