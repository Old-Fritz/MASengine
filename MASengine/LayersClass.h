////////////////////////////////////////////////////////////////////////////////
// Filename: LayersClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LAYERSCLASS_H_
#define _LAYERSCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include"GlobalManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvClass
////////////////////////////////////////////////////////////////////////////////

class LayersClass
{

public:
	LayersClass();
	LayersClass(const LayersClass&);
	~LayersClass();

	//setters
	void setMainColor(GlobalManagerClass::regionType type);
	void setColor(GlobalManagerClass::regionType type, D3DXVECTOR4 color);

	//Getters
	D3DXVECTOR4 getMainColor();
	D3DXVECTOR4 getColor(GlobalManagerClass::regionType type);
private:
	//changable main color
	D3DXVECTOR4* m_mainColor;

	//layers
	D3DXVECTOR4 m_baseColor;
	D3DXVECTOR4 m_blockColor;
	D3DXVECTOR4 m_nationColor;
};

#endif