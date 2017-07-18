////////////////////////////////////////////////////////////////////////////////
// Filename: InterfaceElementClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _INTERFACEELEMENTCLASS_H_
#define _INTERFACEELEMENTCLASS_H_


//////////////
// INCLUDES //
//////////////

#include<string>
#include<codecvt>
#include<fstream>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Bitmapclass.h"
#include "Textclass.h"
#include "InterfaceShaderClass.h"
#include "GlobalManagerClass.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: InterfaceElementClass
////////////////////////////////////////////////////////////////////////////////
class InterfaceElementClass
{
protected:
	struct BitmapInfo
	{
		std::string name;
		std::string filename;
		float width;
		float height;
		float posX;
		float posY;

		std::string selAction;
		std::string unSelAction;
		std::string pickAction;
		std::string unPickAction;

		float vertPercent = 1.0f;
		float horPercent = 1.0f;
		float selIntensivity = 0;
		float transparency = 1.0f;
		D3DXVECTOR4 selCol = D3DXVECTOR4(1, 1, 1, 1);
		bool visible = true;
	};

	struct StringInfo
	{
		std::string text;
		std::string adding = "";
		float maxWidth;
		float size;
		D3DXVECTOR4 color;
		float posX;
		float posY;
	};

	struct TextInfo
	{
		std::string name;
		std::string fontFilename;
		std::string textFilename;
		float maxLength;
		int stringsNum;
		int orientation;
		StringInfo* strings;
		bool visible = true;
	};
public:

	InterfaceElementClass();
	InterfaceElementClass(const InterfaceElementClass&);
	~InterfaceElementClass();

	virtual bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, const std::string& filename,
		int screenWidth, int screenHeight);
	virtual void Shutdown();
	virtual bool Render(FontShaderClass* fontShader, InterfaceShaderClass* interfaceShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix);

	//Getting element`s data
	std::string getName();
	bool isVisible();

	//Updating element`s data
	virtual void setVisible(bool visible);
	virtual void updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY);
	virtual void startDrag( int posX, int posY);
	virtual void setPos(ID3D11DeviceContext* deviceContext, int posX, int posY);
	virtual void endDrag();

	virtual bool addElement(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd);
	virtual void deleteElement(ID3D11DeviceContext* deviceContext, int ind);
	virtual void clear();

	virtual InterfaceElementClass* getElement(int ind);

	//Finding index of element by his name
	int findBMbyName(const std::string& name);
	int findTbyName(const std::string& name);

	//Updating params of bitmaps
	virtual void setBMvisible(const std::string& name, bool visible);
	virtual void updateBMposX(const std::string& name, int posX);
	virtual void updateBMposY(const std::string& name, int posY);
	virtual void updateBMvertPercent(const std::string& name, float vertPercent);
	virtual void updateBMhorPercent(const std::string& name, float horPercent);
	virtual void updateBMtranspar(const std::string& name, float transparency);
	virtual void updateBMselIntens(const std::string& name, float selIntensivity);
	virtual void updateBMselCol(const std::string& name, D3DXVECTOR4 selCol);

	//Updating params of strings
	virtual void setTvisible(const std::string& name, bool visible);
	virtual bool updateTSposY(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, int posY);
	virtual bool updateTSposX(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, int posX);
	virtual bool updateTStext(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, const std::string& text);
	virtual bool updateTScolor(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, D3DXVECTOR4 color);
	virtual bool updateTSadding(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, const std::string& text);

	//Getting data about strings
	int getTSsize(const std::string& name, int ind);
	std::string getTStext(const std::string& name, int ind);

	//Actions on click
	virtual  std::string pick(int posX, int posY);
	virtual  void addSelCommand(const std::string& name);
	virtual  void addUnSelCommand(const std::string& name);
	virtual  void addPickCommand(const std::string& name);
	virtual  void addUnPickCommand(const std::string& name);

	virtual  void setSelCommand(const std::string& name, const std::string& action);
	virtual  void setUnSelCommand(const std::string& name, const std::string& action);
	virtual  void setPickCommand(const std::string& name, const std::string& action);
	virtual  void setUnPickCommand(const std::string& name, const std::string& action);
	//virtual CommandClass* makeChanges(CommandClass* command, int num);

	//Work with files
	//virtual std::string getTextFromFile(const std::string& name, const std::string& filename);
protected:
	//Work with files
	virtual bool readFromFile(const std::string& filename);
	bool readBitmapsInfoFromFile(std::fstream* file);
	bool readTextsInfoFromFile(std::fstream* file);
	bool updateSentence(ID3D11DeviceContext* deviceContext, int textInd, int sentenceInd);
protected:
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> m_converter;

	int m_screenWidth, m_screenHeight;
	std::string m_name;
	int m_posX, m_posY, m_width, m_height;

	int m_bitmapsNum;
	int m_textsNum;
	std::string m_actionsFileName;
	std::string m_fontsFileName;

	BitmapInfo* m_bitmapsInfo;
	TextInfo* m_textsInfo;
	BitmapClass** m_bitmaps;
	TextClass** m_texts;

	bool m_vertOrien;
	bool m_horOrien;
	bool m_visible = false;

	int m_lastX = 0;
	int m_lastY = 0;
};


#endif