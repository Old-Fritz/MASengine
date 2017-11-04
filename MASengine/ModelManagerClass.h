////////////////////////////////////////////////////////////////////////////////
// Filename: ModelManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _MODELMANAGERCLASS_H_
#define _MODELMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GlobalManagerClass.h"
#include "ModelClass.h"
#include "ModelShaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelManagerClass
////////////////////////////////////////////////////////////////////////////////

class ModelManagerClass
{
public:
	void Shutdown();

	static ModelManagerClass& getI();

	void addModel(ModelClass* model);
	void deleteModel(ModelClass* model);

	bool Render(ModelShaderClass* modelShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection,
		D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
		float SCREEN_DEPTH, FrustumClass* frustum);
private:
	ModelManagerClass();
	ModelManagerClass(const ModelManagerClass&);
	~ModelManagerClass();
private:
	std::set<ModelClass*> m_models;

	static ModelManagerClass* m_instance;

};

#endif