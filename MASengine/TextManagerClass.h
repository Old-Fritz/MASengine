////////////////////////////////////////////////////////////////////////////////
// Filename: TextManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _TEXTMANAGERCLASS_H_
#define _TEXTMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "PathClass.h"
#include <corecrt_wstring.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextManagerClass
////////////////////////////////////////////////////////////////////////////////

class TextManagerClass
{
public:
	void Shutdown();

	static TextManagerClass& getI();

	void addText(const std::string& name, PathClass* filename);
	std::wstring getText(const std::string& name, PathClass* filename);

private:
	TextManagerClass();
	TextManagerClass(const TextManagerClass&);
	~TextManagerClass();

private:
	std::map<long long, std::wstring> m_texts;

	static TextManagerClass* m_instance;
};

#endif