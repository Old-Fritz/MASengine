////////////////////////////////////////////////////////////////////////////////
// Filename: FloatParamClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _FLOATPARAMCLASS_H_
#define _FLOATPARAMCLASS_H_

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
ѕараметр с типом числа с плавающей зап¤той
*/
class FloatParamClass : public SetParamClass
{
public:
	FloatParamClass();
	FloatParamClass(const FloatParamClass&);
	~FloatParamClass();

	//! ѕолучение параметра в виде строки \return параметр в виде строки
	virtual std::string getStr();
	//! ”становка параметра в виде строки \param[in] str - параметр в виде строки
	void setStr(const std::string& str);

	//! ѕолучение параметра \return целочисленный параметр типа числа с плав. зап.
	float getParam();
	//! ”становка параметра \param[in] param - параметр типа числа с плав. зап.
	void setParam(float param);

private:
	float m_param;  //!<ѕараметр
};
/*! @} */
#endif