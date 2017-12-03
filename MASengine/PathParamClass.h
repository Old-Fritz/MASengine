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

/*!
\addtogroup Params
@{
*/
/*!
Параметр с типом пути файла
*/
class PathParamClass : public SetParamClass
{
public:
	PathParamClass();
	PathParamClass(const PathParamClass&);
	~PathParamClass();

	void Shutdown();

	//! Получение параметра в виде строки \return параметр в виде строки
	virtual std::string getStr();
	//! Установка параметра в виде строки \param[in] str - параметр в виде строки
	void setStr(const std::string& str);

	//! Получение параметра \return целочисленный параметр типа пути файла
	PathClass* getParam();
	//! Установка параметра \param[in] param - параметр типа числа с плав. зап.
	void setParam(PathClass* param);

private:
	PathClass* m_param; //!<Параметр
};
/*! @} */
#endif