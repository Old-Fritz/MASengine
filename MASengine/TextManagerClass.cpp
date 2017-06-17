#include "TextManagerClass.h"

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
}

bool TextManagerClass::addText(const std::string & name, const std::string & filename)
{
	m_texts.emplace(std::pair<long long, std::wstring&>(ModManager.getHash(name), getTextFromFile(name, filename)));
}

const std::wstring & TextManagerClass::getText(const std::string & name, const std::string & filename)
{
	return m_texts.find(ModManager.getHash(name))->second;
}

std::wstring & TextManagerClass::getTextFromFile(const std::string & name, const std::string & filename)
{
	std::wifstream file;
	file.open(ModManager.getDirectory(ModManager.getHash(filename)) + filename);
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
