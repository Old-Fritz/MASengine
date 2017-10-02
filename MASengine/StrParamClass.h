////////////////////////////////////////////////////////////////////////////////
// Filename: StrParamClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _STRPARAMCLASS_H_
#define _STRPARAMCLASS_H_

#include <string>
#include "SetParamClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SetParamClass
////////////////////////////////////////////////////////////////////////////////

class StrParamClass : public SetParamClass
{
public:
	StrParamClass();
	StrParamClass(const StrParamClass&);
	~StrParamClass();

	void Shutdown();

	virtual std::string getStr();
	void setStr(const std::string& str);

	std::string getParam();
	void setParam(std::string param);

private:
	std::string m_param;
};

#endif