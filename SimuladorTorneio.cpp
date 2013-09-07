// SimuladorTorneio.cpp
// Simulador de torneios. Usa numeros aleatorios para determinar resultado das partidas.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#include "SimuladorTorneio.h"

#include "SimuladorPrinter.h"

#include "util.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

namespace yuriks
{
	namespace simulador
	{
		FonteEquipe::FonteEquipe(const Equipe& equipe)
			: equipe(&equipe)
		{
		}

		const Equipe& FonteEquipe::vencedora() const
		{
			return *equipe;
		}

		Partida::Partida(FonteCompetidor* a, FonteCompetidor* b)
			: vencedora_(0), compet_a(a), compet_b(b)
		{
			simular();
		}

		Partida::~Partida()
		{
			if (compet_a)
				delete compet_a;
			if (compet_b)
				delete compet_b;
		}

		const Equipe& Partida::vencedora() const
		{
			return vencedora_->vencedora();
		}

		void Partida::simular()
		{
			/* Esta funcao gera dois numeros aleatorios no intervalo [0, 24]
			 * e depois, pega a diferenca entre eles para deixar uma das
			 * equipes com 24 pontos, que sao necessarios para uma partida
			 * acabar em vitoria.
			 */
			if (vencedora_)
				return;

			int pa, pb;
			do
			{
				pa = std::rand() % 25;
				pb = std::rand() % 25;
			} // Empates non-eczistem.
			while (pa == pb);

			if (pa > pb)
			{
				pontos_b = 24 - (pa - pb);
				pontos_a = 24;
				vencedora_ = compet_a;
			}
			else
			{
				pontos_a = 24 - (pb - pa);
				pontos_b = 24;
				vencedora_ = compet_b;
			}
		}


		SimuladorTorneio::SimuladorTorneio(const Torneio& torneio)
			: torneio(torneio), arvore(0)
		{
			assert(torneio.num_equipes() > 0);
		}

		SimuladorTorneio::~SimuladorTorneio()
		{
			delete arvore;
		}

		void SimuladorTorneio::simular()
		{
			assert(!arvore);

			arvore = criar_arvore();
		}

		void SimuladorTorneio::imprimir(std::ostream& f) const
		{
			assert(arvore);

			SimuladorPrinter printer(*arvore, torneio);
			printer.imprimir(f);
		}


		FonteCompetidor* SimuladorTorneio::criar_arvore()
		{
			/* Para criar a arvore a funcao cria duas pilhas, uma inicialmente
			 * vazia e a outra contendo todas as equipes participantes. A
			 * seguir ela faz varias varreduras alternando nas pilhas para
			 * combinar equipes/resultados de jogos de dois a dois.
			 */

			PilhaFC a(torneio.num_equipes());
			PilhaFC b(torneio.num_equipes());

			// Inicia com equipes na pilha a
			for (VetorEquipe::ConstIterator i = torneio.begin_equipes();
				i != torneio.end_equipes();
				++i)
			{
				a.push(new FonteEquipe(**i));
			}

			PilhaFC* from = &a;
			PilhaFC* to = &b;
			while (from->size() != 1)
			{
				criar_nivel(*from, *to);

				// Troca from e to
				PilhaFC* tmp = from;
				from = to;
				to = tmp;
			}

			return from->top();
		}

		void SimuladorTorneio::criar_nivel(PilhaFC& from, PilhaFC& to)
		{
			while (from.size() >= 2)
			{
				FonteCompetidor* a = from.top();
				from.pop();
				FonteCompetidor* b = from.top();
				from.pop();
				
				to.push(new Partida(a, b));
			}

			// Move item que nao pode ser pareado para a pilha de saida.
			if (from.size() > 0)
			{
				to.push(from.top());
				from.pop();
			}
		}
	} // namespace simulador
} // namespace yuriks
