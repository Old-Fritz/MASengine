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

void TextManagerClass::addText(const std::string & name, const std::string & filename)
{
	m_texts.emplace(std::pair<long long, std::wstring>(ModManagerClass::getI().getHash(name), getTextFromFile(name, filename)));
}

std::wstring TextManagerClass::getText(const std::string & name, const std::string & filename)
{
	auto text = m_texts.find(ModManagerClass::getI().getHash(name));

	if (text != m_texts.end())
		return text->second;
	else
		return m_converter.from_bytes(name);
}

std::wstring TextManagerClass::getTextFromFile(const std::string & name, const std::string & filename)
{
	std::wifstream file;
	file.open(ModManagerClass::getI().getDirectory(ModManagerClass::getI().getHash(filename)) + filename);
	std::wstring temp1 = m_converter.from_bytes("}");
	std::wstring temp2;
	while (file)
	{
		file >> temp2;
		if (temp2 == name)
		{
			if (temp1[temp1.size() - 1] == '}')
			{
				break;
			}
		}
		temp1 = temp2;
	}
	if (temp2 != m_converter.from_bytes(name))
	{
		file.close();
		return m_converter.from_bytes(name);
	}
		
	else
	{
		temp1.clear();
		wchar_t a = ' ';
		while (a != '{' && file)
		{
			a = file.get();
		}
		a = file.get();
		while (a != '}' && file)
		{
			if (a < ' ')
				a = ' ';
			temp1 += a;
			a = file.get();
		}
		file.close();
		return temp1;
	}
}
