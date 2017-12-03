////////////////////////////////////////////////////////////////////////////////
// Filename: SetParamClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _SETPARAMCLASS_H_
#define _SETPARAMCLASS_H_

#include <string>

////////////////////////////////////////////////////////////////////////////////
// Class name: SetParamClass
////////////////////////////////////////////////////////////////////////////////


/*!
\defgroup Params Параметры настроек
\ingroup Settings
@{
*/
/*!
Некий параметр с определенным типом, абстрактный класс
*/
class SetParamClass
{
public:
	SetParamClass();
	SetParamClass(const SetParamClass&);
	~SetParamClass();

	virtual void Shutdown();

	//! Получение параметра в виде строки \return параметр в виде строки
	virtual std::string getStr();
	//! Установка параметра в виде строки \param[in] str - параметр в виде строки
	virtual void setStr(const std::string& str);

	//! Получение имени параметра \return имя параметра в виде строки
	std::string getName();
	//! Изменение имени пареметра \param[0] name - новое имя параметра
	void setName(const std::string& name);

protected:
	std::string m_name; //!<имя параметра
};
/*! @} */
#endif