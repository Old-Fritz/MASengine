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

/*!
\addtogroup Params
@{
*/
/*!
Строковый параметр
*/
class StrParamClass : public SetParamClass
{
public:
	StrParamClass();
	StrParamClass(const StrParamClass&);
	~StrParamClass();

	void Shutdown();

	//! Получение параметра в виде строки \return параметр в виде строки
	virtual std::string getStr();
	//! Установка параметра в виде строки \param[in] str - параметр в виде строки
	void setStr(const std::string& str);

	//! Получение параметра \return строковый параметр
	std::string getParam();
	//! Установка параметра \param[in] param - строковый параметр
	void setParam(std::string param);

private:
	std::string m_param; //!<Параметр
};
/*! @} */
#endif