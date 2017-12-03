#include "IntParamClass.h"

IntParamClass::IntParamClass()
{
	m_param = 0;
}
IntParamClass::IntParamClass(const IntParamClass &)
{
}
IntParamClass::~IntParamClass()
{
}

std::string IntParamClass::getStr()
{
	return std::to_string(m_param);
}

void IntParamClass::setStr(const std::string & str)
{
	if (str.size() > 0)
		m_param = stoi(str);
	else
		m_param = 0; //def value
}

int IntParamClass::getParam()
{
	return m_param;
}

void IntParamClass::setParam(int param)
{
	m_param = param;
}
