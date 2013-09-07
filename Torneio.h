// Torneio.h
// Estrutura principal do torneio de truco.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#ifndef YURIKS_T1_TORNEIO_H
#define YURIKS_T1_TORNEIO_H

#include "Equipe.h"
#include "VetorEquipe.h"

namespace yuriks
{
	/** Representa um torneio de truco, mantendo uma lista de
	 * jogadores. */
	class Torneio
	{
	public:
		Torneio();
		// Reserva espaco no vetor para num_equipes.
		explicit Torneio(unsigned int num_equipes);

		// Adiciona uma equipe.
		// Retorna id unico da equipe, que sera gerado.
		unsigned int add_equipe(Equipe equipe);
		// Nao gera id, usa equipe.id
		void add_equipe_id(const Equipe& equipe);
		// Reserva espaco de alocacao para n equipes.
		void reserva_equipes(unsigned int n);
		// Remove equipe no iterador.
		// Se equipe == end_equipes(), nao faz nada.
		void remover_equipe(VetorEquipe::Iterator equipe);
		// Encontra equipe com id especificado.
		// Retorna end_equipes() se nao encontrado.
		VetorEquipe::Iterator encontrar_equipe(unsigned int id);
		// Encontra equipe com nome ou integrante especificado.
		// Comeca a busca no iterador inicio.
		VetorEquipe::Iterator encontrar_equipe(const char* str,
				VetorEquipe::Iterator inicio);

		unsigned int num_equipes() const;

		// Retornam ptr->ptr pela necessidade de se usar um vetor de
		// ponteiros. Poderia-se usar iteradores mas nao ha
		// necessidade de se manter a interface neste trabalho.
		VetorEquipe::Iterator begin_equipes();
		VetorEquipe::Iterator end_equipes();
		VetorEquipe::ConstIterator begin_equipes() const;
		VetorEquipe::ConstIterator end_equipes() const;

	private:
		VetorEquipe equipes;

		unsigned int prox_id;
	};
} // namespace yuriks

#endif // YURIKS_T1_TORNEIO_H
