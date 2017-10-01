////////////////////////////////////////////////////////////////////////////////
// Filename: FontManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _FONTMANAGERCLASS_H_
#define _FONTMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "FontClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: FontManagerClass
////////////////////////////////////////////////////////////////////////////////

class FontManagerClass
{
public:

	void Shutdown();

	static FontManagerClass& getI();

	bool addFont(ID3D11Device* device, PathClass* filename);
	FontClass* getFont(PathClass* filename);
	FontClass* getFont(int hash);
private:
	FontManagerClass();
	FontManagerClass(const FontManagerClass&);
	~FontManagerClass();
private:
	std::map<int, FontClass*> m_fonts;

	static FontManagerClass* m_instance;
};

#endif