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

	bool addFont(ID3D11Device* device, const std::string& filename);
	FontClass* getFont(const std::string& filename);
private:
	FontManagerClass();
	FontManagerClass(const FontManagerClass&);
	~FontManagerClass();
private:
	std::map<long long, FontClass*> m_fonts;

	static FontManagerClass* m_instance;
};

#endif