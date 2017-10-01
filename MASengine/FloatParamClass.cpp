#include "FloatParamClass.h"

FloatParamClass::FloatParamClass()
{
}
FloatParamClass::FloatParamClass(const FloatParamClass &)
{
}
FloatParamClass::~FloatParamClass()
{
}

std::string FloatParamClass::getStr()
{
	return std::to_string(m_param);
}

void FloatParamClass::setStr(const std::string & str)
{
	if (str.size() > 0)
		m_param = stof(str);
	else
		m_param = 0; //def value
}

float FloatParamClass::getParam()
{
	return m_param;
}

void FloatParamClass::setParam(float param)
{
	m_param = param;
}
