#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_meshHash = 0;
	m_textureHash = 0;
	m_position = D3DXVECTOR3(0, 0, 0);
	m_rotation = D3DXVECTOR3(0, 0, 0);
}
ModelClass::ModelClass(const ModelClass &)
{
}
ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, PathClass * meshFilename, PathClass * textureFilename)
{
	bool result;

	//add texture
	result = TextureManagerClass::getI().addTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}
	m_textureHash = textureFilename->getHash();

	//add mesh
	result = MeshManagerClass::getI().addModel(device, meshFilename,100,100,100);
	if (!result)
	{
		return false;
	}
	m_meshHash = meshFilename->getHash();

	return true;
}

void ModelClass::Shutdown()
{
}

bool ModelClass::Render(ModelShaderClass * modelShader, ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower, float SCREEN_DEPTH, FrustumClass * frustum)
{
	bool result;

	//find mesh to render
	MeshClass* mesh = MeshManagerClass::getI().getModel(m_meshHash);

	//check if current mesh in frustum
	if (!mesh->checkFrustum(frustum, m_position))
		return true;

	// Render mesh
	mesh->Render(deviceContext);
	result = modelShader->Render(deviceContext, mesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		TextureManagerClass::getI().getTexture(m_textureHash), lightDirection, ambientColor, diffuseColor,
		cameraPosition, specularColor, specularPower);
	if (!result)
	{
		return false;
	}

	return true;
}

D3DXVECTOR3 ModelClass::getPosition()
{
	return m_position;
}

D3DXVECTOR3 ModelClass::getRotation()
{
	return m_rotation;
}

void ModelClass::setPosition(D3DXVECTOR3 position)
{
	m_position = position;
}

void ModelClass::setRotation(D3DXVECTOR3 rotation)
{
	m_rotation = rotation;
}
