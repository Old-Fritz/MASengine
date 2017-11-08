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


class ConditionCheckerClass
{
public:
	ConditionCheckerClass();

	bool checkCondition(const std::string& expression);
private:
	bool inCaseEquel(const std::string& expression);

private:
	std::string m_expression;
	int m_position = 0;
	int m_group;
	double m_groupval;
};

/////////////
// GLOBALS //
/////////////
static ConditionCheckerClass ConditionChecker;

#endif