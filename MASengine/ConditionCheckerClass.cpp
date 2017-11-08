#include "ConditionCheckerClass.h"

ConditionCheckerClass::ConditionCheckerClass()
{
}

bool ConditionCheckerClass::checkCondition(const std::string & expression)
{
	if (expression.find("==") != std::string::npos)
	{
		return inCaseEquel(expression);
	}
}

bool ConditionCheckerClass::inCaseEquel(const std::string & expression)
{
	int pos = expression.find("==");

	std::string leftValue;
	std::string rihtValue;

	for (int i = 0;i < pos;i++)
		leftValue += expression[i];
	for (int i = pos+2;i < expression.size();i++)
		rihtValue += expression[i];

	if (std::stoi(leftValue) == std::stoi(rihtValue))
		return true;

	return false;
}
