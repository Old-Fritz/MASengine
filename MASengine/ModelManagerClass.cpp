#include "ModelManagerClass.h"

ModelManagerClass* ModelManagerClass::m_instance = 0;

void ModelManagerClass::Shutdown()
{
	for (auto model = m_models.begin(); model != m_models.end();model++)
	{
		if (*model)
		{
			(*model)->Shutdown();
			::operator delete(*model, sizeof(**model), 2);
		}
	}
	m_models.clear();

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

ModelManagerClass & ModelManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) ModelManagerClass;
	return *m_instance;
}

void ModelManagerClass::addModel(ModelClass * model)
{
	m_models.emplace(model);
}

void ModelManagerClass::deleteModel(ModelClass * model)
{
	m_models.erase(model);
}

bool ModelManagerClass::Render(ModelShaderClass * modelShader, ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition,
	D3DXVECTOR4 specularColor, float specularPower, float SCREEN_DEPTH, FrustumClass * frustum)
{
	bool result;

	D3DXMATRIX oldWorldMatrix = worldMatrix;

	for (auto model = m_models.begin(); model != m_models.end();model++)
	{
		
		//get model params of matrix
		MeshClass::rotateMatrix(worldMatrix, (*model)->getRotation());
		MeshClass::translateMatrix(worldMatrix, (*model)->getPosition());

		result = (*model)->Render(modelShader, deviceContext, worldMatrix, viewMatrix, projectionMatrix, lightDirection, ambientColor,
			diffuseColor, cameraPosition, specularColor, specularPower, SCREEN_DEPTH, frustum);
		if (!result)
			return false;

		//return to the old params of matrix
		worldMatrix = oldWorldMatrix;
		//MeshClass::translateMatrix(worldMatrix, -(*model)->getPosition());
		//MeshClass::rotateMatrix(worldMatrix, -(*model)->getRotation());
		
	}
	return true;
}

ModelManagerClass::ModelManagerClass()
{
}
ModelManagerClass::ModelManagerClass(const ModelManagerClass &)
{
}
ModelManagerClass::~ModelManagerClass()
{
}
