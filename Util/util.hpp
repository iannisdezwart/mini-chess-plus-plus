#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <bits/stdc++.h>

#define ANSI_RESET "\e[m"

namespace util
{
	bool starts_with(const std::string& str, const std::string& search)
	{
		if (search.size() > str.size()) return false;

		for (size_t i = 0; i < search.size(); i++)
		{
			if (str[i] != search[i]) return false;
		}

		return true;
	}

	std::string repeat(const char *str, size_t repeat_count)
	{
		std::string out;

		for (size_t i = 0; i < repeat_count; i++)
		{
			out += str;
		}

		return out;
	}
};

#endif