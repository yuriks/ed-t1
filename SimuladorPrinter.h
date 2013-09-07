// SimuladorPrinter.h
// Imprime resultados do simulador de campeonato.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#ifndef YURIKS_T1_SIMULADORPRINTER_H
#define YURIKS_T1_SIMULADORPRINTER_H

#include "SimuladorTorneio.h"

#include <iostream>

namespace yuriks
{
	namespace simulador
	{
		class SimuladorPrinter
		{
		public:
			SimuladorPrinter(const FonteCompetidor& arvore, const Torneio& items);
			void imprimir(std::ostream& f);

		private:
			enum TraverseDirection
			{
				DIR_UP,
				DIR_DOWN,
				DIR_BEGIN
			};

			const FonteCompetidor& arvore;
			std::ostream* f;

			unsigned int largura;
			unsigned int profundidade;
			// Recursiva, in-order traversal sequence.
			void imprimir_nodo(const FonteCompetidor& nodo, unsigned int depth, TraverseDirection dir) const;
			// Imprime caracter `c`, `count` vezes.
			void imprimir_chars(char c, unsigned int count) const;
			// Calcula profundidade maxima da arvore.
			static unsigned int calc_profundidade(const FonteCompetidor& arvore);
			// Calcula maior largura de um nome de equipe.
			static unsigned int largura_max(VetorEquipe::ConstIterator begin, VetorEquipe::ConstIterator end);

			// Nao assignable
			SimuladorPrinter& operator=(const SimuladorPrinter&);
		};
	} // namespace simulador
} // namespace yuriks

#endif // YURIKS_T1_SIMULADORPRINTER_H
