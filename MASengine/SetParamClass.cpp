#include "SetParamClass.h"

SetParamClass::SetParamClass()
{
}
SetParamClass::SetParamClass(const SetParamClass &)
{
}
SetParamClass::~SetParamClass()
{
}

void SetParamClass::Shutdown()
{
	m_name.clear();
}

std::string SetParamClass::getStr()
{
	return "";
}

void SetParamClass::setStr(const std::string & str)
{
	return;
}

std::string SetParamClass::getName()
{
	return m_name;
}

void SetParamClass::setName(const std::string & name)
{
	m_name = name;
}
