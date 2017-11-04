////////////////////////////////////////////////////////////////////////////////
// Filename: ModelClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "ModelShaderClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PathClass* meshFilename, PathClass* textureFilename);
	void Shutdown();
	bool Render(ModelShaderClass* modelShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection,
		D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
		float SCREEN_DEPTH, FrustumClass* frustum);

	//pick actions
	//bool pick(ID3D11DeviceContext * deviceContext, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, int& provNum, D3DXVECTOR3& point);

	//Getters
	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getRotation();

	//Setters
	void setPosition(D3DXVECTOR3 position);
	void setRotation(D3DXVECTOR3 rotation);
private:
	//mesh
	int m_meshHash;

	//textures
	int m_textureHash;

	//info
	D3DXVECTOR3 m_position; //position on map
	D3DXVECTOR3 m_rotation; //rotation on xyz
};

#endif
