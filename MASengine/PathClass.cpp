#include "PathClass.h"

PathClass::PathClass()
{
	m_path = "";
	m_prefix = "";
}

PathClass::PathClass(const std::string & filename)
{
	m_path = filename;
	m_prefix = "";
}

PathClass::PathClass(const std::wstring & filename)
{
	m_path = m_converter.to_bytes(filename);
	m_prefix = "";
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
	if (m_prefix.size() == 0) //give only path if there are no prefix
		return m_path;
	else
		return m_prefix + m_path;
}

std::wstring PathClass::getWPath()
{
	if (m_prefix.size() == 0) //give only path if there are no prefix
		return m_converter.from_bytes(m_path);
	else
		return m_converter.from_bytes(m_prefix + m_path);
}

std::string PathClass::getShortPath()
{
	return m_path;
}

std::wstring PathClass::getShortWPath()
{
	return m_converter.from_bytes(m_path);
}

void PathClass::changePath(const std::string & filename)
{
	m_path = filename;
}

void PathClass::changePath(const std::wstring & filename)
{
	m_path = m_converter.to_bytes(filename);
}

void PathClass::changePrefix(const std::string & prefix)
{
	m_prefix = prefix;
}

void PathClass::changePrefix(const std::wstring & prefix)
{
	m_prefix = m_converter.to_bytes(prefix);
}
