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

bool FontManagerClass::addFont(ID3D11Device * device, const std::string & filename)
{
	bool result;

	//check if font already exists
	if (m_fonts.find(ModManagerClass::getI().getHash(filename)) != m_fonts.end())
		return true;

	//create new font
	FontClass* newFont = new(4) FontClass;
	if (!newFont)
		return false;

	result = newFont->Initialize(device, filename);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 11-2");
		return false;
	}

	m_fonts.emplace(std::pair<long long, FontClass*>(ModManagerClass::getI().getHash(filename), newFont));

	return true;
}

FontClass * FontManagerClass::getFont(const std::string & filename)
{
	return m_fonts.find(ModManagerClass::getI().getHash(filename))->second;
}
