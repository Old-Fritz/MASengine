////////////////////////////////////////////////////////////////////////////////
// Filename: IntParamClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _INTPARAMCLASS_H_
#define _INTPARAMCLASS_H_

#include <string>
#include "SetParamClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SetParamClass
////////////////////////////////////////////////////////////////////////////////

class IntParamClass : public SetParamClass
{
public:
	IntParamClass();
	IntParamClass(const IntParamClass&);
	~IntParamClass();

	virtual std::string getStr();
	void setStr(const std::string& str);

	int getParam();
	void setParam(int param);

private:
	int m_param;
};

#endif