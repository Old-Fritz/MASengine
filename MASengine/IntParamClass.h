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

/*!
\addtogroup Params
@{
*/
/*!
Целочисленный параметр
*/
class IntParamClass : public SetParamClass
{
public:
	IntParamClass();
	IntParamClass(const IntParamClass&);
	~IntParamClass();

	//! Получение параметра в виде строки \return параметр в виде строки
	virtual std::string getStr();
	//! Установка параметра в виде строки \param[in] str - параметр в виде строки
	void setStr(const std::string& str);

	//! Получение параметра \return целочисленный параметр
 	int getParam();
	//! Установка параметра \param[in] param - целочисленный параметр
	void setParam(int param);

private:
	int m_param; //!<Параметр
};
/*! @} */
#endif