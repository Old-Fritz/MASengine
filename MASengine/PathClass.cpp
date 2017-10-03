#include "PathClass.h"

PathClass::PathClass()
{
	m_path = "";
	m_prefix = "";
}

PathClass::PathClass(const std::string & filename)
{
	changePath(filename);
}

PathClass::PathClass(const std::wstring & filename)
{
	PathClass(Utils::to_bytes(filename));
}

PathClass::PathClass(const PathClass &)
{

}

PathClass::~PathClass()
{
}

void PathClass::Shutdown()
{
	m_path.clear();
	m_prefix.clear();
}

std::string PathClass::getPath()
{
	//give only path if there are no prefix
	return m_prefix.size() == 0 ? m_path : m_prefix + m_path;
}

std::wstring PathClass::getWPath()
{
	return Utils::from_bytes(getPath());
}

std::string PathClass::getShortPath()
{
	return m_path;
}

std::wstring PathClass::getShortWPath()
{
	return Utils::from_bytes(getShortPath());
}

void PathClass::changePath(const std::string & filename)
{
	m_path = filename;

	changePrefix(ModManagerClass::getI().getDirectory(m_path));
}

void PathClass::changePath(const std::wstring & filename)
{
	changePath(Utils::to_bytes(filename));
}

void PathClass::changePrefix(const std::string & prefix)
{
	m_prefix = prefix;
}

void PathClass::changePrefix(const std::wstring & prefix)
{
	changePrefix(Utils::to_bytes(prefix));
}

int PathClass::getHash()
{
	return Utils::getHash(m_path);
}

std::string PathClass::getExpansion()
{
	std::string exp;

	//find strt of expansion in file
	int i;
	for (i = m_path.size() - 1;i >= 0;i--)
	{
		if (m_path[i] == '.')
			break;
	}
	//check for existing of expansion
	if (i == 0 || i == m_path.size() - 1)
		return "";

	//copy expansion to string
	for (++i;i < m_path.size();i++)
	{
		exp += m_path[i];
	}

	return exp;
}

std::ifstream &operator >> (std::ifstream &in, PathClass* path) {
	std::string str;
	in >> str;

	path->changePath(str);

	return in;
}