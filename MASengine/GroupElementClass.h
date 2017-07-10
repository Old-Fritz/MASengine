////////////////////////////////////////////////////////////////////////////////
// Filename: GroupElementClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _GROUPELEMENTCLASS_H_
#define _GROUPELEMENTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "InterfaceElementClass.h"
#include "ListElementClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GroupElementClass
////////////////////////////////////////////////////////////////////////////////
class GroupElementClass : public InterfaceElementClass
{
public:
	GroupElementClass();
	GroupElementClass(const GroupElementClass&);
	~GroupElementClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, const std::string& filename,
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
	 void deleteElement(int ind);
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

private:
	//Work with files
	bool readFromFile(const std::string& filename);
	int getNumFromName(const std::string& name);

private:
	int m_length = 0; // num of elements
	std::string m_elType;   //Type of element
	std::string m_elementFilename;
	std::vector<InterfaceElementClass*> m_elements;
};

#endif