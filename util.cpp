// util.cpp
// Funcoes sortidas uteis.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#include "util.h"

#include <cctype>
#include <cstring>

namespace yuriks
{
	int istrcmp(const char* s1, const char* s2)
	{
		for (; *s1 != '\0' && *s2 != '\0'; ++s1, ++s2)
		{
			int c1 = std::tolower(*s1);
			int c2 = std::tolower(*s2);
			if (c1 != c2)
				return c1 - c2;
		}

		return 0;
	}

	char* strdup(const char* str)
	{
		return std::strcpy(new char[std::strlen(str)+1], str);
	}

	char *strnzcpy(char *dest, const char *src, size_t n)
	{
		unsigned int i;
		for (i = 0; src[i] != '\0' && i < n-1; ++i)
			dest[i] = src[i];
		dest[i] = '\0';

		return dest;
	}
} // namespace yuriks
