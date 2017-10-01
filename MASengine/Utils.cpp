#include "Utils.h"

int Utils::getHash(const std::string & str)
{
	const int p = 101;

	long long hash = 0;
	long long p_pow = 1;
	for (int i = 0; i < str.length();i++)
	{
		hash += static_cast<long long>(str[i] * p_pow);
		p_pow *= p;
	}

	return hash;
}

std::string Utils::getTextFromFile(const std::string & name, const std::string& filename)
{
	std::ifstream file;
	file.open(filename);
	std::string temp1 = "}";
	std::string temp2;

	//read all find and find line with name
	while (file)
	{
		file >> temp2;
		if (getHash(temp2) == getHash(name))
			//between words in dic symbol }
			if (temp1[temp1.size() - 1] == '}')
				break;
		//save last word to find end
		temp1 = temp2;
	}

	// if temp string and name are different, this is the end of file and there are no such text
	if (getHash(temp2) != getHash(name))
	{
		file.close();
		return name;
	}
	// save all symbols betweeb { and }
	else
	{
		temp1.clear();
		wchar_t a = ' ';

		// find start of text
		while (a != '{' && file)
		{
			a = file.get();
		}
		a = file.get(); //read '{'

						//read and save all symbols before end of text
		while (a != '}' && file)
		{
			// kick system symbols
			if (a < ' ')
				a = ' ';
			temp1 += a;
			a = file.get();
		}
		file.close();
		return temp1;
	}
}

std::wstring Utils::getWTextFromFile(const std::string & name, const std::string& filename)
{
	std::wifstream file;
	file.open(filename);
	std::wstring temp1 = from_bytes("}");
	std::wstring temp2;

	//read all find and find line with name
	while (file)
	{
		file >> temp2;
		if (getHash(to_bytes(temp2)) == getHash(name))
			//between words in dic symbol }
			if (temp1[temp1.size() - 1] == '}')
				break;
		//save last word to find end
		temp1 = temp2;
	}

	// if temp string and name are different, this is the end of file and there are no such text
	if (getHash(to_bytes(temp2)) != getHash(name))
	{
		file.close();
		return from_bytes(name);
	}
	// save all symbols betweeb { and }
	else
	{
		temp1.clear();
		wchar_t a = ' ';

		// find start of text
		while (a != '{' && file)
		{
			a = file.get();
		}
		a = file.get(); //read '{'

		//read and save all symbols before end of text
		while (a != '}' && file)
		{
			// kick system symbols
			if (a < ' ')
				a = ' ';
			temp1 += a;
			a = file.get();
		}
		file.close();
		return temp1;
	}
}

std::wstring Utils::from_bytes(const std::string& str)
{
	return m_converter.from_bytes(str);
}

std::string Utils::to_bytes(const std::wstring& str)
{
	return m_converter.to_bytes(str);
}