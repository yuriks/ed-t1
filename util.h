// util.h
// Funcoes sortidas uteis.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#ifndef YURIKS_T1_UTIL_H
#define YURIKS_T1_UTIL_H

#include <cstring>

namespace yuriks
{
	// strcmp case-insensitive
	// (Nota: difere da original no quesito em que esta so compara ate o comprimento menor das duas string.)
	int istrcmp(const char* s1, const char* s2);
	// Duplica str.
	// strdup eh uma funcao POSIX
	char* strdup(const char* str);
	// strncpy terminado com \0
	char *strnzcpy(char *dest, const char *src, size_t n);
} // namespace yuriks

#endif // YURIKS_T1_UTIL_H
