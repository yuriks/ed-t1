// Equipe.h
// Representa um trio de truco.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#ifndef YURIKS_T1_EQUIPE_H
#define YURIKS_T1_EQUIPE_H

#include <iostream>

namespace yuriks
{
	/** Representa uma Equipe de truco. */
	struct Equipe
	{
		Equipe();

		unsigned int id;
		char nome[21];
		char integrantes[3][33];
	};

	void imprime_equipe(std::ostream& s, const Equipe& e);
} // namespace yuriks

#endif // YURIKS_T1_EQUIPE_H
