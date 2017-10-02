#include "PathParamClass.h"

PathParamClass::PathParamClass()
{
	m_param = PathManagerClass::getI().makePath();
}
PathParamClass::PathParamClass(const PathParamClass &)
{
}
PathParamClass::~PathParamClass()
{
}

void PathParamClass::Shutdown()
{
	SetParamClass::Shutdown();
}

std::string PathParamClass::getStr()
{
	return m_param->getShortPath();
}

void PathParamClass::setStr(const std::string & str)
{
	m_param->changePath(str);
}

PathClass * PathParamClass::getParam()
{
	return m_param;
}

void PathParamClass::setParam(PathClass * param)
{
	m_param = param;
}
