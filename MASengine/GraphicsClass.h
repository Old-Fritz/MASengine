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
#include "TextClass.h"


/////////////
// GLOBALS //
/////////////
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


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
	bool Frame();

private:
	bool Render();

private:

	TextClass* m_test;

	//blocks
	D3DClass* m_D3D;
	ShaderManagerClass* m_shaderManager;
	CameraClass * m_camera;


};

#endif