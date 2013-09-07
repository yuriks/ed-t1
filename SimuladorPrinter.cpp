// SimuladorPrinter.cpp
// Imprime resultados do simulador de campeonato.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#include "SimuladorPrinter.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip>

namespace yuriks
{
	namespace simulador
	{
		SimuladorPrinter::SimuladorPrinter(const FonteCompetidor& arvore, const Torneio& torneio)
			: arvore(arvore), largura(largura_max(torneio.begin_equipes(), torneio.end_equipes())), profundidade(calc_profundidade(arvore))
		{
		}

		void SimuladorPrinter::imprimir(std::ostream& f_)
		{
			f = &f_;
			imprimir_nodo(arvore, profundidade-1, DIR_BEGIN);
			*f << "\n\n";
			imprimir_chars('#', 80);
			*f << "\n\nEquipe vencedora:\n\n";
			imprime_equipe(*f, arvore.vencedora());

		}

		void SimuladorPrinter::imprimir_nodo(const FonteCompetidor& nodo, unsigned int depth, TraverseDirection dir) const
		{
			/* Esta funcao parece complicada mas na verdade eh apenas uma
			 * serie de testes para decidir a formatacao/indentacao de cada linha.
			 *
			 * Originalmente eu planejava com que a saida fosse algo parecido
			 * com isto: http://img15.imageshack.us/img15/6131/30092009140.jpg
			 * porem, falta de tempo e alguma dificuldade fizeram que eu
			 * decidisse usar essa representacao meia boca. =P
			 */

			const Partida* p = dynamic_cast<const Partida*>(&nodo);
			if (p)
			{
				imprimir_nodo(p->equipe_a(), depth-1, DIR_UP);
			}

			if (p)
			{
				const Equipe& equipe_a = p->equipe_a().vencedora();
				const Equipe& equipe_b = p->equipe_b().vencedora();
				const bool a_venceu = p->pontos_equipe_a() > p->pontos_equipe_b();


				for (unsigned int i = 0; i+1 < depth; ++i)
					imprimir_chars(' ', largura);
				imprimir_chars(' ', largura-1);
				*f << '|';

				*f << equipe_a.nome << ": " << std::setw(2) << p->pontos_equipe_a();
				
				if (dir == DIR_DOWN || a_venceu)
				{
					imprimir_chars(a_venceu ? '-' : ' ', largura-std::strlen(equipe_a.nome)-5);
					if (dir != DIR_BEGIN)
						*f << (a_venceu ? (dir == DIR_UP ? 'v' : '^') : '|');
				}
				*f << '\n';
				
				//////////////////////////////////////////////////////
				
				for (unsigned int i = 0; i+1 < depth; ++i)
					imprimir_chars(' ', largura);
				imprimir_chars(' ', largura-1);
				*f << '|';
				
				*f << equipe_b.nome << ": " << std::setw(2) << p->pontos_equipe_b();
				
				if (dir == DIR_UP || !a_venceu)
				{
					imprimir_chars(!a_venceu ? '-' : ' ', largura-std::strlen(equipe_b.nome)-5);
					if (dir != DIR_BEGIN)
						*f << (!a_venceu ? (dir == DIR_UP ? 'v' : '^') : '|');
				}
				*f << '\n';
			}
			else
			{
				*f << nodo.vencedora().nome;
				imprimir_chars('-', largura*(depth+1) - std::strlen(nodo.vencedora().nome) - 1);
				*f << (dir == DIR_UP ? 'v' : '^') << '\n';
			}

			if (p)
			{
				imprimir_nodo(p->equipe_b(), depth-1, DIR_DOWN);
			}
		}

		void SimuladorPrinter::imprimir_chars(char c, unsigned int count) const
		{
			while (count-- > 0)
				f->put(c);
		}

		unsigned int SimuladorPrinter::calc_profundidade(const FonteCompetidor& arvore)
		{
			// Isto funciona devido ao modo de que a arvore eh construida.

			const Partida *const p = dynamic_cast<const Partida*>(&arvore);
			if (p)
				return 1 + calc_profundidade(p->equipe_b());
			else
				return 1;
		}

		unsigned int SimuladorPrinter::largura_max(VetorEquipe::ConstIterator i /*begin*/, VetorEquipe::ConstIterator end)
		{
			unsigned int larg = 0;
			for (; i != end; ++i)
			{
				unsigned int len = std::strlen((*i)->nome);
				if (larg < len)
					larg = len;
			}

			return larg + 6;
		}
	} // namespace simulador
} // namespace yuriks
