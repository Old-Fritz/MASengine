////////////////////////////////////////////////////////////////////////////////
// Filename: LoadManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LOADSCREENMANAGERCLASS_H_
#define _LOADSCREENMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "D3DClass.h"
#include "GlobalManagerClass.h"
#include "InterfaceElementClass.h"
#include "ShaderManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: LoadManagerClass
////////////////////////////////////////////////////////////////////////////////
class LoadScreenManagerClass
{

public:
	bool Initialize(D3DClass* D3D, ShaderManagerClass* shaders, D3DXMATRIX baseViewMatrix, HWND hwnd, PathClass*  filename);
	void Shutdown();
	bool Render();

	static LoadScreenManagerClass& getI();

	void changeLine(const std::string& text, float perCent);
	void showElements();
	void hideElements();
private:
	LoadScreenManagerClass();
	LoadScreenManagerClass(const LoadScreenManagerClass&);
	~LoadScreenManagerClass();

	bool readFromFile(PathClass*  filename);

	bool updateElements();
	

	//line element
	void changeLinePosition(float perCent);
	bool changeLineText(const std::string& text);

	//image element
	bool changeImage();

	//quote element
	bool changeQuote();
private:
	static LoadScreenManagerClass* m_instance;
	long long int m_lastTime;

	//graphics parts
	D3DXMATRIX m_baseViewMatrix;
	ShaderManagerClass* m_shaders;
	D3DClass* m_D3D;

	//elements
	InterfaceElementClass* m_loadImage;
	InterfaceElementClass* m_loadLine;
	InterfaceElementClass* m_loadQuote;
	std::vector<InterfaceElementClass*> m_additionalElements;

	//Image parts
	PathClass*  m_imageElementName;
	std::vector<PathClass*> m_loadImageNames;
	bool m_imageChangeRand;
	int m_imageChangeInterval;
	int m_imageCurrentInterval;
	int m_currentImage;

	//Quote parts
	PathClass*  m_quoteElementName;
	std::vector<std::string> m_loadQuoteNames;
	bool m_quoteChangeRand;
	int m_quoteChangeInterval;
	int m_quoteCurrentInterval;
	int m_currentQuote;

	//Line parts
	PathClass*  m_lineElementName;

	//Additional elements parts
	std::vector<PathClass* > m_additionalElementNames;

};


#endif