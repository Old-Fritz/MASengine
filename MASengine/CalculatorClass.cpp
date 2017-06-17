#include "CalculatorClass.h"

CalculatorClass::CalculatorClass()
{
}

double CalculatorClass::calculate(const std::string& expression)
{
	m_expression = expression;
	m_position = 0;
	next();
	return expr();
}

int CalculatorClass::next()
{
	while (1)
	{
		int c;
		if (m_position < m_expression.size())
		{
			c = m_expression[m_position];
			m_position++;
		}
		else return 0;
		if (strchr("+-*/^()\n", c) != NULL)
			return m_group = c;
		if (isdigit(c) || c == '.')
		{
			m_position--;
			std::string str;
			while ((isdigit(c) || c == '.') && m_position<m_expression.size())
			{
				str += c;
				m_position++;
				c = m_expression[m_position];
			}
			m_groupval = stof(str);
			return m_group = 'n';
		}
	}
}

void CalculatorClass::skip(int t)
{
	//if (m_group != t)
	//return;
	next();
}

double CalculatorClass::numpar()
{
	if (m_group == 'n')
	{
		double x = m_groupval;
		skip('n'); return x;
	}
	skip('(');
	double x = expr();
	skip(')');
	return x;
}

double CalculatorClass::factor()
{
	double x = numpar();
	if (m_group == '^')
	{
		skip('^'); x = pow(x, factor());
	}
	return x;
}

double CalculatorClass::term()
{
	double x = factor();
	while (1)
	{
		if (m_group == '*')
		{
			skip('*'); x *= factor();
		}
		else if (m_group == '/')
		{
			skip('/'); x /= factor();
		}
		else return x;
	}
}

double CalculatorClass::expr()
{
	double x = term();
	while (1)
	{
		if (m_group == '+')
		{
			skip('+');
			x += term();
		}
		else if (m_group == '-')
		{
			skip('-');
			x -= term();
		}
		else return x;
	}
}
