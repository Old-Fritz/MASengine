////////////////////////////////////////////////////////////////////////////////
// Filename: PathManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PATHMANAGERCLASS_H_
#define _PATHMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "PathClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GlobalManagerClass
////////////////////////////////////////////////////////////////////////////////
class PathManagerClass
{
public:
	void Shutdown();

	static PathManagerClass& getI();

	bool Initialize();

	PathClass* makePath();
	PathClass* makePath(const std::string& filename);
	PathClass* makePath(const std::wstring& filename);

private:
	PathManagerClass();
	PathManagerClass(const PathManagerClass&);
	~PathManagerClass();
private:
	static PathManagerClass* m_instance;

	std::vector<PathClass*> m_pathes;
};

#endif