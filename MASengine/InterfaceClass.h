////////////////////////////////////////////////////////////////////////////////
// Filename: interfaceclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INTERFACECLASS_H_
#define _INTERFACECLASS_H_

/////////////
// GLOBALS //
/////////////


//////////////
// INCLUDES //
//////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "InterfaceShaderClass.h"
#include "GroupElementClass.h"
#include "FontShaderClass.h"
#include "LoadScreenManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: InterfaceClass
////////////////////////////////////////////////////////////////////////////////
class InterfaceClass
{
public:
	InterfaceClass();
	InterfaceClass(const InterfaceClass&);
	~InterfaceClass();

	//bool baseInitialize(ID3D10Device* device, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX BaseViewMatrix,
	//	ResourceManagerClass* resources);
	//bool elementsInitialize(ID3D10Device* device, HWND hwnd, int screenWidth, int screenHeight,
	//	D3DXMATRIX BaseViewMatrix, D3DClass* D3D);

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Frame(ID3D11DeviceContext* deviceContext, int mouseX, int mouseY);
	bool Render(InterfaceShaderClass* interfaceShader, FontShaderClass* fontShader,
		ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix);

	//update data of interface
	void setElVisible(const std::string& elname, bool visible);
	//void updateLoadScreen(string loadText, float loadPercent, D3DClass* D3D);
	void updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY);
	void startDrag(const std::string& elname);
	void endDrag(const std::string& elname);
	void setPos(ID3D11DeviceContext* deviceContext, const std::string& elname, int posX, int posY);

	//Updating params of bitmaps
	void setElBMvisible(const std::string& elname, const std::string& bmname, bool visible);
	void updateElBMposX(const std::string& elname, const std::string& bmname, int posX);
	void updateElBMposY(const std::string& elname, const std::string& bmname, int posY);
	void updateElBMvertPercent(const std::string& elname, const std::string& bmname, float vertPercent);
	void updateElBMhorPercent(const std::string& elname, const std::string& bmname, float horPercent);
	void updateElBMtranspar(const std::string& elname, const std::string& bmname, float transparency);
	void updateElBMselIntens(const std::string& elname, const std::string& bmname, float selIntensivity);
	void updateElBMselCol(const std::string& elname, const std::string& bmname, D3DXVECTOR4 selCol);
	void setBMCommand(const std::string& elname, const std::string& bmname, const std::string& commandType, const std::string& command);
	bool setNewBM(ID3D11Device* device, const std::string& elname, const std::string& bmname, const std::string& filename);

	//Updating params of strings
	void setElTvisible(const std::string& elname, const std::string& tname, bool visible);
	bool updateElTSposY(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, int posY);
	bool updateElTSposX(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, int posX);
	bool updateElTStext(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, const std::string& text);
	bool updateElTScolor(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, D3DXVECTOR4 color);
	bool updateElTSadding(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, const std::string& adding);

	//Special Slider commands
	void nextSector(const std::string& elname);
	void backSector(const std::string& elname);

	//Special List commands
	bool addElement(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, const std::string& elname);
	void deleteElement(ID3D11DeviceContext* deviceContext, const std::string& elname, int ind);
	void setLastElement(ID3D11DeviceContext * deviceContext, const std::string& elname, int ind);
	void clear(const std::string& elname);

	//Special settings command
	//CommandClass* getSettings(string elname, string tname, int ind, string settingsType, string filename, ID3D10Device* device, HWND hwnd);

	//Finding index of element by his name
	InterfaceElementClass* findElbyName(const std::string& name);
	//CommandClass* makeChanges(CommandClass* command, string name);

	//Getting data about elements
	bool getEl(int posX, int posY, int& ind, std::string& name);
	int getElTSsize(const std::string& elname, const std::string& tname, int ind);
	bool isElVisible(const std::string& elname);

	void UpdateDebug(ID3D11DeviceContext* deviceContext, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int mouseX, int mouseY);

	//Actions on pick and unpick
	void pick(int ind, const std::string& name);
	void unPick(int ind, const std::string& name);
	int getNumFromName(const std::string& name);
private:
	void CheckSelection(ID3D11DeviceContext* deviceContext, int mouseX, int mouseY);
private:
	std::vector<std::string> m_dragElements;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	int m_elementsNum;

	InterfaceElementClass** m_interfaceElements;

	int m_lastInd = 0;
	std::string m_lastName = "";

	bool m_isUnSel = true;
	bool m_LBMDown = false;
	
	CalculatorClass* m_calculator;
};

#endif