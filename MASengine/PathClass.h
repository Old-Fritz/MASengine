////////////////////////////////////////////////////////////////////////////////
// Filename: PathClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PATHCLASS_H_
#define _PATHCLASS_H_

#include <string>
#include "Utils.h"
#include "ModManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: PathClass
////////////////////////////////////////////////////////////////////////////////
class PathClass
{
public:
	PathClass();
	PathClass(const std::string& filename);
	PathClass(const std::wstring& filename);
	PathClass(const PathClass&);
	~PathClass();

	void Shutdown();

	std::string getPath();
	std::wstring getWPath();

	std::string getShortPath();
	std::wstring getShortWPath();

	void changePath(const std::string& filename);
	void changePath(const std::wstring& filename);

	void changePrefix(const std::string& prefix);
	void changePrefix(const std::wstring& prefix);

	int getHash();

	std::string getExpansion();
private:
	std::string m_path;
	std::string m_prefix; // special adding that change filepath in case of mods
};

std::ifstream &operator >> (std::ifstream &in, PathClass* path);

#endif