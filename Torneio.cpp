// Torneio.cpp
// Estrutura principal do torneio de truco.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#include "Torneio.h"

#include "util.h"

namespace yuriks
{
	Torneio::Torneio()
		: prox_id(1)
	{
	}

	Torneio::Torneio(unsigned int num_equipes)
		: equipes(num_equipes), prox_id(1)
	{
	}


	unsigned int Torneio::add_equipe(Equipe equipe)
	{
		equipe.id = prox_id++;
		add_equipe_id(equipe);

		return equipe.id;
	}

	void Torneio::add_equipe_id(const Equipe& equipe)
	{
		// TODO Verificar por id duplicado?
		equipes.push_back(equipe);
	}

	void Torneio::reserva_equipes(unsigned int n)
	{
		equipes.reserve(n);
	}

	void Torneio::remover_equipe(VetorEquipe::Iterator equipe)
	{
		equipes.erase(equipe);
	}

	VetorEquipe::Iterator Torneio::encontrar_equipe(unsigned int id)
	{
		VetorEquipe::Iterator i;
		for (i = equipes.begin(); i != equipes.end(); ++i)
		{
			if ((*i)->id == id)
				break;
		}

		return i;
	}

	VetorEquipe::Iterator Torneio::encontrar_equipe(const char* str,
			VetorEquipe::Iterator i)
	{
		if (i == equipes.end())
			return equipes.end();

		for (; i != equipes.end(); ++i)
		{
			if (istrcmp((*i)->nome, str) == 0)
				return i;
			for (unsigned int j = 0; j < 3; ++j)
				if (istrcmp((*i)->integrantes[j], str) == 0)
					return i;

		}

		return i;
	}


	unsigned int Torneio::num_equipes() const
	{
		return equipes.size();
	}


	VetorEquipe::Iterator Torneio::begin_equipes()
	{
		return equipes.begin();
	}

	VetorEquipe::Iterator Torneio::end_equipes()
	{
		return equipes.end();
	}

	VetorEquipe::ConstIterator Torneio::begin_equipes() const
	{
		return equipes.begin();
	}

	VetorEquipe::ConstIterator Torneio::end_equipes() const
	{
		return equipes.end();
	}
} // namespace yuriks
