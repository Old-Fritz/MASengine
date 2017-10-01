#include "StrParamClass.h"

StrParamClass::StrParamClass()
{
}
StrParamClass::StrParamClass(const StrParamClass &)
{
}
StrParamClass::~StrParamClass()
{
}

std::string StrParamClass::getStr()
{
	return m_param;
}

void StrParamClass::setStr(const std::string & str)
{
	m_param = str;
}

std::string StrParamClass::getParam()
{
	return m_param;
}

void StrParamClass::setParam(std::string param)
{
	m_param = param;
}
