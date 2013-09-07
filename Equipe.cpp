// Equipe.cpp
// Representa um trio de truco.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#include "Equipe.h"

#include <iostream>

namespace yuriks
{
	Equipe::Equipe()
		: id(0)
	{
		nome[0] = '\0';
		for (unsigned int i = 0; i < 3; ++i)
			integrantes[i][0] = '\0';
	}

	void imprime_equipe(std::ostream& s, const Equipe& e)
	{
		s << "Equipe " << e.id << ": " << e.nome << '\n';
		for (unsigned int i = 0; i < 3; ++i)
		{
			s << "  Integrante " << i+1 << ": " <<
				e.integrantes[i] << '\n';
		}
	}
} // namespace yuriks
