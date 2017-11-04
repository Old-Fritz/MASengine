///////////////////////////////////////////////////////////////////////////////
// Filename: ShaderManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADERMANAGERCLASS_H_
#define _SHADERMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "GlobalManagerClass.h"
#include "InterfaceShaderClass.h"
#include "FontShaderClass.h"
#include "TerrainShaderClass.h"
#include "ModelShaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderManagerClass
////////////////////////////////////////////////////////////////////////////////
class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();

	//getters
	InterfaceShaderClass* getInterfaceShader();
	FontShaderClass* getFontShader();
	TerrainShaderClass* getTerrainShader();
	ModelShaderClass* getModelShader();
private:
	//shaders
	InterfaceShaderClass* m_interfaceShader;
	FontShaderClass* m_fontShader;
	TerrainShaderClass* m_terrainShader;
	ModelShaderClass* m_modelShader;
};

#endif