#include "Utils.h"

int Utils::getHash(const std::string & str)
{
	const int p = 101;

	long long hash = 0;
	long long p_pow = 1;
	for (int i = 0; i < str.length();i++)
	{
		hash += static_cast<long long>(str[i] * p_pow);
		p_pow *= p;
	}

	return hash;
}
