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
#include "ModManagerClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TextManagerClass
////////////////////////////////////////////////////////////////////////////////

class TextManagerClass
{
public:

	TextManagerClass();
	TextManagerClass(const TextManagerClass&);
	~TextManagerClass();

	void Shutdown();

	bool addText(const std::string& name, const std::string& filename);
	const std::wstring& getText(const std::string& name, const std::string& filename);

private:
	const std::wstring& getTextFromFile(const std::string& name, const std::string& filename);

private:
	std::map<long long, const std::wstring&> m_texts;
};

#endif