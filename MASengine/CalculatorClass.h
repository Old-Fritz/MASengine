////////////////////////////////////////////////////////////////////////////////
// Filename: CalculatorClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _CALCULATORCLASS_H_
#define _CALCULATORCLASS_H_

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
// Class name: CalculatorClass
////////////////////////////////////////////////////////////////////////////////


class CalculatorClass
{
public:
	CalculatorClass();


	double calculate(const std::string& expression);
private:
	//the main function, checks EOF, ' ', floats and incorrect input.
	//Deals with +-*=()/ and manipulate other functions.
	int next();
	//skiping symbols like ( and ) after
	void skip(int t);
	// Dealing with ( and ) and continuing to calculate
	double numpar();
	// Dealing with pow.
	double factor();
	// Dealing with * and /
	double term();
	//Dealing with + and -
	double expr();

private:
	std::string m_expression;
	int m_position = 0;
	int m_group;
	double m_groupval;
};

/////////////
// GLOBALS //
/////////////
static CalculatorClass Calculator;

#endif