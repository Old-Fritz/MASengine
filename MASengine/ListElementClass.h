////////////////////////////////////////////////////////////////////////////////
// Filename: ListElementClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _LISTELEMENTCLASS_H_
#define _LISTELEMENTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "SliderElementClass.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: ListElementClass
////////////////////////////////////////////////////////////////////////////////
class ListElementClass : public InterfaceElementClass
{
public:

	ListElementClass();
	ListElementClass(const ListElementClass&);
	~ListElementClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, PathClass* filename,
		int screenWidth, int screenHeight);
	void Shutdown();
	 bool Render(FontShaderClass* fontShader, InterfaceShaderClass* interfaceShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix);

	//Elements changing
	 void setVisible(bool visible);
	 void updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY);
	 void startDrag(int posX, int posY);
	 void endDrag();

	 bool addElement(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd);
	 void deleteElement(ID3D11DeviceContext* deviceContext, int ind);
	 void setLastElement(ID3D11DeviceContext* deviceContext, int ind);
	void clear();


	//Actions on click
	  std::string pick(int posX, int posY);
	  void addSelCommand(const std::string& name);
	  void addUnSelCommand(const std::string& name);
	  void addPickCommand(const std::string& name);
	  void addUnPickCommand(const std::string& name);

	//CommandClass* makeChanges(CommandClass* command, int num);

	//get elements
	InterfaceElementClass* getElement(int ind);

protected:
	//Work with files
	bool readFromFile(PathClass* filename);
	int getNumFromName(const std::string& name);

private:
	int m_lastElement = -1;   //index of last element in visible list
	int m_maxSize;    //max num of visible elements
	int m_interval;   //dist betwen elements
	int m_orientation;  //direction of elements
	int m_elementPosX; //position X of first element
	int m_elementPosY; //position Y of first element
	PathClass* m_sliderFilename;

	SliderElementClass* m_slider;

	int m_length = 0; // num of elements
	std::string m_elType;   //Type of element
	PathClass* m_elementFilename;
	std::vector<InterfaceElementClass*> m_elements;
};


#endif