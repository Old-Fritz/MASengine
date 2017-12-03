////////////////////////////////////////////////////////////////////////////////
// Filename: ConditionCheckerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _CONDITIONCHECKERCLASS_H_
#define _CONDITIONCHECKERCLASS_H_

//////////////
// INCLUDES //
//////////////

#include<string>
#include<codecvt>
#include<fstream>
#include<vector>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: ConditionCheckerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup CommandManagment
@{
*/
/*!
Обработка логических выражений
*/
class ConditionCheckerClass
{
public:
	ConditionCheckerClass();

	//! \brief Вычисление выражения \param[in] expression - логическое выражение \return Результат выражения
	//! \details Пока работает только для выражений типа XXXzzYYY, где XXX - левый параметр, YYY - правый параметр, а zz - некий оператор
	bool checkCondition(const std::string& expression);
private:
	//! Вычисление условия на ==  \param[in] expression - логическое выражение \return Результат выражения
	bool inCaseEquel(const std::string& expression);
};

/////////////
// GLOBALS //
/////////////
static ConditionCheckerClass ConditionChecker;
/*! @} */
#endif