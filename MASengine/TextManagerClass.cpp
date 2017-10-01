#include "TextManagerClass.h"

TextManagerClass* TextManagerClass::m_instance = 0;

TextManagerClass::TextManagerClass()
{
}
TextManagerClass::TextManagerClass(const TextManagerClass &)
{
}
TextManagerClass::~TextManagerClass()
{
}

void TextManagerClass::Shutdown()
{
	while (!m_texts.empty())
	{
		m_texts.begin()->second.clear(); //delete all commands
		m_texts.erase(m_texts.begin());
	}

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

TextManagerClass & TextManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) TextManagerClass;
	return *m_instance;
}

void TextManagerClass::addText(const std::string & name, PathClass* filename)
{
	m_texts.emplace(std::pair<int, std::wstring>(Utils::getHash(name), Utils::getWTextFromFile(name, filename->getPath())));
}

std::wstring TextManagerClass::getText(const std::string & name, PathClass* filename)
{
	auto text = m_texts.find(Utils::getHash(name));

	if (text != m_texts.end())
		return text->second;
	else
	{
		addText(name, filename);
		return m_texts[Utils::getHash(name)];
	}
}
