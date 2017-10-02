////////////////////////////////////////////////////////////////////////////////
// Filename: PathParamClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _PATHPARAMCLASS_H_
#define _PATHPARAMCLASS_H_

#include <string>
#include "SetParamClass.h"
#include "PathManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: PathParamClass
////////////////////////////////////////////////////////////////////////////////

class PathParamClass : public SetParamClass
{
public:
	PathParamClass();
	PathParamClass(const PathParamClass&);
	~PathParamClass();

	void Shutdown();

	virtual std::string getStr();
	void setStr(const std::string& str);

	PathClass* getParam();
	void setParam(PathClass* param);

private:
	PathClass* m_param;
};

#endif