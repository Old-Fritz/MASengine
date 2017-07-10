////////////////////////////////////////////////////////////////////////////////
// Filename: SliderElementClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _SLIDERELEMENTCLASS_H_
#define _SLIDERELEMENTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "InterfaceElementClass.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: SliderElementClass
////////////////////////////////////////////////////////////////////////////////
class SliderElementClass : public InterfaceElementClass
{
public:

	SliderElementClass();
	SliderElementClass(const SliderElementClass&);
	~SliderElementClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, const std::string& filename,
		int screenWidth, int screenHeight);


	//sectors changing
	void updateSectors(int num);
	void setCurrentSector(int sector);
	int findSector(float posX, float posY);
	void updateSliderPos();

	void updateSector(float posX, float posY);
	void setVisible(bool visible);
	 void updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY);
	 void endDrag();
	void nextSector();
	void backSector();

	//Actions on click
	std::string pick(int posX, int posY);
	void addSelCommand(const std::string& name);
	void addUnSelCommand(const std::string& name);
	void addPickCommand(const std::string& name);
	void addUnPickCommand(const std::string& name);
	void addSliderCommand();
	//CommandClass* makeChanges(CommandClass* command, int num);

private:
	//Work with files
	bool readFromFile(const std::string& filename);

private:
	int m_currentSector;
	int m_sectorsNum;
	int m_orientation;
	bool m_dragType = 0;  //Type of drag. 0 - drag of element, 1 - drag of slider
	std::string m_sliderCommand;
};


#endif