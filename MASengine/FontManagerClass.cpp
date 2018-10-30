#include "FontManagerClass.h"

FontManagerClass* FontManagerClass::m_instance = 0;

FontManagerClass::FontManagerClass()
{
}
FontManagerClass::FontManagerClass(const FontManagerClass &)
{
}
FontManagerClass::~FontManagerClass()
{
}

void FontManagerClass::Shutdown()
{
	while (!m_fonts.empty())
	{
		m_fonts.begin()->second->Shutdown(); //delete texture
		::operator delete(m_fonts.begin()->second, sizeof(FontClass), 2);
		m_fonts.erase(m_fonts.begin());
	}

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

FontManagerClass & FontManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) FontManagerClass;
	return *m_instance;
}

bool FontManagerClass::addFont(ID3D11Device * device, PathClass* filename)
{
	bool result;

	//check if font already exists
	if (m_fonts.find(filename->getHash()) != m_fonts.end())
		return true;

	//create new font
	FontClass* newFont = new(4) FontClass;
	if (!newFont)
		return false;

	result = newFont->Initialize(device, filename);
	if (!result)
	{
		GM::LM()->addLog("Error 11-2");
		return false;
	}

	m_fonts.emplace(std::pair<int, FontClass*>(filename->getHash(), newFont));

	return true;
}

FontClass * FontManagerClass::getFont(PathClass* filename)
{
	return getFont(filename->getHash());
}

FontClass * FontManagerClass::getFont(int hash)
{
	auto font = m_fonts.find(hash);
	return font != m_fonts.end() ? font->second : NULL;
}
