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
#include <corecrt_wstring.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextManagerClass
////////////////////////////////////////////////////////////////////////////////

class TextManagerClass
{
public:
	void Shutdown();

	static TextManagerClass& getI();

	void addText(const std::string& name, const std::string& filename);
	std::wstring getText(const std::string& name, const std::string& filename);

private:
	TextManagerClass();
	TextManagerClass(const TextManagerClass&);
	~TextManagerClass();

	std::wstring getTextFromFile(const std::string& name, const std::string& filename);

private:
	std::map<long long, std::wstring> m_texts;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> m_converter;

	static TextManagerClass* m_instance;
};

#endif