////////////////////////////////////////////////////////////////////////////////
// Filename: ProvManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROVMANAGERCLASS_H_
#define _PROVMANAGERCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "provclass.h"
#include "LoadScreenManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvClass
////////////////////////////////////////////////////////////////////////////////

class ProvManagerClass
{

public:
	

	bool Initialize(PathClass* filename);
	void Shutdown();


	//Getters
	ProvClass* getProv(int provNum);

	static ProvManagerClass& getI();

private:
	ProvManagerClass();
	ProvManagerClass(const ProvManagerClass&);
	~ProvManagerClass();
private:
	static ProvManagerClass* m_instance;


	std::vector<ProvClass*> m_provs;
};

#endif