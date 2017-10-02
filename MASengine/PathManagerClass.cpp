#include "PathManagerClass.h"

PathManagerClass* PathManagerClass::m_instance = 0;

void PathManagerClass::Shutdown()
{
	for (auto path = m_pathes.begin();path != m_pathes.end();path++)
	{
		(*path)->Shutdown();
		::operator delete(*path, sizeof(*(*path)), 2);
		*path = 0;
	}
	m_pathes.clear();

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

PathManagerClass & PathManagerClass::getI()
{
	if (!m_instance)
		m_instance = new PathManagerClass;
	return *m_instance;
}

bool PathManagerClass::Initialize()
{
	return true;
}

PathClass * PathManagerClass::makePath()
{
	PathClass* path = new(4) PathClass;
	m_pathes.emplace_back(path);
	return path;
}

PathClass * PathManagerClass::makePath(const std::string & filename)
{
	PathClass* path = new(4) PathClass(filename);
	m_pathes.emplace_back(path);
	return path;
}

PathClass * PathManagerClass::makePath(const std::wstring & filename)
{
	PathClass* path = new(4) PathClass(filename);
	m_pathes.emplace_back(path);
	return path;
}

PathManagerClass::PathManagerClass()
{
}

PathManagerClass::PathManagerClass(const PathManagerClass &)
{
}

PathManagerClass::~PathManagerClass()
{
}

