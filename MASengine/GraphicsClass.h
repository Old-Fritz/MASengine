////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "ShaderManagerClass.h"
#include "CameraClass.h"
#include "InterfaceClass.h"
#include "TerrainClass.h"
#include "LightClass.h"


/////////////
// GLOBALS //
/////////////
const float SCREEN_DEPTH = 5000.0f;
const float SCREEN_NEAR = 0.1f;
const int TEST_NUM = 1;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(HWND hwnd);
	void Shutdown();
	bool Frame(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int mouseX, int mouseY);

	bool updateInterface(CommandClass* command, int ind);
	bool updateGraphics(CommandClass* command,int ind);

	
private:
	bool Render();

	// Functions on pick
	void pick(int mouseX, int mouseY);
	void unPick(int mouseX, int mouseY);
	bool interfacePick(int mouseX, int mouseY, int& ind, std::string& name);
private:
	//params
	D3DXMATRIX m_baseViewMatrix;
	HWND m_hwnd;


	TerrainClass* m_test[TEST_NUM];
	//blocks
	D3DClass* m_D3D;
	ShaderManagerClass* m_shaderManager;
	CameraClass * m_camera;
	LightClass * m_light;
	InterfaceClass* m_interface;

	

};

#endif